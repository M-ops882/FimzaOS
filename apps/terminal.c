#include "common.h"

char terminal_buffer[1024];
int term_pos = 0;

void terminal_app() {
    clear_screen(COLOR_BLACK, COLOR_LIGHT_GREEN);
    
    // Интерфейс терминала
    print_at("💻 Терминал разработчика - MyOS", 2, 1, MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW));
    print_at("gcc hello.c -o hello", 2, 3, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    print_at("./hello", 2, 4, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    print_at("Hello MyOS!", 2, 5, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_CYAN));
    
    print_at("Доступные команды:", 2, 7, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_MAGENTA));
    print_at("gcc <file> - компиляция C", 4, 8, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    print_at("./<app> - запуск программы", 4, 9, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    print_at("ls - список файлов", 4, 10, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    print_at("edit <file> - редактор", 4, 11, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    
    // Эмуляция вывода компилятора
    print_at("$ gcc hello.c - o hello", 2, 13, MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    print_at("Компиляция успешна!", 2, 14, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREEN));
    print_at("$ ./hello", 2, 15, MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    print_at("Hello MyOS!", 2, 16, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_CYAN));
    
    print_at(">>> _", 2, 18, MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW));
}