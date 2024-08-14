@echo off
rem build hello.neo
rem
rem Uses a special version of none.lib that is built in the nonelib directory.
rem
rem Uses a special version of none.lib that is built in the nonelib directory.
rem
rem --------------------------------------------------
rem EDIT TO SET LOCATION OF CC65 EXECUTABLES:
set CC65_HOME=D:\Users\Pete\Prog\cc65\bin
rem --------------------------------------------------
rem EDIT TO SET LOCATION OF NEO EXECUTABLES:
set NEO_HOME=D:\Users\Pete\Source\Repos\neo6502-firmware
rem --------------------------------------------------
rem EDIT TO SET LOCATION OF Neo6502-nonelib
set NEO_NONELIB=D:\Users\Pete\Source\Repos\Neo6502-programs\Neo6502_nonelib
rem --------------------------------------------------
rem EDIT TO SET LOCATION OF Neo6502inc (include files)
set NeoInc=D:\Users\Pete\Source\Repos\Neo6502-programs\Neo6502inc
rem --------------------------------------------------
rem
rem
rem Get the latest Neo6502-specific nonelib:
echo Copying Neo6502-specific none.lib...
copy %NEO_NONELIB%\none.lib .
rem
%CC65_HOME%\cl65 -I %NeoInc% --static-locals -t none -C neo6502.cfg -O --cpu 65c02 -o hello.nee hello.c Neo6502APILib.c
rem
rem Add neo header to *.nee file to make *.neo:
python3 %NEO_HOME%\exec.zip hello.nee@800 run@800 -o"hello.neo"
echo Copying hello.neo to emulator storage directory...
copy hello.neo storage\hello.neo

