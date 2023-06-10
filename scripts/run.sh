#!/bin/bash

source config.sh

pushd $SNAKE_BIN_DIR > /dev/null
    ./snake
popd > /dev/null
