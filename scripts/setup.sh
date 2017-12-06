#! /bin/sh

sudo apt install libx11-dev libxinerama-dev libxcursor-dev libjemalloc-dev cmake cmake-curses-gui ninja-build

git config --local include.path ../.gitconfig

git submodule update --init --recursive
