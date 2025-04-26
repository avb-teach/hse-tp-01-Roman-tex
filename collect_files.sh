#!/bin/bash

SRC="$1"
DST="$2"
DEPTH=""

if echo $3 | grep -q "^--max_depth="; then
  DEPTH=`echo $3 | cut -d'=' -f2`
fi

which g++ >/dev/null
if [ $? -ne 0 ]; then
  sudo apt-get update
  sudo apt-get install g++
fi

g++ -std=c++17 -o run_collector collector_core.cpp

./run_collector $SRC $DST $DEPTH