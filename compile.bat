echo off
echo "clean all binaries"
del *.bin
del *.o
del *.elf
del *.objdump
del *.log

echo "compile boot.asm"
fasm boot.asm

echo "compile kernel.c"
wsl gcc -g -m32 -ffreestanding -c *.c drivers/*.c utils/*.c drivers/pci/*.c drivers/ata/*.c -nostartfiles -nostdlib

echo "link all c object files"
wsl ld -m elf_i386 -nostdlib -T linker.ld *.o -o kernel.elf

echo "compile loader.asm"
fasm loader.asm

echo "Link loader and kernel"
wsl ld -m elf_i386 -nostdlib -T linker.ld loader.o kernel.elf -o kernel_full.elf

echo "objdump loader file"
objdump -phxd loader.o > loader.objdump

echo "objdump kernel file"
objdump -phxd kernel_full.elf > kernel.objdump

echo "Producing elf file"
wsl objcopy kernel_full.elf -O binary kernel.bin

echo "Creating image...."
type boot.bin kernel.bin > os_image.bin

echo "Launching QEMU"
qemu-system-x86_64 os_image.bin
rem bochs -f bochsconfig.conf
rem bochsdbg -f bochsconfig.conf