#nasm -f bin test.asm
all:
	nasm -f elf test.asm -g
	ld -m elf_i386 -s -o test test.o
clean:
	rm *.o
