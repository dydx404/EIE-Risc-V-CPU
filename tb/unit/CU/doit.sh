#!/bin/bash

# cleanup
rm -rf obj_dir
rm -f *.vcd

# Path to RTL
RTL="../../../rtl/single_cycle/control/ControlUnit.sv"

echo "Compiling RTL: $RTL"

verilator  -Wall --trace \
           -cc "$RTL" \
           --top-module ControlUnit \
           --exe ../ControlUnit_tb.cpp \
           --prefix "VControlUnit" \
           -o VControlUnit \
           -CFLAGS "-std=c++17 -isystem /opt/homebrew/Cellar/googletest/1.17.0/include" \
           -LDFLAGS "-L/opt/homebrew/Cellar/googletest/1.17.0/lib -lgtest -lgtest_main -lpthread"

# Build using auto-generated makefile
make -j -C obj_dir -f VControlUnit.mk

# Run test
./obj_dir/VControlUnit
