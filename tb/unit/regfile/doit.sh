#!/bin/sh

# cleanup
rm -rf obj_dir
rm -f regfile.vcd

# build with Verilator
verilator -Wall --cc --trace \
  ../../../rtl/single_cycle/regfile/RegFile.sv \
  --top-module RegFile \
  --exe regfile_tb.cpp

make -j -C obj_dir -f VRegFile.mk VRegFile

# run
obj_dir/VRegFile
