#!/bin/bash
#
# build Neo6502-specific version of none.lib 
# To use none.lib, copy it to your project directory.
#
# -----------------------------------------------------
# EDIT TO SET LOCATION OF CC65 DIRECTORY:
CC65_HOME=~/bin/cc65
# -----------------------------------------------------
# EDIT TO SET LOCATION OF Neo6502inc (include files)
NEOINC=~/Prog/Neo6502-programs/Neo6502inc
# -----------------------------------------------------
# 
ASMOPTS="-I $NEOINC -t none --cpu 65c02"
#VIP: copy FRESH version of original none.lib:
echo 'Retrieving original none.lib...'
cp $CC65_HOME/lib/none.lib .
#
# CC65 arciver requires *.o files:
echo 'Building Neo6502 specific low-level functions...'
$CC65_HOME/bin/ca65 $ASMOPTS crt0.s
$CC65_HOME/bin/ca65 $ASMOPTS close.s
$CC65_HOME/bin/ca65 $ASMOPTS lseek.s
$CC65_HOME/bin/ca65 $ASMOPTS open.s
$CC65_HOME/bin/ca65 $ASMOPTS oserror.s
$CC65_HOME/bin/ca65 $ASMOPTS read.s
$CC65_HOME/bin/ca65 $ASMOPTS syschdir.s
$CC65_HOME/bin/ca65 $ASMOPTS sysmkdir.s
$CC65_HOME/bin/ca65 $ASMOPTS sysremove.s
$CC65_HOME/bin/ca65 $ASMOPTS sysrename.s
$CC65_HOME/bin/ca65 $ASMOPTS sysrmdir.s
$CC65_HOME/bin/ca65 $ASMOPTS sysuname.s
$CC65_HOME/bin/ca65 $ASMOPTS write.s
$CC65_HOME/bin/ca65 $ASMOPTS get_morpheus_vers.s
#
# add "common" files (i.e. C runtime) to none.lib (for the "none" target)
#
echo 'Adding Neo6502 specific low-level functions to none.lib...'
$CC65_HOME/bin/ar65 r none.lib crt0.o close.o lseek.o open.o oserror.o read.o syschdir.o sysmkdir.o sysremove.o sysrename.o sysrmdir.o sysuname.o write.o get_morpheus_vers.o
#
echo 'Remember to copy this custom none.lib to your CC65 project directory'
echo 'if it is not done automatically.'



