IAT patcher 
==========
[![Build status](https://ci.appveyor.com/api/projects/status/dv42sbge09b3i77h?svg=true)](https://ci.appveyor.com/project/hasherezade/iat-patcher)
[![Build status](https://travis-ci.org/hasherezade/IAT_patcher.svg?branch=master)](https://travis-ci.org/hasherezade/IAT_patcher)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/hasherezade/IAT_patcher.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/hasherezade/IAT_patcher/alerts/)

Persistent IAT hooking application.<br/>
Read more: http://hasherezade.github.io/IAT_patcher/<br/>

Please report any bugs and remarks to: hasherezade@gmail.com<br/>

Requires:
+ bearparser: https://github.com/hasherezade/bearparser<br/>
+ Qt5 SDK<br/>
+ Qt5 Core<br/>
+ Qt5 GUI<br/>
+ cmake http://www.cmake.org/<br/>

Autobuild:
-

To build it on Linux or MacOS you can use the given script - it automatically downloads this repository and all the dependencies:<br/>
https://raw.githubusercontent.com/hasherezade/IAT_patcher/master/iatp_autobuild.sh<br/>
Just run it and it will do everything for you!

Manual build:
-

To have more control on the process of building, you can also build IAT Patcher manualy, by following several simple steps:<br/>

<b>1. Fetch sources:</b><br/>

Use recursive clone to get the repo together with the submodule:
<pre>
git clone --recursive https://github.com/hasherezade/IAT_patcher.git
</pre>

<b>2. Build (Linux example):</b><br/>

_NOTE: The same source should compile on Windows without problems, only generator (cmake -G ...) will be different_<br/><br/>
After you clone the repository with sources, create a new directory for the build:
<pre>
mkdir build
cd build
cmake -G [enviroment of your choice] ../IAT_patcher/
i. e
cmake -G "CodeBlocks - Unix Makefiles" ../IAT_patcher/
make
</pre>
...the application is here:
<pre>
../build/patcher/IAT_Patcher
</pre>
