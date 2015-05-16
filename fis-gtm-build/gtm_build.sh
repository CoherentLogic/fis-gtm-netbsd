#!/usr/bin/env bash

export LC_CTYPE=$(locale -a | gawk 'BEGIN{IGNORECASE=1}/en_us.utf-*8/{print;exit}')
export LANG=C LC_ALL= LC_COLLATE=C

echo "Build Environment"
echo "-----------------"
echo "LC_CTYPE=${LC_CTYPE} LANG=${LANG} LC_ALL=${LC_ALL} LC_COLLATE=${LC_COLLATE}"
echo ""
echo "cmake -D CMAKE_INSTALL_PREFIX:PATH=${PWD}/package ../"
echo "* * * * * * * *"

cmake -D CMAKE_BUILD_TYPE=DEBUG -D CMAKE_INSTALL_PREFIX:PATH=${PWD}/package ../
echo "make; sudo make install"
echo "* * * * * * * *"

make; sudo make install


