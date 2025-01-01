all: task1 task2 task2.1 task2.2 task2.3

task1: task1.asm
	nasm -f elf task1.asm -o task1.o
	gcc -m32 driver.o task1.o asm_io.o -o task1

task2.: task2.asm
	nasm -f elf task2.asm -o task2.o
	gcc -m32 driver.o task2.o asm_io.o -o task2

task2.1: task2.1.asm
	nasm -f elf task2.1.asm -o task2.1.o
	gcc -m32 driver.o task2.1.o asm_io.o -o task2.1

task2.2: task2.2.asm
	nasm -f elf task2.2.asm -o task2.2.o
	gcc -m32 driver.o task2.2.o asm_io.o -o task2.2

task2.3: task2.3.asm
	nasm -f elf task2.3.asm -o task2.3.o
	gcc -m32 driver.o task2.3.o asm_io.o -o task2.3