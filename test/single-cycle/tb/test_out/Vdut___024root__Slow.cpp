// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdut.h for the primary calling header

#include "verilated.h"

#include "Vdut__Syms.h"
#include "Vdut___024root.h"

void Vdut___024root___ctor_var_reset(Vdut___024root* vlSelf);

Vdut___024root::Vdut___024root(Vdut__Syms* symsp, const char* name)
    : VerilatedModule{name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    Vdut___024root___ctor_var_reset(this);
}

void Vdut___024root::__Vconfigure(bool first) {
    if (false && first) {}  // Prevent unused
}

Vdut___024root::~Vdut___024root() {
}
