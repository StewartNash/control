# Control
Signal processing and control application

## Introduction

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
- Research and implement Python bindings

## Future Work
