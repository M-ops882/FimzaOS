#include "common.h"

#define EDITOR_WIDTH 70
#define EDITOR_HEIGHT 18

char code_buffer[10][80] = {
    "#include <stdio.h>",
    "",
    "int main() {",
    "    printf(\"Hello MyOS!\\n\");",
    "    ",
    "    for(int i = 0; i < 5; i++) {",
    "        printf(\"Count: %d\\n\", i);",
    "    }",
    "    ",
    "    return 0;"
};

int cursor_x = 4;
int cursor_y = 2;
char filename[20] = "hello.c";

void code_editor() {
    clear_screen(COLOR_BLUE, COLOR_LIGHT_GREY);
    
    // Заголовок редактора
    print_at("📝 Редактор кода - ", 2, 1, MAKE_COLOR(COLOR_BLUE, COLOR_YELLOW));
    print_at(filename, 20, 1, MAKE_COLOR(COLOR_BLUE, COLOR_LIGHT_CYAN));
    
    // Панель инструментов
    print_at("[F1]Сохранить [F2]Открыть [F3]Компилировать [ESC]Назад", 
             2, 2, MAKE_COLOR(COLOR_BLUE, COLOR_WHITE));
    
    // Окно редактора
    for (int y = 0; y < 10; y++) {
        print_at(code_buffer[y], 4, y + 4, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREEN));
    }
    
    // Боковая панель с номерами строк
    for (int y = 0; y < 10; y++) {
        char line_num[4];
        // Простая конвертация числа в строку
        line_num[0] = '0' + ((y + 1) / 10);
        line_num[1] = '0' + ((y + 1) % 10);
        line_num[2] = '\0';
        print_at(line_num, 1, y + 4, MAKE_COLOR(COLOR_BLUE, COLOR_LIGHT_MAGENTA));
    }
    
    // Статусная строка
    print_at("Строка 4, Столбец 5 | C | READY", 2, 22, MAKE_COLOR(COLOR_BLUE, COLOR_GREEN));
}

void save_file() {
    print_at("💾 Файл сохранен: ", 2, 24, MAKE_COLOR(COLOR_BLUE, COLOR_LIGHT_CYAN));
    print_at(filename, 20, 24, MAKE_COLOR(COLOR_BLUE, COLOR_YELLOW));
}

void compile_file() {
    print_at("🔨 Компиляция...", 50, 24, MAKE_COLOR(COLOR_BLUE, COLOR_LIGHT_BLUE));
    print_at("✅ Успешно!", 50, 23, MAKE_COLOR(COLOR_BLUE, COLOR_GREEN));
}