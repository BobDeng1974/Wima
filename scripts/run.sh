#! /bin/sh

dir=$(cd `dirname $0` && pwd)
build="../build"

cd "$dir"
cd "$build"

src/tests/app
