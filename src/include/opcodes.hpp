#ifndef OPCODES_HPP
#define OPCODES_HPP

#include <cstdint>

// Prefixed Group
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
constexpr uint8_t LD_IXY = 0x21;
constexpr uint8_t LD_IXY_d = 0x36;
constexpr uint8_t PREFIX_DD = 0xDD;
constexpr uint8_t PREFIX_FD = 0xFD;


// 8-bit Arithmetic Group
constexpr uint8_t ADD_A_B = 0x80;
constexpr uint8_t ADD_A_C = 0x81;
constexpr uint8_t ADD_A_D = 0x82;
constexpr uint8_t ADD_A_E = 0x83;
constexpr uint8_t ADD_A_H = 0x84;
constexpr uint8_t ADD_A_L = 0x85;
constexpr uint8_t ADD_A_HL = 0x86;
constexpr uint8_t ADD_A_A = 0x87;
constexpr uint8_t ADD_A_N = 0xC6;

constexpr uint8_t ADC_A_B = 0x88;
constexpr uint8_t ADC_A_C = 0x89;
constexpr uint8_t ADC_A_D = 0x8A;
constexpr uint8_t ADC_A_E = 0x8B;
constexpr uint8_t ADC_A_H = 0x8C;
constexpr uint8_t ADC_A_L = 0x8D;
constexpr uint8_t ADC_A_HL = 0x8E;
constexpr uint8_t ADC_A_A = 0x8F;
constexpr uint8_t ADC_A_N = 0xCE;

constexpr uint8_t SUB_B = 0x90;
constexpr uint8_t SUB_C = 0x91;
constexpr uint8_t SUB_D = 0x92;
constexpr uint8_t SUB_E = 0x93;
constexpr uint8_t SUB_H = 0x94;
constexpr uint8_t SUB_L = 0x95;
constexpr uint8_t SUB_HL = 0x96;
constexpr uint8_t SUB_A = 0x97;
constexpr uint8_t SUB_N = 0xD6;

constexpr uint8_t SBC_A_B = 0x98;
constexpr uint8_t SBC_A_C = 0x99;
constexpr uint8_t SBC_A_D = 0x9A;
constexpr uint8_t SBC_A_E = 0x9B;
constexpr uint8_t SBC_A_H = 0x9C;
constexpr uint8_t SBC_A_L = 0x9D;
constexpr uint8_t SBC_A_HL = 0x9E;
constexpr uint8_t SBC_A_A = 0x9F;
constexpr uint8_t SBC_A_N = 0xDE;

// Logical Group
constexpr uint8_t AND_B = 0xA0;
constexpr uint8_t AND_C = 0xA1;
constexpr uint8_t AND_D = 0xA2;
constexpr uint8_t AND_E = 0xA3;
constexpr uint8_t AND_H = 0xA4;
constexpr uint8_t AND_L = 0xA5;
constexpr uint8_t AND_HL = 0xA6;
constexpr uint8_t AND_A = 0xA7;
constexpr uint8_t AND_N = 0xE6;

constexpr uint8_t OR_B = 0xB0;
constexpr uint8_t OR_C = 0xB1;
constexpr uint8_t OR_D = 0xB2;
constexpr uint8_t OR_E = 0xB3;
constexpr uint8_t OR_H = 0xB4;
constexpr uint8_t OR_L = 0xB5;
constexpr uint8_t OR_HL = 0xB6;
constexpr uint8_t OR_A = 0xB7;
constexpr uint8_t OR_N = 0xF6;

constexpr uint8_t XOR_B = 0xA8;
constexpr uint8_t XOR_C = 0xA9;
constexpr uint8_t XOR_D = 0xAA;
constexpr uint8_t XOR_E = 0xAB;
constexpr uint8_t XOR_H = 0xAC;
constexpr uint8_t XOR_L = 0xAD;
constexpr uint8_t XOR_HL = 0xAE;
constexpr uint8_t XOR_A = 0xAF;
constexpr uint8_t XOR_N = 0xEE;

// Compare Group
constexpr uint8_t CP_B = 0xB8;
constexpr uint8_t CP_C = 0xB9;
constexpr uint8_t CP_D = 0xBA;
constexpr uint8_t CP_E = 0xBB;
constexpr uint8_t CP_H = 0xBC;
constexpr uint8_t CP_L = 0xBD;
constexpr uint8_t CP_HL = 0xBE;
constexpr uint8_t CP_A = 0xBF;
constexpr uint8_t CP_N = 0xFE;

// IncDec Group
constexpr uint8_t INC_B = 0x04;
constexpr uint8_t INC_C = 0x0C;
constexpr uint8_t INC_D = 0x14;
constexpr uint8_t INC_E = 0x1C;
constexpr uint8_t INC_H = 0x24;
constexpr uint8_t INC_L = 0x2C;
constexpr uint8_t INC_A = 0x3C;
constexpr uint8_t INC_HL = 0x34;

constexpr uint8_t DEC_B = 0x05;
constexpr uint8_t DEC_C = 0x0D;
constexpr uint8_t DEC_D = 0x15;
constexpr uint8_t DEC_E = 0x1D;
constexpr uint8_t DEC_H = 0x25;
constexpr uint8_t DEC_L = 0x2D;
constexpr uint8_t DEC_A = 0x3D;
constexpr uint8_t DEC_HL = 0x35;

// Load Group

// 00 <-r-> 110, where r(3 bits) is a register identifier

//LD r, n
constexpr uint8_t LD_A_N = 0x3E; // 0b00111110
constexpr uint8_t LD_B_N = 0x06; // 0b00000110
constexpr uint8_t LD_C_N = 0x0E; // 0b00001110
constexpr uint8_t LD_D_N = 0x16; // 0b00010110
constexpr uint8_t LD_E_N = 0x1E; // 0b00011110
constexpr uint8_t LD_H_N = 0x26; // 0b00100110
constexpr uint8_t LD_L_N = 0x2E; // 0b00101110

constexpr uint8_t LD_HL_N = 0x36;


// 01 <-r-> 110, where r(3 bits) is a register identifier
//LD r, (HL)
constexpr uint8_t LD_A_HL = 0x7E; // 0b01111110
constexpr uint8_t LD_B_HL = 0x46; // 0b01000110  
constexpr uint8_t LD_C_HL = 0x4E; // 0b01001110
constexpr uint8_t LD_D_HL = 0x56; // 0b01010110
constexpr uint8_t LD_E_HL = 0x5E; // 0b01011110
constexpr uint8_t LD_H_HL = 0x66; // 0b01100110
constexpr uint8_t LD_L_HL = 0x6E; // 0b01101110


// LD r,r'
// 01 xxx yyy, (xxx - destination, yyy - source)
constexpr uint8_t LD_B_B = 0x40; // 0b01000000
constexpr uint8_t LD_B_C = 0x41; // 0b01000001
constexpr uint8_t LD_B_D = 0x42; // 0b01000010
constexpr uint8_t LD_B_E = 0x43; // 0b01000011
constexpr uint8_t LD_B_H = 0x44; // 0b01000100
constexpr uint8_t LD_B_L = 0x45; // 0b01000101
constexpr uint8_t LD_B_A = 0x47; // 0b01000111

constexpr uint8_t LD_C_B = 0x48; // 0b01001000
constexpr uint8_t LD_C_C = 0x49; // 0b01001001
constexpr uint8_t LD_C_D = 0x4A; // 0b01001010
constexpr uint8_t LD_C_E = 0x4B; // 0b01001011
constexpr uint8_t LD_C_H = 0x4C; // 0b01001100
constexpr uint8_t LD_C_L = 0x4D; // 0b01001101
constexpr uint8_t LD_C_A = 0x4F; // 0b01001111

constexpr uint8_t LD_D_B = 0x50; // 0b01010000
constexpr uint8_t LD_D_C = 0x51; // 0b01010001
constexpr uint8_t LD_D_D = 0x52; // 0b01010010
constexpr uint8_t LD_D_E = 0x53; // 0b01010011
constexpr uint8_t LD_D_H = 0x54; // 0b01010100
constexpr uint8_t LD_D_L = 0x55; // 0b01010101
constexpr uint8_t LD_D_A = 0x57; // 0b01010111

constexpr uint8_t LD_E_B = 0x58; // 0b01011000
constexpr uint8_t LD_E_C = 0x59; // 0b01011001
constexpr uint8_t LD_E_D = 0x5A; // 0b01011010
constexpr uint8_t LD_E_E = 0x5B; // 0b01011011
constexpr uint8_t LD_E_H = 0x5C; // 0b01011100
constexpr uint8_t LD_E_L = 0x5D; // 0b01011101
constexpr uint8_t LD_E_A = 0x5F; // 0b01011111

constexpr uint8_t LD_H_B = 0x60; // 0b01100000
constexpr uint8_t LD_H_C = 0x61; // 0b01100001
constexpr uint8_t LD_H_D = 0x62; // 0b01100010
constexpr uint8_t LD_H_E = 0x63; // 0b01100011
constexpr uint8_t LD_H_H = 0x64; // 0b01100100
constexpr uint8_t LD_H_L = 0x65; // 0b01100101
constexpr uint8_t LD_H_A = 0x67; // 0b01100111

constexpr uint8_t LD_L_B = 0x68; // 0b01101000
constexpr uint8_t LD_L_C = 0x69; // 0b01101001
constexpr uint8_t LD_L_D = 0x6A; // 0b01101010
constexpr uint8_t LD_L_E = 0x6B; // 0b01101011
constexpr uint8_t LD_L_H = 0x6C; // 0b01101100
constexpr uint8_t LD_L_L = 0x6D; // 0b01101101
constexpr uint8_t LD_L_A = 0x6F; // 0b01101111

constexpr uint8_t LD_A_B = 0x78; // 0b01111000
constexpr uint8_t LD_A_C = 0x79; // 0b01111001
constexpr uint8_t LD_A_D = 0x7A; // 0b01111010
constexpr uint8_t LD_A_E = 0x7B; // 0b01111011
constexpr uint8_t LD_A_H = 0x7C; // 0b01111100
constexpr uint8_t LD_A_L = 0x7D; // 0b01111101
constexpr uint8_t LD_A_A = 0x7F; // 0b01111111


// LD (HL), r
// 01110 yyy (yyy - source)
constexpr uint8_t LD_HL_B = 0x70; // 0b01110000
constexpr uint8_t LD_HL_C = 0x71; // 0b01110001
constexpr uint8_t LD_HL_D = 0x72; // 0b01110010
constexpr uint8_t LD_HL_E = 0x73; // 0b01110011
constexpr uint8_t LD_HL_H = 0x74; // 0b01110100
constexpr uint8_t LD_HL_L = 0x75; // 0b01110101
constexpr uint8_t LD_HL_A = 0x77; // 0b01110111


// LD dd, nn
// 00 xx 0001 (xx - register pair id)
constexpr uint8_t LD_BC_NN = 0x01; // 0b00000001
constexpr uint8_t LD_DE_NN = 0x11; // 0b00010001
constexpr uint8_t LD_HL_NN = 0x21; // 0b00100001
constexpr uint8_t LD_SP_NN = 0x31; // 0b00110001


// Exchange Group
constexpr uint8_t EX_DE_HL = 0xEB;
constexpr uint8_t EX_AF_AF = 0x08;
constexpr uint8_t EXX = 0xD9;

// Jump Group
constexpr uint8_t JP_NN = 0xC3;
constexpr uint8_t JP_NZ = 0xC2;
constexpr uint8_t JP_Z = 0xCA;
constexpr uint8_t JP_NC = 0xD2;
constexpr uint8_t JP_C = 0xDA;
constexpr uint8_t JP_PO = 0xE2;
constexpr uint8_t JP_PE = 0xEA;
constexpr uint8_t JP_P = 0xF2;
constexpr uint8_t JP_M = 0xFA;

constexpr uint8_t JR = 0x18;
constexpr uint8_t JR_NZ = 0x20;
constexpr uint8_t JR_Z = 0x28;
constexpr uint8_t JR_NC = 0x30;
constexpr uint8_t JR_C = 0x38;

// Call/Return Group
constexpr uint8_t CALL_NN = 0xCD;
constexpr uint8_t CALL_NZ = 0xC4;
constexpr uint8_t CALL_Z = 0xCC;
constexpr uint8_t CALL_NC = 0xD4;
constexpr uint8_t CALL_C = 0xDC;
constexpr uint8_t CALL_PO = 0xE4;
constexpr uint8_t CALL_PE = 0xEC;
constexpr uint8_t CALL_P = 0xF4;
constexpr uint8_t CALL_M = 0xFC;

constexpr uint8_t RET = 0xC9;
constexpr uint8_t RET_NZ = 0xC0;
constexpr uint8_t RET_Z = 0xC8;
constexpr uint8_t RET_NC = 0xD0;
constexpr uint8_t RET_C = 0xD8;
constexpr uint8_t RET_PO = 0xE0;
constexpr uint8_t RET_PE = 0xE8;
constexpr uint8_t RET_P = 0xF0;
constexpr uint8_t RET_M = 0xF8;

// Stack Group
// 11qq0101, qq - register id
constexpr uint8_t PUSH_BC = 0xC5; //0b11000101
constexpr uint8_t PUSH_DE = 0xD5; //0b11010101
constexpr uint8_t PUSH_HL = 0xE5; //0b11100101
constexpr uint8_t PUSH_AF = 0xF5; //0b11110101

constexpr uint8_t POP_BC = 0xC1; //0b11000001
constexpr uint8_t POP_DE = 0xD1; //0b11010001
constexpr uint8_t POP_HL = 0xE1; //0b11100001
constexpr uint8_t POP_AF = 0xF1; //0b11110001

// Control Group
constexpr uint8_t HALT = 0x76;
constexpr uint8_t SCF = 0x37;
constexpr uint8_t DAA = 0x27;


namespace Conditions {
    constexpr uint8_t NZ = 0;
    constexpr uint8_t Z = 1;
    constexpr uint8_t NC = 2;
    constexpr uint8_t C = 3;
    constexpr uint8_t PO = 4;
    constexpr uint8_t PE = 5;
    constexpr uint8_t P = 6;
    constexpr uint8_t M = 7;
}

namespace Regs {
    constexpr uint8_t B = 0;
    constexpr uint8_t C = 1;
    constexpr uint8_t D = 2;
    constexpr uint8_t E = 3;
    constexpr uint8_t H = 4;
    constexpr uint8_t L = 5;
    constexpr uint8_t A = 7;
};


#endif
