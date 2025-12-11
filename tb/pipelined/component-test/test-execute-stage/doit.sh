#!/bin/sh

# cleanup
rm -rf obj_dir
rm -f *.vcd

# compile ALL required RTL locally
verilator -Wall --trace \
          --cc EXECUTE_STAGE.sv ALU.sv \
          --exe Execute_tb.cpp \
          --top-module EXECUTE_STAGE \
          --prefix "VEXECUTE_STAGE" \
          -CFLAGS "-std=c++17 -I/usr/include" \
          -LDFLAGS "-lgtest -lgtest_main -lpthread"

# build
make -j -C obj_dir -f VEXECUTE_STAGE.mk VEXECUTE_STAGE

# run
./obj_dir/VEXECUTE_STAGE
