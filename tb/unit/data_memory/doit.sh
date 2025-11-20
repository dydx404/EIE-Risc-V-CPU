#!/bin/bash

# cleanup
rm -rf obj_dir
rm -f *.vcd

# Verilate DataMemory subsystem (standalone DUT)
verilator -Wall --trace \
    -cc DataMemory.sv \
    --exe data_memory_tb.cpp \
    --prefix "VDataMemory" \
    -o VDataMemory_basic \
    -CFLAGS "-std=c++17 -isystem /opt/homebrew/include" \
    -LDFLAGS "-L/opt/homebrew/lib -lgtest -lgtest_main -lpthread"

# build
make -j -C obj_dir -f VDataMemory.mk

# run executable
./obj_dir/VDataMemory_basic
