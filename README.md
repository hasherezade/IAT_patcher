IAT patcher 
==========

[![Build status](https://ci.appveyor.com/api/projects/status/dv42sbge09b3i77h?svg=true)](https://ci.appveyor.com/project/hasherezade/iat-patcher)
[![Build status](https://travis-ci.org/hasherezade/IAT_patcher.svg?branch=master)](https://travis-ci.org/hasherezade/IAT_patcher)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/e5a1d1892c2642faba08d678c0a6fbf6)](https://www.codacy.com/manual/hasherezade/IAT_patcher?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=hasherezade/IAT_patcher&amp;utm_campaign=Badge_Grade)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/hasherezade/IAT_patcher.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/hasherezade/IAT_patcher/alerts/)
[![GitHub release](https://img.shields.io/github/release/hasherezade/IAT_patcher.svg)](https://github.com/hasherezade/IAT_patcher/releases)
[![Github All Releases](https://img.shields.io/github/downloads/hasherezade/IAT_patcher/total.svg)](https://github.com/hasherezade/IAT_patcher/releases)

**Persistent IAT hooking application (for PE files).**

📖 Read more: http://hasherezade.github.io/IAT_patcher/

Please report any bugs and remarks to: [issues](https://github.com/hasherezade/IAT_patcher/issues).

Requires:
+ bearparser: https://github.com/hasherezade/bearparser
+ Qt5 SDK
+ Qt5 Core
+ Qt5 GUI
+ cmake http://www.cmake.org

## Clone

Use recursive clone to get the repo together with the submodule:

```
git clone --recursive https://github.com/hasherezade/IAT_patcher.git
```

## Autobuild

To build it on Linux or MacOS you can use the given script - it automatically downloads this repository and all the dependencies:<br/>
[iatp_autobuild.sh](https://raw.githubusercontent.com/hasherezade/IAT_patcher/master/iatp_autobuild.sh)<br/>
Just run it, and it will automatically download and build everything.

## Download builds

You can download the latest stable Windows builds from the [releases](https://github.com/hasherezade/IAT_patcher/releases)

## Sample DLLs

Sample DLLs to be used with IAT Patcher can be found in [IAT_patcher_samples](https://github.com/hasherezade/IAT_patcher_samples)
