#!/bin/bash

rm -rf obj_dir
rm -f *.vcd

verilator -Wall --trace \
    -cc PCFlat.sv \
    --exe pc_tb.cpp \
    --prefix "VPCFlat" \
    -o VPCFlat \
    -CFLAGS "-std=c++17 -isystem /opt/homebrew/include" \
    -LDFLAGS "-L/opt/homebrew/lib -lgtest -lgtest_main -lpthread"

make -j -C obj_dir -f VPCFlat.mk

./obj_dir/VPCFlat
