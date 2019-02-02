#!/bin/bash

eval "./beforeCompiling $1"
eval "make"
eval "./main"