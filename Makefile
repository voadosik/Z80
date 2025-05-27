all:
	g++ -std=c++17 -I include/ Z80/cpu.cpp tests/Z80tests.cpp Z80/main.cpp -o z80_emulator

start: all
	chmod +x z80_emulator
	./z80_emulator

clean:
	rm -rf z80_emulator
