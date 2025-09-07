@echo off
rem
rem +------------------------------------------------------------------
rem | EDIT TO SET LOCATION OF NEO EXECUTABLES:
rem +------------------------------------------------------------------
set NEO_HOME=D:\Users\Pete\Source\Repos\neo6502-firmware
rem Launch the emulator:
echo Launching...
%NEO_HOME%\neo.exe "Test-CC65-IO.neo" cold
