echo off
echo "clean all binaries"
del *.bin
del *.o
del *.elf

echo "compile boot.asm"
fasm boot.asm

echo "compile loader.asm"
fasm loader.asm

echo "compile kernel.c"
wsl gcc -m32 -ffreestanding -o kernel.o kernel.c ports.c

wsl objcopy kernel.o -O elf32-i386 kernel.elf
wsl /usr/local/i386elfgcc/bin/i386-elf-ld -o kernel.bin -Ttext 0x1000 loader.o kernel.elf --oformat binary
type boot.bin kernel.bin > os_image.bin
qemu-system-x86_64 os_image.bin