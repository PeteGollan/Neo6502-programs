#!/bin/bash
#
# build Test-CC65-IO.neo
#
# Uses a special version of none.lib that is built in the nonelib directory.
#
# -----------------------------------------------------
# EDIT TO SET LOCATION OF CC65 EXECUTABLES:
CC65_HOME=~/bin/cc65/bin
# -----------------------------------------------------
# EDIT TO SET LOCATION OF NEO EXECUTABLES:
NEO_HOME=~/Prog/Neo/neo6502-firmware/bin
# -----------------------------------------------------
# EDIT TO SET LOCATION OF Neo6502-nonelib
NEO_NONELIB=~/Prog/Neo6502-programs/Neo6502_nonelib
# -----------------------------------------------------
# EDIT TO SET LOCATION OF Neo6502inc (include files)
NeoInc=~/Prog/Neo6502-programs/Neo6502inc
# -----------------------------------------------------
# Get the latest Neo6502-specific nonelib:
echo 'Copying Neo6502-specific none.lib...'
cp $NEO_NONELIB/none.lib .

$CC65_HOME/cl65 -I $NeoInc --static-locals -t none -C neo6502.cfg -O --cpu 65c02 -o Test-CC65-IO.nee Test-CC65-IO.c
#
# Add neo header to *.nee file to make *.neo:
python3 $NEO_HOME/exec.zip Test-CC65-IO.nee@800 run@800 -o"Test-CC65-IO.neo"
echo 'Copying hello.neo to emulator storage directory...'
cp Test-CC65-IO.neo storage/Test-CC65-IO.neo

