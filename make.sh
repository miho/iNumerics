#!/bin/bash


# Copyright 2012 Michael Hoffer <info@michaelhoffer.de>. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are
# permitted provided that the following conditions are met:
#
#    1. Redistributions of source code must retain the above copyright notice, this list of
#       conditions and the following disclaimer.
#
#    2. Redistributions in binary form must reproduce the above copyright notice, this list
#       of conditions and the following disclaimer in the documentation and/or other materials
#       provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY Michael Hoffer <info@michaelhoffer.de> "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Michael Hoffer <info@michaelhoffer.de> OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are those of the
# authors and should not be interpreted as representing official policies, either expressed
# or implied, of Michael Hoffer <info@michaelhoffer.de>.
#/


if [ "$1" == "clean" ]
then 
	echo ">> cleaning..."
	rm -rf build/* dist/*
	exit 0
fi

rm -rf build/* && mkdir -p build && cd build

if [ "$1" == "ios-simulator" ]
then
	cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain/iOS.cmake -DTARGET_PLATFORM=IOS -DIOS_PLATFORM=SIMULATOR -GXcode .. 
	xcodebuild -target install -configuration Release
	cp src/Release-iphonesimulator/libinumerics.a ../dist/libinumerics.a
elif [ "$1" == "ios-device" ] 
then
	cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain/iOS.cmake -DTARGET_PLATFORM=IOS -GXcode ..
	xcodebuild -target install -configuration Release
	cp src/Release-iphoneos/libinumerics.a ../dist/libinumerics.a
elif [ "$1" == "x86" ]
then 
	cmake -DTARGET_PLATFORM="X86" ..
	make install
else
	echo ">> ERROR: no build platform specified!"
	echo ">> USAGE:"
	echo ">> ./make.sh ios-simulator # build for simulator"
	echo ">> ./make.sh ios-device    # build for device (iphone/ipad)"
	echo ">> ./make.sh x86           # build for x86 (builds example programs)"
	exit 1
fi

