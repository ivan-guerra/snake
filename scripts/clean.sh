#!/bin/bash

source config.sh

# Remove the binary directory.
if [ -d $SNAKE_BIN_DIR ]
then
    echo "removing '$SNAKE_BIN_DIR'"
    rm -r $SNAKE_BIN_DIR
fi

# Remove the CMake build directory.
if [ -d $SNAKE_BUILD_DIR ]
then
    echo "removing '$SNAKE_BUILD_DIR'"
    rm -r $SNAKE_BUILD_DIR
fi
