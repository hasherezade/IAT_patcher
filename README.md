IAT patcher 
==========
Persistent IAT hooking application.<br/>
<b>
Site: http://hasherezade.net/IAT_patcher
</b>

Please report any bugs and remarks to: hasherezade@op.pl<br/>

Requires:
+ bearparser: https://github.com/hasherezade/bearparser<br/>
+ Qt SDK (qt-sdk)<br/>
+ Qt4 Core (libqt4-core)<br/>
+ Qt4 GUI (libqt4-gui)<br/>
+ cmake http://www.cmake.org/<br/>

<b>Before build - download bearparser source into bearparser directory</b><br/>
From main directory:
<pre>git clone https://github.com/hasherezade/bearparser.git</pre>

How to fetch complete source:
===
<pre>
git clone https://github.com/hasherezade/IAT_patcher.git
cd IAT_patcher
git clone https://github.com/hasherezade/bearparser.git
</pre>

How to build (Linux example):
===
If you are in directory IAT_patcher, go up:
<pre>
cd ../
</pre>
and then:
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

