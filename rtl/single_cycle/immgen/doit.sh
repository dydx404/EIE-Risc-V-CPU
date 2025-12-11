#!/bin/bash

# Always evaluate paths from this directory:
/usr/bin/env bash
cd "$(dirname "$0")"

verilator -Wall --cc ../../../rtl/single_cycle/extend/Extend.sv \
    --exe Extend_tb.cpp \
    -I../../../rtl/single_cycle/extend \
    -o Extend_tb

make -C obj_dir -f VExtend.mk || exit 1

./obj_dir/Extend_tb
