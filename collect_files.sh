#!/bin/bash

SRC="$1"
DST="$2"
DEPTH=""

if [ "$3" = "--max_depth" ]; then
  DEPTH="$4"
elif echo "$3" | grep -q "^--max_depth="; then
  DEPTH=`echo $3 | cut -d'=' -f2`
fi

which g++ >/dev/null
if [ $? -ne 0 ]; then
  sudo apt-get update
  sudo apt-get install g++
fi

g++ -std=c++17 -o run_collector collector_core.cpp

if [ -n "$DEPTH" ]; then
  ./run_collector $SRC "$DST" "$DEPTH"
else
  ./run_collector "$SRC" "$DST"
fi