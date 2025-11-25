#!/bin/bash
set -e

rm -rf obj_dir
rm -f *.vcd

verilator -Wall --trace -Wno-UNUSED \
    -cc ../../../rtl/single_cycle/control/ControlUnit.sv \
    --exe ControlUnit_tb.cpp \
    --prefix "VControlUnit" \
    -o VControlUnit \
    -CFLAGS "-std=c++17" \
    -LDFLAGS "-lgtest -lgtest_main -pthread"

make -j -C obj_dir -f VControlUnit.mk

./obj_dir/VControlUnit
