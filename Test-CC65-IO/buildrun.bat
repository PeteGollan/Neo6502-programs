@echo off
rem
echo Building...
call build.bat
rem Launch the emulator:
echo Launching...
%NEO_HOME%\neo.exe "Test-CC65-IO.neo" cold

