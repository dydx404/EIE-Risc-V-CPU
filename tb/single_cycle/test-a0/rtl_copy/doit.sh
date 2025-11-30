#!/bin/bash
rm -rf obj_dir
rm -f top.vcd


verilator -Wall --trace \
    --cc top.sv \
    ALU.sv \
    ControlUnit.sv \
    DataMemory.sv \
    Extend.sv \
    InstrMem.sv \
    PCFlat.sv \
    RegFile.sv \
    --exe top_tb.cpp \
    --Wno-WIDTH \
    --Wno-UNOPTFLAT \
    --Wno-UNDRIVEN \
    --Wno-UNUSED \
    --Wno-EOFNEWLINE \
    --Wno-CASEINCOMPLETE \

make -j -C obj_dir -f Vtop.mk Vtop

./obj_dir/Vtop
