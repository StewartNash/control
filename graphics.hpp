#pragma once
#include "models.hpp"

class Grid {

};

// Graphics

class IRenderer {
	public:
		virtual ~IRenderer() = default;
		
		virtual void setDrawColor(int r, int g, int b, int a) = 0;
		virtual void drawLine(double x1, double y1, double x2, double y2) = 0;
		virtual void fillRect(double x, double y, double w, double h) = 0; // Add rotation
		virtual void fillTriangle(double x, double y, double w, double h) = 0; // Add rotation
		// Add more primitives (circles, polygons, etc.)
};

class IShape {
	public:
		virtual ~IShape() = default;
		virtual void setPosition(double x, double y) = 0;
		virtual void setSize(double width, double height) = 0;
		virtual void setRotation(double angle) = 0;
		virtual void setColor(double r, double g, double b, double a);
		
		virtual void draw(IRenderer& renderer) const = 0;
};

// Graph Theory

class GraphicalVertex : public IVertex, public IShape {
	public:
		explicit GraphicalVertex() : x(0), y(0),
			width(0), height(0),
			angle(0),
			red(255), green(255), blue(255), alpha(255) {}
		void setPosition(double x_, double y_) override {
			x = x_;
			y = y_;
		}
		void setSize(double w, double h) override {
			width = w;
			height = h;
		}
		void setRotation(double rotation) override {
			angle = rotation;
		}
		void setColor(double r, double g, double b, double a) {
			red = r;
			green = g;
			blue = b;
			alpha = a;		
		}
		void draw(IRenderer& renderer) const override {
			renderer.setDrawColor(red, green, blue, alpha);
			renderer.fillRect(x, y, width, height);
		}
	protected:
		double x, y;
		double width, height;
		double angle;
		double red, green, blue, alpha;
};

// DSP Signal Flow Graphs

class GraphicalDelay : public GraphicalVertex {
	void draw(IRenderer& renderer) const override {
		renderer.setDrawColor(red, green, blue, alpha);
		renderer.fillRect(x, y, width, height);
	}
};

class GraphicalAdder : public GraphicalVertex {
	void draw(IRenderer& renderer) const override {
		renderer.setDrawColor(red, green, blue, alpha);
		renderer.fillRect(x, y, width, height);
	}
};

class GraphicalMultiplier : public GraphicalVertex {
	void draw(IRenderer& renderer) const override {
		renderer.setDrawColor(red, green, blue, alpha);
		renderer.fillRect(x, y, width, height);
	}
};

class GraphicalBranchPoint : public GraphicalVertex {
	void draw(IRenderer& renderer) const override {
		renderer.setDrawColor(red, green, blue, alpha);
		renderer.fillRect(x, y, width, height);
	}
};

class GraphicalFilterStructureDiagram : public FilterStructureDiagram {
	public:
		void draw(IRenderer& renderer) const {

		}
};
