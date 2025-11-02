#include "common.h"

#define MAX_FILES 50
#define MAX_FILENAME 20

typedef struct {
    char name[MAX_FILENAME];
    char content[256];
    int size;
} File;

File files[MAX_FILES];
int file_count = 0;

void file_manager() {
    clear_screen(COLOR_BLACK, COLOR_LIGHT_CYAN);
    
    // Интерфейс файлового менеджера
    print_at("📁 Файловый менеджер - MyOS", 2, 1, MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW));
    print_at("Создать [N] | Удалить [D] | Редактировать [E] | Назад [ESC]", 
             2, 3, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    
    // Список файлов
    int y = 5;
    for (int i = 0; i < file_count; i++) {
        char line[40];
        // Простой вывод без sprintf
        strcpy(line, "   ");
        line[0] = '0' + (i + 1) / 10;
        line[1] = '0' + (i + 1) % 10;
        line[2] = '.';
        line[3] = ' ';
        strcpy(line + 4, files[i].name);
        
        print_at(line, 4, y, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREEN));
        y++;
    }
    
    if (file_count == 0) {
        print_at("Файлы не найдены. Создайте новый файл [N]", 
                 4, 7, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_RED));
    }
}

void init_file_manager() {
    // Добавляем демо файлы
    strcpy(files[0].name, "hello.c");
    strcpy(files[0].content, "#include <stdio.h>\n\nint main() {\n    printf(\"Hello MyOS!\\n\");\n    return 0;\n}");
    files[0].size = strlen(files[0].content);
    
    strcpy(files[1].name, "kernel.c");
    strcpy(files[1].content, "// Основное ядро ОС\nvoid kernel_main() {\n    // Инициализация системы\n}");
    files[1].size = strlen(files[1].content);
    
    file_count = 2;
}