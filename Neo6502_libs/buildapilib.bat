@echo off
rem buildapilib.bat
rem build api.lib for Neo6502 development
rem
rem +------------------------------------------------------------------
rem | EDIT TO SET LOCATION OF CC65 EXECUTABLES:
rem +------------------------------------------------------------------
set CC65_HOME=D:\Users\Pete\Source\Repos\cc65-snapshot-win64
rem +------------------------------------------------------------------
rem | EDIT TO SET LOCATION OF Neo6502inc (include files)
rem +------------------------------------------------------------------
set NEO_INC=D:\Users\Pete\Source\Repos\Neo6502-programs\Neo6502inc
rem
set ASMOPTS=-I %NEO_INC% -t none --cpu 65C02
rem
rem CC65 archiver requires *.o files:
%CC65_HOME%\bin\ca65 %ASMOPTS% api\apiDelay10ms.s
%CC65_HOME%\bin\ca65 %ASMOPTS% api\apiGetInkPaperColour.s
%CC65_HOME%\bin\ca65 %ASMOPTS% api\apiSetInkPaperColour.s
%CC65_HOME%\bin\ca65 %ASMOPTS% api\apiGetSysTimer16.s
%CC65_HOME%\bin\ca65 %ASMOPTS% api\apiHideCursor.s
%CC65_HOME%\bin\ca65 %ASMOPTS% api\apiKeyWait.s
%CC65_HOME%\bin\ca65 %ASMOPTS% api\apiRand16.s
%CC65_HOME%\bin\ca65 %ASMOPTS% api\apiShowCursor.s
%CC65_HOME%\bin\ca65 %ASMOPTS% api\apiSetCursorPos.s
%CC65_HOME%\bin\ca65 %ASMOPTS% api\apiPlaySoundEffect.s
copy api\*.o . 1>nul
rem
%CC65_HOME%\bin\ar65 r api.lib apiDelay10ms.o apiGetInkPaperColour.o apiSetInkPaperColour.o apiGetSysTimer16.o apiHideCursor.o apiKeyWait.o apiRand16.o apiShowCursor.o apiSetCursorPos.o apiPlaySoundEffect.o
del *.o
