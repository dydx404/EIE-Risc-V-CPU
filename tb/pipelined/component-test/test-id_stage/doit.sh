#!/bin/sh

# cleanup
rm -rf obj_dir
rm -f *.vcd

# run Verilator to translate SystemVerilog into C++, including GTest testbench
verilator -Wall --trace \
          --cc Decode.sv \
          --exe Decode_tb.cpp \
          --top-module Decode \
          --prefix "VDecode" \
          -CFLAGS "-std=c++17 -isystem /opt/homebrew/Cellar/googletest/1.17.0/include" \
          -LDFLAGS "-L/opt/homebrew/Cellar/googletest/1.17.0/lib -lgtest -lgtest_main -lpthread"

# build C++ project via auto-generated makefile
make -j -C obj_dir -f VDecode.mk VDecode

# run executable simulation file
./obj_dir/VDecode
