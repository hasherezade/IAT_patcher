#!/bin/bash
echo "Trying to autobuild IAT_patcher..."

#QT check
QTV="version 4."
if echo `qmake-qt4 -version` | grep -q "$QTV"; then
    echo "[+] Qt4 found!"
elseif echo `qmake -v` | grep -q "$QTV"; then
    echo "[+] Qt4 found!"
elseif
    echo "[-] Qt4 NOT found!"
    echo "Install qt-sdk first"
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
git checkout qt4
echo "[+] IAT_patcher cloned"
mv IAT_patcher src
cd src
git clone https://github.com/hasherezade/bearparser.git
echo "[+] bearparser cloned"
cd bearparser
git checkout qt4
cd ..
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
