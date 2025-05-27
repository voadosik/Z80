#ifndef CPU_HPP
#define CPU_HPP

#include "opcodes.hpp"
#include <cstdint>
#include <cstring>
#include <array>
#include <iostream>
#include <vector>

/**
* @class Z80
* @brief Zilog Z80 CPU emulator.
* 
* Emulates the functionality of core instructions of a Z80
* processor, including data transfer, branching, arithmetics, logic and stack operations.
*/
class Z80 {
private:

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
    bool halted;
    uint16_t pc; // Program Counter
    uint16_t sp; // Stack Pointer
    uint16_t ix; // Index Register X
    uint16_t iy; // Index Register Y
    uint8_t memory[65536]; // 64KB Memory

public:

    Z80();
    
    /**
    * @brief CPU flag bitmasks
    */
    enum Flags {
        C_FLAG = 0x01, //Carry flag
        N_FLAG = 0x02, // Add/Subtract flag
        PV_FLAG = 0x04,// Parity/Overflow flag
        H_FLAG = 0x10, //Half carry flag
        Z_FLAG = 0x40, // Zero flag
        S_FLAG = 0x80  // Sign flag
    };

    /**
    * @brief Reset CPU to initial state
    */
    void reset();

    /**
    * @brief Read a byte from memory
    * @param addr - 16-bit memory address
    * @return 8 bit value at specified address
    */
    uint8_t readByte(uint16_t addr) const;


    /**
    * @brief Write a byte to memory
    * @param addr - 16-bit memory address
    * @param value - 8-bit value to write at the address location 
    */
    void writeByte(uint16_t addr, uint8_t value);


    /**
    * @brief Execute one CPU instruction
    */
    void step();

    //Register accessors
    uint8_t getA() const;
    uint8_t getF() const;
    uint8_t getB() const;
    uint8_t getC() const;
    uint8_t getD() const;
    uint8_t getE() const;
    uint8_t getL() const;
    uint8_t getH() const;
    uint8_t getA_P () const;
    uint8_t getF_P() const;
    uint8_t getB_P() const;
    uint8_t getC_P() const;
    uint8_t getD_P() const;
    uint8_t getE_P() const;
    uint8_t getL_P() const;
    uint8_t getH_P() const;
    uint16_t getAF() const;
    uint16_t getBC() const;
    uint16_t getDE() const;
    uint16_t getHL() const;
    uint16_t getAF_P() const;
    uint16_t getBC_P() const;
    uint16_t getDE_P() const;
    uint16_t getHL_P() const;
    uint16_t getIX() const;
    uint16_t getIY() const;
    uint16_t getSP() const;
    uint16_t getPC() const;


private:

    /**
    * @brief Handle prefixed opcode for the prefixes 0xDD/0xFD
    * @param prefix - 8-bit prefix (0xDD/0xFD)
    * @param opcode - actual instruction to execute
    */
    void handlePrefixedOpcode(uint8_t prefix, uint8_t opcode);
    

    /**
    * @brief Main opcode execution dispatcher
    * @param opcode instruction to execute
    */
    void handleOpcode(uint8_t opcode);

    /**
    * @brief HALT instruction handler, ends the execution of the program when encountered
    *    
    */
    void halt();

    // Helper functions for arithmetic operations
    /**
    * @brief ADD A,n - Add to accumulator
    * @param value 8-bit operand (register/memory/immediate)
    */
    void addA(uint8_t value);

    /**
    * @brief ADC A, n - Add with Carry
    * @param value 8-bit operand
    * @details A = A + value + carry
    */
    void adcA(uint8_t value);

    /**
    * @brief SUB n - Subtract from accumulator
    * @param value 8-bit operand
    */
    void sub(uint8_t value);

    /**
    * @brief SBC A,n - Subtract with Carry
    * @param value 8-bit operand
    * @details A = A - value - C_flag
    */
    void sbcA(uint8_t value);

    //Logical operations

    /**
    * @brief AND n - Logical AND
    * @param value 8-bit operand
    */
    void andA(uint8_t value);

    /**
    * @brief OR n - Logical OR
    * @param value 8-bit operand
    */
    void orA(uint8_t value);

    /**
    * @brief XOR n - Logical Exclusive OR
    * @param value 8-bit operand
    */
    void xorA(uint8_t value);


    /**
    * @brief CP n - Compare with Accumulator
    * @param value 8-bit operand
    */
    void cp(uint8_t value);

    
    /**
    * @brief INC instruction for 8-bit register
    * @param reg reference to register to increment
    */
    void inc(uint8_t& reg);
    
    /**
    * @brief INC instruction helper for memory values
    * @param 8-bit value to increment
    * @return 8-bit incremented value
    */
    uint8_t inc_(uint8_t value);

    /**
    * @brief DEC instruction for 8-bit registers
    * @param 8-bit value to decrement
    */
    void dec(uint8_t& reg);

    /**
    * @brief DEC instruction helper for memory values
    * @param 8-bit value to decrement
    * @return 8-bit decremented value
    */
    uint8_t dec_(uint8_t value);

    // Helper functions for load operations
    /**
    * @brief LD reg, nn instruction
    * @param reg reference to 16-bit register pair to which we load nn
    */
    void ld(uint16_t& reg);

    /**
    * @brief LD (HL),n instruction
    */
    void ldHL();

    // Exchange operations
    /**
    * @brief EXX instruction
    * @details Exchange BC/DE/HL with their alternate registers
    */
    void exx();

    // Jump and Conditional Jump
    /**
    * @brief JP cc,nn - Conditional absolute jump
    * @param opcode opcode with condition encoding in bits 5-3
    */
    void jp();

    /**
    * @brief Conditional JP instruction
    * @param opcode Opcode containing condition, which gets
    * evaluated by checkCondition(uint8_t) later on
    */
    void condJP(uint8_t opcode);

    /**
    * @brief JR e - Jump Relative instruction
    * @details Performs PC-relative jump using signed 8-bit displacement
    * PC = PC + offset (e - offset)
    */
    void jr();

    /**
    * @brief Conditional JR cc,e - Jump Relative if condition
    * @param opcode Full opcode containing condition in bits 5-3
    */
    void condJR(uint8_t opcode);

    // Call/return operations
    /**
    * @brief CALL nn - Absolute Call instruction
    * @details Performs subroutine call:
    * 1. Pushes current PC+3 onto stack
    * 2. Loads PC with 16-bit immediate operand
    */
    void call();

    /**
    * @brief CALL cc,nn - Conditional Call
    * @param opcode full opcode containing condition in bits 5-3
    */
    void condCall(uint8_t opcode);

    /**
    * @brief RET cc - Conditional Return from Subroutine
    * @param opcode full opcode containing condition in bits 5-3
    * @details pops return address from stack only if condition met
    */
    void condRet(uint8_t opcode);

    /**
    * @brief SCF - Set Carry Flag
    * @details Sets Carry flag and clears N/H flags:
    */
    void setCarry();


    /**
    * @brief DAA - Decimal Adjust Accumulator
    * @details Corrects addition/subtraction results:
    * For addition (N=0): adjusts if half-carry occurred (H=1)
    * For subtraction (N=1): adjusts using two's complement method
    * Updates all flags 
    */
    void daa();


    /**
    * @brief Check condition code against current flags
    * @param condition 3-bit condition code (0-7)
    */
    bool checkCondition(uint8_t condition);

    // Stack operations

    /**
    * @brief PUSH qq - Push value onto stack
    * @param value 16-bit value to push
    */
    void push(uint16_t value);

    /**
    * @brief POP qq - Pop value from stack
    * @return uint16_t popped value
    */
    uint16_t pop();

    // Register helpers 

    /**
    * @brief Get register value by ID
    * @param reg 3-bit register code
    */
    uint8_t getReg(uint8_t reg);
    
    /**
    * @brief Set register value by ID
    * @param reg 3-bit register code
    */
    void setReg(uint8_t reg, uint8_t value);


    // Flag update helpers

    /**
    * @brief Update flags after addition operation
    * @param original_a register A before addition
    * @param value Value added
    * @param res 16-bit addition result
    */
    void updateFlagsAdd(uint8_t original_a, uint8_t value, uint16_t res);

    /**
    * @brief Update flags after subtraction operation
    * @param original_a register A before subtraction
    * @param value Value subtracted
    * @param res 16-bit subtraction result
    */
    void updateFlagsSub(uint8_t original_a, uint8_t value, uint16_t res);


    /**
    * @brief Update Sign and Zero flags
    */
    void updateSZ(uint8_t value);

    /**
    * @brief Update flags after INC/DEC operation
    * @param res Result value
    * @param old Original value
    * @param inc true = INC, false = DEC
    */
    void updateFlagsIncDec(uint8_t res, uint8_t old, bool inc = true);


    /**
    * @brief Calculate parity of a value
    * @param value 8-bit value to check
    * @detail Used for PV flag in logical operations
    */
    bool parityEven(uint8_t value);


    // Prefixed instruction handlers

    /**
    * @brief Handle ADD A,(IX/IY+d)
    * @param prefix Index register
    */ 
    void handleAdd(uint8_t prefix);

    /**
    * @brief Handle ADC A,(IX/IY+d)
    * @param prefix Index register
    */ 
    void handleAdc(uint8_t prefix);

    /**
    * @brief Handle SUB (IX/IY+d)
    * @param prefix Index register
    */ 
    void handleSub(uint8_t prefix);

    /**
    * @brief Handle SBC A, (IX/IY+d)
    */ 
    void handleSbc(uint8_t prefix);

    /**
    * @brief Handle AND (IX/IY+d)
    */
    void handleAnd(uint8_t prefix);

    /**
    * @brief Handle OR (IX/IY+d)
    */
    void handleOr(uint8_t prefix);

    /**
    * @brief Handle XOR (IX/IY+d)
    */
    void handleXor(uint8_t prefix);
    
    /**
    * @brief Handle CP (IX/IY+d)
    */
    void handleCp(uint8_t prefix);

    /**
    * @brief Handle INC (IX/IY+d)
    */
    void handleIncMem(uint8_t prefix);

    /**
    * @brief Handle DEC (IX/IY+d)
    */
    void handleDecMem(uint8_t prefix);

    /**
    * @brief Handle 8-bit LD operations
    */
    void handleLd(uint8_t opcode);

    /**
    * @brief Handle indexed LD operations(LD r,(IX/IY+d) or LD (IX/IY+d),r)
    */
    void handleIndexedLd(uint8_t prefix, uint8_t opcode);

};

inline uint8_t Z80::getA() const { return a; }
inline uint8_t Z80::getF() const { return f; }
inline uint8_t Z80::getB() const { return b; }
inline uint8_t Z80::getC() const { return c; }
inline uint8_t Z80::getD() const { return d; }
inline uint8_t Z80::getE() const { return e; }
inline uint8_t Z80::getL() const { return l; }
inline uint8_t Z80::getH() const { return h; }
inline uint8_t Z80::getA_P() const { return a_prime; }
inline uint8_t Z80::getF_P() const { return f_prime; }
inline uint8_t Z80::getB_P() const { return b_prime; }
inline uint8_t Z80::getC_P() const { return c_prime; }
inline uint8_t Z80::getD_P() const { return d_prime; }
inline uint8_t Z80::getE_P() const { return e_prime; }
inline uint8_t Z80::getL_P() const { return l_prime; }
inline uint8_t Z80::getH_P() const { return h_prime; }
inline uint16_t Z80::getAF() const { return af; }
inline uint16_t Z80::getBC() const { return bc; }
inline uint16_t Z80::getDE() const { return de; }
inline uint16_t Z80::getHL() const { return hl; }
inline uint16_t Z80::getAF_P() const { return af_prime; }
inline uint16_t Z80::getBC_P() const { return bc_prime; }
inline uint16_t Z80::getDE_P() const { return de_prime; }
inline uint16_t Z80::getHL_P() const { return hl_prime; }
inline uint16_t Z80::getIX() const { return ix; }
inline uint16_t Z80::getIY() const { return iy; }
inline uint16_t Z80::getPC() const { return pc; }
inline uint16_t Z80::getSP() const { return sp; }


#endif
