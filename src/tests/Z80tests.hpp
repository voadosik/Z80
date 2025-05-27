#ifndef Z80_TESTS_HPP
#define Z80_TESTS_HPP

#include "../include/cpu.hpp"
#include <cassert>
#include <iostream>


class Z80Tests {
public:
    void runAllTests();
    void loadProgram(const std::vector<uint8_t>& program);
    void executeUntilHalt();
    Z80 cpu;

private:

    void returnFinalState();
    void test8BitLoads();
    void test16BitLoads();
    void testExchangeOps();
    void test8BitArithmetic();
    void testLogicalOps();
    void testCompareOps();
    void testIncDec();
    void testJumpOps();
    void testCallReturn();
    void testStackOps();
    void testIndexedOps();
    void testFlagOps();
    void testConditionalOps();
    void testConditionalJump();

};


#endif
