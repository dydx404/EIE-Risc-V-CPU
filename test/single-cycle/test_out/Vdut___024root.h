// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vdut.h for the primary calling header

#ifndef VERILATED_VDUT___024ROOT_H_
#define VERILATED_VDUT___024ROOT_H_  // guard

#include "verilated.h"

class Vdut__Syms;

class Vdut___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst,0,0);
    CData/*0:0*/ top__DOT__RegWrite;
    CData/*3:0*/ top__DOT__ALUControl;
    CData/*0:0*/ top__DOT__ALUSrc;
    CData/*0:0*/ top__DOT__MemWrite;
    CData/*1:0*/ top__DOT__PCSrc;
    CData/*1:0*/ top__DOT__ResultSrc;
    CData/*2:0*/ top__DOT__ImmSrc;
    CData/*2:0*/ top__DOT__AddressingControl;
    CData/*7:0*/ top__DOT__datamem__DOT__unnamedblk2__DOT__b;
    CData/*7:0*/ top__DOT__datamem__DOT__unnamedblk3__DOT__lo;
    CData/*7:0*/ top__DOT__datamem__DOT__unnamedblk3__DOT__hi;
    CData/*0:0*/ __Vclklast__TOP__clk;
    CData/*0:0*/ __Vclklast__TOP__rst;
    SData/*15:0*/ top__DOT__datamem__DOT__unnamedblk3__DOT__half;
    VL_OUT(a0,31,0);
    IData/*31:0*/ top__DOT__pc;
    IData/*31:0*/ top__DOT__instr;
    IData/*31:0*/ top__DOT__immext;
    IData/*31:0*/ top__DOT__rd1;
    IData/*31:0*/ top__DOT__rd2;
    IData/*31:0*/ top__DOT__result;
    IData/*31:0*/ top__DOT__wd_safe;
    IData/*31:0*/ top__DOT__srcb;
    IData/*31:0*/ top__DOT__alu_out;
    IData/*31:0*/ top__DOT__read_data;
    IData/*31:0*/ top__DOT__cycle;
    IData/*31:0*/ top__DOT__datamem__DOT__unnamedblk1__DOT__file;
    IData/*31:0*/ top__DOT__pc_unit__DOT__pc_next;
    VlUnpacked<IData/*31:0*/, 1024> top__DOT__imem__DOT__memory;
    VlUnpacked<IData/*31:0*/, 32> top__DOT__regfile__DOT__reg_file;
    VlUnpacked<CData/*7:0*/, 131072> top__DOT__datamem__DOT__ram_array;
    VlUnpacked<CData/*0:0*/, 3> __Vm_traceActivity;
    std::string top__DOT__datamem__DOT__unnamedblk1__DOT__path;

    // INTERNAL VARIABLES
    Vdut__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vdut___024root(Vdut__Syms* symsp, const char* name);
    ~Vdut___024root();
    VL_UNCOPYABLE(Vdut___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);


#endif  // guard
