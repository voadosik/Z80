#include <cstdint>
#include <cstring>
#include <array>
#include <iostream>

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
    union {
        struct { uint8_t f_prime, a_prime; };
        uint16_t af_prime;
    };
    union {
        struct { uint8_t c_prime, b_prime; };
        uint16_t bc_prime;
    };
    union {
        struct { uint8_t e_prime, d_prime; };
        uint16_t de_prime;
    };
    union {
        struct { uint8_t l_prime, h_prime; };
        uint16_t hl_prime;
    };
    uint16_t pc; // Program Counter
    uint16_t sp; // Stack Pointer
    uint16_t ix; // Index Register X
    uint16_t iy; // Index Register Y
    uint8_t memory[65536]; // 64KB Memory



    Z80();

    // Flag Masks
    enum Flags {
        C_FLAG = 0x01,
        N_FLAG = 0x02,
        PV_FLAG = 0x04,
        H_FLAG = 0x10,
        Z_FLAG = 0x40,
        S_FLAG = 0x80
    };

    void reset();
    uint8_t readByte(uint16_t addr) const;
    void writeByte(uint16_t addr, uint8_t value);
    void step();

    uint8_t getA() const;
    uint8_t getF() const;
    uint8_t getB() const;
    uint8_t getC() const;
    uint8_t getL() const;
    uint8_t getH() const;
    uint16_t getAF() const;
    uint16_t getBC() const;
    uint16_t getHL() const;
    uint16_t getIX() const;
    uint16_t getIY() const;
    uint16_t getSP() const;
    uint16_t getPC() const;


    void setIX(uint16_t value);

private:
    void handlePrefixedOpcode(uint8_t prefix, uint8_t opcode);
    void handleOpcode(uint8_t opcode);
    void halt();

    // Helper functions for arithmetic operations
    void addA(uint8_t value);

    void adcA(uint8_t value);

    void sub(uint8_t value);

    void sbcA(uint8_t value);

    void andA(uint8_t value);

    void orA(uint8_t value);

    void xorA(uint8_t value);

    void cp(uint8_t value);

    void inc(uint8_t& reg);
    uint8_t inc_(uint8_t value);

    void dec(uint8_t& reg);
    uint8_t dec_(uint8_t value);

    void ld(uint16_t& reg);
    void ldHL();

    void exx();

    void jp();
    void condJP(uint8_t opcode);
    
    void jr();
    void condJR(uint8_t opcode);

    void call();
    void condCall(uint8_t opcode);

    void condRet(uint8_t opcode);
    void retin();

    void setCarry();

    void daa();

    bool checkCondition(uint8_t condition);


    void push(uint16_t value);
    uint16_t pop();


    uint8_t getReg(uint8_t reg);
    void setReg(uint8_t reg, uint8_t value);


    // Flag update helpers
    void updateFlagsAdd(uint8_t value, uint16_t res);

    void updateFlagsSub(uint8_t value, uint16_t res);

    void updateSZ(uint8_t value);

    void updateFlagsIncDec(uint8_t res, uint8_t old, bool inc = true);

    bool parityEven(uint8_t value);

    // Prefixed instruction handlers
    void handleAdd(uint8_t prefix);
    void handleAdc(uint8_t prefix);
    void handleSub(uint8_t prefix);
    void handleSbc(uint8_t prefix);
    void handleAnd(uint8_t prefix);
    void handleOr(uint8_t prefix);
    void handleXor(uint8_t prefix);
    void handleCp(uint8_t prefix);
    void handleIncMem(uint8_t prefix);
    void handleDecMem(uint8_t prefix);
    void handleLd(uint8_t opcode);
    void handleIndexedLd(uint8_t prefix, uint8_t opcode);
    
};

inline uint8_t Z80::getA() const { return a; }
inline uint8_t Z80::getF() const { return f; }
inline uint8_t Z80::getB() const { return b; }
inline uint8_t Z80::getC() const { return c; }
inline uint8_t Z80::getL() const { return l; }
inline uint8_t Z80::getH() const { return h; }
inline uint16_t Z80::getAF() const { return af; }
inline uint16_t Z80::getBC() const { return bc; }
inline uint16_t Z80::getHL() const { return hl; }
inline uint16_t Z80::getIX() const { return ix; }
inline uint16_t Z80::getIY() const { return iy; }
inline uint16_t Z80::getPC() const { return pc; }
inline uint16_t Z80::getSP() const { return sp; }
