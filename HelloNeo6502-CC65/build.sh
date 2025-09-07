#!/bin/bash
#
# build hello.neo
# Uses a special version of none.lib that is built in the nonelib directory.
#
# +------------------------------------------------------------------
# | EDIT TO SET LOCATION OF CC65 EXECUTABLES:
CC65_HOME=~/bin/cc65/bin
# +------------------------------------------------------------------
# | EDIT TO SET LOCATION OF NEO EXECUTABLES:
# +------------------------------------------------------------------
NEO_HOME=~/Prog/Neo/neo6502-firmware/bin
# +------------------------------------------------------------------
# | EDIT TO SET LOCATION OF Neo6502-nonelib
# +------------------------------------------------------------------
NEO_NONELIB=~/Prog/Neo6502-programs/Neo6502_nonelib
# +------------------------------------------------------------------
# | EDIT TO SET LOCATION OF api.lib
# +------------------------------------------------------------------
NEO_APILIB=~/Prog/Neo6502-programs/Neo6502_libs
# +------------------------------------------------------------------
# | EDIT TO SET LOCATION OF Neo6502inc (include files)
# +------------------------------------------------------------------
NEO_INC=~/Prog/Neo6502-programs/Neo6502inc
# +------------------------------------------------------------------
#
$CC65_HOME/cl65 --lib-path $NEO_NONELIB --lib-path $NEO_APILIB -I $NEO_INC --static-locals -t none -C neo6502.cfg -O --cpu 65c02 -o hello.neo hello.c api.lib
#
# Add neo header to program file::
python3 $NEO_HOME/exec.zip hello.neo@800 run@800 -o"hello.neo"
echo 'Copying hello.neo to emulator storage directory...'
cp hello.neo storage/hello.neo

