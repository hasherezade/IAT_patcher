#!/bin/bash
echo "Trying to autobuild IAT_patcher..."

#QT check
QT_VER=`qmake -v`
QTV="version 5."
if echo "$QT_VER" | grep -q "$QTV"; then
    echo "[+] Qt5 found!"
else
    echo "[-] Qt5 NOT found!"
    echo "Install Qt5 SDK (qt5-default) first"
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

mkdir IAT_patcher
cd IAT_patcher
git clone https://github.com/hasherezade/IAT_patcher.git
echo "[+] IAT_patcher cloned"
cd IAT_patcher
git checkout qt5
cd ..
mv IAT_patcher src
cd src
git clone https://github.com/hasherezade/bearparser.git
echo "[+] bearparser cloned"
cd ..
mkdir build
echo "[+] build directory created"
cd build
cmake -G "Unix Makefiles" ../src/
make
cd ..
cp build/patcher/IAT_Patcher ./
echo "[+] Success! IAT_Patcher is here:"
pwd
