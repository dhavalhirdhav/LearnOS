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
wsl gcc -g -m32 -ffreestanding drivers/ports.h drivers/screen.h kernel.c drivers/ports.c drivers/screen.c -o kernel.o

echo "Producing elf file"
wsl objcopy kernel.o -O elf32-i386 kernel.elf

echo "Linking files"
wsl /usr/local/i386elfgcc/bin/i386-elf-ld -o kernel.bin -Ttext 0x1000 loader.o kernel.elf --oformat binary

echo "Creating image...."
type boot.bin kernel.bin > os_image.bin

echo "Launching QEMU"
rem qemu-system-x86_64 os_image.bin
rem bochs -f bochsconfig.txt
bochsdbg -f bochsconfig.conf