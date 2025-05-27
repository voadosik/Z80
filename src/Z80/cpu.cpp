#include "../include/cpu.hpp"


/*
Notation:

r - Identifies any of the registers A, B, C, D, E, H, or L.

(HL) - Identifies the contents of the memory location, 
whose address is specified by the contents of the register pair HL.

(IX+d) - Identifies the contents of the memory location, whose address is specified by 
the contents of the Index register pair IX plus the signed displacement d.

(IY+d) - Identifies the contents of the memory location, whose address is specified by 
the contents of the Index register pair IY plus the signed displacement d.

n -  Identifies a one-byte unsigned integer expression in the range (0 to 255).

nn - Identifies a two-byte unsigned integer expression in the range (0 to 65535).

d -  Identifies a one-byte signed integer expression in the range (-128 to +127).

b -  Identifies a one-bit expression in the range (0 to 7).  The most-significant bit 
to the left is bit 7 and the least-significant bit to the right is bit 0.

e - Identifies a one-byte signed integer expression in the range (-126 to +129)
for relative jump offset from current location.

cc - Identifies the status of the Flag Register as any of (NZ, Z, NC, C, PV or H)
for the conditional jumps, calls, and return instructions.

*/


Z80::Z80() { reset(); }

/**
 * @brief Reset CPU to initial state
 * Resets all registers, program counter and stack pointer
 * Clears memory
 */
void Z80::reset() {
    af = bc = de = hl = 0;
    af_prime = bc_prime = de_prime = hl_prime = 0;
    pc = sp = ix = iy = 0;
    halted = false;
    std::fill(std::begin(memory), std::end(memory), 0);
}


uint8_t Z80::readByte(uint16_t addr) const {
    return memory[addr];
}


void Z80::writeByte(uint16_t addr, uint8_t value) {
    memory[addr] = value;
}

/**
* Single instruction execution:
* Fetch opcode from memory at PC
* Handle prefixes (DD/FD for IX/IY) if present
* Call opcode handler
*/
void Z80::step() {
    if (halted) return;
    uint8_t opcode = readByte(pc++);

    if (opcode == PREFIX_DD || opcode == PREFIX_FD) {
        uint8_t prefix = opcode;
        uint8_t nextOp = readByte(pc++);
        handlePrefixedOpcode(prefix, nextOp);
    }
    else {
        handleOpcode(opcode);
    }
}


/**
 * Handle prefixed opcodes:
 * IX/IY operations like ADD A,(IX+d)
 * LD instructions for IX/IY
 * @param prefix 0xDD (for IX) or 0xFD (for IY)
 * @param opcode actual instruction after prefix
 */
void Z80::handlePrefixedOpcode(uint8_t prefix, uint8_t opcode) {
    
    switch (opcode) {
        case ADD: handleAdd(prefix); break;
        case ADC: handleAdc(prefix); break;
        case SUB: handleSub(prefix); break;
        case SBC: handleSbc(prefix); break;
        case AND: handleAnd(prefix); break;
        case OR: handleOr(prefix); break;
        case XOR: handleXor(prefix); break;
        case CP: handleCp(prefix); break;
        case INC: handleIncMem(prefix); break;
        case DEC: handleDecMem(prefix); break;
    
        // LD IX/IY,nn
        case LD_IXY: 
            if (prefix == PREFIX_DD) {
                ix = readByte(pc++) | (readByte(pc++) << 8);
            }
            else {
                iy = readByte(pc++) | (readByte(pc++) << 8);
            }
            break;

        // LD (IX/IY+d),n
        case LD_IXY_d: { 
            int8_t d = readByte(pc++);
            uint8_t n = readByte(pc++);
            uint16_t addr = prefix == PREFIX_DD ? ix + d : iy + d;
            writeByte(addr, n);
            break;
        }

        // Handle IX/IY LD operations (40-7F range)
        default:
            if ((opcode & 0xC0) == 0x40) {
                handleIndexedLd(prefix, opcode);
            }
            break;
    }
}
/**
 * Main opcode handler:
 * Processes all non-prefixed instructions via a switch-case
 * and helper functions for instructions
 * @param opcode instruction to execute
 */
void Z80::handleOpcode(uint8_t opcode) {
    switch (opcode) {
        // ADD A, r
        case ADD_A_B: addA(b); break;
        case ADD_A_C: addA(c); break;
        case ADD_A_D: addA(d); break;
        case ADD_A_E: addA(e); break;
        case ADD_A_H: addA(h); break;
        case ADD_A_L: addA(l); break;
        case ADD_A_HL: addA(readByte(hl)); break;
        case ADD_A_A: addA(a); break;
        // ADD A, n
        case ADD_A_N: addA(readByte(pc++)); break;

        // ADC A, s
        case ADC_A_B: adcA(b); break;
        case ADC_A_C: adcA(c); break;
        case ADC_A_D: adcA(d); break;
        case ADC_A_E: adcA(e); break;
        case ADC_A_H: adcA(h); break;
        case ADC_A_L: adcA(l); break;
        case ADC_A_HL: adcA(readByte(hl)); break;
        case ADC_A_A: adcA(a); break;
        // ADC A, n
        case ADC_A_N: adcA(readByte(pc++)); break;

        // SUB s
        case SUB_B: sub(b); break;
        case SUB_C: sub(c); break;
        case SUB_D: sub(d); break;
        case SUB_E: sub(e); break;
        case SUB_H: sub(h); break;
        case SUB_L: sub(l); break;
        case SUB_HL: sub(readByte(hl)); break;
        case SUB_A: sub(a); break;
        // SUB n
        case SUB_N: sub(readByte(pc++)); break;

        // SBC A, s
        case SBC_A_B: sbcA(b); break;
        case SBC_A_C: sbcA(c); break;
        case SBC_A_D: sbcA(d); break;
        case SBC_A_E: sbcA(e); break;
        case SBC_A_H: sbcA(h); break;
        case SBC_A_L: sbcA(l); break;
        case SBC_A_HL: sbcA(readByte(hl)); break;
        case SBC_A_A: sbcA(a); break;
        // SBC A, n
        case SBC_A_N: sbcA(readByte(pc++)); break;

        // AND s
        case AND_B: andA(b); break;
        case AND_C: andA(c); break;
        case AND_D: andA(d); break;
        case AND_E: andA(e); break;
        case AND_H: andA(h); break;
        case AND_L: andA(l); break;
        case AND_HL: andA(readByte(hl)); break;
        case AND_A: andA(a); break;
        // AND n
        case AND_N: andA(readByte(pc++)); break;

        // OR s
        case OR_B: orA(b); break;
        case OR_C: orA(c); break;
        case OR_D: orA(d); break;
        case OR_E: orA(e); break;
        case OR_H: orA(h); break;
        case OR_L: orA(l); break;
        case OR_HL: orA(readByte(hl)); break;
        case OR_A: orA(a); break;
        // OR n
        case OR_N: orA(readByte(pc++)); break;

        // XOR s
        case XOR_B: xorA(b); break;
        case XOR_C: xorA(c); break;
        case XOR_D: xorA(d); break;
        case XOR_E: xorA(e); break;
        case XOR_H: xorA(h); break;
        case XOR_L: xorA(l); break;
        case XOR_HL: xorA(readByte(hl)); break;
        case XOR_A: xorA(a); break;
        // XOR n
        case XOR_N: xorA(readByte(pc++)); break; 

        // CP s
        case CP_B: cp(b); break;
        case CP_C: cp(c); break;
        case CP_D: cp(d); break;
        case CP_E: cp(e); break;
        case CP_H: cp(h); break;
        case CP_L: cp(l); break;
        case CP_HL: cp(readByte(hl)); break;
        case CP_A: cp(a); break;
        // CP n
        case CP_N: cp(readByte(pc++)); break;

        // INC r
        case INC_B: inc(b); break;
        case INC_C: inc(c); break;
        case INC_D: inc(d); break;
        case INC_E: inc(e); break;
        case INC_H: inc(h); break;
        case INC_L: inc(l); break;
        case INC_A: inc(a); break;
        // INC (HL)
        case INC_HL: { 
            uint16_t addr = hl;
            writeByte(addr, inc_(readByte(addr)));
            break;
        }

        // DEC m
        case DEC_B: dec(b); break;
        case DEC_C: dec(c); break;
        case DEC_D: dec(d); break;
        case DEC_E: dec(e); break;
        case DEC_H: dec(h); break;
        case DEC_L: dec(l); break;
        case DEC_A: dec(a); break;
        // DEC (HL)
        case DEC_HL: {
            uint16_t addr = hl;
            writeByte(addr, dec_(readByte(addr)));
            break;
        }

        // 8-bit Loads
        case LD_A_N: // LD A,n
            setReg(Regs::A, readByte(pc++)); break;
        case LD_B_N: // LD B,n
            setReg(Regs::B, readByte(pc++)); break;
        case LD_C_N: // LD C,n
            setReg(Regs::C, readByte(pc++)); break;
        case LD_D_N: // LD D,n
            setReg(Regs::D, readByte(pc++)); break;
        case LD_E_N: // LD E,n
            setReg(Regs::E, readByte(pc++)); break;
        case LD_H_N: // LD H,n
            setReg(Regs::H, readByte(pc++)); break;
        case LD_L_N: // LD L,n
            setReg(Regs::L, readByte(pc++)); break;

        case LD_HL_N: ldHL(); break;


        // 16-bit Loads

        // LD BC,nn
        case LD_BC_NN:
            ld(bc); break;

        // LD DE,nn
        case LD_DE_NN:
            ld(de); break;

        // LD HL,nn    
        case LD_HL_NN:
            ld(hl); break;

        // LD SP,nn
        case LD_SP_NN:
            ld(sp); break;

        // Exchange instructions
        // EX DE,HL
        case EX_DE_HL: {
            std::swap(de, hl);
            break;
        }
        // EX AF,AF'
        case EX_AF_AF: {
            std::swap(af, af_prime);
            break;
        }
        // EXX
        case EXX: {
            exx();
            break;
        }
        // JP nn
        case JP_NN: {
            jp();
            break;
        }

        // JP cc,nn
        case JP_NZ: case JP_Z: case JP_NC: case JP_C:
        case JP_PO: case JP_PE: case JP_P: case JP_M: {
            condJP(opcode);
            break;
        }

        // JR e
        case JR: {
            jr();
            break;
        }

        // JR cc,e
        case JR_NZ: case JR_Z: case JR_NC: case JR_C: {
            condJR(opcode);
            break;
        }

        // CALL nn
        case CALL_NN: {
            call();
            break;
        }

        // CALL cc,nn
        case CALL_NZ: case CALL_Z: case CALL_NC: case CALL_C:
        case CALL_PO: case CALL_PE: case CALL_P: case CALL_M: {
            condCall(opcode);
            break;
        }

        // RET
        case RET:
            pc = pop();
            break;

        // RET cc
        case RET_NZ: case RET_Z: case RET_NC: case RET_C:
        case RET_PO: case RET_PE: case RET_P: case RET_M: {
            condRet(opcode);
            break;
        }

        // PUSH BC
        case PUSH_BC:
            push(bc);
            break;

        // PUSH DE
        case PUSH_DE:
            push(de);
            break;
        
        // PUSH HL
        case PUSH_HL:
            push(hl);
            break;

        // PUSH AF
        case PUSH_AF:
            push(af);
            break;

        // POP BC
        case POP_BC:
            bc = pop();
            break;

        // POP DE
        case POP_DE:
            de = pop();
            break;

        // POP HL
        case POP_HL:
            hl = pop();
            break;

        // POP AF
        case POP_AF:
            af = pop();
            break;
        
        //HALT
        case HALT:
            halt();
            return;
        
        // SCF
        case SCF:
            setCarry();
            break;

        // DAA
        case DAA:
            daa();
            break;

        default:
            // Handle LD r, r', LD r,(HL), LD (HL),r
            // take bits 7-6, check if they are equal to 01
            if ((opcode & 0xC0) == 0x40) {
                handleLd(opcode);
            }
            break; 
    }
}

void Z80::halt() {
    halted = true;
    pc--;
    return;
}

/**
 * Add value to accumulator,
 * Update flags (Zero, Sign, Carry)
 * @param value operand to add
 */
void Z80::addA(uint8_t value) {
    uint8_t original_a = a;
    uint16_t res = original_a + value;
    a = res & 0xFF;
    updateFlagsAdd(original_a, value, res);
}

/**
 * Add with carry:
 * Similar to addA but includes carry flag
 */
void Z80::adcA(uint8_t value) {
    uint8_t original_a = a;
    uint8_t carry = (f & C_FLAG) ? 1 : 0;
    uint16_t res = original_a + value + carry;
    a = res & 0xFF;
    updateFlagsAdd(original_a, value + carry, res);
}

/**
* Subtract value from the accumulator
* Update flags
*/
void Z80::sub(uint8_t value) {
    uint8_t original_a = a;  
    uint16_t res = original_a - value;
    a = res & 0xFF;
    updateFlagsSub(original_a, value, res);
}

/**
* Subtract with carry:
* Similar to SUB A but includes carry flag
*/
void Z80::sbcA(uint8_t value) {
    uint8_t original_a = a;  
    uint8_t carry = (f & C_FLAG) ? 1 : 0;
    uint16_t total_sub = value + carry;
    uint16_t res = original_a - total_sub;
    a = res & 0xFF;
    updateFlagsSub(original_a, total_sub, res); 
}
/**
 * Logical AND:
 * Sets Zero, Sign, Parity flags, Half-carry flag set
 */
void Z80::andA(uint8_t value) {
    a &= value;
    f = H_FLAG | (parityEven(a) ? PV_FLAG : 0);
    updateSZ(a);
}

/**
* Logical OR:
* Updates Sign, Zero, and Parity flags
* Reset Carry, Half-Carry and N flags
*/
void Z80::orA(uint8_t value) {
    a |= value;
    f = 0;
    updateSZ(a);
    f |= parityEven(a) ? PV_FLAG : 0;
}

/**
* Logical XOR:
* Updates Sign, Zero, and Parity flags
* Reset Carry, Half-Carry and N flags
*/
void Z80::xorA(uint8_t value) {
    a ^= value;
    f = 0;
    updateSZ(a);
    f |= parityEven(a) ? PV_FLAG : 0;
}

/**
* Compare:
* Performs A - value
* Updates all flags based on subtraction
*/
void Z80::cp(uint8_t value) {
    uint8_t original_a = a;  
    uint16_t res = original_a - value;
    updateFlagsSub(original_a, value, res);  
}

/**
* Increment:
* reg <- reg + 1
* Updates Sign, Zero, Half-Carry, Overflow flags
*/
void Z80::inc(uint8_t& reg) {
    uint8_t res = reg + 1;
    updateFlagsIncDec(res, reg);
    reg = res;
}

/**
* Returns value + 1
* Updates flags as Increment
*/
uint8_t Z80::inc_(uint8_t value) {
    uint8_t res = value + 1;
    updateFlagsIncDec(res, value);
    return res;
}

/**
* Decrement:
* reg <- reg - 1
* Updates Sign, Zero, Half-Carry, Overflow, Subtract flags
*/
void Z80::dec(uint8_t& reg) {
    uint8_t res = reg - 1;
    updateFlagsIncDec(res, reg, false);
    reg = res;
}

/**
* Returns value - 1
* Updates same flags as Decrement
*/
uint8_t Z80::dec_(uint8_t value) {
    uint8_t res = value - 1;
    updateFlagsIncDec(res, value, false);
    return res;
}

/**
* Load 16-bit value
*/
void Z80::ld(uint16_t& reg) {
    reg = readByte(pc++) | (readByte(pc++) << 8);
}

/**
* Exchange alternate register pairs:
* Swap BC, DE, HL with BC', DE', HL'
*/
void Z80::exx() {
    std::swap(bc, bc_prime);
    std::swap(de, de_prime);
    std::swap(hl, hl_prime);
}

/**
* Load PC into HL
*/
void Z80::ldHL() {
    writeByte(hl, readByte(pc++));
}


/**
* Absolute jump
*/
void Z80::jp() {
    uint16_t addr = readByte(pc) | (readByte(pc + 1) << 8);
    pc = addr;
}

/**
 * Conditional jump:
 * Checks condition code and jumps if met
 * @param opcode condition encoded in bits 5-3
 */
void Z80::condJP(uint8_t opcode) {
    uint8_t condition = (opcode >> 3) & 0x07;
    uint16_t addr = readByte(pc) | (readByte(pc + 1) << 8);
    pc += 2;
    if (checkCondition(condition)) {
        pc = addr;
    }
    
}

/**
* Relative jump:
* PC += signed 8-bit offset
*/
void Z80::jr() {
    int8_t offset = readByte(pc++);
    pc += offset;
}


/**
* Conditional relative jump:
* Extracts condition code from opcode bits 5-3
* Reads signed offset from the next byte
* Jumps if condition is satisfied
*/
void Z80::condJR(uint8_t opcode) {
    uint8_t condition = (opcode >> 3) & 0x03;
    int8_t offset = readByte(pc++);
    if (checkCondition(condition)) {
        pc += offset;
    }
}

/**
* Subroutine call:
* Reads 16-bit target address
* pushes return address onto stack
* sets PC to target address
*/
void Z80::call() {
    uint16_t addr = readByte(pc++) | (readByte(pc++) << 8);
    push(pc); 
    pc = addr;
}

/**
* Conditional soubroutine call:
* Check condition from the opcode
* Execute if condition met
*/
void Z80::condCall(uint8_t opcode) {
    uint8_t condition = (opcode >> 3) & 0x07;
    uint16_t addr = readByte(pc++) | (readByte(pc++) << 8);
    if (checkCondition(condition)) {
        push(pc);
        pc = addr;
    }
}

/**
* Conditional return from subroutine:
* check condition from the opcode
* Pops return address if condition met
*/
void Z80::condRet(uint8_t opcode) {
    uint8_t condition = (opcode >> 3) & 0x07;
    if (checkCondition(condition)) {
        pc = pop();
    }
    else {
        pc++;
    }
}

//Helper function to check conditions 
bool Z80::checkCondition(uint8_t condition) {
    switch (condition) {
    case Conditions::NZ: return !(f & Z_FLAG); // NZ
    case Conditions::Z: return (f & Z_FLAG);  // Z
    case Conditions::NC: return !(f & C_FLAG); // NC
    case Conditions::C: return (f & C_FLAG);  // C
    case Conditions::PO: return !(f & PV_FLAG); // PO
    case Conditions::PE: return (f & PV_FLAG);  // PE
    case Conditions::P: return !(f & S_FLAG); // P
    case Conditions::M: return (f & S_FLAG);  // M
    default: return false;
    }
}


/**
* Push 16-bit value onto stack:
* Store high byte at SP-1
* Low byte at SP-2
* (Z80 is LE)
*/
void Z80::push(uint16_t value) {
    writeByte(--sp, (value >> 8) & 0xFF);
    writeByte(--sp, value & 0xFF);
}

/**
* Pop 16-bit value from the stack:
* Read low Byte on SP
* High byte on SP + 1
*/
uint16_t Z80::pop() {
    uint16_t lo = readByte(sp++);
    uint16_t hi = readByte(sp++);
    return (hi << 8) | lo;
}

/**
* Get register value by the 3-bit code
*/
uint8_t Z80::getReg(uint8_t reg) {
    switch (reg & 0x07) {
        case Regs::B: return (bc >> 8) & 0xFF;  
        case Regs::C: return bc & 0xFF;         
        case Regs::D: return (de >> 8) & 0xFF;  
        case Regs::E: return de & 0xFF;         
        case Regs::H: return (hl >> 8) & 0xFF;  
        case Regs::L: return hl & 0xFF;       
        case Regs::A: return (af >> 8) & 0xFF; 
        default: return 0;
    }
}

/**
* Set register value by 3-bit code
*/
void Z80::setReg(uint8_t reg, uint8_t value) {
    switch (reg & 0x07) {
        case Regs::B: bc = (value << 8) | (bc & 0x00FF); break;  
        case Regs::C: bc = (bc & 0xFF00) | value; break;         
        case Regs::D: de = (value << 8) | (de & 0x00FF); break;  
        case Regs::E: de = (de & 0xFF00) | value; break;        
        case Regs::H: hl = (value << 8) | (hl & 0x00FF); break;  
        case Regs::L: hl = (hl & 0xFF00) | value; break;         
        case Regs::A: af = (value << 8) | (af & 0x00FF); break;  
    }
}

// Flag update helpers

/**
* Update after addition operation:
* Zero - result is zero
* Sign - result is negative
* Half-carry - carry from bit 3
* Parity/Overflow - signed overflow
* Carry - result exceeds 8 bits
*/
void Z80::updateFlagsAdd(uint8_t original_a, uint8_t value, uint16_t res) {
    f = 0;
    uint8_t result = res & 0xFF;

    if (result == 0) f |= Z_FLAG;

    if (result & 0x80) f |= S_FLAG;

    if (((original_a & 0x0F) + (value & 0x0F)) > 0x0F)
        f |= H_FLAG;

    if (((original_a ^ result) & (value ^ result)) & 0x80)
        f |= PV_FLAG;

    if (res > 0xFF) f |= C_FLAG;
}

/**
 * Update after subtraction operation:
 * Sets Subtract flag
 * Zero - Result is zero
 * Sign - Result negative
 * Half-carry - Borrow from bit 4
 * Parity/Overflow - Signed underflow
 * Carry - Result underflows (value > original)
 */
void Z80::updateFlagsSub(uint8_t original_a, uint8_t value, uint16_t res) {
    f = N_FLAG;
    uint8_t result = res & 0xFF;
    updateSZ(result);

    if ((original_a & 0x0F) < (value & 0x0F))
        f |= H_FLAG;

    if (((original_a ^ value) & (original_a ^ result)) & 0x80)
        f |= PV_FLAG;

    if (res > 0xFF) f |= C_FLAG;
}

/**
 * Update Sign and Zero flags:
 * Zero flag set if value is 0
 * Sign flag set if bit 7 is set
 */
void Z80::updateSZ(uint8_t value) {
    if (value == 0) f |= Z_FLAG;
    if (value & 0x80) f |= S_FLAG;
}


/**
 * Update flags for INC/DEC operations:
 * Zero flag set if value is 0
 * Sign flag set if bit 7 is set
 * Half-carry: Overflow in lower nibble
 * Parity: Overflow from 0x7F (INC) or 0x80 (DEC)
 */
void Z80::updateFlagsIncDec(uint8_t res, uint8_t old, bool inc) {
    f &= ~(N_FLAG | Z_FLAG | S_FLAG | H_FLAG | PV_FLAG);

    if (!inc)
        f |= N_FLAG;

    if (res == 0)
        f |= Z_FLAG;

    if (res & 0x80)
        f |= S_FLAG;

    if (inc) 
        if ((old & 0x0F) == 0x0F) 
            f |= H_FLAG;
    else 
        if ((old & 0x0F) == 0x00)
            f |= H_FLAG;

    if ((inc && old == 0x7F) || (!inc && old == 0x80))
        f |= PV_FLAG;
}

/**
 * Calculate parity flag
 * Uses XOR folding
 * @return true for even parity, false for odd
 */
bool Z80::parityEven(uint8_t value) {
    value ^= value >> 4;
    value ^= value >> 2;
    value ^= value >> 1;
    return (value & 1) == 0;
}

// Prefixed instruction handlers

/**
 * Handle ADD A, (IX/IY+d):
 * Calculates address in memory
 * Performs addition with memory value
 */
void Z80::handleAdd(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == PREFIX_DD) ? ix + d : iy + d;
    addA(readByte(addr));
}

/**
 * Handle ADC A, (IX/IY+d):
 * Add memory value with carry flag
 */
void Z80::handleAdc(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == PREFIX_DD) ? ix + d : iy + d;
    adcA(readByte(addr));
}

/**
 * Handle SUB (IX/IY+d):
 * Subtracts memory value from accumulator
 * Updates flags
 */
void Z80::handleSub(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == PREFIX_DD) ? ix + d : iy + d;
    sub(readByte(addr));
}

/**
* Handle SBC A,(IX/IY+d):
* Same as SUB (IX/IY+d), only subtracts with carry
*/
void Z80::handleSbc(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == PREFIX_DD) ? ix + d : iy + d;
    sbcA(readByte(addr));
}

/**
* Handle AND (IX/IY+d):
* Performs bitwise AND between A and value at (IX/IY + d)
* Sets flags Z, S, PV, H, carry cleared
*/
void Z80::handleAnd(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == PREFIX_DD) ? ix + d : iy + d;
    andA(readByte(addr));
}

/**
* Handle OR (IX/IY+d):
* Performs bitwise OR between A and value at (IX/IY + d)
* Sets flags Z, S, PV, carry cleared
*/
void Z80::handleOr(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == PREFIX_DD) ? ix + d : iy + d;
    orA(readByte(addr));
}

/**
* Handle XOR (IX/IY+d):
* Performs bitwise XOR between A and value at (IX/IY + d)
* Flags are the same as in handleOR
*/
void Z80::handleXor(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == PREFIX_DD) ? ix + d : iy + d;
    xorA(readByte(addr));
}

/**
* Handle CP (IX/IY+d):
* Compares A with value at (IX/IY+d) by performing A - value
* A remains unchanged, flags set as in subtraction
*/
void Z80::handleCp(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == PREFIX_DD) ? ix + d : iy + d;
    cp(readByte(addr));
}

/**
 * Handle INC (IX/IY+d):
 * Increments memory at indexed address
 * Updates flags as normal INC
 */
void Z80::handleIncMem(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == PREFIX_DD) ? ix + d : iy + d;
    writeByte(addr, inc_(readByte(addr)));
}

/**
 * Handle DEC (IX/IY+d):
 * Decrements memory at indexed address
 * Updates flags as normal DEC
 */
void Z80::handleDecMem(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == PREFIX_DD) ? ix + d : iy + d;
    writeByte(addr, dec_(readByte(addr)));
}

/**
 * Handle LD r,r', LD r,(HL), LD (HL),r:
 * Copies values between 8-bit registers and memory via HL
 * LD (HL), (HL) is ignored
 */
void Z80::handleLd(uint8_t opcode) {
    uint8_t dest = (opcode >> 3) & 0x07; // bits 5-3 - destination
    uint8_t src = opcode & 0x07;         // bits 2-0 - source

    if (src == 6 && dest == 6) return; // LD (HL),(HL) ignored

    if (src == 6) { // LD r,(HL)
        setReg(dest, readByte(hl));
    }
    else if (dest == 6) { // LD (HL),r
        writeByte(hl, getReg(src));
    }
    else if (src != 6 && dest != 6)  { // LD r,r'
        uint8_t value = getReg(src);
        setReg(dest, value);
    }
}

/**
 * Handle LD r,(IX/IY+d) and LD (IX/IY+d), r:
 * Performs indexed load/store using IX/IY + displacement
 */
void Z80::handleIndexedLd(uint8_t prefix, uint8_t opcode) {
    uint8_t dest = (opcode >> 3) & 0x07;
    uint8_t src = opcode & 0x07;
    int8_t d = readByte(pc++);

    uint16_t addr = (prefix == PREFIX_DD) ? ix + d : iy + d;

    if (src == 6) { // LD r,(IX/IY+d)
        setReg(dest, readByte(addr));
    }
    else if (dest == 6) { // LD (IX/IY+d),r
        writeByte(addr, getReg(src));
    }
}
/**
* Set carry flag, clears subtract and half-carry
*/
void Z80::setCarry() {
    f = (f | C_FLAG) & ~(N_FLAG | H_FLAG);
}

/**
* Decimal Adjust Accumulator:
* Adjusts the accumulator after addition/subtraction
* Updates flags based on the adjusted results
*/
void Z80::daa() {
    uint8_t a = this->a;
    uint8_t adjust = 0;
    bool subtract = (f & N_FLAG);
    bool new_carry = (f & C_FLAG);

    if (!subtract) {
        // Addition
        if ((f & H_FLAG) || (a & 0x0F) > 9) {
            adjust += 0x06;
        }
        if ((f & C_FLAG) || (a > 0x99) || ((a + adjust) > 0x99)) {
            adjust += 0x60;
            new_carry = true;
        }
    }
    else {
        // Subtraction
        if ((f & H_FLAG) || (a & 0x0F) > 9) {
            adjust += 0xFA; 
        }
        if (f & C_FLAG) {
            adjust += 0xA0; 
            new_carry = true;
        }
        else {
            new_carry = (a < adjust);
        }
    }

    a += adjust;
    new_carry |= (a > 0x99);
    f = 0;

    f |= (a == 0) ? Z_FLAG : 0;
    f |= (a & 0x80) ? S_FLAG : 0;
    f |= parityEven(a) ? PV_FLAG : 0;
    f |= new_carry ? C_FLAG : 0;
    if (!subtract) {
        f |= ((this->a & 0x0F) + (adjust & 0x0F) > 0x0F) ? H_FLAG : 0;
    }

    this->a = a;
}

