#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>

// librsvg + cairo
#include <cairo.h>
#include <librsvg/rsvg.h>

// Helper: run node -e "..." and pass latex via stdin, capture stdout (SVG)
std::string render_with_katex_via_node(const std::string &latex) {
    // Small JS program: read stdin, require('katex'), renderToString(..., {output: 'svg'})
    const char *js = R"JS(
const fs = require('fs');
const katex = require('katex');
let input = '';
process.stdin.setEncoding('utf8');
process.stdin.on('data', function(chunk){ input += chunk; });
process.stdin.on('end', function(){
  try {
    // Accept either raw math or with $...$ — prefer display mode if contains \[ or $$ or starts/ends with $
    const trimmed = input.trim();
    const displayMode = (/^\$\$?/.test(trimmed) || /^\[/.test(trimmed) || trimmed.includes('\\[') || trimmed.includes('\\\\['));
    const math = trimmed.replace(/^\$+|\\$+$/g, ''); // strip leading/trailing $
    const svg = katex.renderToString(math, {output: 'svg', throwOnError: false, displayMode: displayMode});
    // katex.renderToString returns SVG wrapped in <span class="katex">...<svg ...>...</svg></span>
    // We attempt to extract only the <svg ...>...</svg> inner element if present.
    const m = svg.match(/<svg[\\s\\S]*<\\/svg>/);
    if(m) {
      console.log(m[0]);
    } else {
      console.log(svg);
    }
  } catch (err) {
    console.error('KATEX_ERROR:' + err.toString());
    process.exit(2);
  }
});
)JS";

    // create pipes for stdin/stdout
    int inpipe[2], outpipe[2];
    if (pipe(inpipe) != 0 || pipe(outpipe) != 0) {
        throw std::runtime_error("pipe() failed");
    }

    pid_t pid = fork();
    if (pid == -1) {
        throw std::runtime_error("fork() failed");
    }

    if (pid == 0) {
        // child
        // Connect pipes: child's stdin <- inpipe[0]; child's stdout -> outpipe[1]
        dup2(inpipe[0], STDIN_FILENO);
        dup2(outpipe[1], STDOUT_FILENO);
        // close unused fds
        close(inpipe[0]); close(inpipe[1]);
        close(outpipe[0]); close(outpipe[1]);

        // exec node with -e and the JS program
        // Use execlp to run "node"
        // Supply the JS code via argv[2] to -e
        execlp("node", "node", "-e", js, (char*)nullptr);
        // if exec fails:
        _exit(127);
    } else {
        // parent: close ends we don't use
        close(inpipe[0]);
        close(outpipe[1]);

        // write latex to child's stdin, then close that end
        ssize_t to_write = (ssize_t)latex.size();
        const char *buf = latex.c_str();
        while (to_write > 0) {
            ssize_t w = write(inpipe[1], buf, to_write);
            if (w <= 0) break;
            to_write -= w;
            buf += w;
        }
        close(inpipe[1]); // signal EOF to child

        // read child's stdout
        std::string out;
        std::array<char, 4096> readbuf;
        ssize_t r;
        while ((r = read(outpipe[0], readbuf.data(), readbuf.size())) > 0) {
            out.append(readbuf.data(), r);
        }
        close(outpipe[0]);

        // wait for child
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            if (code == 2) {
                throw std::runtime_error("KaTeX rendering error (see node stderr).");
            } else if (code != 0) {
                throw std::runtime_error("Node process exited with code " + std::to_string(code));
            }
        } else if (WIFSIGNALED(status)) {
            throw std::runtime_error("Node process killed by signal");
        }

        return out;
    }
}

// Rasterize SVG string via librsvg + cairo, write PNG output
void svg_to_png(const std::string &svg, const std::string &out_png, int scale = 2) {
    // Initialize librsvg (since 2.40 it is rsvg_init, older versions may not need it)
    rsvg_init();

    GError *error = nullptr;
    RsvgHandle *handle = rsvg_handle_new_from_data(reinterpret_cast<const guint8*>(svg.data()), svg.size(), &error);
    if (!handle) {
        std::string msg = "Failed to create RsvgHandle: ";
        if (error) {
            msg += error->message;
            g_clear_error(&error);
        }
        throw std::runtime_error(msg);
    }

    RsvgDimensionData dim;
    rsvg_handle_get_dimensions(handle, &dim);
    // dim.width/height are in "units" (usually px). If 0, pick a default.
    int width = (dim.width > 0) ? dim.width : 200;
    int height = (dim.height > 0) ? dim.height : 50;

    // apply scale factor (to increase resolution)
    width *= scale;
    height *= scale;

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);

    // scale context so that the SVG renders at desired pixels
    cairo_scale(cr, (double)scale, (double)scale);

    // Render SVG into the cairo context
    if (!rsvg_handle_render_cairo(handle, cr)) {
        cairo_destroy(cr);
        cairo_surface_destroy(surface);
        g_object_unref(handle);
        throw std::runtime_error("Failed to render SVG with librsvg");
    }

    // Write PNG to file
    cairo_surface_write_to_png(surface, out_png.c_str());

    // cleanup
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    g_object_unref(handle);
    rsvg_term();
}

int main(int argc, char **argv) {
    try {
        std::string latex;
        if (argc > 1) {
            // join all argv[1..] as latex
            for (int i = 1; i < argc; ++i) {
                latex += argv[i];
                if (i + 1 < argc) latex += " ";
            }
        } else {
            // default example
            latex = R"($\displaystyle \int_0^\infty e^{-x^2}\,dx = \frac{\sqrt{\pi}}{2}$)";
        }

        std::cout << "Rendering LaTeX to SVG via KaTeX (node)...\n";
        std::string svg = render_with_katex_via_node(latex);

        if (svg.empty()) {
            throw std::runtime_error("Received empty SVG from KaTeX");
        }

        // Optionally you can inspect the SVG:
        // std::cout << "SVG length: " << svg.size() << "\n";
        // std::cout << svg << "\n";

        std::string out_png = "equation.png";
        std::cout << "Rasterizing SVG to " << out_png << " ...\n";
        svg_to_png(svg, out_png, /*scale=*/4);
        std::cout << "Saved " << out_png << "\n";
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}

