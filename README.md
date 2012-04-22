# Simple ODE framework for iOS.

iNumerics currently uses ODE solvers from [odeint] (https://github.com/headmyshoulder/odeint-v2).


## Requirements:

- Mac OS X 10.7
- Xcode 4.2
- CMake 2.8

## Compile:

Simple build:

    cd path/to/iNumerics

    ./make.sh ios-device  (building for device)

or

    ./make.sh ios-simulator  (building for simulator)

or

    ./make.sh x86  (builds for x86, builds example programs)


## Manual build:

    cd path/to/iNumerics
    mkdir build && cd build

## Building for iOS:

if you are building for device:

    cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain/iOS.cmake -DTARGET_PLATFORM=IOS -GXcode ..

or if you are building for simulator:

    cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain/iOS.cmake -DTARGET_PLATFORM=IOS -DIOS_PLATFORM=SIMULATOR -GXcode .. 


to finally start the build process:

    xcodebuild -target install -configuration Release

## Building for x86:

    cmake ..

to start the build process and install everything to `path/to/iNumerics/dist`:

    make install

## Cleaning builds:

    ./make.sh clean

## Usage:

The static library `libinumerics.a` can be found in `path/to/iNumerics/dist`

Example programs (only x86) can be found in `path/to/iNumerics/dist/examples`.

To use this library in an existing Xcode project, add the library as framework
and add dist/include to the the project. 

> **NOTE:** Make sure you add the libraries and the header files as reference only!
> This makes it a lot easier to switch between target platforms (device/simulator).
