#!/bin/bash

CWD=$(pwd)

# Root directory.
SNAKE_PROJECT_PATH=$(dirname ${CWD})

# Scripts directory.
SNAKE_SCRIPTS_PATH="${SNAKE_PROJECT_PATH}/scripts"

# Binary directory.
SNAKE_BIN_DIR="${SNAKE_PROJECT_PATH}/bin"

# CMake build files and cache.
SNAKE_BUILD_DIR="${SNAKE_PROJECT_PATH}/build"
