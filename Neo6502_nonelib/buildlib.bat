@echo off
rem +------------------------------------------------------------------
rem | Build none.lib for Neo6502 development
rem +------------------------------------------------------------------
rem | The original none.lib is from the lib directory of CC65.
rem | It contains many C functions. We add a few of our own, plus
rem | the critical "C-runtime" crt0, which provides startup and
rem | shutdown code.
rem +------------------------------------------------------------------
rem
rem +------------------------------------------------------------------
rem | EDIT TO SET LOCATION OF CC65 EXECUTABLES:
rem +------------------------------------------------------------------
set CC65_HOME=D:\Users\Pete\Source\Repos\cc65-snapshot-win64
rem
rem +------------------------------------------------------------------
rem |EDIT TO SET LOCATION OF Neo6502inc (include files)
rem +------------------------------------------------------------------
set NeoInc=D:\Users\Pete\Source\Repos\Neo6502-programs\Neo6502inc
rem
set ASMOPTS=-I %NeoInc% -t none --cpu 65C02
rem
rem VIP: copy FRESH version of original none.lib:
echo Retrieving original none.lib from CC65 installation...
rem copy %CC65_HOME%\lib\none.lib .
copy %CC65_HOME%\lib\none.lib .
rem
rem CC65 arciver requires *.o files:
echo Building Neo6502 specific low-level functions...
%CC65_HOME%\bin\ca65 %ASMOPTS% crt0.s
%CC65_HOME%\bin\ca65 %ASMOPTS% close.s
%CC65_HOME%\bin\ca65 %ASMOPTS% lseek.s
%CC65_HOME%\bin\ca65 %ASMOPTS% open.s
%CC65_HOME%\bin\ca65 %ASMOPTS% oserror.s
%CC65_HOME%\bin\ca65 %ASMOPTS% read.s
%CC65_HOME%\bin\ca65 %ASMOPTS% syschdir.s
%CC65_HOME%\bin\ca65 %ASMOPTS% sysmkdir.s
%CC65_HOME%\bin\ca65 %ASMOPTS% sysremove.s
%CC65_HOME%\bin\ca65 %ASMOPTS% sysrename.s
%CC65_HOME%\bin\ca65 %ASMOPTS% sysrmdir.s
%CC65_HOME%\bin\ca65 %ASMOPTS% sysuname.s
%CC65_HOME%\bin\ca65 %ASMOPTS% write.s
%CC65_HOME%\bin\ca65 %ASMOPTS% get_morpheus_vers.s
rem
rem add "common" files (i.e. C runtime) to none.lib (for the "none" target)
rem
echo Adding Neo6502 specific low-level functions to none.lib...
%CC65_HOME%\bin\ar65 r none.lib crt0.o close.o lseek.o open.o oserror.o read.o syschdir.o sysmkdir.o sysremove.o sysrename.o sysrmdir.o sysuname.o write.o get_morpheus_vers.o

