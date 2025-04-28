#include "cpu.hpp"


Z80::Z80() { reset(); }

void Z80::reset() {
    af = bc = de = hl = 0;
    af_prime = bc_prime = de_prime = hl_prime = 0xCCCC;
    pc = sp = ix = iy = 0;
    std::fill(std::begin(memory), std::end(memory), 0);
    //std::fill_n(memory, sizeof(memory), 0);
    //memset(memory, 0, sizeof(memory));
}

uint8_t Z80::readByte(uint16_t addr) const {
    return memory[addr];
}

void Z80::writeByte(uint16_t addr, uint8_t value) {
    memory[addr] = value;
}

void Z80::step() {
    uint8_t opcode = readByte(pc++);
    if (opcode == 0xDD || opcode == 0xFD) {
        uint8_t prefix = opcode;
        uint8_t nextOp = readByte(pc++);
        handlePrefixedOpcode(prefix, nextOp);
    }
    else {
        handleOpcode(opcode);
    }
}

void Z80::setIX(uint16_t value) { ix = value; }


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
    case 0x21: // LD IX/IY,nn
        if (prefix == 0xDD) {
            ix = readByte(pc++) | (readByte(pc++) << 8);
        }
        else {
            iy = readByte(pc++) | (readByte(pc++) << 8);
        }
        break;

    case 0x36: { // LD (IX/IY+d),n
        int8_t d = readByte(pc++);
        uint8_t n = readByte(pc++);
        uint16_t addr = prefix == 0xDD ? ix + d : iy + d;
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

void Z80::handleOpcode(uint8_t opcode) {
    switch (opcode) {
        // ADD A, r
    case 0x80: addA(b); break;
    case 0x81: addA(c); break;
    case 0x82: addA(d); break;
    case 0x83: addA(e); break;
    case 0x84: addA(h); break;
    case 0x85: addA(l); break;
    case 0x86: addA(readByte(hl)); break;
    case 0x87: addA(a); break;
    case 0xC6: addA(readByte(pc++)); break; // ADD A, n

        // ADC A, s
    case 0x88: adcA(b); break;
    case 0x89: adcA(c); break;
    case 0x8A: adcA(d); break;
    case 0x8B: adcA(e); break;
    case 0x8C: adcA(h); break;
    case 0x8D: adcA(l); break;
    case 0x8E: adcA(readByte(hl)); break;
    case 0x8F: adcA(a); break;
    case 0xCE: adcA(readByte(pc++)); break; // ADC A, n

        // SUB s
    case 0x90: sub(b); break;
    case 0x91: sub(c); break;
    case 0x92: sub(d); break;
    case 0x93: sub(e); break;
    case 0x94: sub(h); break;
    case 0x95: sub(l); break;
    case 0x96: sub(readByte(hl)); break;
    case 0x97: sub(a); break;
    case 0xD6: sub(readByte(pc++)); break; // SUB n

        // SBC A, s
    case 0x98: sbcA(b); break;
    case 0x99: sbcA(c); break;
    case 0x9A: sbcA(d); break;
    case 0x9B: sbcA(e); break;
    case 0x9C: sbcA(h); break;
    case 0x9D: sbcA(l); break;
    case 0x9E: sbcA(readByte(hl)); break;
    case 0x9F: sbcA(a); break;
    case 0xDE: sbcA(readByte(pc++)); break; // SBC A, n

        // AND s
    case 0xA0: andA(b); break;
    case 0xA1: andA(c); break;
    case 0xA2: andA(d); break;
    case 0xA3: andA(e); break;
    case 0xA4: andA(h); break;
    case 0xA5: andA(l); break;
    case 0xA6: andA(readByte(hl)); break;
    case 0xA7: andA(a); break;
    case 0xE6: andA(readByte(pc++)); break; // AND n

        // OR s
    case 0xB0: orA(b); break;
    case 0xB1: orA(c); break;
    case 0xB2: orA(d); break;
    case 0xB3: orA(e); break;
    case 0xB4: orA(h); break;
    case 0xB5: orA(l); break;
    case 0xB6: orA(readByte(hl)); break;
    case 0xB7: orA(a); break;
    case 0xF6: orA(readByte(pc++)); break; // OR n

        // XOR s
    case 0xA8: xorA(b); break;
    case 0xA9: xorA(c); break;
    case 0xAA: xorA(d); break;
    case 0xAB: xorA(e); break;
    case 0xAC: xorA(h); break;
    case 0xAD: xorA(l); break;
    case 0xAE: xorA(readByte(hl)); break;
    case 0xAF: xorA(a); break;
    case 0xEE: xorA(readByte(pc++)); break; // XOR n

        // CP s
    case 0xB8: cp(b); break;
    case 0xB9: cp(c); break;
    case 0xBA: cp(d); break;
    case 0xBB: cp(e); break;
    case 0xBC: cp(h); break;
    case 0xBD: cp(l); break;
    case 0xBE: cp(readByte(hl)); break;
    case 0xBF: cp(a); break;
    case 0xFE: cp(readByte(pc++)); break; // CP n

        // INC r
    case 0x04: inc(b); break;
    case 0x0C: inc(c); break;
    case 0x14: inc(d); break;
    case 0x1C: inc(e); break;
    case 0x24: inc(h); break;
    case 0x2C: inc(l); break;
    case 0x3C: inc(a); break;
    case 0x34: { // INC (HL)
        uint16_t addr = hl;
        writeByte(addr, inc_(readByte(addr)));
        break;
    }

             // DEC m
    case 0x05: dec(b); break;
    case 0x0D: dec(c); break;
    case 0x15: dec(d); break;
    case 0x1D: dec(e); break;
    case 0x25: dec(h); break;
    case 0x2D: dec(l); break;
    case 0x3D: dec(a); break;
    case 0x35: { // DEC (HL)
        uint16_t addr = hl;
        writeByte(addr, dec_(readByte(addr)));
        break;
    }

             // 8-bit Loads
    case 0x36: { // LD (HL),n
        writeByte(hl, readByte(pc++));
        break;
    }

             // 16-bit Loads
    case 0x01: // LD BC,nn
        bc = readByte(pc++) | (readByte(pc++) << 8);
        break;
    case 0x11: // LD DE,nn
        de = readByte(pc++) | (readByte(pc++) << 8);
        break;
    case 0x21: // LD HL,nn
        hl = readByte(pc++) | (readByte(pc++) << 8);
        break;
    case 0x31: // LD SP,nn
        sp = readByte(pc++) | (readByte(pc++) << 8);
        break;

        // Exchange instructions
    case 0xEB: { // EX DE,HL
        std::swap(de, hl);
        break;
    }
    case 0x08: { // EX AF,AF'
        std::swap(af, af_prime);
        break;
    }
    case 0xD9: { // EXX
        std::swap(bc, bc_prime);
        std::swap(de, de_prime);
        std::swap(hl, hl_prime);
        break;
    }
             // JP nn
    case 0xC3: {
        uint16_t addr = readByte(pc) | (readByte(pc + 1) << 8);
        pc = addr;
        break;
    }

             // JP cc,nn
    case 0xC2: case 0xCA: case 0xD2: case 0xDA:
    case 0xE2: case 0xEA: case 0xF2: case 0xFA: {
        uint8_t condition = (opcode >> 3) & 0x07;
        uint16_t addr = readByte(pc) | (readByte(pc + 1) << 8);
        if (checkCondition(condition)) {
            pc = addr;
        }
        else {
            pc += 2;
        }
        break;
    }

             // JR e
    case 0x18: {
        int8_t offset = readByte(pc++);
        pc += offset;
        break;
    }

             // JR cc,e
    case 0x20: case 0x28: case 0x30: case 0x38: {
        uint8_t condition = (opcode >> 3) & 0x03;
        int8_t offset = readByte(pc++);
        if (checkCondition(condition)) {
            pc += offset;
        }
        break;
    }

             // CALL nn
    case 0xCD: {
        uint16_t addr = readByte(pc++) | (readByte(pc++) << 8);
        push(pc);  // Return address
        pc = addr;
        break;
    }

             // CALL cc,nn
    case 0xC4: case 0xCC: case 0xD4: case 0xDC:
    case 0xE4: case 0xEC: case 0xF4: case 0xFC: {
        uint8_t condition = (opcode >> 3) & 0x07;
        uint16_t addr = readByte(pc) | (readByte(pc + 1) << 8);
        if (checkCondition(condition)) {
            push(pc + 3);
            pc = addr;
        }
        else {
            pc += 2;
        }
        break;
    }

             // RET
    case 0xC9:
        pc = pop();
        break;

        // RET cc
    case 0xC0: case 0xC8: case 0xD0: case 0xD8:
    case 0xE0: case 0xE8: case 0xF0: case 0xF8: {
        uint8_t condition = (opcode >> 3) & 0x07;
        if (checkCondition(condition)) {
            pc = pop();
        }
        break;
    }

             // RETI/RETN (simplified)
    case 0xED: {
        uint8_t next = readByte(pc++);
        if (next == 0x4D) {  // RETI
            pc = pop();
        }
        else if (next == 0x45) {  // RETN
            pc = pop();
        }
        break;
    }

    case 0xC5: // PUSH BC
        push(bc);
        break;
    case 0xD1: // POP DE
        de = pop();
        break;

    default:
        if ((opcode & 0xC0) == 0x40) {
            handleLd(opcode);
        }
        break; // Unsupported opcode
    }
}



void Z80::addA(uint8_t value) {
    uint16_t res = a + value;
    updateFlagsAdd(value, res);
    a = res & 0xFF;
}

void Z80::adcA(uint8_t value) {
    uint8_t carry = (f & C_FLAG) ? 1 : 0;
    uint16_t res = a + value + carry;
    updateFlagsAdd(value, res);
    a = res & 0xFF;
}

void Z80::sub(uint8_t value) {
    uint16_t res = a - value;
    updateFlagsSub(value, res);
    a = res & 0xFF;
}

void Z80::sbcA(uint8_t value) {
    uint8_t carry = (f & C_FLAG) ? 1 : 0;
    uint16_t res = a - value - carry;
    updateFlagsSub(value + carry, res);
    a = res & 0xFF;
}

void Z80::andA(uint8_t value) {
    a &= value;
    f = H_FLAG | (parityEven(a) ? PV_FLAG : 0);
    updateSZ(a);
}

void Z80::orA(uint8_t value) {
    a |= value;
    f = 0;
    updateSZ(a);
    f |= parityEven(a) ? PV_FLAG : 0;
}

void Z80::xorA(uint8_t value) {
    a ^= value;
    f = 0;
    updateSZ(a);
    f |= parityEven(a) ? PV_FLAG : 0;
}

void Z80::cp(uint8_t value) {
    uint16_t res = a - value;
    updateFlagsSub(value, res);
}

void Z80::inc(uint8_t& reg) {
    uint8_t res = reg + 1;
    updateFlagsIncDec(res, reg);
    reg = res;
}

uint8_t Z80::inc_(uint8_t value) {
    uint8_t res = value + 1;
    updateFlagsIncDec(res, value);
    return res;
}

void Z80::dec(uint8_t& reg) {
    uint8_t res = reg - 1;
    updateFlagsIncDec(res, reg, false);
    reg = res;
}

uint8_t Z80::dec_(uint8_t value) {
    uint8_t res = value - 1;
    updateFlagsIncDec(res, value, false);
    return res;
}


bool Z80::checkCondition(uint8_t condition) {
    switch (condition) {
    case 0: return !(f & Z_FLAG); // NZ
    case 1: return (f & Z_FLAG);  // Z
    case 2: return !(f & C_FLAG); // NC
    case 3: return (f & C_FLAG);  // C
    case 4: return !(f & PV_FLAG); // PO
    case 5: return (f & PV_FLAG);  // PE
    case 6: return !(f & S_FLAG); // P
    case 7: return (f & S_FLAG);  // M
    default: return false;
    }
}

void Z80::push(uint16_t value) {
    writeByte(--sp, (value >> 8) & 0xFF); // High byte
    writeByte(--sp, value & 0xFF);         // Low byte
}

uint16_t Z80::pop() {
    uint16_t lo = readByte(sp++);
    uint16_t hi = readByte(sp++);
    return (hi << 8) | lo;
}


uint8_t Z80::getReg(uint8_t reg) {
    switch (reg) {
    case 0: return b;
    case 1: return c;
    case 2: return d;
    case 3: return e;
    case 4: return h;
    case 5: return l;
    case 7: return a;
    default: return 0;
    }
}

void Z80::setReg(uint8_t reg, uint8_t value) {
    switch (reg) {
    case 0: b = value; break;
    case 1: c = value; break;
    case 2: d = value; break;
    case 3: e = value; break;
    case 4: h = value; break;
    case 5: l = value; break;
    case 7: a = value; break;
    default: break;
    }
}

// Flag update helpers
void Z80::updateFlagsAdd(uint8_t value, uint16_t res) {
    f = 0;
    if ((res & 0xFF) == 0) f |= Z_FLAG;
    if (res & 0x80) f |= S_FLAG;
    if (((a & 0x0F) + (value & 0x0F) + ((res >> 8) ? 1 : 0)) > 0x0F) f |= H_FLAG;
    if (res > 0xFF) f |= C_FLAG;
    if (((a ^ res) & (value ^ res)) & 0x80) f |= PV_FLAG;
}

void Z80::updateFlagsSub(uint8_t value, uint16_t res) {
    f = N_FLAG;
    if ((res & 0xFF) == 0) f |= Z_FLAG;
    if (res & 0x80) f |= S_FLAG;
    if ((a & 0x0F) < (value & 0x0F)) f |= H_FLAG;
    if (res > 0xFF) f |= C_FLAG;
    if (((a ^ value) & (a ^ res)) & 0x80) f |= PV_FLAG;
}

void Z80::updateSZ(uint8_t value) {
    if (value == 0) f |= Z_FLAG;
    if (value & 0x80) f |= S_FLAG;
}

void Z80::updateFlagsIncDec(uint8_t res, uint8_t old, bool inc) {
    f &= ~(N_FLAG | Z_FLAG | S_FLAG | H_FLAG | PV_FLAG);
    if (inc) f |= 0;
    else f |= N_FLAG;
    if (res == 0) f |= Z_FLAG;
    if (res & 0x80) f |= S_FLAG;
    if ((old & 0x0F) == 0x0F && inc) f |= H_FLAG;
    if ((old == 0x7F && inc) || (old == 0x80 && !inc)) f |= PV_FLAG;
}

bool Z80::parityEven(uint8_t value) {
    value ^= value >> 4;
    value ^= value >> 2;
    value ^= value >> 1;
    return (value & 1) == 0;
}

// Prefixed instruction handlers
void Z80::handleAdd(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
    addA(readByte(addr));
}

void Z80::handleAdc(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
    adcA(readByte(addr));
}

void Z80::handleSub(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
    sub(readByte(addr));
}

void Z80::handleSbc(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
    sbcA(readByte(addr));
}

void Z80::handleAnd(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
    andA(readByte(addr));
}

void Z80::handleOr(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
    orA(readByte(addr));
}

void Z80::handleXor(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
    xorA(readByte(addr));
}

void Z80::handleCp(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
    cp(readByte(addr));
}

void Z80::handleIncMem(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
    writeByte(addr, inc_(readByte(addr)));
}

void Z80::handleDecMem(uint8_t prefix) {
    int8_t d = readByte(pc++);
    uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
    writeByte(addr, dec_(readByte(addr)));
}

void Z80::handleLd(uint8_t opcode) {
    uint8_t dest = (opcode >> 3) & 0x07;
    uint8_t src = opcode & 0x07;

    if (src == 6 && dest == 6) return; // LD (HL),(HL)

    if (src == 6) { // LD r,(HL)
        setReg(dest, readByte(hl));
    }
    else if (dest == 6) { // LD (HL),r
        writeByte(hl, getReg(src));
    }
    else { // LD r,r'
        setReg(dest, getReg(src));
    }
}

void Z80::handleIndexedLd(uint8_t prefix, uint8_t opcode) {
    uint8_t dest = (opcode >> 3) & 0x07;
    uint8_t src = opcode & 0x07;
    int8_t d = readByte(pc++);

    uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;

    if (src == 6) { // LD r,(IX/IY+d)
        setReg(dest, readByte(addr));
    }
    else if (dest == 6) { // LD (IX/IY+d),r
        writeByte(addr, getReg(src));
    }
}