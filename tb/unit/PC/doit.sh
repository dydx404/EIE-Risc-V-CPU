#!/bin/bash

rm -rf obj_dir
rm -f *.vcd

verilator -Wall --trace \
    -cc PC.sv \
    --exe pc_tb.cpp \
    --prefix "VPC" \
    -o VPC \
    -CFLAGS "-std=c++17 -isystem /opt/homebrew/include" \
    -LDFLAGS "-L/opt/homebrew/lib -lgtest -lgtest_main -lpthread"

make -j -C obj_dir -f VPC.mk

./obj_dir/VPC
