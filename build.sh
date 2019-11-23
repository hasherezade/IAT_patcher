#!/bin/bash
echo "Trying to autobuild IAT_patcher..."

#QT check
QT_VER=`qmake -v`
QTV="version"
if echo "$QT_VER" | grep -q "$QTV"; then
    QT_FOUND=`whereis qt5`
    if echo "$QT_FOUND" | grep -q "lib"; then
        echo "[+] Qt5 found!"
    else
        echo "Install Qt5 SDK first"
        exit -2
    fi
else
    echo "Install Qt5 SDK first"
    exit -1
fi

CMAKE_VER=`cmake --version`
CMAKEV="cmake version"
if echo "$CMAKE_VER" | grep -q "$CMAKEV"; then
    echo "[+] CMake found!"
else
    echo "[-] CMake NOT found!"
    echo "Install cmake first"
    exit -1
fi

mkdir build
echo "[+] build directory created"
cd build
mkdir $(pwd)/out
cmake -DCMAKE_INSTALL_PREFIX:PATH=$(pwd)/out ..
cmake --build . --target install

