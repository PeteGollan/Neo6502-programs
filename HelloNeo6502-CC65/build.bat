@echo off
rem +------------------------------------------------------------------
rem | Build hello.neo
rem | Uses a special version of none.lib that is built in the nonelib directory.
rem +------------------------------------------------------------------
rem
rem +------------------------------------------------------------------
rem | EDIT TO SET LOCATION OF CC65 EXECUTABLES:
rem +------------------------------------------------------------------
set CC65_HOME=D:\Users\Pete\Source\Repos\cc65-snapshot-win64
rem +------------------------------------------------------------------
rem | EDIT TO SET LOCATION OF NEO EXECUTABLES:
rem +------------------------------------------------------------------
set NEO_HOME=D:\Users\Pete\Source\Repos\neo6502-firmware
rem +------------------------------------------------------------------
rem | EDIT TO SET LOCATION OF Neo6502-nonelib
rem +------------------------------------------------------------------
set NEO_NONELIB=D:\Users\Pete\Source\Repos\Neo6502-programs\Neo6502_nonelib
rem +------------------------------------------------------------------
rem | EDIT TO SET LOCATION OF api.lib
rem +------------------------------------------------------------------
set NEO_APILIB=D:\Users\Pete\Source\Repos\Neo6502-programs\Neo6502_libs
rem +------------------------------------------------------------------
rem | EDIT TO SET LOCATION OF Neo6502inc (include files)
rem +------------------------------------------------------------------
set NEO_INC=D:\Users\Pete\Source\Repos\Neo6502-programs\Neo6502inc
rem +------------------------------------------------------------------
rem
%CC65_HOME%\bin\cl65 --lib-path %NEO_NONELIB% --lib-path %NEO_APILIB% -I %NEO_INC% --static-locals -t none -C neo6502.cfg -O --cpu 65c02 -o hello.neo hello.c api.lib
rem
rem Add neo header to program file:
python3 %NEO_HOME%\exec.zip hello.neo@800 run@800 -o"hello.neo"
echo Copying hello.neo to emulator storage directory...
copy hello.neo storage\hello.neo
