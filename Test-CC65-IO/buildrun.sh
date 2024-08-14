#!/bin/bash
#
# build and run Test-CC65-IO.neo
#
source ./build.sh

$NEO_HOME/neo "Test-CC65-IO.neo" cold
