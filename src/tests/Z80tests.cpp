#include "Z80tests.hpp"

void Z80Tests::returnFinalState() {
    std::cout << "\nFinal state:\n";
    std::cout << "A: 0x" << (int)(cpu.getA()) << "\n";
    std::cout << "B: 0x" << (int)(cpu.getB()) << "\n";
    std::cout << "C: 0x" << (int)(cpu.getC()) << "\n";
    std::cout << "D: 0x" << (int)(cpu.getD()) << "\n";
    std::cout << "E: 0x" << (int)(cpu.getE()) << "\n";
    std::cout << "H: 0x" << (int)(cpu.getH()) << "\n";
    std::cout << "L: 0x" << (int)(cpu.getL()) << "\n";
    std::cout << "HL: 0x" << (int)(cpu.readByte(cpu.getHL())) << "\n\n";
    std::cout << "A': 0x" << (int)(cpu.getA_P()) << "\n";
    std::cout << "B': 0x" << (int)(cpu.getB_P()) << "\n";
    std::cout << "C': 0x" << (int)(cpu.getC_P()) << "\n";
    std::cout << "D': 0x" << (int)(cpu.getD_P()) << "\n";
    std::cout << "E': 0x" << (int)(cpu.getE_P()) << "\n";
    std::cout << "H': 0x" << (int)(cpu.getH_P()) << "\n";
    std::cout << "L': 0x" << (int)(cpu.getL_P()) << "\n\n";

}

void Z80Tests::runAllTests() {
    test8BitLoads();
    test16BitLoads();
    testExchangeOps();
    test8BitArithmetic();
    testLogicalOps();
    testCompareOps();
    testIncDec();
    testJumpOps();
    testCallReturn();
    testStackOps();
    testIndexedOps();
    testFlagOps();
    testConditionalOps();
    testConditionalJump();
    std::cout << "\nAll tests passed\n\n";
}

void Z80Tests::loadProgram(const std::vector<uint8_t>& program) {
    cpu.reset();
    std::cout << "Program:\n\n";
    for (uint16_t i = 0; i < (uint16_t)program.size(); i++) {
        cpu.writeByte(i, program[i]);
        std::cout << "[0x" << std::hex << i << "] = 0x" << static_cast<int>(program[i]) << "\n";
    }
    std::cout << "\n\n";
}

void Z80Tests::executeUntilHalt() {
    while (cpu.readByte(cpu.getPC()) != HALT) {
        uint16_t prev_pc = cpu.getPC();
        uint8_t opcode = cpu.readByte(cpu.getPC());

        cpu.step();

        std::cout << "Executed opcode 0x" << std::hex 
            << static_cast<int>(opcode) 
            << " at 0x" << prev_pc << "\n";
        std::cout << "Registers: AF=0x" << std::hex << cpu.getAF()
            << std::hex << " BC=0x" << cpu.getBC()
            <<std::hex << " DE=0x" << cpu.getDE()
            << std::hex << " HL=0x" << cpu.getHL()
            << std::hex << "\n";
        std::cout << "Flags: " 
            << (cpu.getF() & Z80::S_FLAG ? "S" : "-")
            << (cpu.getF() & Z80::Z_FLAG ? "Z" : "-")
            << (cpu.getF() & Z80::H_FLAG ? "H" : "-")
            << (cpu.getF() & Z80::PV_FLAG ? "PV" : "-")
            << (cpu.getF() & Z80::N_FLAG ? "N" : "-")
            << (cpu.getF() & Z80::C_FLAG ? "C" : "-")
            << "\n\n";
    }
    cpu.step();
}

void Z80Tests::test8BitLoads() {
    cpu.reset();
    std::cout << "8-bit Load Instructions:\n";

    loadProgram({
        LD_A_N, 0x00,     // LD A, 0x00
        LD_B_N, 0x11,     // LD B, 0x11
        LD_C_N, 0x22,     // LD C, 0x22
        LD_D_N, 0x33,     // LD D, 0x33
        LD_E_N, 0x44,     // LD E, 0x44
        LD_H_N, 0x55,     // LD H, 0x55
        LD_L_N, 0x66,     // LD L, 0x66
        LD_HL_N, 0x77,    // LD HL, 0x77
        HALT              // HALT
        });

    std::cout << "Executing test:\n";
    executeUntilHalt();
    returnFinalState();

    assert(cpu.getA() == 0x00);
    assert(cpu.getB() == 0x11);
    assert(cpu.getC() == 0x22);
    assert(cpu.getD() == 0x33);
    assert(cpu.getE() == 0x44);
    assert(cpu.getH() == 0x55);
    assert(cpu.getL() == 0x66);
    assert(cpu.readByte(cpu.getHL()) == 0x77);

    std::cout << "Test passed\n";
}

void Z80Tests::test16BitLoads() {
    cpu.reset();
    std::cout << "16-bit Load Instructions:\n";
    loadProgram({
        LD_BC_NN, 0x34, 0x12,               // LD BC, 0x1234
        LD_DE_NN, 0x78, 0x56,               // LD DE, 0x5678
        LD_HL_NN, 0xBC, 0x9A,               // LD HL, 0x9ABC
        LD_SP_NN, 0x11, 0x11,               // LD SP, 0x1111
        PREFIX_DD, LD_IXY, 0xCD, 0xAB,      // LD IX, 0xABCD
        PREFIX_FD, LD_IXY, 0xEF, 0xCD,      // LD IY, 0xCDEF
        HALT                                // HALT
        });

    std::cout << "Executing test:\n";
    executeUntilHalt();
    returnFinalState();

    assert(cpu.getBC() == 0x1234);
    assert(cpu.getDE() == 0x5678);
    assert(cpu.getHL() == 0x9ABC);
    assert(cpu.getSP() == 0x1111);
    assert(cpu.getIX() == 0xABCD);
    assert(cpu.getIY() == 0xCDEF);

    std::cout << "Test passed\n";

}

void Z80Tests::testExchangeOps() {
    cpu.reset();
    std::cout << "Exchange operations:\n";
    loadProgram({
        LD_HL_NN, 0x34, 0x12,   // LD HL, 0x1234
        LD_DE_NN, 0x78, 0x56,   // LD DE, 0x5678
        EX_DE_HL,               // EX DE, HL
        EX_AF_AF,               // EX AF, AF'
        EXX,                    // EXX
        HALT                    // HALT
        });

    std::cout << "Executing test:\n";
    executeUntilHalt();
    returnFinalState();
    // EXX
    assert(cpu.getHL() == 0x0000);     
    assert(cpu.getBC() == 0x0000);
    assert(cpu.getDE() == 0x0000);

    assert(cpu.getHL_P() == 0x5678);
    assert(cpu.getDE_P() == 0x1234);
    assert(cpu.getBC_P() == 0x0000);

    std::cout << "Test passed\n";

}

void Z80Tests::test8BitArithmetic() {
    cpu.reset();
    std::cout << "8-bit Arithmetic:";
    //loadProgram({
    //    LD_A_N,  0xF0,     // LD A, 0xF0
    //    LD_B_N, 0x30,     // LD B, 0x30
    //    ADD_A_B,           // ADD A, B
    //    ADC_A_A,           // ADC A, A 
    //    SUB_B,           // SUB B
    //    SBC_A_B,           // SBC A,B 
    //    ADD_A_N, 0x20,     // ADD A, 0x20
    //    ADC_A_N, 0x10,     // ADC A, 0x10
    //    SUB_N, 0x15,     // SUB 0x15
    //    SBC_A_N, 0x05,     // SBC A, 0x05
    //    HALT            // HALT
    //    });

    loadProgram({
        LD_A_N, 0x7F,
        ADD_A_N, 0x01,
        HALT
        });

    std::cout << "Executing test:\n";
    executeUntilHalt();
    returnFinalState();

    assert(cpu.getA() == 0x80);

    std::cout << "Test passed\n";
}

void Z80Tests::testLogicalOps() {
    cpu.reset();
    std::cout << "Logical operations:\n";
    loadProgram( {
        LD_A_N, 0xAA,         // LD A, 0xAA
        LD_B_N, 0x55,         // LD B, 0x55
        AND_B,               // AND B
        OR_B,               // OR B
        XOR_B,               // XOR B
        AND_N, 0xF0,         // AND 0xF0
        OR_N, 0x0F,         // OR 0x0F
        XOR_N, 0xFF,         // XOR 0xFF
        HALT                // HALT
        });

    executeUntilHalt();
    returnFinalState();
    
    assert(cpu.getA() == 0xF0);
    assert((cpu.getF() & Z80::Z_FLAG) == 0);

    std::cout << "Test passed\n";
}

void Z80Tests::testCompareOps() {
    cpu.reset();
    std::cout << "Compare operations:\n";
    loadProgram( {
        LD_A_N, 0x50,         // LD A, 0x50
        LD_B_N, 0x30,         //LD B, 0x30
        CP_B,               // CP B
        CP_N, 0x50,         // CP 0x50
        SCF,               // SCF
        DAA,               // DAA
        HALT                // HALT
        });
    std::cout << "Executing test:\n";
    //LD A,0x50
    cpu.step();
    // LD B,0x30
    cpu.step();

    // CP B
    cpu.step(); 
    assert(cpu.getA() == 0x50); 
    assert((cpu.getF() & Z80::N_FLAG) != 0);
    assert((cpu.getF() & Z80::Z_FLAG) == 0); 
    assert((cpu.getF() & Z80::C_FLAG) == 0);

    //CP 0x50
    cpu.step(); 
    assert((cpu.getF() & Z80::Z_FLAG) != 0); 
    assert((cpu.getF() & Z80::N_FLAG) != 0); 

    // SCF
    cpu.step(); 
    assert(cpu.getA() == 0x50);
    assert((cpu.getF() & Z80::C_FLAG) != 0); 
    assert((cpu.getF() & Z80::N_FLAG) == 0); 

    // DAA
    cpu.step(); 
    assert(cpu.getA() == 0xB0);
    assert((cpu.getF() & Z80::S_FLAG) != 0); 
    assert((cpu.getF() & Z80::C_FLAG) != 0); 

    std::cout << "Test passed\n";
}

void Z80Tests::testIncDec() {
    cpu.reset();
    std::cout << "Increment decrement instructions:\n";
    loadProgram({
        LD_A_N, 0x7F,         // LD A ,0x7F
        LD_B_N, 0x01,         // LD B, 0x01
        LD_C_N, 0xFF,         // LD C, 0xFF
        INC_A,               // INC A 
        INC_B,               // INC B 
        DEC_C,               // DEC C 
        LD_HL_NN, 0x00, 0x10,   // LD HL, 0x1000
        LD_HL_N, 0x42,         // LD HL, 0x42
        INC_HL,               // INC HL
        DEC_HL,               // DEC HL
        HALT                // HALT
        });

    std::cout << "Executing test:\n";
    
    // LD A, 0x7F
    cpu.step();
    std::cout << "hello 0x" << (int)cpu.getA() << "\n";
    assert(cpu.getA() == 0x7F);

    // LD B, 0x01
    cpu.step();
    assert(cpu.getB() == 0x01);

    // LD C, 0xFF
    cpu.step();
    assert(cpu.getC() == 0xFF);

    // INC A 
    cpu.step();
    assert(cpu.getA() == 0x80);
    assert((cpu.getF() & Z80::PV_FLAG) == Z80::PV_FLAG);
    assert((cpu.getF() & Z80::S_FLAG) == Z80::S_FLAG);
    assert((cpu.getF() & Z80::H_FLAG) == Z80::H_FLAG);

    // INC B 
    cpu.step();
    assert(cpu.getB() == 0x02);

    // DEC C 
    cpu.step();
    assert(cpu.getC() == 0xFE);
    assert((cpu.getF() & Z80::N_FLAG) == Z80::N_FLAG);
    
    executeUntilHalt();
    assert(cpu.readByte(0x1000) == 0x42);
    std::cout << "Test passed\n";
}

void Z80Tests::testJumpOps() {
    cpu.reset();
    std::cout << "Jump operations:\n";
    loadProgram({
        JR, 0x04,      //  JR 0x04
        OR_A,          //  OR A
        JR_Z, 0xFD,    //  JR_Z 0xFD
        HALT,          //  HALT    
        LD_A_N, 0x10,  //  LD A, 0x10  
        JR, 0xF8       //  JR 0xF8  
        });
    std::cout << "Executing test:\n";
    executeUntilHalt();
    returnFinalState();

    assert(cpu.getA() == 0x10);
    std::cout << "Test passed\n";
}

void Z80Tests::testCallReturn() {
    cpu.reset();
    std::cout << "Call Return instructions:\n";
    loadProgram({
        LD_SP_NN, 0x00, 0x20,           // LD SP, 0x2000
        CALL_NN, 0x07, 0x00,           // CALL 0x0007
        HALT,                       // HALT (unreachable)
        RET,                       // RET
        HALT                        // HALT 
        });
    std::cout << "Executing test:\n";
    executeUntilHalt();
    returnFinalState();
    assert(cpu.readByte(0x2000 - 1) == 0x00); 
    assert(cpu.readByte(0x2000 - 2) == 0x06);
    assert(cpu.getSP() == 0x2000);
    std::cout << "Test passed\n";
}

void Z80Tests::testStackOps() {
    cpu.reset();
    std::cout << "Stack operations:\n";
    loadProgram({
        LD_SP_NN, 0x00, 0x10,       // LD SP, 0x1000
        LD_BC_NN, 0x34, 0x12,       // LD BC, 0x1234
        PUSH_BC,                   // PUSH BC
        POP_DE,                   // POP DE
        HALT                    // HALT
        });
    std::cout << "Executing test:\n";
    executeUntilHalt();
    returnFinalState();

    assert(cpu.getDE() == 0x1234);
    assert(cpu.getSP() == 0x1000);

    std::cout << "Test passed\n";
}

void Z80Tests::testIndexedOps() {
    cpu.reset();
    std::cout << "Indexed operations:\n";
    loadProgram( {
        PREFIX_DD, LD_IXY, 0x00, 0x10,          // LD IX, 0x1000
        PREFIX_FD, LD_IXY, 0x00, 0x20,          // LD IY, 0x2000
        PREFIX_DD, LD_IXY_d, 0x05, 0xAA,        // LD (IX+5),0xAA
        PREFIX_FD, LD_IXY_d, 0x05, 0x55,        // LD (IY+5),0x55
        PREFIX_DD, 0x7E, 0x05,                  // LD A, (IX+5)
        PREFIX_FD, ADD, 0x05,                   // ADD A, (IY+5)
        HALT                                    // HALT
        });
    std::cout << "Executing test:\n";
    executeUntilHalt();
    returnFinalState();

    assert(cpu.readByte(0x1005) == 0xAA);   // LD (IX+5),0xAA
    assert(cpu.readByte(0x2005) == 0x55);   // LD (IY+5),0x55
    assert(cpu.getA() == 0xFF);             // 0xAA + 0x55 = 0xFF

    std::cout << "Test passed\n";
}

void Z80Tests::testFlagOps() {
    cpu.reset();
    std::cout << "Flag operations:\n";
    loadProgram({
        LD_A_N, 0x10,    // LD A, 0x10
        SCF,             // Set Carry Flag
        DAA,             
        OR_A,           
        LD_A_N, 0x91,    // LD A, 0x91
        DAA,             
        HALT
        });
    std::cout << "Executing test:\n";
    executeUntilHalt();
    returnFinalState();
    std::cout << (int)cpu.getA();
    assert(cpu.getA() == 0x91);
    assert((cpu.getF() & Z80::C_FLAG) == 0);
    std::cout << "Test passed\n";
}

void Z80Tests::testConditionalOps() {
    std::cout << "Conditional operations:\n";
    cpu.reset();
    loadProgram({
        LD_A_N, 0x00,         // LD A, 0x00
        OR_A,               // OR A -> Z flag
        JP_Z, 0x0C, 0x00,   // JP Z, 0x000C 
        0x00,               // NOP (skipped)
        LD_A_N, 0x01,         // LD A,0x01 (skipped)
        JP_NN, 0x0F, 0x00,   // JP 0x000F 
        LD_A_N, 0x04,         // address 0x000C: LD A, 0x04 
        HALT                // HALT
        });
    std::cout << "Executing test:\n";
    executeUntilHalt();
    returnFinalState();
    assert(cpu.getA() == 0x04);
    std::cout << "Test passed\n";
}

void Z80Tests::testConditionalJump() {
    cpu.reset();
    std::cout << "Conditional jump test:\n";

    loadProgram({
        LD_B_N, 0x03,     // LD B, 0x03
        JR_NZ, 0x02,     // JR NZ, 0x02 (should jump to LD A, 0xFF)
        DEC_B,           // DEC B
        HALT,           // HALT
        LD_A_N, 0xFF,     // LD A, 0xff
        JR, 0xF9      // JR 0xF9 (should jump to DEC B)
        });
    std::cout << "Executing test:\n";
    executeUntilHalt();
    returnFinalState();
    

    assert(cpu.getB() == 0x02);  
    assert(cpu.getA() == 0xFF);
    std::cout << "Test passed\n";
}


