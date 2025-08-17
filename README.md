# Control
Control system and digital signal processing application

## Introduction

This is an application made to visually build control systems and export them to Python, LaTeX and (secondarily) MATLAB. The control systems can be designed using either signal flow graphs or (functional) block diagrams. The application also has the capability to visually build digital signal processing filters through filter structure diagrams. The filters can then be exported in Python's SciPy, the custom "Signal" library -- available in repository on this site -- and (secondarily) MATLAB.

The application also has a system for visually identifying and translating filter structure diagrams and block diagrams into its own custom format for import into the application. It includes a useful training set curating tool which will be separated into its own repository.

### Philosophy

For longevity, dependencies on third-party libraries in the code base should be segregated by an interface layer. So when third party libraries change their implementation or become obsolete it is easy to change libraries. To the extent possible, this philosophy should be adopted in all code that any person or organization produces.

## Build Instructions
When cloning the repository, please use
```
git clone --recursive https://github.com/StewartNash/control.git
```
or
```
git clone --recurse-submodules https://github.com/StewartNash/control.git
```
If the repository has already been cloned without fetching the subodules, you can enter the following command
```
git submodule update --init --recursive --remote
```
Or if a comprehensive submodule update which affects nested submodules and fetches changes from the origin is not needed, you may simply enter
```
git submodule init
git submodule update
```
TODO: Please reduce the vcaerbosity of the above to one single line.
If the build directory does not yet exist:
```
mkdir build
```
And then
```
cd build
cmake ..
cmake --build .
./control
```

## TODO
- Files shall have the extension '.blk'
- .blk files are XML
- Research and implement Python bindings
- Use artificial intelligence and image processing to recognize and import filter structure diagrams
- Training data annotation tool
- Neural network script

## Future Work

## Notes
```
git submodule add https://github.com/opencv/opencv.git external/opencv
git submodule update --init --recursive
cd external/opencv
mkdir build && cd build
cmake ..
make -j$(nproc)
```
