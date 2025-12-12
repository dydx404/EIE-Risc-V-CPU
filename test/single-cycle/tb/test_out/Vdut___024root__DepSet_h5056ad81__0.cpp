// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdut.h for the primary calling header

#include "verilated.h"

#include "Vdut___024root.h"

VL_INLINE_OPT void Vdut___024root___sequent__TOP__0(Vdut___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root___sequent__TOP__0\n"); );
    // Init
    IData/*31:0*/ __Vdly__top__DOT__cycle;
    CData/*4:0*/ __Vdlyvdim0__top__DOT__regfile__DOT__reg_file__v0;
    IData/*31:0*/ __Vdlyvval__top__DOT__regfile__DOT__reg_file__v0;
    CData/*0:0*/ __Vdlyvset__top__DOT__regfile__DOT__reg_file__v0;
    IData/*16:0*/ __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v0;
    CData/*7:0*/ __Vdlyvval__top__DOT__datamem__DOT__ram_array__v0;
    CData/*0:0*/ __Vdlyvset__top__DOT__datamem__DOT__ram_array__v0;
    IData/*16:0*/ __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v1;
    CData/*7:0*/ __Vdlyvval__top__DOT__datamem__DOT__ram_array__v1;
    CData/*0:0*/ __Vdlyvset__top__DOT__datamem__DOT__ram_array__v1;
    IData/*16:0*/ __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v2;
    CData/*7:0*/ __Vdlyvval__top__DOT__datamem__DOT__ram_array__v2;
    IData/*16:0*/ __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v3;
    CData/*7:0*/ __Vdlyvval__top__DOT__datamem__DOT__ram_array__v3;
    CData/*0:0*/ __Vdlyvset__top__DOT__datamem__DOT__ram_array__v3;
    IData/*16:0*/ __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v4;
    CData/*7:0*/ __Vdlyvval__top__DOT__datamem__DOT__ram_array__v4;
    IData/*16:0*/ __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v5;
    CData/*7:0*/ __Vdlyvval__top__DOT__datamem__DOT__ram_array__v5;
    IData/*16:0*/ __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v6;
    CData/*7:0*/ __Vdlyvval__top__DOT__datamem__DOT__ram_array__v6;
    // Body
    __Vdlyvset__top__DOT__datamem__DOT__ram_array__v0 = 0U;
    __Vdlyvset__top__DOT__datamem__DOT__ram_array__v1 = 0U;
    __Vdlyvset__top__DOT__datamem__DOT__ram_array__v3 = 0U;
    __Vdlyvset__top__DOT__regfile__DOT__reg_file__v0 = 0U;
    __Vdly__top__DOT__cycle = vlSelf->top__DOT__cycle;
    if (VL_UNLIKELY(vlSelf->top__DOT__MemWrite)) {
        VL_WRITEF("[DMEM-WRITE] addr=%x data=%x\n",
                  17,(0x1ffffU & vlSelf->top__DOT__alu_out),
                  8,(0xffU & vlSelf->top__DOT__rd2));
    }
    if (vlSelf->top__DOT__MemWrite) {
        if ((0U == (3U & (IData)(vlSelf->top__DOT__AddressingControl)))) {
            __Vdlyvval__top__DOT__datamem__DOT__ram_array__v0 
                = (0xffU & vlSelf->top__DOT__rd2);
            __Vdlyvset__top__DOT__datamem__DOT__ram_array__v0 = 1U;
            __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v0 
                = (0x1ffffU & vlSelf->top__DOT__alu_out);
        } else if ((1U == (3U & (IData)(vlSelf->top__DOT__AddressingControl)))) {
            __Vdlyvval__top__DOT__datamem__DOT__ram_array__v1 
                = (0xffU & vlSelf->top__DOT__rd2);
            __Vdlyvset__top__DOT__datamem__DOT__ram_array__v1 = 1U;
            __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v1 
                = (0x1ffffU & vlSelf->top__DOT__alu_out);
            __Vdlyvval__top__DOT__datamem__DOT__ram_array__v2 
                = (0xffU & (vlSelf->top__DOT__rd2 >> 8U));
            __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v2 
                = (0x1ffffU & ((IData)(1U) + vlSelf->top__DOT__alu_out));
        } else if ((2U == (3U & (IData)(vlSelf->top__DOT__AddressingControl)))) {
            __Vdlyvval__top__DOT__datamem__DOT__ram_array__v3 
                = (0xffU & vlSelf->top__DOT__rd2);
            __Vdlyvset__top__DOT__datamem__DOT__ram_array__v3 = 1U;
            __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v3 
                = (0x1ffffU & vlSelf->top__DOT__alu_out);
            __Vdlyvval__top__DOT__datamem__DOT__ram_array__v4 
                = (0xffU & (vlSelf->top__DOT__rd2 >> 8U));
            __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v4 
                = (0x1ffffU & ((IData)(1U) + vlSelf->top__DOT__alu_out));
            __Vdlyvval__top__DOT__datamem__DOT__ram_array__v5 
                = (0xffU & (vlSelf->top__DOT__rd2 >> 0x10U));
            __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v5 
                = (0x1ffffU & ((IData)(2U) + vlSelf->top__DOT__alu_out));
            __Vdlyvval__top__DOT__datamem__DOT__ram_array__v6 
                = (vlSelf->top__DOT__rd2 >> 0x18U);
            __Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v6 
                = (0x1ffffU & ((IData)(3U) + vlSelf->top__DOT__alu_out));
        }
    }
    if (((IData)(vlSelf->top__DOT__RegWrite) & (0U 
                                                != 
                                                (0x1fU 
                                                 & (vlSelf->top__DOT__instr 
                                                    >> 7U))))) {
        __Vdlyvval__top__DOT__regfile__DOT__reg_file__v0 
            = vlSelf->top__DOT__wd_safe;
        __Vdlyvset__top__DOT__regfile__DOT__reg_file__v0 = 1U;
        __Vdlyvdim0__top__DOT__regfile__DOT__reg_file__v0 
            = (0x1fU & (vlSelf->top__DOT__instr >> 7U));
    }
    __Vdly__top__DOT__cycle = ((IData)(1U) + vlSelf->top__DOT__cycle);
    VL_WRITEF("============================\nCycle %0d\nPC           = %x\ninstr        = %x\nopcode       = %b\nfunct3       = %b\nfunct7       = %b\nImmSrc       = %b\nImmExt       = %x\nsrca (rs1)   = %x\nsrcb         = %x\nALUSrc       = %b\nALUControl   = %b\nALU OUT      = %x\nzero         = %b\nRegWrite     = %b\nrd           = %0#\nwd_safe      = %x\nResultSrc    = %b\nresult       = %x\na0 (x10)     = %x\n",
              32,vlSelf->top__DOT__cycle,32,vlSelf->top__DOT__pc,
              32,vlSelf->top__DOT__instr,7,(0x7fU & vlSelf->top__DOT__instr),
              3,(7U & (vlSelf->top__DOT__instr >> 0xcU)),
              7,(vlSelf->top__DOT__instr >> 0x19U),
              3,(IData)(vlSelf->top__DOT__ImmSrc),32,
              vlSelf->top__DOT__immext,32,vlSelf->top__DOT__rd1,
              32,vlSelf->top__DOT__srcb,1,(IData)(vlSelf->top__DOT__ALUSrc),
              4,vlSelf->top__DOT__ALUControl,32,vlSelf->top__DOT__alu_out,
              1,(0U == vlSelf->top__DOT__alu_out),1,
              (IData)(vlSelf->top__DOT__RegWrite),5,
              (0x1fU & (vlSelf->top__DOT__instr >> 7U)),
              32,vlSelf->top__DOT__wd_safe,2,(IData)(vlSelf->top__DOT__ResultSrc),
              32,vlSelf->top__DOT__result,32,vlSelf->a0);
    if (__Vdlyvset__top__DOT__datamem__DOT__ram_array__v0) {
        vlSelf->top__DOT__datamem__DOT__ram_array[__Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v0] 
            = __Vdlyvval__top__DOT__datamem__DOT__ram_array__v0;
    }
    if (__Vdlyvset__top__DOT__datamem__DOT__ram_array__v1) {
        vlSelf->top__DOT__datamem__DOT__ram_array[__Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v1] 
            = __Vdlyvval__top__DOT__datamem__DOT__ram_array__v1;
        vlSelf->top__DOT__datamem__DOT__ram_array[__Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v2] 
            = __Vdlyvval__top__DOT__datamem__DOT__ram_array__v2;
    }
    if (__Vdlyvset__top__DOT__datamem__DOT__ram_array__v3) {
        vlSelf->top__DOT__datamem__DOT__ram_array[__Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v3] 
            = __Vdlyvval__top__DOT__datamem__DOT__ram_array__v3;
        vlSelf->top__DOT__datamem__DOT__ram_array[__Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v4] 
            = __Vdlyvval__top__DOT__datamem__DOT__ram_array__v4;
        vlSelf->top__DOT__datamem__DOT__ram_array[__Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v5] 
            = __Vdlyvval__top__DOT__datamem__DOT__ram_array__v5;
        vlSelf->top__DOT__datamem__DOT__ram_array[__Vdlyvdim0__top__DOT__datamem__DOT__ram_array__v6] 
            = __Vdlyvval__top__DOT__datamem__DOT__ram_array__v6;
    }
    if (__Vdlyvset__top__DOT__regfile__DOT__reg_file__v0) {
        vlSelf->top__DOT__regfile__DOT__reg_file[__Vdlyvdim0__top__DOT__regfile__DOT__reg_file__v0] 
            = __Vdlyvval__top__DOT__regfile__DOT__reg_file__v0;
    }
    vlSelf->top__DOT__cycle = __Vdly__top__DOT__cycle;
    vlSelf->a0 = vlSelf->top__DOT__regfile__DOT__reg_file
        [0xaU];
}

VL_INLINE_OPT void Vdut___024root___sequent__TOP__1(Vdut___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root___sequent__TOP__1\n"); );
    // Body
    vlSelf->top__DOT__pc = ((IData)(vlSelf->rst) ? 0U
                             : vlSelf->top__DOT__pc_unit__DOT__pc_next);
    vlSelf->top__DOT__instr = vlSelf->top__DOT__imem__DOT__memory
        [(0x3ffU & (vlSelf->top__DOT__pc >> 2U))];
    vlSelf->top__DOT__MemWrite = 0U;
    vlSelf->top__DOT__RegWrite = 0U;
    vlSelf->top__DOT__AddressingControl = 0U;
    if ((1U & (~ (vlSelf->top__DOT__instr >> 6U)))) {
        if ((0x20U & vlSelf->top__DOT__instr)) {
            if ((1U & (~ (vlSelf->top__DOT__instr >> 4U)))) {
                if ((1U & (~ (vlSelf->top__DOT__instr 
                              >> 3U)))) {
                    if ((1U & (~ (vlSelf->top__DOT__instr 
                                  >> 2U)))) {
                        if ((2U & vlSelf->top__DOT__instr)) {
                            if ((1U & vlSelf->top__DOT__instr)) {
                                vlSelf->top__DOT__MemWrite = 1U;
                                vlSelf->top__DOT__AddressingControl 
                                    = (7U & (vlSelf->top__DOT__instr 
                                             >> 0xcU));
                            }
                        }
                    }
                }
            }
        } else if ((1U & (~ (vlSelf->top__DOT__instr 
                             >> 4U)))) {
            if ((1U & (~ (vlSelf->top__DOT__instr >> 3U)))) {
                if ((1U & (~ (vlSelf->top__DOT__instr 
                              >> 2U)))) {
                    if ((2U & vlSelf->top__DOT__instr)) {
                        if ((1U & vlSelf->top__DOT__instr)) {
                            vlSelf->top__DOT__MemWrite = 0U;
                            vlSelf->top__DOT__AddressingControl 
                                = (7U & (vlSelf->top__DOT__instr 
                                         >> 0xcU));
                        }
                    }
                }
            }
        }
    }
    vlSelf->top__DOT__ResultSrc = 0U;
    vlSelf->top__DOT__ALUControl = 0U;
    vlSelf->top__DOT__rd1 = ((0U == (0x1fU & (vlSelf->top__DOT__instr 
                                              >> 0xfU)))
                              ? 0U : vlSelf->top__DOT__regfile__DOT__reg_file
                             [(0x1fU & (vlSelf->top__DOT__instr 
                                        >> 0xfU))]);
    vlSelf->top__DOT__ALUSrc = 0U;
    vlSelf->top__DOT__rd2 = ((0U == (0x1fU & (vlSelf->top__DOT__instr 
                                              >> 0x14U)))
                              ? 0U : vlSelf->top__DOT__regfile__DOT__reg_file
                             [(0x1fU & (vlSelf->top__DOT__instr 
                                        >> 0x14U))]);
    vlSelf->top__DOT__ImmSrc = 0U;
    if ((0x40U & vlSelf->top__DOT__instr)) {
        if ((0x20U & vlSelf->top__DOT__instr)) {
            if ((1U & (~ (vlSelf->top__DOT__instr >> 4U)))) {
                if ((8U & vlSelf->top__DOT__instr)) {
                    if ((4U & vlSelf->top__DOT__instr)) {
                        if ((2U & vlSelf->top__DOT__instr)) {
                            if ((1U & vlSelf->top__DOT__instr)) {
                                vlSelf->top__DOT__RegWrite = 1U;
                                vlSelf->top__DOT__ResultSrc = 2U;
                                vlSelf->top__DOT__ALUControl = 0U;
                                vlSelf->top__DOT__ALUSrc = 0U;
                                vlSelf->top__DOT__ImmSrc = 3U;
                            }
                        }
                    }
                } else if ((4U & vlSelf->top__DOT__instr)) {
                    if ((2U & vlSelf->top__DOT__instr)) {
                        if ((1U & vlSelf->top__DOT__instr)) {
                            vlSelf->top__DOT__RegWrite = 1U;
                            vlSelf->top__DOT__ResultSrc = 2U;
                            vlSelf->top__DOT__ALUControl = 0U;
                            vlSelf->top__DOT__ALUSrc = 1U;
                            vlSelf->top__DOT__ImmSrc = 0U;
                        }
                    }
                } else if ((2U & vlSelf->top__DOT__instr)) {
                    if ((1U & vlSelf->top__DOT__instr)) {
                        vlSelf->top__DOT__RegWrite = 0U;
                        vlSelf->top__DOT__ALUControl = 1U;
                        vlSelf->top__DOT__ALUSrc = 0U;
                        vlSelf->top__DOT__ImmSrc = 2U;
                    }
                }
            }
        }
    } else if ((0x20U & vlSelf->top__DOT__instr)) {
        if ((0x10U & vlSelf->top__DOT__instr)) {
            if ((1U & (~ (vlSelf->top__DOT__instr >> 3U)))) {
                if ((4U & vlSelf->top__DOT__instr)) {
                    if ((2U & vlSelf->top__DOT__instr)) {
                        if ((1U & vlSelf->top__DOT__instr)) {
                            vlSelf->top__DOT__RegWrite = 1U;
                            vlSelf->top__DOT__ResultSrc = 0U;
                            vlSelf->top__DOT__ALUControl = 0xfU;
                            vlSelf->top__DOT__ALUSrc = 1U;
                            vlSelf->top__DOT__ImmSrc = 4U;
                        }
                    }
                } else if ((2U & vlSelf->top__DOT__instr)) {
                    if ((1U & vlSelf->top__DOT__instr)) {
                        vlSelf->top__DOT__RegWrite = 1U;
                        vlSelf->top__DOT__ResultSrc = 0U;
                        vlSelf->top__DOT__ALUControl 
                            = ((0x4000U & vlSelf->top__DOT__instr)
                                ? ((0x2000U & vlSelf->top__DOT__instr)
                                    ? ((0x1000U & vlSelf->top__DOT__instr)
                                        ? 2U : 3U) : 
                                   ((0x1000U & vlSelf->top__DOT__instr)
                                     ? ((0U == (vlSelf->top__DOT__instr 
                                                >> 0x19U))
                                         ? 8U : 0xbU)
                                     : 4U)) : ((0x2000U 
                                                & vlSelf->top__DOT__instr)
                                                ? (
                                                   (0x1000U 
                                                    & vlSelf->top__DOT__instr)
                                                    ? 6U
                                                    : 5U)
                                                : (
                                                   (0x1000U 
                                                    & vlSelf->top__DOT__instr)
                                                    ? 7U
                                                    : 
                                                   ((0x20U 
                                                     == 
                                                     (vlSelf->top__DOT__instr 
                                                      >> 0x19U))
                                                     ? 1U
                                                     : 0U))));
                        vlSelf->top__DOT__ALUSrc = 0U;
                    }
                }
            }
        } else if ((1U & (~ (vlSelf->top__DOT__instr 
                             >> 3U)))) {
            if ((1U & (~ (vlSelf->top__DOT__instr >> 2U)))) {
                if ((2U & vlSelf->top__DOT__instr)) {
                    if ((1U & vlSelf->top__DOT__instr)) {
                        vlSelf->top__DOT__RegWrite = 0U;
                        vlSelf->top__DOT__ALUControl = 0U;
                        vlSelf->top__DOT__ALUSrc = 1U;
                        vlSelf->top__DOT__ImmSrc = 1U;
                    }
                }
            }
        }
    } else if ((0x10U & vlSelf->top__DOT__instr)) {
        if ((1U & (~ (vlSelf->top__DOT__instr >> 3U)))) {
            if ((1U & (~ (vlSelf->top__DOT__instr >> 2U)))) {
                if ((2U & vlSelf->top__DOT__instr)) {
                    if ((1U & vlSelf->top__DOT__instr)) {
                        vlSelf->top__DOT__RegWrite = 1U;
                        vlSelf->top__DOT__ResultSrc = 0U;
                        vlSelf->top__DOT__ALUControl 
                            = ((0x4000U & vlSelf->top__DOT__instr)
                                ? ((0x2000U & vlSelf->top__DOT__instr)
                                    ? ((0x1000U & vlSelf->top__DOT__instr)
                                        ? 2U : 3U) : 
                                   ((0x1000U & vlSelf->top__DOT__instr)
                                     ? ((0U == (vlSelf->top__DOT__instr 
                                                >> 0x19U))
                                         ? 8U : 0xbU)
                                     : 4U)) : ((0x2000U 
                                                & vlSelf->top__DOT__instr)
                                                ? (
                                                   (0x1000U 
                                                    & vlSelf->top__DOT__instr)
                                                    ? 6U
                                                    : 5U)
                                                : (
                                                   (0x1000U 
                                                    & vlSelf->top__DOT__instr)
                                                    ? 7U
                                                    : 0U)));
                        vlSelf->top__DOT__ALUSrc = 1U;
                        vlSelf->top__DOT__ImmSrc = 0U;
                    }
                }
            }
        }
    } else if ((1U & (~ (vlSelf->top__DOT__instr >> 3U)))) {
        if ((1U & (~ (vlSelf->top__DOT__instr >> 2U)))) {
            if ((2U & vlSelf->top__DOT__instr)) {
                if ((1U & vlSelf->top__DOT__instr)) {
                    vlSelf->top__DOT__RegWrite = 1U;
                    vlSelf->top__DOT__ResultSrc = 1U;
                    vlSelf->top__DOT__ALUControl = 0U;
                    vlSelf->top__DOT__ALUSrc = 1U;
                    vlSelf->top__DOT__ImmSrc = 0U;
                }
            }
        }
    }
    vlSelf->top__DOT__immext = ((4U & (IData)(vlSelf->top__DOT__ImmSrc))
                                 ? ((2U & (IData)(vlSelf->top__DOT__ImmSrc))
                                     ? 0U : ((1U & (IData)(vlSelf->top__DOT__ImmSrc))
                                              ? 0U : 
                                             (0xfffff000U 
                                              & vlSelf->top__DOT__instr)))
                                 : ((2U & (IData)(vlSelf->top__DOT__ImmSrc))
                                     ? ((1U & (IData)(vlSelf->top__DOT__ImmSrc))
                                         ? (((- (IData)(
                                                        (vlSelf->top__DOT__instr 
                                                         >> 0x1fU))) 
                                             << 0x15U) 
                                            | ((0x100000U 
                                                & (vlSelf->top__DOT__instr 
                                                   >> 0xbU)) 
                                               | ((0xff000U 
                                                   & vlSelf->top__DOT__instr) 
                                                  | ((0x800U 
                                                      & (vlSelf->top__DOT__instr 
                                                         >> 9U)) 
                                                     | (0x7feU 
                                                        & (vlSelf->top__DOT__instr 
                                                           >> 0x14U))))))
                                         : (((- (IData)(
                                                        (vlSelf->top__DOT__instr 
                                                         >> 0x1fU))) 
                                             << 0xdU) 
                                            | ((0x1000U 
                                                & (vlSelf->top__DOT__instr 
                                                   >> 0x13U)) 
                                               | ((0x800U 
                                                   & (vlSelf->top__DOT__instr 
                                                      << 4U)) 
                                                  | ((0x7e0U 
                                                      & (vlSelf->top__DOT__instr 
                                                         >> 0x14U)) 
                                                     | (0x1eU 
                                                        & (vlSelf->top__DOT__instr 
                                                           >> 7U)))))))
                                     : ((1U & (IData)(vlSelf->top__DOT__ImmSrc))
                                         ? (((- (IData)(
                                                        (vlSelf->top__DOT__instr 
                                                         >> 0x1fU))) 
                                             << 0xcU) 
                                            | ((0xfe0U 
                                                & (vlSelf->top__DOT__instr 
                                                   >> 0x14U)) 
                                               | (0x1fU 
                                                  & (vlSelf->top__DOT__instr 
                                                     >> 7U))))
                                         : (((- (IData)(
                                                        (vlSelf->top__DOT__instr 
                                                         >> 0x1fU))) 
                                             << 0xcU) 
                                            | (vlSelf->top__DOT__instr 
                                               >> 0x14U)))));
    vlSelf->top__DOT__srcb = ((IData)(vlSelf->top__DOT__ALUSrc)
                               ? vlSelf->top__DOT__immext
                               : vlSelf->top__DOT__rd2);
    vlSelf->top__DOT__alu_out = ((8U & (IData)(vlSelf->top__DOT__ALUControl))
                                  ? ((4U & (IData)(vlSelf->top__DOT__ALUControl))
                                      ? ((2U & (IData)(vlSelf->top__DOT__ALUControl))
                                          ? ((1U & (IData)(vlSelf->top__DOT__ALUControl))
                                              ? vlSelf->top__DOT__srcb
                                              : 0U)
                                          : 0U) : (
                                                   (2U 
                                                    & (IData)(vlSelf->top__DOT__ALUControl))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelf->top__DOT__ALUControl))
                                                     ? 
                                                    VL_SHIFTRS_III(32,32,5, vlSelf->top__DOT__rd1, 
                                                                   (0x1fU 
                                                                    & vlSelf->top__DOT__srcb))
                                                     : 0U)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelf->top__DOT__ALUControl))
                                                     ? 0U
                                                     : 
                                                    (vlSelf->top__DOT__rd1 
                                                     >> 
                                                     (0x1fU 
                                                      & vlSelf->top__DOT__srcb)))))
                                  : ((4U & (IData)(vlSelf->top__DOT__ALUControl))
                                      ? ((2U & (IData)(vlSelf->top__DOT__ALUControl))
                                          ? ((1U & (IData)(vlSelf->top__DOT__ALUControl))
                                              ? (vlSelf->top__DOT__rd1 
                                                 << 
                                                 (0x1fU 
                                                  & vlSelf->top__DOT__srcb))
                                              : ((vlSelf->top__DOT__rd1 
                                                  < vlSelf->top__DOT__srcb)
                                                  ? 1U
                                                  : 0U))
                                          : ((1U & (IData)(vlSelf->top__DOT__ALUControl))
                                              ? (VL_LTS_III(32, vlSelf->top__DOT__rd1, vlSelf->top__DOT__srcb)
                                                  ? 1U
                                                  : 0U)
                                              : (vlSelf->top__DOT__rd1 
                                                 ^ vlSelf->top__DOT__srcb)))
                                      : ((2U & (IData)(vlSelf->top__DOT__ALUControl))
                                          ? ((1U & (IData)(vlSelf->top__DOT__ALUControl))
                                              ? (vlSelf->top__DOT__rd1 
                                                 | vlSelf->top__DOT__srcb)
                                              : (vlSelf->top__DOT__rd1 
                                                 & vlSelf->top__DOT__srcb))
                                          : ((1U & (IData)(vlSelf->top__DOT__ALUControl))
                                              ? (vlSelf->top__DOT__rd1 
                                                 - vlSelf->top__DOT__srcb)
                                              : (vlSelf->top__DOT__rd1 
                                                 + vlSelf->top__DOT__srcb)))));
    vlSelf->top__DOT__PCSrc = 0U;
    if ((0x40U & vlSelf->top__DOT__instr)) {
        if ((0x20U & vlSelf->top__DOT__instr)) {
            if ((1U & (~ (vlSelf->top__DOT__instr >> 4U)))) {
                if ((8U & vlSelf->top__DOT__instr)) {
                    if ((4U & vlSelf->top__DOT__instr)) {
                        if ((2U & vlSelf->top__DOT__instr)) {
                            if ((1U & vlSelf->top__DOT__instr)) {
                                vlSelf->top__DOT__PCSrc = 1U;
                            }
                        }
                    }
                } else if ((4U & vlSelf->top__DOT__instr)) {
                    if ((2U & vlSelf->top__DOT__instr)) {
                        if ((1U & vlSelf->top__DOT__instr)) {
                            vlSelf->top__DOT__PCSrc = 2U;
                        }
                    }
                } else if ((2U & vlSelf->top__DOT__instr)) {
                    if ((1U & vlSelf->top__DOT__instr)) {
                        vlSelf->top__DOT__PCSrc = (
                                                   (0U 
                                                    == 
                                                    (7U 
                                                     & (vlSelf->top__DOT__instr 
                                                        >> 0xcU)))
                                                    ? 
                                                   ((0U 
                                                     == vlSelf->top__DOT__alu_out)
                                                     ? 1U
                                                     : 0U)
                                                    : 
                                                   ((1U 
                                                     == 
                                                     (7U 
                                                      & (vlSelf->top__DOT__instr 
                                                         >> 0xcU)))
                                                     ? 
                                                    ((0U 
                                                      == vlSelf->top__DOT__alu_out)
                                                      ? 0U
                                                      : 1U)
                                                     : 0U));
                    }
                }
            }
        }
    } else if ((0x20U & vlSelf->top__DOT__instr)) {
        if ((0x10U & vlSelf->top__DOT__instr)) {
            if ((1U & (~ (vlSelf->top__DOT__instr >> 3U)))) {
                if ((4U & vlSelf->top__DOT__instr)) {
                    if ((2U & vlSelf->top__DOT__instr)) {
                        if ((1U & vlSelf->top__DOT__instr)) {
                            vlSelf->top__DOT__PCSrc = 0U;
                        }
                    }
                } else if ((2U & vlSelf->top__DOT__instr)) {
                    if ((1U & vlSelf->top__DOT__instr)) {
                        vlSelf->top__DOT__PCSrc = 0U;
                    }
                }
            }
        } else if ((1U & (~ (vlSelf->top__DOT__instr 
                             >> 3U)))) {
            if ((1U & (~ (vlSelf->top__DOT__instr >> 2U)))) {
                if ((2U & vlSelf->top__DOT__instr)) {
                    if ((1U & vlSelf->top__DOT__instr)) {
                        vlSelf->top__DOT__PCSrc = 0U;
                    }
                }
            }
        }
    } else if ((0x10U & vlSelf->top__DOT__instr)) {
        if ((1U & (~ (vlSelf->top__DOT__instr >> 3U)))) {
            if ((1U & (~ (vlSelf->top__DOT__instr >> 2U)))) {
                if ((2U & vlSelf->top__DOT__instr)) {
                    if ((1U & vlSelf->top__DOT__instr)) {
                        vlSelf->top__DOT__PCSrc = 0U;
                    }
                }
            }
        }
    } else if ((1U & (~ (vlSelf->top__DOT__instr >> 3U)))) {
        if ((1U & (~ (vlSelf->top__DOT__instr >> 2U)))) {
            if ((2U & vlSelf->top__DOT__instr)) {
                if ((1U & vlSelf->top__DOT__instr)) {
                    vlSelf->top__DOT__PCSrc = 0U;
                }
            }
        }
    }
    if ((1U == (IData)(vlSelf->top__DOT__ResultSrc))) {
        if ((0U == (3U & (IData)(vlSelf->top__DOT__AddressingControl)))) {
            vlSelf->top__DOT__datamem__DOT__unnamedblk2__DOT__b 
                = vlSelf->top__DOT__datamem__DOT__ram_array
                [(0x1ffffU & vlSelf->top__DOT__alu_out)];
            vlSelf->top__DOT__read_data = ((4U & (IData)(vlSelf->top__DOT__AddressingControl))
                                            ? (IData)(vlSelf->top__DOT__datamem__DOT__unnamedblk2__DOT__b)
                                            : (((- (IData)(
                                                           (1U 
                                                            & ((IData)(vlSelf->top__DOT__datamem__DOT__unnamedblk2__DOT__b) 
                                                               >> 7U)))) 
                                                << 8U) 
                                               | (IData)(vlSelf->top__DOT__datamem__DOT__unnamedblk2__DOT__b)));
        } else if ((1U == (3U & (IData)(vlSelf->top__DOT__AddressingControl)))) {
            vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__lo 
                = vlSelf->top__DOT__datamem__DOT__ram_array
                [(0x1ffffU & vlSelf->top__DOT__alu_out)];
            vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__hi 
                = vlSelf->top__DOT__datamem__DOT__ram_array
                [(0x1ffffU & ((IData)(1U) + vlSelf->top__DOT__alu_out))];
            vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__half 
                = (((IData)(vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__hi) 
                    << 8U) | (IData)(vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__lo));
            vlSelf->top__DOT__read_data = ((4U & (IData)(vlSelf->top__DOT__AddressingControl))
                                            ? (IData)(vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__half)
                                            : (((- (IData)(
                                                           (1U 
                                                            & ((IData)(vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__half) 
                                                               >> 0xfU)))) 
                                                << 0x10U) 
                                               | (IData)(vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__half)));
        } else {
            vlSelf->top__DOT__read_data = ((2U == (3U 
                                                   & (IData)(vlSelf->top__DOT__AddressingControl)))
                                            ? ((vlSelf->top__DOT__datamem__DOT__ram_array
                                                [(0x1ffffU 
                                                  & ((IData)(3U) 
                                                     + vlSelf->top__DOT__alu_out))] 
                                                << 0x18U) 
                                               | ((vlSelf->top__DOT__datamem__DOT__ram_array
                                                   [
                                                   (0x1ffffU 
                                                    & ((IData)(2U) 
                                                       + vlSelf->top__DOT__alu_out))] 
                                                   << 0x10U) 
                                                  | ((vlSelf->top__DOT__datamem__DOT__ram_array
                                                      [
                                                      (0x1ffffU 
                                                       & ((IData)(1U) 
                                                          + vlSelf->top__DOT__alu_out))] 
                                                      << 8U) 
                                                     | vlSelf->top__DOT__datamem__DOT__ram_array
                                                     [
                                                     (0x1ffffU 
                                                      & vlSelf->top__DOT__alu_out)])))
                                            : 0U);
        }
    } else {
        vlSelf->top__DOT__read_data = 0U;
    }
    vlSelf->top__DOT__pc_unit__DOT__pc_next = ((0U 
                                                == (IData)(vlSelf->top__DOT__PCSrc))
                                                ? ((IData)(4U) 
                                                   + vlSelf->top__DOT__pc)
                                                : (
                                                   (1U 
                                                    == (IData)(vlSelf->top__DOT__PCSrc))
                                                    ? 
                                                   (vlSelf->top__DOT__pc 
                                                    + vlSelf->top__DOT__immext)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelf->top__DOT__PCSrc))
                                                     ? 
                                                    (0xfffffffeU 
                                                     & vlSelf->top__DOT__alu_out)
                                                     : 0U)));
    vlSelf->top__DOT__result = ((0U == (IData)(vlSelf->top__DOT__ResultSrc))
                                 ? vlSelf->top__DOT__alu_out
                                 : ((1U == (IData)(vlSelf->top__DOT__ResultSrc))
                                     ? ((1U == (IData)(vlSelf->top__DOT__ResultSrc))
                                         ? vlSelf->top__DOT__read_data
                                         : 0U) : ((2U 
                                                   == (IData)(vlSelf->top__DOT__ResultSrc))
                                                   ? 
                                                  ((IData)(4U) 
                                                   + vlSelf->top__DOT__pc)
                                                   : 0U)));
    vlSelf->top__DOT__wd_safe = ((IData)(vlSelf->top__DOT__RegWrite)
                                  ? vlSelf->top__DOT__result
                                  : 0U);
}

void Vdut___024root___eval(Vdut___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root___eval\n"); );
    // Body
    if (((IData)(vlSelf->clk) & (~ (IData)(vlSelf->__Vclklast__TOP__clk)))) {
        Vdut___024root___sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[1U] = 1U;
    }
    if ((((IData)(vlSelf->clk) & (~ (IData)(vlSelf->__Vclklast__TOP__clk))) 
         | ((IData)(vlSelf->rst) & (~ (IData)(vlSelf->__Vclklast__TOP__rst))))) {
        Vdut___024root___sequent__TOP__1(vlSelf);
        vlSelf->__Vm_traceActivity[2U] = 1U;
    }
    // Final
    vlSelf->__Vclklast__TOP__clk = vlSelf->clk;
    vlSelf->__Vclklast__TOP__rst = vlSelf->rst;
}

#ifdef VL_DEBUG
void Vdut___024root___eval_debug_assertions(Vdut___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((vlSelf->rst & 0xfeU))) {
        Verilated::overWidthError("rst");}
}
#endif  // VL_DEBUG
