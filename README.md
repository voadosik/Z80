# Z80 CPU Emulator 

## Build instructions

### Windows (MSVC 2022)
1. Open the 'Z80.vcxproj` project in Visual Studio 2022.
2. Ensure you have C++17 installed and enabled in project settings.
3. Build the solution (Ctrl+Shift+B).
4. Run the generated executable, to see the results of the tests.

### Linux (GCC)
1. Ensure you have GCC compiler(tested on version GCC 14.2).
2. Go to the src folder.
3. Using command line execute 'make', which will build the project.
4. Afterwards execute 'make start' to run the program tests.
5. To remove the executable type 'make clean'.



## Project Overview:
This project is an implementation of a core subset of Zilog Z80 CPU instructions. 
It emulates essential functionalities including data transfer, arithmetic, logical operations,
branching and stack operations. Emulator has tests to validate the correctness of the instructions.

### CPU Architecture:
The Z80 CPU has the following registers:

- 8-bit Registers: A (accumulator), B, C, D, E, H, L, F (flags)
- 16-bit Register Pairs: BC, DE, HL, AF, SP (Stack Pointer), PC (Program Counter), IX, IY
- Alternate Register Set: A', B', C', D', E', H', L', F'

### Flag Register:
The flag register (F) contains the following status bits:

- S (Sign): Set if the result is negative
- Z (Zero): Set if the result is zero
- H (Half Carry): Set if there was a carry from bit 3 to bit 4
- PV (Parity/Overflow): Set if the result has even parity / Set if arithmetic overflow occurred
- N (Subtract): Set if the last operation was a subtraction
- C (Carry): Set if the result generated a carry

### Memory Model
The Z80 CPU has a 16-bit address bus, allowing it to address 64KB of memory (0x0000 to 0xFFFF). 
The emulator implements this as a simple array of bytes.

## Implemented Features:
- **Core instructions**:

	### 8-bit Load Operations:
	- `LD r, r'` - Load register r with value from register r'
	- `LD r, n` - Load register r with value n
	- `LD r, (HL)` - Load register r with value at address (HL)
	- `LD (HL), r` - Store value from register r at address (HL)
	- `LD (HL), n` - Store value n at address (HL)

	### 16-bit Load Operations
	- `LD BC, nn` - Load register pair BC with value nn
	- `LD DE, nn` - Load register pair DE with value nn
	- `LD HL, nn` - Load register pair HL with value nn
	- `LD SP, nn` - Load stack pointer with value nn

	### Exchange Operations
	- `EX DE, HL` - Exchange the contents of DE and HL
	- `EX AF, AF'` - Exchange the contents of AF and AF'
	- `EXX` - Exchange BC, DE, HL with BC', DE', HL'

	### Arithmetic Operations
	- `ADD A, r` - Add register r to A
	- `ADD A, n` - Add value n to A
	- `ADD A, (HL)` - Add value at address (HL) to A
	- `ADC A, r` - Add register r and carry flag to A
	- `SUB r` - Subtract register r from A
	- `SUB n` - Subtract value n from A
	- `SBC A, r` - Subtract register r and carry flag from A
	- `INC r` - Increment register r
	- `INC (HL)` - Increment value at address (HL)
	- `DEC r` - Decrement register r
	- `DEC (HL)` - Decrement value at address (HL)
	- `DAA` - Decimal adjust accumulator

	### Logical Operations
	- `AND r` - Logical AND of register r with A
	- `AND n` - Logical AND of value n with A
	- `OR r` - Logical OR of register r with A
	- `OR n` - Logical OR of value n with A
	- `XOR r` - Logical XOR of register r with A
	- `XOR n` - Logical XOR of value n with A
	- `CP r` - Compare register r with A
	- `CP n` - Compare value n with A

	### Jump and Call Operations
	- `JP nn` - Jump to address nn
	- `JP cc, nn` - Conditional jump to address nn
	- `JR e` - Relative jump by signed offset e
	- `JR cc, e` - Conditional relative jump
	- `CALL nn` - Call subroutine at address nn
	- `CALL cc, nn` - Conditional call
	- `RET` - Return from subroutine
	- `RET cc` - Conditional return from subroutine

	### Stack Operations
	- `PUSH rr` - Push register pair onto stack
	- `POP rr` - Pop value from stack into register pair

	### Operationals
	- `HALT` - Halt CPU operation
	- `SCF` - Set carry flag

	### Indexed Operations
	- `ADD A, (IX+d)` - Add value at (IX+d) to A
	- `ADD A, (IY+d)` - Add value at (IY+d) to A
	- `LD r, (IX+d)` - Load register r with value at (IX+d)
	- `LD r, (IY+d)` - Load register r with value at (IY+d)
	- `LD (IX+d), r` - Store value from register r at (IX+d)
	- `LD (IY+d), r` - Store value from register r at (IY+d)
	- `LD (IX+d), n` - Store value n at (IX+d)
	- `LD (IY+d), n` - Store value n at (IY+d)


## Example Usage
```cpp
#include "../tests/Z80tests.hpp"  

void test() {
    Z80Tests tests;
    
    // Load a simple program that adds 0x10 + 0x20
    tests.loadProgram({
        LD_A_N, 0x10,   // Load 0x10 into A
        LD_B_N, 0x20,   // Load 0x20 into B
        ADD_A_B,        // A = A + B
        HALT            // Stop execution
    });
    
    // Execute the loaded program
    tests.executeUntilHalt();
    
    // Display the result
    std::cout << "Result = 0x" 
              << std::hex << (int)tests.cpu.getA() 
              << std::endl;
}
```

### Expected Output:
```plaintext
Result = 0x30
```
