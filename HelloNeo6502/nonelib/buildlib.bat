@echo off
rem build none.lib for Neo6502 development
rem the original none.lib is from the libsrc directory of CC65.
rem
rem *** EDIT TO SET LOCATION OF CC65 EXECUTABLES:
set CC65_HOME=D:\Users\Pete\Prog\cc65\bin
rem
rem
rem CC65 arciver requires *.o files:
%CC65_HOME%\ca65 crt0.s
%CC65_HOME%\ca65 write.s
%CC65_HOME%\ca65 read.s
rem
rem add "common" files (i.e. C runtime) to none.lib (for the "none" target)
rem
%CC65_HOME%\ar65 r none.lib crt0.o write.o read.o
rem
rem copy the updated library to the main directory:
copy none.lib ..\

