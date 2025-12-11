#!/bin/bash
set -e

# Clean previous build
rm -rf obj_dir

# Attach Vbuddy (Virtual Buddy) USB (Universal Serial Bus) device under WSL (Windows Subsystem for Linux).
# Comment this line out if you are on macOS (Mac Operating System) or have already attached the device manually.
~/Documents/iac/lab0-devtools/tools/attach_usb.sh

# Run Verilator (Verilog Simulator) to generate C++ model
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

# Build the executable
make -j -C obj_dir -f Vtop.mk Vtop

# Run the simulation
./obj_dir/Vtop
