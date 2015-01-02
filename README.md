IAT patcher
==========
Persistent IAT hooking application.<br/>

WARNING: this is an early beta version, some elements are unfinished!<br/>
Please report any bugs and remarks to: hasherezade@op.pl<br/>

Requires:
+ bearparser: https://github.com/hasherezade/bearparser<br/>
+ Qt4 Core<br/>
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
<pre>
cmake -G [enviroment of your choice] ../IAT_patcher/
i. e
cmake -G "CodeBlocks - Unix Makefiles" ../IAT_patcher/
make
</pre>
Read more:
===
http://hasherezade.net/IAT_patcher/
