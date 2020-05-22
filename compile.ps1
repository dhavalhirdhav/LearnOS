Write-Output "Clearing all binaries...."
del *.bin
del *.o
del *.elf
del *.objdump
del *.log

Write-Output "compile boot.asm"
fasm boot.asm

Write-Output "compile kernel.c"
wsl gcc -g -m32 -ffreestanding -c *.c drivers/*.c utils/*.c drivers/pci/*.c drivers/ata/*.c -nostartfiles -nostdlib

Write-Output "link all c object files"
wsl ld -m elf_i386 -nostdlib -T linker.ld *.o -o kernel.elf

Write-Output "compile loader.asm"
fasm loader.asm

Write-Output "Link loader and kernel"
wsl ld -m elf_i386 -nostdlib -T linker.ld loader.o kernel.elf -o kernel_full.elf

Write-Output "objdump loader file"
objdump -phxd loader.o > loader.objdump

Write-Output "objdump kernel file"
objdump -phxd kernel_full.elf > kernel.objdump

Write-Output "Producing elf file"
wsl objcopy kernel_full.elf -O binary kernel.bin

Write-Output "Padding kernel file"
$kernel_file_size=$(wsl wc -c kernel.bin).Split(" ")[0]
$kernel_sectors=[math]::ceiling($kernel_file_size/512)
$kernel_final_bytes=$kernel_sectors * 512
$kernel_pad_bytes=$kernel_final_bytes-$kernel_file_size
wsl dd if=/dev/null of=kernel.bin bs=1 count=$kernel_pad_bytes seek=$kernel_file_size
wsl dd if=/dev/zero of=kernel.bin bs=1 count=$kernel_pad_bytes seek=$kernel_file_size

Write-Output "Clearing .o and .elf"
del *.o
del *.elf

Write-Output "Compiling Kernel...."
wsl gcc -g -m32 -ffreestanding utils/*.c drivers/*.c prog/*.c -nostartfiles -nostdlib -o prog.o -T linker.ld
#wsl ld -m elf_i386 -nostdlib -T linker.ld *.o -o prog.elf
objdump prog.o -phxd > prog.objdump

Write-Output "Creating Empty Raw 10 MB HDD Image..."
wsl dd if=/dev/zero of=hdd.bin bs=1024 count=10240

Write-Output "Writing Kernel to HDD Image...."
$second_kernel_size=$(wsl wc -c prog.o).Split(" ")[0]
wsl dd if=prog.o of=hdd.bin count=$second_kernel_size conv=notrunc

Write-Output "Creating image...."
Get-Content boot.bin, kernel.bin, hdd.bin -Raw | Set-Content os_image.bin -NoNewline

Write-Output "Launching QEMU"
qemu-system-x86_64 os_image.bin
#bochs -f bochsconfig.conf
#bochsdbg -f bochsconfig.conf