wsl /usr/local/i386elfgcc/bin/i386-elf-gcc -m32 -ffreestanding -c kernel.c -o kernel.o
wsl /usr/local/i386elfgcc/bin/i386-elf-ld -o kernel.bin -Ttext 0x1000 loader.obj kernel.o
type boot.bin kernel.bin > os_image.bin
qemu-system-x86_64 os_image.bin