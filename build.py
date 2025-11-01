import os
import subprocess

print("🔨 Компиляция Python Shell OS...")

# Команды сборки
commands = [
    "gcc -m32 -nostdlib -ffreestanding -c boot.s -o boot.o",
    "gcc -m32 -nostdlib -ffreestanding -c kernel.c -o kernel.o",
    "ld -m elf_i386 -T linker.ld -o kernel.bin boot.o kernel.o"
]

for cmd in commands:
    print(f"Выполняется: {cmd}")
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"❌ Ошибка: {result.stderr}")
        exit(1)

print("✅ Сборка успешна!")
print("🚀 Запуск QEMU...")
os.system("qemu-system-i386 -kernel kernel.bin")