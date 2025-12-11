#!/bin/sh

# cleanup
rm -rf obj_dir
rm -f *.vcd

# compile RTL locally
verilator -Wall --trace \
          --cc MEM_STAGE.sv DataMemory.sv \
          --exe Mem_tb.cpp \
          --top-module MEM_STAGE \
          --prefix "VMEM_STAGE" \
          -CFLAGS "-std=c++17 -I/usr/include" \
          -LDFLAGS "-lgtest -lgtest_main -lpthread"

# build
make -j -C obj_dir -f VMEM_STAGE.mk VMEM_STAGE

# run
./obj_dir/VMEM_STAGE
