// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdut.h for the primary calling header

#include "verilated.h"

#include "Vdut___024root.h"

VL_ATTR_COLD void Vdut___024root___initial__TOP__0(Vdut___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root___initial__TOP__0\n"); );
    // Init
    VlWide<4>/*127:0*/ __Vtemp_hf855cb2c__0;
    VlWide<3>/*95:0*/ __Vtemp_h5fe04b9a__0;
    // Body
    VL_WRITEF("[DataMemory] Trying to open datamem_path.txt...\n");
    __Vtemp_hf855cb2c__0[0U] = 0x2e747874U;
    __Vtemp_hf855cb2c__0[1U] = 0x70617468U;
    __Vtemp_hf855cb2c__0[2U] = 0x6d656d5fU;
    __Vtemp_hf855cb2c__0[3U] = 0x64617461U;
    vlSelf->top__DOT__datamem__DOT__unnamedblk1__DOT__file = VL_FOPEN_NN(
                                                                         VL_CVT_PACK_STR_NW(4, __Vtemp_hf855cb2c__0)
                                                                         , 
                                                                         std::string{"r"});
    if (VL_UNLIKELY((0U != vlSelf->top__DOT__datamem__DOT__unnamedblk1__DOT__file))) {
        VL_WRITEF("[DataMemory] datamem_path.txt opened successfully.\n");
        (void)VL_FGETS_NI(vlSelf->top__DOT__datamem__DOT__unnamedblk1__DOT__path, vlSelf->top__DOT__datamem__DOT__unnamedblk1__DOT__file);VL_FCLOSE_I(vlSelf->top__DOT__datamem__DOT__unnamedblk1__DOT__file); vlSelf->top__DOT__datamem__DOT__unnamedblk1__DOT__file = 0;
        VL_WRITEF("[DataMemory] Loading '%@' into data memory @ 0x10000...\n",
                  -1,&(vlSelf->top__DOT__datamem__DOT__unnamedblk1__DOT__path));
        VL_READMEM_N(true, 8, 131072, 0, VL_CVT_PACK_STR_NN(vlSelf->top__DOT__datamem__DOT__unnamedblk1__DOT__path)
                     ,  &(vlSelf->top__DOT__datamem__DOT__ram_array)
                     , 0x10000U, ~0ULL);
        VL_WRITEF("[DataMemory] Data memory populated from '%@'.\n",
                  -1,&(vlSelf->top__DOT__datamem__DOT__unnamedblk1__DOT__path));
    } else {
        VL_WRITEF("[DataMemory] ERROR: Could not open datamem_path.txt.\n[DataMemory] No data preloaded into memory.\n");
    }
    __Vtemp_h5fe04b9a__0[0U] = 0x2e6d656dU;
    __Vtemp_h5fe04b9a__0[1U] = 0x6772616dU;
    __Vtemp_h5fe04b9a__0[2U] = 0x70726fU;
    VL_READMEM_N(true, 32, 1024, 0, VL_CVT_PACK_STR_NW(3, __Vtemp_h5fe04b9a__0)
                 ,  &(vlSelf->top__DOT__imem__DOT__memory)
                 , 0, ~0ULL);
    VL_WRITEF("[InstrMem] memory[0] = %x\n[InstrMem] memory[1] = %x\n[InstrMem] memory[2] = %x\n",
              32,vlSelf->top__DOT__imem__DOT__memory
              [0U],32,vlSelf->top__DOT__imem__DOT__memory
              [1U],32,vlSelf->top__DOT__imem__DOT__memory
              [2U]);
    vlSelf->top__DOT__cycle = 0U;
}

VL_ATTR_COLD void Vdut___024root___settle__TOP__0(Vdut___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root___settle__TOP__0\n"); );
    // Body
    vlSelf->a0 = vlSelf->top__DOT__regfile__DOT__reg_file
        [0xaU];
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

VL_ATTR_COLD void Vdut___024root___eval_initial(Vdut___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root___eval_initial\n"); );
    // Body
    Vdut___024root___initial__TOP__0(vlSelf);
    vlSelf->__Vm_traceActivity[2U] = 1U;
    vlSelf->__Vm_traceActivity[1U] = 1U;
    vlSelf->__Vm_traceActivity[0U] = 1U;
    vlSelf->__Vclklast__TOP__clk = vlSelf->clk;
    vlSelf->__Vclklast__TOP__rst = vlSelf->rst;
}

VL_ATTR_COLD void Vdut___024root___eval_settle(Vdut___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root___eval_settle\n"); );
    // Body
    Vdut___024root___settle__TOP__0(vlSelf);
    vlSelf->__Vm_traceActivity[2U] = 1U;
    vlSelf->__Vm_traceActivity[1U] = 1U;
    vlSelf->__Vm_traceActivity[0U] = 1U;
}

VL_ATTR_COLD void Vdut___024root___final(Vdut___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root___final\n"); );
}

VL_ATTR_COLD void Vdut___024root___ctor_var_reset(Vdut___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clk = VL_RAND_RESET_I(1);
    vlSelf->rst = VL_RAND_RESET_I(1);
    vlSelf->a0 = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__pc = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__instr = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__immext = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__RegWrite = VL_RAND_RESET_I(1);
    vlSelf->top__DOT__ALUControl = VL_RAND_RESET_I(4);
    vlSelf->top__DOT__ALUSrc = VL_RAND_RESET_I(1);
    vlSelf->top__DOT__MemWrite = VL_RAND_RESET_I(1);
    vlSelf->top__DOT__PCSrc = VL_RAND_RESET_I(2);
    vlSelf->top__DOT__ResultSrc = VL_RAND_RESET_I(2);
    vlSelf->top__DOT__ImmSrc = VL_RAND_RESET_I(3);
    vlSelf->top__DOT__AddressingControl = VL_RAND_RESET_I(3);
    vlSelf->top__DOT__rd1 = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__rd2 = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__result = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__wd_safe = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__srcb = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__alu_out = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__read_data = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__cycle = VL_RAND_RESET_I(32);
    for (int __Vi0=0; __Vi0<1024; ++__Vi0) {
        vlSelf->top__DOT__imem__DOT__memory[__Vi0] = VL_RAND_RESET_I(32);
    }
    for (int __Vi0=0; __Vi0<32; ++__Vi0) {
        vlSelf->top__DOT__regfile__DOT__reg_file[__Vi0] = VL_RAND_RESET_I(32);
    }
    for (int __Vi0=0; __Vi0<131072; ++__Vi0) {
        vlSelf->top__DOT__datamem__DOT__ram_array[__Vi0] = VL_RAND_RESET_I(8);
    }
    vlSelf->top__DOT__datamem__DOT__unnamedblk1__DOT__file = 0;
    vlSelf->top__DOT__datamem__DOT__unnamedblk2__DOT__b = VL_RAND_RESET_I(8);
    vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__lo = VL_RAND_RESET_I(8);
    vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__hi = VL_RAND_RESET_I(8);
    vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__half = VL_RAND_RESET_I(16);
    vlSelf->top__DOT__pc_unit__DOT__pc_next = VL_RAND_RESET_I(32);
    for (int __Vi0=0; __Vi0<3; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = VL_RAND_RESET_I(1);
    }
}
