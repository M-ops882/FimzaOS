import os
import subprocess
import sys

print("🔨 Компиляция MyOS Developer Edition...")
print("Текущая директория:", os.getcwd())

# Проверяем наличие необходимых файлов
required_files = ["boot.s", "kernel.c", "linker.ld"]
for file in required_files:
    if not os.path.exists(file):
        print(f"❌ Файл {file} не найден!")
        sys.exit(1)

# Удаляем старые файлы
for f in ["boot.o", "kernel.o", "kernel.bin"]:
    if os.path.exists(f):
        os.remove(f)

# Шаг 1: Компиляция boot.s
print("📝 Компиляция boot.s...")
result = subprocess.run([
    "gcc", "-m32", "-nostdlib", "-ffreestanding", 
    "-c", "boot.s", "-o", "boot.o"
], capture_output=True, text=True)

if result.returncode != 0:
    print(f"❌ Ошибка компиляции boot.s:")
    print(result.stderr)
    sys.exit(1)

# Шаг 2: Компиляция kernel.c  
print("📝 Компиляция kernel.c...")
result = subprocess.run([
    "gcc", "-m32", "-nostdlib", "-ffreestanding",
    "-I.", "-c", "kernel.c", "-o", "kernel.o"
], capture_output=True, text=True)

if result.returncode != 0:
    print(f"❌ Ошибка компиляции kernel.c:")
    print(result.stderr)
    sys.exit(1)

# Шаг 3: Линковка
print("🔗 Линковка...")
result = subprocess.run([
    "ld", "-m", "elf_i386", "-T", "linker.ld", 
    "-o", "kernel.bin", "boot.o", "kernel.o"
], capture_output=True, text=True)

if result.returncode != 0:
    print(f"❌ Ошибка линковки:")
    print(result.stderr)
    sys.exit(1)

# Проверяем создание kernel.bin
if not os.path.exists("kernel.bin"):
    print("❌ kernel.bin не создан!")
    sys.exit(1)

print("✅ Сборка успешна!")
print("📁 Созданные файлы:", [f for f in os.listdir('.') if f.endswith(('.o', '.bin'))])

# Запуск QEMU
print("🚀 Запуск QEMU...")
os.system("qemu-system-i386 -kernel kernel.bin")