#!/bin/bash
# build none.lib for Neo6502 development
# the original none.lib is from the libsrc directory of CC65.
#
# *** EDIT TO SET LOCATION OF CC65 EXECUTABLES:
CC65_HOME=~/bin/cc65/bin
#
#
# CC65 arciver requires *.o files:
$CC65_HOME/ca65 crt0.s
$CC65_HOME/ca65 write.s
$CC65_HOME/ca65 read.s
#
# add "common" files (i.e. C runtime) to none.lib (for the "none" target)
#
$CC65_HOME/ar65 r none.lib crt0.o write.o read.o
#
# copy the updated library to the main directory:
cp none.lib ../

