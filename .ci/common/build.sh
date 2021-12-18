#!/bin/bash

PLATFORM=$1

qmake
if [[ "$PLATFORM" == "win32" ]]; then
	mingw32-make.exe -j4
elif [[ "$PLATFORM" == "win64" ]]; then
	make -j4
elif [[ "$PLATFORM" == "macos" ]]; then
	make -j6
else
	make -j$(nproc --all)
fi
