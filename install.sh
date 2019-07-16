#!/bin/bash
git clone https://github.com/hdelanoe/graphic_lib.git
mv graphic_lib/* ./
rm -rf graphic_lib
export LD_LIBRARY_PATH=LD_LIBRARY_PATH:lib/linux
