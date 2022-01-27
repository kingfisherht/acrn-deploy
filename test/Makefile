all:
	nasm -f elf datasegment.asm -g
	ld -m elf_i386 -s -o test datasegment.o
clean:
	rm *.o
