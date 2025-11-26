#!/bin/bash
set -e

rm -rf obj_dir
rm -f *.vcd

verilator -Wall --trace \
    -cc Extend.sv \
    --exe Extend_tb.cpp \
    --prefix "VExtend" \
    -o VExtend \
    -CFLAGS "-std=c++17" \
    -LDFLAGS "-lgtest -lgtest_main -pthread"

make -j -C obj_dir -f VExtend.mk

./obj_dir/VExtend
