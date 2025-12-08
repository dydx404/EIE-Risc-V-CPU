#!/bin/bash
rm -rf obj_dir

verilator -Wall --trace --sv \
    --cc PipelineTop.sv \
    ALU.sv \
    ControlUnit.sv \
    DataCache.sv \
    MainMemory.sv \
    Decode.sv \
    EX_MEM.sv \
    IF_ID.sv \
    ID_EX.sv \
    MEM_WB.sv \
    EXECUTE_STAGE.sv \
    Extend.sv \
    fetch.sv \
    HazardUnit.sv \
    InstrMem.sv \
    MEM_STAGE.sv \
    PCFlat.sv \
    RegFile.sv \
    WB_STAGE.sv \
    --exe PipelineTop_tb.cpp \
    --Wno-WIDTH \
    --Wno-UNOPTFLAT \
    --Wno-UNDRIVEN \
    --Wno-UNUSED \
    --Wno-EOFNEWLINE

make -j -C obj_dir -f VPipelineTop.mk VPipelineTop

./obj_dir/VPipelineTop
