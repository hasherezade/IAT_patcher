IAT patcher 
==========
[![Build status](https://ci.appveyor.com/api/projects/status/dv42sbge09b3i77h?svg=true)](https://ci.appveyor.com/project/hasherezade/iat-patcher)
[![Build status](https://travis-ci.org/hasherezade/IAT_patcher.svg?branch=master)](https://travis-ci.org/hasherezade/IAT_patcher)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/e5a1d1892c2642faba08d678c0a6fbf6)](https://www.codacy.com/manual/hasherezade/IAT_patcher?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=hasherezade/IAT_patcher&amp;utm_campaign=Badge_Grade)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/hasherezade/IAT_patcher.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/hasherezade/IAT_patcher/alerts/)

Persistent IAT hooking application.<br/>
Read more: http://hasherezade.github.io/IAT_patcher/<br/>

Please report any bugs and remarks to: [issues](https://github.com/hasherezade/IAT_patcher/issues).

Requires:
+ bearparser: https://github.com/hasherezade/bearparser<br/>
+ Qt5 SDK<br/>
+ Qt5 Core<br/>
+ Qt5 GUI<br/>
+ cmake http://www.cmake.org/<br/>

Clone
-
Use recursive clone to get the repo together with the submodule:
<pre>
git clone --recursive https://github.com/hasherezade/IAT_patcher.git
</pre>

Download
-
You can download the latest build from [releases](https://github.com/hasherezade/IAT_patcher/releases)

Sample DLLs
-
Sample DLLs to be used with IAT Patcher can be found in [IAT_patcher_samples](https://github.com/hasherezade/IAT_patcher_samples)

Autobuild
-

To build it on Linux or MacOS you can use the given script - it automatically downloads this repository and all the dependencies:<br/>
[iatp_autobuild.sh](https://raw.githubusercontent.com/hasherezade/IAT_patcher/master/iatp_autobuild.sh)<br/>
Just run it, and it will automatically download and build everything.
