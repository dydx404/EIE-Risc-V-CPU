#!/bin/bash

# ----------------------------
# Clean
# ----------------------------
rm -rf obj_dir
rm -f *.vcd

# ----------------------------
# Top module
# ----------------------------
TOP=PipelineTop

# ----------------------------
# RTL file list (explicit)
# ----------------------------
RTL_FILES="
PipelineTop.sv
ALU.sv
ControlUnit.sv
DataMemory.sv
Decode.sv
EX_MEM.sv
EXECUTE_STAGE.sv
Extend.sv
fetch.sv
HazardUnit.sv
ID_EX.sv
IF_ID.sv
InstrMem.sv
MEM_STAGE.sv
MEM_WB.sv
PCFlat.sv
RegFile.sv
WB_STAGE.sv
"

echo "Compiling PipelineTop with all RTL dependencies..."

# ----------------------------
# Verilator compile
# ----------------------------
verilator -Wall --trace \
    -cc $RTL_FILES \
    --top-module PipelineTop \
    --exe PipelineTop_tb.cpp \
    -o VPipelineTop

# ----------------------------
# Build
# ----------------------------
make -j -C obj_dir -f VPipelineTop.mk VPipelineTop

# ----------------------------
# Run
# ----------------------------
./obj_dir/VPipelineTop

