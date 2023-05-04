#!/bin/sh -eu

path="$(pwd)"
meson setup bin . --wipe --prefix="$path/bin/mini-fps.app" --bindir='Contents/MacOS'
cd bin
meson install
cd ..