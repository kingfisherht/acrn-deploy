all:
	# nasm -f elf dispstr.asm -o dispstr.o
	# gcc -o test test.c dispstr.o -m32
	nasm -f elf callc.asm -o main.o
	gcc -m32 -c  acallc.c -o acallc.o
	ld -m elf_i386 main.o acallc.o -o test
clean:
	rm *.o test

