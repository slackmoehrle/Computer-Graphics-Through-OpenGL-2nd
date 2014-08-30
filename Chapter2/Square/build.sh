#/bin/bash

mkdir cmakeOutput
cd cmakeOutput

# supress 10.9 warnings about deprecated functions
export SDKROOT=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk

CC=clang CXX=clang++ cmake ../.

make VERBOSE=1
