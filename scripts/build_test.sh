#!/bin/bash

export BUILD_DIR=$PWD/build
export SRC_DIR=$PWD/src

function prepare_env()
{
    rm -rf $BUILD_DIR
    mkdir -p $BUILD_DIR
}

function compile()
{
    cd $BUILD_DIR
    cmake $SRC_DIR \
        -DCMAKE_CXX_COMPILER=clang++ \
        -G Ninja
    cmake --build $BUILD_DIR
}

function main()
{
    prepare_env
    compile
}

main
