#!/bin/bash

cd "`dirname $BASH_SOURCE`"
./autogen.sh
./configure --enable-editor
make

# alias make="make -j2" in your ~/.bashrc for more speed
