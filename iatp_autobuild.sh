#!/bin/bash
echo "Trying to autobuild IAT_patcher..."
git clone --recursive https://github.com/hasherezade/IAT_patcher.git
echo "[+] IAT_patcher cloned"
echo $$
cd IAT_patcher
sh build.sh

