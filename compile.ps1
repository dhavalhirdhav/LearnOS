param($paramName)
Write-Output $paramName
Function Create-Directory($directoryPath) {
    if(Test-Path -Path $directoryPath -PathType Container) {
        Write-Output "$directoryPath directory exists..."
    }
    else {
        Write-Output "Creating $directoryPath Directory..."
        New-Item -Path $directoryPath -ItemType Directory
    }
}

Function CleanBinaries() {
    Write-Output "Clearing all binaries...."
    Remove-Item debug/bin/*.bin
    Remove-Item debug/obj/*.o
    Remove-Item debug/elf/*.elf
    Remove-Item debug/objdump/*.objdump
    Remove-Item *.log
}

Function Build() {
    Write-Output "Creating Output Directories..."
    Create-Directory('debug\bin')
    Create-Directory('debug\obj')
    Create-Directory('debug\objdump')
    Create-Directory('debug\elf')
    
    Write-Output "compile boot.asm"
    fasm boot/boot.asm debug/bin/boot.bin
    
    Write-Output "Compile second stage loader"
    wsl gcc -g -m32 -ffreestanding -c boot/*.c drivers/*.c utils/*.c drivers/ata/*.c -nostartfiles -nostdlib
    Move-Item *.o 'debug/obj'
    
    Write-Output "link all c object files"
    wsl ld -m elf_i386 -nostdlib -T ld/loader.ld debug/obj/*.o -o debug/elf/loader.elf
    
    Write-Output "compile loader.asm"
    fasm boot/loader.asm debug/obj/loader.o
    
    Write-Output "Link loader asm and loader c"
    wsl ld -m elf_i386 -nostdlib -T ld/loader.ld debug/obj/loader.o debug/elf/loader.elf -o debug/elf/loader_full.elf
    
    Write-Output "objdump loader file"
    objdump -phxd debug/obj/loader.o > debug/objdump/loader.objdump
    
    Write-Output "objdump loader C file"
    objdump -phxd debug/elf/loader_full.elf > debug/objdump/loader_full.objdump
    
    Write-Output "Producing elf file"
    wsl objcopy debug/elf/loader_full.elf -O binary debug/bin/loader.bin
    
    Write-Output "Padding loader bin file"
    $loader_file_size=$(wsl wc -c debug/bin/loader.bin).Split(" ")[0]
    $loader_sectors=[math]::ceiling($loader_file_size/512)
    $loader_final_bytes=$loader_sectors * 512
    $loader_pad_bytes=$loader_final_bytes-$loader_file_size
    wsl dd if=/dev/zero of=debug/bin/loader.bin bs=1 count=$loader_pad_bytes seek=$loader_file_size
    
    Write-Output "Clearing .o and .elf"
    Remove-Item debug/obj/*.o
    Remove-Item debug/elf/*.elf
    
    Write-Output "Compiling Kernel...."
    wsl gcc -g -m32 -ffreestanding utils/*.c drivers/*.c kernel/*.c -nostartfiles -nostdlib -o kernel.o -T ld/kernel.ld
    Move-Item *.o 'debug/obj'
    #wsl ld -m elf_i386 -nostdlib -T linker.ld *.o -o prog.elf
    objdump debug/obj/kernel.o -phxd > debug/objdump/kernel.objdump
    
    Write-Output "Creating Empty Raw 10 MB HDD Image..."
    wsl dd if=/dev/zero of=debug/bin/hdd.bin bs=1024 count=10240
    
    Write-Output "Writing Kernel to HDD Image...."
    $second_kernel_size=$(wsl wc -c debug/obj/kernel.o).Split(" ")[0]
    wsl dd if=debug/obj/kernel.o of=debug/bin/hdd.bin count=$second_kernel_size conv=notrunc
}

Function Make()
{
    Write-Output "Creating image...."
    Get-Content debug/bin/boot.bin, debug/bin/loader.bin, debug/bin/hdd.bin -Raw | Set-Content debug/bin/os_image.bin -NoNewline
}

Function RunOS()
{
    Write-Output "Launching QEMU"
    qemu-system-x86_64 debug/bin/os_image.bin
    # #bochs -f bochsconfig.conf
    # #bochsdbg -f bochsconfig.conf
}

if($paramName -eq "all" -Or $Null -eq $paramName)
{
    CleanBinaries
    Build
    Make
    RunOS
}

if($paramName -eq "clean")
{
    CleanBinaries
}

if($paramName -eq "build")
{
    Build
}

if($paramName -eq "make")
{
    Make
}

if($paramName -eq "run")
{
    RunOS
}