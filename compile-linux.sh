#!/bin/bash

# Run the cmake command
cmake -B build/linux -DCMAKE_TOOLCHAIN_FILE=./TC-linux.cmake

# Navigate to the build directory
cd build/linux || { echo "Failed to change directory to build/linux"; exit 1; }

# Rename the existing link.txt file
if [ -f CMakeFiles/Game.dir/link.txt ]; then
    mv CMakeFiles/Game.dir/link.txt CMakeFiles/Game.dir/link_old.txt
else
    echo "link.txt does not exist, skipping rename."
fi

# Copy the link.txt from the root directory
if [ -f ../../link.txt ]; then
    cp ../../link.txt CMakeFiles/Game.dir/link.txt
else
    echo "link.txt in the root directory does not exist, skipping copy."
fi

make run