#! /bin/sh

dir=$(cd `dirname $0` && pwd)
build="../build"

cd "$dir"

mkdir "$build"
cd "$build"

cmake -GNinja -DCMAKE_C_COMPILER=clang ".."
ninja
