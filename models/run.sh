#!/bin/bash

cppfile="$1.cpp"
command1="g++ $cppfile -o $1 && ./$1"
eval $command1
eval "../viewer/main output.ofv"