#!/bin/bash
#
# build and run hello.neo
#
source ./build.sh

$NEO_HOME/neo "hello.neo" cold
