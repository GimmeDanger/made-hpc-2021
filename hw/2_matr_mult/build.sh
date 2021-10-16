#!/bin/bash
mkdir -p build
cp Makefile build/
cd build
make
cp mult_bench ../
