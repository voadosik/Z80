#include <cstdint>
#include <cstring>


constexpr uint8_t ADD = 0x86;
constexpr uint8_t ADC = 0x8E;
constexpr uint8_t SUB = 0x96;
constexpr uint8_t SBC = 0x9E;
constexpr uint8_t AND = 0xA6;
constexpr uint8_t OR = 0xB6;
constexpr uint8_t XOR = 0xAE;
constexpr uint8_t CP = 0xBE;
constexpr uint8_t INC = 0x34;
constexpr uint8_t DEC = 0x35;



class Z80 {
public:

    // Registers
    union {
        struct { uint8_t f, a; };
        uint16_t af;
    };
    union {
        struct { uint8_t c, b; };
        uint16_t bc;
    };
    union {
        struct { uint8_t e, d; };
        uint16_t de;
    };
    union {
        struct { uint8_t l, h; };
        uint16_t hl;
    };
    uint16_t pc; // Program Counter
    uint16_t sp; // Stack Pointer
    uint16_t ix; // Index Register X
    uint16_t iy; // Index Register Y
    uint8_t memory[65536]; // 64KB Memory

    

    Z80() {
        reset();
    }

    // Flag Masks
    enum Flags {
        C_FLAG = 0x01,
        N_FLAG = 0x02,
        PV_FLAG = 0x04,
        H_FLAG = 0x10,
        Z_FLAG = 0x40,
        S_FLAG = 0x80
    };

    void reset() {
        af = bc = de = hl = 0;
        pc = sp = ix = iy = 0;
        memset(memory, 0, sizeof(memory));
    }

    uint8_t readByte(uint16_t addr) const {
        return memory[addr];
    }

    void writeByte(uint16_t addr, uint8_t value) {
        memory[addr] = value;
    }

    void step() {
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

    uint8_t getA() const { return a; }
    uint8_t getF() const { return f; }
    uint8_t getB() const { return b; }
    uint8_t getC() const { return c; }
    uint16_t getIX() const { return ix; }
    uint16_t getPC() const { return pc; }
    void setIX(uint16_t value) { ix = value; }

private:
    void handlePrefixedOpcode(uint8_t prefix, uint8_t opcode) {
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
        default: break; // Unsupported prefixed opcode
        }
    }

    void handleOpcode(uint8_t opcode) {
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

        default: break; // Unsupported opcode
        }
    }

    // Helper functions for arithmetic operations
    void addA(uint8_t value) {
        uint16_t res = a + value;
        updateFlagsAdd(value, res);
        a = res & 0xFF;
    }

    void adcA(uint8_t value) {
        uint8_t carry = (f & C_FLAG) ? 1 : 0;
        uint16_t res = a + value + carry;
        updateFlagsAdd(value, res);
        a = res & 0xFF;
    }

    void sub(uint8_t value) {
        uint16_t res = a - value;
        updateFlagsSub(value, res);
        a = res & 0xFF;
    }

    void sbcA(uint8_t value) {
        uint8_t carry = (f & C_FLAG) ? 1 : 0;
        uint16_t res = a - value - carry;
        updateFlagsSub(value + carry, res);
        a = res & 0xFF;
    }

    void andA(uint8_t value) {
        a &= value;
        f = H_FLAG | (parityEven(a) ? PV_FLAG : 0);
        updateSZ(a);
    }

    void orA(uint8_t value) {
        a |= value;
        f = 0;
        updateSZ(a);
        f |= parityEven(a) ? PV_FLAG : 0;
    }

    void xorA(uint8_t value) {
        a ^= value;
        f = 0;
        updateSZ(a);
        f |= parityEven(a) ? PV_FLAG : 0;
    }

    void cp(uint8_t value) {
        uint16_t res = a - value;
        updateFlagsSub(value, res);
    }

    void inc(uint8_t& reg) {
        uint8_t res = reg + 1;
        updateFlagsIncDec(res, reg);
        reg = res;
    }

    uint8_t inc_(uint8_t value) {
        uint8_t res = value + 1;
        updateFlagsIncDec(res, value);
        return res;
    }

    void dec(uint8_t& reg) {
        uint8_t res = reg - 1;
        updateFlagsIncDec(res, reg, false);
        reg = res;
    }

    uint8_t dec_(uint8_t value) {
        uint8_t res = value - 1;
        updateFlagsIncDec(res, value, false);
        return res;
    }

    // Flag update helpers
    void updateFlagsAdd(uint8_t value, uint16_t res) {
        f = 0;
        if ((res & 0xFF) == 0) f |= Z_FLAG;
        if (res & 0x80) f |= S_FLAG;
        if (((a & 0x0F) + (value & 0x0F) + ((res >> 8) ? 1 : 0)) > 0x0F) f |= H_FLAG;
        if (res > 0xFF) f |= C_FLAG;
        if (((a ^ res) & (value ^ res)) & 0x80) f |= PV_FLAG;
    }

    void updateFlagsSub(uint8_t value, uint16_t res) {
        f = N_FLAG;
        if ((res & 0xFF) == 0) f |= Z_FLAG;
        if (res & 0x80) f |= S_FLAG;
        if ((a & 0x0F) < (value & 0x0F)) f |= H_FLAG;
        if (res > 0xFF) f |= C_FLAG;
        if (((a ^ value) & (a ^ res)) & 0x80) f |= PV_FLAG;
    }

    void updateSZ(uint8_t value) {
        if (value == 0) f |= Z_FLAG;
        if (value & 0x80) f |= S_FLAG;
    }

    void updateFlagsIncDec(uint8_t res, uint8_t old, bool inc = true) {
        f &= ~(N_FLAG | Z_FLAG | S_FLAG | H_FLAG | PV_FLAG);
        if (inc) f |= 0;
        else f |= N_FLAG;
        if (res == 0) f |= Z_FLAG;
        if (res & 0x80) f |= S_FLAG;
        if ((old & 0x0F) == 0x0F && inc) f |= H_FLAG;
        if ((old == 0x7F && inc) || (old == 0x80 && !inc)) f |= PV_FLAG;
    }

    bool parityEven(uint8_t value) {
        value ^= value >> 4;
        value ^= value >> 2;
        value ^= value >> 1;
        return (value & 1) == 0;
    }

    // Prefixed instruction handlers
    void handleAdd(uint8_t prefix) {
        int8_t d = readByte(pc++);
        uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
        addA(readByte(addr));
    }

    void handleAdc(uint8_t prefix) {
        int8_t d = readByte(pc++);
        uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
        adcA(readByte(addr));
    }

    void handleSub(uint8_t prefix) {
        int8_t d = readByte(pc++);
        uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
        sub(readByte(addr));
    }

    void handleSbc(uint8_t prefix) {
        int8_t d = readByte(pc++);
        uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
        sbcA(readByte(addr));
    }

    void handleAnd(uint8_t prefix) {
        int8_t d = readByte(pc++);
        uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
        andA(readByte(addr));
    }

    void handleOr(uint8_t prefix) {
        int8_t d = readByte(pc++);
        uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
        orA(readByte(addr));
    }

    void handleXor(uint8_t prefix) {
        int8_t d = readByte(pc++);
        uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
        xorA(readByte(addr));
    }

    void handleCp(uint8_t prefix) {
        int8_t d = readByte(pc++);
        uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
        cp(readByte(addr));
    }

    void handleIncMem(uint8_t prefix) {
        int8_t d = readByte(pc++);
        uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
        writeByte(addr, inc_(readByte(addr)));
    }

    void handleDecMem(uint8_t prefix) {
        int8_t d = readByte(pc++);
        uint16_t addr = (prefix == 0xDD) ? ix + d : iy + d;
        writeByte(addr, dec_(readByte(addr)));
    }
};