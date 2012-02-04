#!/bin/bash

mkdir -p build && cd build

cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain/iOS.cmake -GXcode ..
xcodebuild -target install -configuration Release
