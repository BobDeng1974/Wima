#! /bin/sh

sudo apt install libx11-dev libjemalloc-dev libglfw3-dev cmake cmake-curses-gui clang ninja-build

git config --local include.path ../.gitconfig

git submodule update --init --recursive
