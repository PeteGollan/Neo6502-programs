@echo off
rem build hello.nee and hello.neo
rem
rem Uses a special version of none.lib that is built in the nonelib directory.
rem
rem *** EDIT TO SET LOCATION OF CC65 EXECUTABLES:
set CC65_HOME=D:\Users\Pete\Prog\cc65\bin
rem *** EDIT TO SET LOCATION OF NEO EXECUTABLES:
set NEO_HOME=D:\Users\Pete\Prog\Neo6502v0.33.0
rem
%CC65_HOME%\cl65 --static-locals -t none -C neo6502.cfg -O --cpu 65c02 -o hello.nee hello.c Neo6502APILib.c
rem
rem Add neo header to *.nee file to make *.neo:
python3 %NEO_HOME%\exec.zip hello.nee@800 run@800 -o"hello.neo"

