// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vdut__Syms.h"


void Vdut___024root__trace_chg_sub_0(Vdut___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vdut___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root__trace_chg_top_0\n"); );
    // Init
    Vdut___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vdut___024root*>(voidSelf);
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vdut___024root__trace_chg_sub_0((&vlSymsp->TOP), bufp);
}

void Vdut___024root__trace_chg_sub_0(Vdut___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root__trace_chg_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[0U])) {
        bufp->chgIData(oldp+0,(vlSelf->top__DOT__datamem__DOT__unnamedblk1__DOT__file),32);
    }
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[1U])) {
        bufp->chgIData(oldp+1,(vlSelf->top__DOT__cycle),32);
        bufp->chgIData(oldp+2,(vlSelf->top__DOT__regfile__DOT__reg_file[0]),32);
        bufp->chgIData(oldp+3,(vlSelf->top__DOT__regfile__DOT__reg_file[1]),32);
        bufp->chgIData(oldp+4,(vlSelf->top__DOT__regfile__DOT__reg_file[2]),32);
        bufp->chgIData(oldp+5,(vlSelf->top__DOT__regfile__DOT__reg_file[3]),32);
        bufp->chgIData(oldp+6,(vlSelf->top__DOT__regfile__DOT__reg_file[4]),32);
        bufp->chgIData(oldp+7,(vlSelf->top__DOT__regfile__DOT__reg_file[5]),32);
        bufp->chgIData(oldp+8,(vlSelf->top__DOT__regfile__DOT__reg_file[6]),32);
        bufp->chgIData(oldp+9,(vlSelf->top__DOT__regfile__DOT__reg_file[7]),32);
        bufp->chgIData(oldp+10,(vlSelf->top__DOT__regfile__DOT__reg_file[8]),32);
        bufp->chgIData(oldp+11,(vlSelf->top__DOT__regfile__DOT__reg_file[9]),32);
        bufp->chgIData(oldp+12,(vlSelf->top__DOT__regfile__DOT__reg_file[10]),32);
        bufp->chgIData(oldp+13,(vlSelf->top__DOT__regfile__DOT__reg_file[11]),32);
        bufp->chgIData(oldp+14,(vlSelf->top__DOT__regfile__DOT__reg_file[12]),32);
        bufp->chgIData(oldp+15,(vlSelf->top__DOT__regfile__DOT__reg_file[13]),32);
        bufp->chgIData(oldp+16,(vlSelf->top__DOT__regfile__DOT__reg_file[14]),32);
        bufp->chgIData(oldp+17,(vlSelf->top__DOT__regfile__DOT__reg_file[15]),32);
        bufp->chgIData(oldp+18,(vlSelf->top__DOT__regfile__DOT__reg_file[16]),32);
        bufp->chgIData(oldp+19,(vlSelf->top__DOT__regfile__DOT__reg_file[17]),32);
        bufp->chgIData(oldp+20,(vlSelf->top__DOT__regfile__DOT__reg_file[18]),32);
        bufp->chgIData(oldp+21,(vlSelf->top__DOT__regfile__DOT__reg_file[19]),32);
        bufp->chgIData(oldp+22,(vlSelf->top__DOT__regfile__DOT__reg_file[20]),32);
        bufp->chgIData(oldp+23,(vlSelf->top__DOT__regfile__DOT__reg_file[21]),32);
        bufp->chgIData(oldp+24,(vlSelf->top__DOT__regfile__DOT__reg_file[22]),32);
        bufp->chgIData(oldp+25,(vlSelf->top__DOT__regfile__DOT__reg_file[23]),32);
        bufp->chgIData(oldp+26,(vlSelf->top__DOT__regfile__DOT__reg_file[24]),32);
        bufp->chgIData(oldp+27,(vlSelf->top__DOT__regfile__DOT__reg_file[25]),32);
        bufp->chgIData(oldp+28,(vlSelf->top__DOT__regfile__DOT__reg_file[26]),32);
        bufp->chgIData(oldp+29,(vlSelf->top__DOT__regfile__DOT__reg_file[27]),32);
        bufp->chgIData(oldp+30,(vlSelf->top__DOT__regfile__DOT__reg_file[28]),32);
        bufp->chgIData(oldp+31,(vlSelf->top__DOT__regfile__DOT__reg_file[29]),32);
        bufp->chgIData(oldp+32,(vlSelf->top__DOT__regfile__DOT__reg_file[30]),32);
        bufp->chgIData(oldp+33,(vlSelf->top__DOT__regfile__DOT__reg_file[31]),32);
    }
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[2U])) {
        bufp->chgIData(oldp+34,(vlSelf->top__DOT__pc),32);
        bufp->chgIData(oldp+35,(vlSelf->top__DOT__instr),32);
        bufp->chgIData(oldp+36,(vlSelf->top__DOT__immext),32);
        bufp->chgCData(oldp+37,((0x7fU & vlSelf->top__DOT__instr)),7);
        bufp->chgCData(oldp+38,((0x1fU & (vlSelf->top__DOT__instr 
                                          >> 7U))),5);
        bufp->chgCData(oldp+39,((7U & (vlSelf->top__DOT__instr 
                                       >> 0xcU))),3);
        bufp->chgCData(oldp+40,((0x1fU & (vlSelf->top__DOT__instr 
                                          >> 0xfU))),5);
        bufp->chgCData(oldp+41,((0x1fU & (vlSelf->top__DOT__instr 
                                          >> 0x14U))),5);
        bufp->chgCData(oldp+42,((vlSelf->top__DOT__instr 
                                 >> 0x19U)),7);
        bufp->chgBit(oldp+43,(vlSelf->top__DOT__RegWrite));
        bufp->chgCData(oldp+44,(vlSelf->top__DOT__ALUControl),4);
        bufp->chgBit(oldp+45,(vlSelf->top__DOT__ALUSrc));
        bufp->chgBit(oldp+46,(vlSelf->top__DOT__MemWrite));
        bufp->chgCData(oldp+47,(vlSelf->top__DOT__PCSrc),2);
        bufp->chgCData(oldp+48,(vlSelf->top__DOT__ResultSrc),2);
        bufp->chgCData(oldp+49,(vlSelf->top__DOT__ImmSrc),3);
        bufp->chgCData(oldp+50,(vlSelf->top__DOT__AddressingControl),3);
        bufp->chgBit(oldp+51,((0U == vlSelf->top__DOT__alu_out)));
        bufp->chgIData(oldp+52,(vlSelf->top__DOT__rd1),32);
        bufp->chgIData(oldp+53,(vlSelf->top__DOT__rd2),32);
        bufp->chgIData(oldp+54,(vlSelf->top__DOT__result),32);
        bufp->chgIData(oldp+55,(vlSelf->top__DOT__wd_safe),32);
        bufp->chgIData(oldp+56,(vlSelf->top__DOT__srcb),32);
        bufp->chgIData(oldp+57,(vlSelf->top__DOT__alu_out),32);
        bufp->chgIData(oldp+58,(vlSelf->top__DOT__read_data),32);
        bufp->chgIData(oldp+59,(((IData)(4U) + vlSelf->top__DOT__pc)),32);
        bufp->chgIData(oldp+60,(((1U == (IData)(vlSelf->top__DOT__ResultSrc))
                                  ? vlSelf->top__DOT__read_data
                                  : 0U)),32);
        bufp->chgBit(oldp+61,((1U == (IData)(vlSelf->top__DOT__ResultSrc))));
        bufp->chgCData(oldp+62,((3U & (IData)(vlSelf->top__DOT__AddressingControl))),2);
        bufp->chgBit(oldp+63,((1U & ((IData)(vlSelf->top__DOT__AddressingControl) 
                                     >> 2U))));
        bufp->chgIData(oldp+64,((0x1ffffU & vlSelf->top__DOT__alu_out)),17);
        bufp->chgCData(oldp+65,(vlSelf->top__DOT__datamem__DOT__unnamedblk2__DOT__b),8);
        bufp->chgCData(oldp+66,(vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__lo),8);
        bufp->chgCData(oldp+67,(vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__hi),8);
        bufp->chgSData(oldp+68,(vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__half),16);
        bufp->chgIData(oldp+69,((vlSelf->top__DOT__instr 
                                 >> 7U)),25);
        bufp->chgIData(oldp+70,(((IData)(4U) + vlSelf->top__DOT__pc)),32);
        bufp->chgIData(oldp+71,((vlSelf->top__DOT__pc 
                                 + vlSelf->top__DOT__immext)),32);
        bufp->chgIData(oldp+72,(((0U == (IData)(vlSelf->top__DOT__PCSrc))
                                  ? ((IData)(4U) + vlSelf->top__DOT__pc)
                                  : ((1U == (IData)(vlSelf->top__DOT__PCSrc))
                                      ? (vlSelf->top__DOT__pc 
                                         + vlSelf->top__DOT__immext)
                                      : ((2U == (IData)(vlSelf->top__DOT__PCSrc))
                                          ? (0xfffffffeU 
                                             & vlSelf->top__DOT__alu_out)
                                          : 0U)))),32);
        bufp->chgIData(oldp+73,((0xfffffffeU & vlSelf->top__DOT__alu_out)),32);
    }
    bufp->chgBit(oldp+74,(vlSelf->clk));
    bufp->chgBit(oldp+75,(vlSelf->rst));
    bufp->chgIData(oldp+76,(vlSelf->a0),32);
}

void Vdut___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root__trace_cleanup\n"); );
    // Init
    Vdut___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vdut___024root*>(voidSelf);
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[2U] = 0U;
}
