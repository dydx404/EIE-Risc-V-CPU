// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vdut__Syms.h"


VL_ATTR_COLD void Vdut___024root__trace_init_sub__TOP__0(Vdut___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+75,"clk", false,-1);
    tracep->declBit(c+76,"rst", false,-1);
    tracep->declBus(c+77,"a0", false,-1, 31,0);
    tracep->pushNamePrefix("top ");
    tracep->declBit(c+75,"clk", false,-1);
    tracep->declBit(c+76,"rst", false,-1);
    tracep->declBus(c+77,"a0", false,-1, 31,0);
    tracep->declBus(c+35,"pc", false,-1, 31,0);
    tracep->declBus(c+36,"instr", false,-1, 31,0);
    tracep->declBus(c+37,"immext", false,-1, 31,0);
    tracep->declBus(c+38,"opcode", false,-1, 6,0);
    tracep->declBus(c+39,"rd", false,-1, 4,0);
    tracep->declBus(c+40,"funct3", false,-1, 2,0);
    tracep->declBus(c+41,"rs1", false,-1, 4,0);
    tracep->declBus(c+42,"rs2", false,-1, 4,0);
    tracep->declBus(c+43,"funct7", false,-1, 6,0);
    tracep->declBit(c+44,"RegWrite", false,-1);
    tracep->declBus(c+45,"ALUControl", false,-1, 3,0);
    tracep->declBit(c+46,"ALUSrc", false,-1);
    tracep->declBit(c+47,"MemWrite", false,-1);
    tracep->declBus(c+48,"PCSrc", false,-1, 1,0);
    tracep->declBus(c+49,"ResultSrc", false,-1, 1,0);
    tracep->declBus(c+50,"ImmSrc", false,-1, 2,0);
    tracep->declBus(c+51,"AddressingControl", false,-1, 2,0);
    tracep->declBit(c+52,"zero", false,-1);
    tracep->declBus(c+53,"rd1", false,-1, 31,0);
    tracep->declBus(c+54,"rd2", false,-1, 31,0);
    tracep->declBus(c+55,"result", false,-1, 31,0);
    tracep->declBus(c+56,"wd_safe", false,-1, 31,0);
    tracep->declBus(c+53,"srca", false,-1, 31,0);
    tracep->declBus(c+57,"srcb", false,-1, 31,0);
    tracep->declBus(c+58,"alu_out", false,-1, 31,0);
    tracep->declBus(c+59,"read_data", false,-1, 31,0);
    tracep->declBus(c+60,"pc_plus4", false,-1, 31,0);
    tracep->declBus(c+61,"read_data_safe", false,-1, 31,0);
    tracep->declBus(c+2,"cycle", false,-1, 31,0);
    tracep->pushNamePrefix("alu ");
    tracep->declBus(c+78,"LEN", false,-1, 31,0);
    tracep->declBus(c+53,"aluop1", false,-1, 31,0);
    tracep->declBus(c+57,"aluop2", false,-1, 31,0);
    tracep->declBus(c+45,"alu_ctrl", false,-1, 3,0);
    tracep->declBus(c+58,"aluout", false,-1, 31,0);
    tracep->declBit(c+52,"zero", false,-1);
    tracep->popNamePrefix(1);
    tracep->pushNamePrefix("control ");
    tracep->declBit(c+52,"zero", false,-1);
    tracep->declBus(c+38,"op", false,-1, 6,0);
    tracep->declBus(c+40,"funct3", false,-1, 2,0);
    tracep->declBus(c+43,"funct7", false,-1, 6,0);
    tracep->declBit(c+44,"RegWrite", false,-1);
    tracep->declBus(c+45,"ALUControl", false,-1, 3,0);
    tracep->declBit(c+46,"ALUSrc", false,-1);
    tracep->declBit(c+47,"MemWrite", false,-1);
    tracep->declBus(c+48,"PCSrc", false,-1, 1,0);
    tracep->declBus(c+49,"ResultSrc", false,-1, 1,0);
    tracep->declBus(c+50,"ImmSrc", false,-1, 2,0);
    tracep->declBus(c+51,"AddressingControl", false,-1, 2,0);
    tracep->declBus(c+79,"ALU_ADD", false,-1, 3,0);
    tracep->declBus(c+80,"ALU_SUB", false,-1, 3,0);
    tracep->declBus(c+81,"ALU_AND", false,-1, 3,0);
    tracep->declBus(c+82,"ALU_OR", false,-1, 3,0);
    tracep->declBus(c+83,"ALU_XOR", false,-1, 3,0);
    tracep->declBus(c+84,"ALU_SLT", false,-1, 3,0);
    tracep->declBus(c+85,"ALU_SLTU", false,-1, 3,0);
    tracep->declBus(c+86,"ALU_SLL", false,-1, 3,0);
    tracep->declBus(c+87,"ALU_SRL", false,-1, 3,0);
    tracep->declBus(c+88,"ALU_SRA", false,-1, 3,0);
    tracep->declBus(c+89,"ALU_LUI", false,-1, 3,0);
    tracep->popNamePrefix(1);
    tracep->pushNamePrefix("datamem ");
    tracep->declBus(c+90,"STORAGE_WIDTH", false,-1, 31,0);
    tracep->declBus(c+91,"ADDR_WIDTH", false,-1, 31,0);
    tracep->declBit(c+75,"clk_i", false,-1);
    tracep->declBit(c+62,"mem_read_i", false,-1);
    tracep->declBit(c+47,"mem_write_i", false,-1);
    tracep->declBus(c+58,"addr_i", false,-1, 31,0);
    tracep->declBus(c+54,"write_data_i", false,-1, 31,0);
    tracep->declBus(c+51,"access_ctrl_i", false,-1, 2,0);
    tracep->declBus(c+59,"read_data_o", false,-1, 31,0);
    tracep->declBus(c+63,"size", false,-1, 1,0);
    tracep->declBit(c+64,"zero_extend", false,-1);
    tracep->declBus(c+65,"addr", false,-1, 16,0);
    tracep->pushNamePrefix("unnamedblk1 ");
    tracep->declBus(c+1,"file", false,-1, 31,0);
    tracep->popNamePrefix(1);
    tracep->pushNamePrefix("unnamedblk2 ");
    tracep->declBus(c+66,"b", false,-1, 7,0);
    tracep->popNamePrefix(1);
    tracep->pushNamePrefix("unnamedblk3 ");
    tracep->declBus(c+67,"lo", false,-1, 7,0);
    tracep->declBus(c+68,"hi", false,-1, 7,0);
    tracep->declBus(c+69,"half", false,-1, 15,0);
    tracep->popNamePrefix(2);
    tracep->pushNamePrefix("imem ");
    tracep->declBus(c+78,"DATA_WIDTH", false,-1, 31,0);
    tracep->declBus(c+92,"ADDR_WIDTH", false,-1, 31,0);
    tracep->declBus(c+35,"addr", false,-1, 31,0);
    tracep->declBus(c+36,"instr", false,-1, 31,0);
    tracep->popNamePrefix(1);
    tracep->pushNamePrefix("immgen ");
    tracep->declBus(c+78,"DATA_WIDTH", false,-1, 31,0);
    tracep->declBus(c+70,"Immediate", false,-1, 31,7);
    tracep->declBus(c+50,"ImmSrc", false,-1, 2,0);
    tracep->declBus(c+37,"ImmExt", false,-1, 31,0);
    tracep->popNamePrefix(1);
    tracep->pushNamePrefix("pc_unit ");
    tracep->declBus(c+78,"DATA_WIDTH", false,-1, 31,0);
    tracep->declBit(c+75,"clk", false,-1);
    tracep->declBit(c+76,"rst", false,-1);
    tracep->declBus(c+48,"pcsrc", false,-1, 1,0);
    tracep->declBus(c+37,"immop", false,-1, 31,0);
    tracep->declBus(c+58,"alu_out", false,-1, 31,0);
    tracep->declBus(c+35,"pc", false,-1, 31,0);
    tracep->declBus(c+71,"pc_plus4", false,-1, 31,0);
    tracep->declBus(c+72,"pc_target", false,-1, 31,0);
    tracep->declBus(c+73,"pc_next", false,-1, 31,0);
    tracep->declBus(c+74,"jalr_target", false,-1, 31,0);
    tracep->popNamePrefix(1);
    tracep->pushNamePrefix("regfile ");
    tracep->declBus(c+78,"DATA_WIDTH", false,-1, 31,0);
    tracep->declBus(c+93,"ADDR_WIDTH", false,-1, 31,0);
    tracep->declBus(c+78,"NUM_REGS", false,-1, 31,0);
    tracep->declBit(c+75,"clk", false,-1);
    tracep->declBit(c+44,"we3", false,-1);
    tracep->declBus(c+41,"ad1", false,-1, 4,0);
    tracep->declBus(c+42,"ad2", false,-1, 4,0);
    tracep->declBus(c+39,"ad3", false,-1, 4,0);
    tracep->declBus(c+56,"wd3", false,-1, 31,0);
    tracep->declBus(c+53,"rd1", false,-1, 31,0);
    tracep->declBus(c+54,"rd2", false,-1, 31,0);
    tracep->declBus(c+94,"testRegAddress", false,-1, 4,0);
    tracep->declBus(c+77,"testRegData", false,-1, 31,0);
    for (int i = 0; i < 32; ++i) {
        tracep->declBus(c+3+i*1,"reg_file", true,(i+0), 31,0);
    }
    tracep->popNamePrefix(2);
}

VL_ATTR_COLD void Vdut___024root__trace_init_top(Vdut___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root__trace_init_top\n"); );
    // Body
    Vdut___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vdut___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vdut___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vdut___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void Vdut___024root__trace_register(Vdut___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root__trace_register\n"); );
    // Body
    tracep->addFullCb(&Vdut___024root__trace_full_top_0, vlSelf);
    tracep->addChgCb(&Vdut___024root__trace_chg_top_0, vlSelf);
    tracep->addCleanupCb(&Vdut___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vdut___024root__trace_full_sub_0(Vdut___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vdut___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root__trace_full_top_0\n"); );
    // Init
    Vdut___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vdut___024root*>(voidSelf);
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vdut___024root__trace_full_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vdut___024root__trace_full_sub_0(Vdut___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vdut__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdut___024root__trace_full_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullIData(oldp+1,(vlSelf->top__DOT__datamem__DOT__unnamedblk1__DOT__file),32);
    bufp->fullIData(oldp+2,(vlSelf->top__DOT__cycle),32);
    bufp->fullIData(oldp+3,(vlSelf->top__DOT__regfile__DOT__reg_file[0]),32);
    bufp->fullIData(oldp+4,(vlSelf->top__DOT__regfile__DOT__reg_file[1]),32);
    bufp->fullIData(oldp+5,(vlSelf->top__DOT__regfile__DOT__reg_file[2]),32);
    bufp->fullIData(oldp+6,(vlSelf->top__DOT__regfile__DOT__reg_file[3]),32);
    bufp->fullIData(oldp+7,(vlSelf->top__DOT__regfile__DOT__reg_file[4]),32);
    bufp->fullIData(oldp+8,(vlSelf->top__DOT__regfile__DOT__reg_file[5]),32);
    bufp->fullIData(oldp+9,(vlSelf->top__DOT__regfile__DOT__reg_file[6]),32);
    bufp->fullIData(oldp+10,(vlSelf->top__DOT__regfile__DOT__reg_file[7]),32);
    bufp->fullIData(oldp+11,(vlSelf->top__DOT__regfile__DOT__reg_file[8]),32);
    bufp->fullIData(oldp+12,(vlSelf->top__DOT__regfile__DOT__reg_file[9]),32);
    bufp->fullIData(oldp+13,(vlSelf->top__DOT__regfile__DOT__reg_file[10]),32);
    bufp->fullIData(oldp+14,(vlSelf->top__DOT__regfile__DOT__reg_file[11]),32);
    bufp->fullIData(oldp+15,(vlSelf->top__DOT__regfile__DOT__reg_file[12]),32);
    bufp->fullIData(oldp+16,(vlSelf->top__DOT__regfile__DOT__reg_file[13]),32);
    bufp->fullIData(oldp+17,(vlSelf->top__DOT__regfile__DOT__reg_file[14]),32);
    bufp->fullIData(oldp+18,(vlSelf->top__DOT__regfile__DOT__reg_file[15]),32);
    bufp->fullIData(oldp+19,(vlSelf->top__DOT__regfile__DOT__reg_file[16]),32);
    bufp->fullIData(oldp+20,(vlSelf->top__DOT__regfile__DOT__reg_file[17]),32);
    bufp->fullIData(oldp+21,(vlSelf->top__DOT__regfile__DOT__reg_file[18]),32);
    bufp->fullIData(oldp+22,(vlSelf->top__DOT__regfile__DOT__reg_file[19]),32);
    bufp->fullIData(oldp+23,(vlSelf->top__DOT__regfile__DOT__reg_file[20]),32);
    bufp->fullIData(oldp+24,(vlSelf->top__DOT__regfile__DOT__reg_file[21]),32);
    bufp->fullIData(oldp+25,(vlSelf->top__DOT__regfile__DOT__reg_file[22]),32);
    bufp->fullIData(oldp+26,(vlSelf->top__DOT__regfile__DOT__reg_file[23]),32);
    bufp->fullIData(oldp+27,(vlSelf->top__DOT__regfile__DOT__reg_file[24]),32);
    bufp->fullIData(oldp+28,(vlSelf->top__DOT__regfile__DOT__reg_file[25]),32);
    bufp->fullIData(oldp+29,(vlSelf->top__DOT__regfile__DOT__reg_file[26]),32);
    bufp->fullIData(oldp+30,(vlSelf->top__DOT__regfile__DOT__reg_file[27]),32);
    bufp->fullIData(oldp+31,(vlSelf->top__DOT__regfile__DOT__reg_file[28]),32);
    bufp->fullIData(oldp+32,(vlSelf->top__DOT__regfile__DOT__reg_file[29]),32);
    bufp->fullIData(oldp+33,(vlSelf->top__DOT__regfile__DOT__reg_file[30]),32);
    bufp->fullIData(oldp+34,(vlSelf->top__DOT__regfile__DOT__reg_file[31]),32);
    bufp->fullIData(oldp+35,(vlSelf->top__DOT__pc),32);
    bufp->fullIData(oldp+36,(vlSelf->top__DOT__instr),32);
    bufp->fullIData(oldp+37,(vlSelf->top__DOT__immext),32);
    bufp->fullCData(oldp+38,((0x7fU & vlSelf->top__DOT__instr)),7);
    bufp->fullCData(oldp+39,((0x1fU & (vlSelf->top__DOT__instr 
                                       >> 7U))),5);
    bufp->fullCData(oldp+40,((7U & (vlSelf->top__DOT__instr 
                                    >> 0xcU))),3);
    bufp->fullCData(oldp+41,((0x1fU & (vlSelf->top__DOT__instr 
                                       >> 0xfU))),5);
    bufp->fullCData(oldp+42,((0x1fU & (vlSelf->top__DOT__instr 
                                       >> 0x14U))),5);
    bufp->fullCData(oldp+43,((vlSelf->top__DOT__instr 
                              >> 0x19U)),7);
    bufp->fullBit(oldp+44,(vlSelf->top__DOT__RegWrite));
    bufp->fullCData(oldp+45,(vlSelf->top__DOT__ALUControl),4);
    bufp->fullBit(oldp+46,(vlSelf->top__DOT__ALUSrc));
    bufp->fullBit(oldp+47,(vlSelf->top__DOT__MemWrite));
    bufp->fullCData(oldp+48,(vlSelf->top__DOT__PCSrc),2);
    bufp->fullCData(oldp+49,(vlSelf->top__DOT__ResultSrc),2);
    bufp->fullCData(oldp+50,(vlSelf->top__DOT__ImmSrc),3);
    bufp->fullCData(oldp+51,(vlSelf->top__DOT__AddressingControl),3);
    bufp->fullBit(oldp+52,((0U == vlSelf->top__DOT__alu_out)));
    bufp->fullIData(oldp+53,(vlSelf->top__DOT__rd1),32);
    bufp->fullIData(oldp+54,(vlSelf->top__DOT__rd2),32);
    bufp->fullIData(oldp+55,(vlSelf->top__DOT__result),32);
    bufp->fullIData(oldp+56,(vlSelf->top__DOT__wd_safe),32);
    bufp->fullIData(oldp+57,(vlSelf->top__DOT__srcb),32);
    bufp->fullIData(oldp+58,(vlSelf->top__DOT__alu_out),32);
    bufp->fullIData(oldp+59,(vlSelf->top__DOT__read_data),32);
    bufp->fullIData(oldp+60,(((IData)(4U) + vlSelf->top__DOT__pc)),32);
    bufp->fullIData(oldp+61,(((1U == (IData)(vlSelf->top__DOT__ResultSrc))
                               ? vlSelf->top__DOT__read_data
                               : 0U)),32);
    bufp->fullBit(oldp+62,((1U == (IData)(vlSelf->top__DOT__ResultSrc))));
    bufp->fullCData(oldp+63,((3U & (IData)(vlSelf->top__DOT__AddressingControl))),2);
    bufp->fullBit(oldp+64,((1U & ((IData)(vlSelf->top__DOT__AddressingControl) 
                                  >> 2U))));
    bufp->fullIData(oldp+65,((0x1ffffU & vlSelf->top__DOT__alu_out)),17);
    bufp->fullCData(oldp+66,(vlSelf->top__DOT__datamem__DOT__unnamedblk2__DOT__b),8);
    bufp->fullCData(oldp+67,(vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__lo),8);
    bufp->fullCData(oldp+68,(vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__hi),8);
    bufp->fullSData(oldp+69,(vlSelf->top__DOT__datamem__DOT__unnamedblk3__DOT__half),16);
    bufp->fullIData(oldp+70,((vlSelf->top__DOT__instr 
                              >> 7U)),25);
    bufp->fullIData(oldp+71,(((IData)(4U) + vlSelf->top__DOT__pc)),32);
    bufp->fullIData(oldp+72,((vlSelf->top__DOT__pc 
                              + vlSelf->top__DOT__immext)),32);
    bufp->fullIData(oldp+73,(((0U == (IData)(vlSelf->top__DOT__PCSrc))
                               ? ((IData)(4U) + vlSelf->top__DOT__pc)
                               : ((1U == (IData)(vlSelf->top__DOT__PCSrc))
                                   ? (vlSelf->top__DOT__pc 
                                      + vlSelf->top__DOT__immext)
                                   : ((2U == (IData)(vlSelf->top__DOT__PCSrc))
                                       ? (0xfffffffeU 
                                          & vlSelf->top__DOT__alu_out)
                                       : 0U)))),32);
    bufp->fullIData(oldp+74,((0xfffffffeU & vlSelf->top__DOT__alu_out)),32);
    bufp->fullBit(oldp+75,(vlSelf->clk));
    bufp->fullBit(oldp+76,(vlSelf->rst));
    bufp->fullIData(oldp+77,(vlSelf->a0),32);
    bufp->fullIData(oldp+78,(0x20U),32);
    bufp->fullCData(oldp+79,(0U),4);
    bufp->fullCData(oldp+80,(1U),4);
    bufp->fullCData(oldp+81,(2U),4);
    bufp->fullCData(oldp+82,(3U),4);
    bufp->fullCData(oldp+83,(4U),4);
    bufp->fullCData(oldp+84,(5U),4);
    bufp->fullCData(oldp+85,(6U),4);
    bufp->fullCData(oldp+86,(7U),4);
    bufp->fullCData(oldp+87,(8U),4);
    bufp->fullCData(oldp+88,(0xbU),4);
    bufp->fullCData(oldp+89,(0xfU),4);
    bufp->fullIData(oldp+90,(8U),32);
    bufp->fullIData(oldp+91,(0x11U),32);
    bufp->fullIData(oldp+92,(0xaU),32);
    bufp->fullIData(oldp+93,(5U),32);
    bufp->fullCData(oldp+94,(0xaU),5);
}
