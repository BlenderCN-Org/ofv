#!/bin/bash

cppfile="$1.cpp"
eval "cd ../rtViewer"
eval "./beforeCompiling ../models/$cppfile"
eval "make"
eval "./main"
eval "cd ../models"