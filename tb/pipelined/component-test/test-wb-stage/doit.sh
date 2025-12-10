#!/bin/sh

# cleanup
rm -rf obj_dir
rm -f *.vcd

# compile WB stage locally
verilator -Wall --trace \
          --cc WB_STAGE.sv \
          --exe WB_tb.cpp \
          --top-module WB_STAGE \
          --prefix "VWB_STAGE" \
          -CFLAGS "-std=c++17 -I/usr/include" \
          -LDFLAGS "-lgtest -lgtest_main -lpthread"

# build
make -j -C obj_dir -f VWB_STAGE.mk VWB_STAGE

# run
./obj_dir/VWB_STAGE
