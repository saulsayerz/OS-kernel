# Makefile
all: diskimage bootloader interrupt stdlib kernel shell

# Recipes
diskimage:
	# TODO : Tambahkan untuk pembuatan image
	dd if=/dev/zero of=out/system.img bs=512 count=2880

bootloader:
	# TODO : Tambahkan untuk pembuatan bootloader
	nasm src/asm/bootloader.asm -o out/bootloader
	dd if=out/bootloader of=out/system.img bs=512 count=1 conv=notrunc

interrupt :
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o

kernel:
	# TODO : Tambahkan untuk pembuatan kernel
	bcc -ansi -c -o out/kernel.o src/c/kernel.c
	bcc -ansi -c -o out/string.o src/c/string.c
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/std_lib.o out/lib_interrupt.o out/string.o
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1

stdlib :
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c

shell:

	bcc -ansi -c -o out/fileio.o src/c/fileio.c
	bcc -ansi -c -o out/program.o src/c/program.c
	bcc -ansi -c -o out/string.o src/c/string.c
	bcc -ansi -c -o out/utils.o src/c/utils.c
	bcc -ansi -c -o out/textio.o src/c/textio.c
	bcc -ansi -c -o out/shell.o src/c/shell.c
	bcc -ansi -c -o out/mkdir.o src/c/mkdir.c
	bcc -ansi -c -o out/mv.o src/c/mv.c
	bcc -ansi -c -o out/cd.o src/c/cd.c
	bcc -ansi -c -o out/cp.o src/c/cp.c
	bcc -ansi -c -o out/ls.o src/c/ls.c
	bcc -ansi -c -o out/cat.o src/c/cat.c

	nasm -f as86 src/asm/utils.asm -o out/utils_asm.o

	ld86 -o out/shell -d out/shell.o out/lib_interrupt.o out/string.o out/textio.o out/program.o out/fileio.o out/utils.o out/std_lib.o out/utils_asm.o
	ld86 -o out/ls -d out/ls.o out/fileio.o out/program.o out/std_lib.o out/string.o out/textio.o out/lib_interrupt.o out/utils.o out/kernel_asm.o out/utils_asm.o
	ld86 -o out/cd -d out/cd.o out/fileio.o out/program.o out/std_lib.o out/string.o out/textio.o out/lib_interrupt.o out/utils.o out/kernel_asm.o out/utils_asm.o
	ld86 -o out/mv -d out/mv.o out/fileio.o out/program.o out/std_lib.o out/string.o out/textio.o out/lib_interrupt.o out/utils.o out/kernel_asm.o out/utils_asm.o
	ld86 -o out/mkdir -d out/mkdir.o out/fileio.o out/program.o out/std_lib.o out/string.o out/textio.o out/lib_interrupt.o out/utils.o out/kernel_asm.o out/utils_asm.o
	ld86 -o out/cat -d out/cat.o out/fileio.o out/program.o out/std_lib.o out/string.o out/textio.o out/lib_interrupt.o out/utils.o out/kernel_asm.o out/utils_asm.o
	ld86 -o out/cp -d out/cp.o out/fileio.o out/program.o out/std_lib.o out/string.o out/textio.o out/lib_interrupt.o out/utils.o out/kernel_asm.o out/utils_asm.o

run:
	bochs -f src/config/if2230.config

build-run: all run
