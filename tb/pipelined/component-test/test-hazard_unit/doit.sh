#!/bin/bash

rm -rf obj_dir
rm -f *.vcd

RTL="HazardUnit.sv"

verilator -Wall --cc --trace "$RTL" \
          --exe HazardUnit_tb.cpp \
          --top-module HazardUnit \
          --prefix "VHazardUnit" \
          -CFLAGS "-std=c++17 -isystem /opt/homebrew/Cellar/googletest/1.17.0/include" \
          -LDFLAGS "-L/opt/homebrew/Cellar/googletest/1.17.0/lib -lgtest -lgtest_main -lpthread"

make -j -C obj_dir -f VHazardUnit.mk

./obj_dir/VHazardUnit
