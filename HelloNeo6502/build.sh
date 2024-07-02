#!/bin/bash
#
# build hello.nee and hello.neo
#
# Uses a special version of none.lib that is built in the nonelib directory.
#
# *** EDIT TO SET LOCATION OF CC65 EXECUTABLES:
CC65_HOME=~/bin/cc65/bin
#export CC65_HOME= /home/pete/bin/cc65/bin
# *** EDIT TO SET LOCATION OF NEO EXECUTABLES:
NEO_HOME=~/Prog/Neo/neo6502-firmware-0.33.0/bin
#
$CC65_HOME/cl65 --static-locals -t none -C neo6502.cfg -O --cpu 65c02 -o hello.nee hello.c neo6502apilib.c
#
# Add neo header to *.nee file to make *.neo:
python3 $NEO_HOME/exec.zip hello.nee@800 run@800 -o"hello.neo"

