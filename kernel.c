#include "common.h"

// Определяем video
volatile char *video = (volatile char*)VGA_MEMORY;

// Системные переменные
int cursor_x = 0;
int cursor_y = 0;
int current_selection = 0;
int in_menu = 1;
int mouse_x = 40;
int mouse_y = 12;
char input_buffer[256];
int input_pos = 0;

// Структура для хранения состояния ввода
typedef struct {
    int key_pressed;
    int mouse_click;
    int mouse_x;
    int mouse_y;
} InputState;

InputState current_input = {0, 0, 40, 12};

// Базовые функции
int strlen(const char *str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

void strcpy(char *dest, const char *src) {
    int i = 0;
    while (src[i]) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

void clear_screen(char bg_color, char fg_color) {
    char color = MAKE_COLOR(bg_color, fg_color);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = color;
    }
    cursor_x = 0;
    cursor_y = 0;
}

void putchar_color(char c, char color) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        int idx = (cursor_y * VGA_WIDTH + cursor_x) * 2;
        video[idx] = c;
        video[idx + 1] = color;
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = VGA_HEIGHT - 1;
    }
}

void print_color(const char *str, char color) {
    while (*str) {
        putchar_color(*str, color);
        str++;
    }
}

void print(const char *str) {
    print_color(str, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREY));
}

void print_at(const char *str, int x, int y, char color) {
    int old_x = cursor_x;
    int old_y = cursor_y;
    
    cursor_x = x;
    cursor_y = y;
    
    print_color(str, color);
    
    cursor_x = old_x;
    cursor_y = old_y;
}

// Функции графического интерфейса
void draw_window(int x, int y, int w, int h, char *title, char border_color) {
    // Простая рамка
    if (title) {
        print_at(title, x + 1, y, border_color);
    }
    
    // Границы
    for (int i = x; i < x + w && i < VGA_WIDTH; i++) {
        int top_idx = (y * VGA_WIDTH + i) * 2;
        int bottom_idx = ((y + h - 1) * VGA_WIDTH + i) * 2;
        video[top_idx + 1] = border_color;
        if (y + h - 1 < VGA_HEIGHT) {
            video[bottom_idx + 1] = border_color;
        }
    }
    
    for (int j = y; j < y + h && j < VGA_HEIGHT; j++) {
        int left_idx = (j * VGA_WIDTH + x) * 2;
        int right_idx = (j * VGA_WIDTH + (x + w - 1)) * 2;
        video[left_idx + 1] = border_color;
        if (x + w - 1 < VGA_WIDTH) {
            video[right_idx + 1] = border_color;
        }
    }
}

void draw_button(int x, int y, int w, int h, char *text, char color, int selected) {
    // Кнопка с выделением если выбрана
    char button_color = selected ? MAKE_COLOR(COLOR_YELLOW, COLOR_BLACK) : color;
    
    for (int j = y; j < y + h && j < VGA_HEIGHT; j++) {
        for (int i = x; i < x + w && i < VGA_WIDTH; i++) {
            int idx = (j * VGA_WIDTH + i) * 2;
            video[idx + 1] = button_color;
        }
    }
    
    print_at(text, x + 1, y + h/2, button_color);
}

// Эмуляция ввода с клавиатуры
void update_input() {
    static int input_cycle = 0;
    input_cycle++;
    
    // Эмуляция разных сценариев ввода в зависимости от приложения
    if (in_menu) {
        // В меню - переключаем выбор
        if (input_cycle % 50 == 0) {
            current_selection = (current_selection + 1) % 6;
            current_input.key_pressed = 1;
        }
        
        // Эмуляция нажатия ENTER
        if (input_cycle == 200) {
            current_input.key_pressed = 2; // ENTER
            input_cycle = 0;
        }
    } else {
        // В приложениях - эмуляция разного ввода
        if (input_cycle % 30 == 0) {
            current_input.mouse_x = (current_input.mouse_x + 5) % 70 + 5;
            current_input.mouse_y = (current_input.mouse_y + 3) % 20 + 5;
        }
        
        if (input_cycle == 150) {
            current_input.mouse_click = 1;
        }
        
        if (input_cycle == 180) {
            current_input.key_pressed = 27; // ESC
            input_cycle = 0;
        }
    }
}

// Отрисовка курсора мыши
void draw_mouse() {
    int x = current_input.mouse_x;
    int y = current_input.mouse_y;
    
    if (x < VGA_WIDTH && y < VGA_HEIGHT) {
        int idx = (y * VGA_WIDTH + x) * 2;
        video[idx] = 'X';
        video[idx + 1] = MAKE_COLOR(COLOR_RED, COLOR_WHITE);
    }
}

// Главное меню
void main_menu() {
    clear_screen(COLOR_BLUE, COLOR_WHITE);
    
    print_at("🚀 MyOS Developer Edition v2.0", 25, 2, MAKE_COLOR(COLOR_BLUE, COLOR_YELLOW));
    print_at("ОС для разработчиков", 32, 3, MAKE_COLOR(COLOR_BLUE, COLOR_LIGHT_CYAN));
    
    draw_window(20, 5, 40, 15, "Главное меню", MAKE_COLOR(COLOR_LIGHT_BLUE, COLOR_BLACK));
    
    // Пункты меню
    char *menu_items[] = {
        "📝 Редактор кода",
        "💻 Терминал разработчика", 
        "📁 Файловый менеджер",
        "🔧 Системная информация",
        "🎮 Демо ввода",
        "🚪 Выход"
    };
    
    for (int i = 0; i < 6; i++) {
        if (i == current_selection) {
            // Выбранный пункт
            print_at("> ", 23, 7 + i, MAKE_COLOR(COLOR_LIGHT_BLUE, COLOR_YELLOW));
            print_at(menu_items[i], 25, 7 + i, MAKE_COLOR(COLOR_LIGHT_BLUE, COLOR_YELLOW));
        } else {
            // Обычный пункт
            print_at("  ", 23, 7 + i, MAKE_COLOR(COLOR_LIGHT_BLUE, COLOR_WHITE));
            print_at(menu_items[i], 25, 7 + i, MAKE_COLOR(COLOR_LIGHT_BLUE, COLOR_WHITE));
        }
    }
    
    // Инструкции
    print_at("Эмуляция ввода: Автоматическое переключение и клики", 15, 22, MAKE_COLOR(COLOR_BLUE, COLOR_LIGHT_GREEN));
    
    // Отрисовка мыши
    draw_mouse();
}

// Файловый менеджер
#define MAX_FILES 50
#define MAX_FILENAME 20

typedef struct {
    char name[MAX_FILENAME];
    char content[256];
    int size;
    int selected;
} File;

File files[MAX_FILES];
int file_count = 0;
int selected_file = 0;

void init_file_manager() {
    // Добавляем демо файлы
    strcpy(files[0].name, "hello.c");
    strcpy(files[0].content, "#include <stdio.h>\n\nint main() {\n    printf(\"Hello MyOS!\\n\");\n    return 0;\n}");
    files[0].size = strlen(files[0].content);
    files[0].selected = 0;
    
    strcpy(files[1].name, "kernel.c");
    strcpy(files[1].content, "// Основное ядро ОС\nvoid kernel_main() {\n    // Инициализация системы\n}");
    files[1].size = strlen(files[1].content);
    files[1].selected = 0;
    
    strcpy(files[2].name, "build.py");
    strcpy(files[2].content, "import os\nprint('Building MyOS...')");
    files[2].size = strlen(files[2].content);
    files[2].selected = 0;
    
    strcpy(files[3].name, "README.md");
    strcpy(files[3].content, "# MyOS Developer Edition\nCustom operating system for developers");
    files[3].size = strlen(files[3].content);
    files[3].selected = 1; // Выбран по умолчанию
    
    file_count = 4;
}

void file_manager_app() {
    clear_screen(COLOR_BLACK, COLOR_LIGHT_CYAN);
    
    print_at("📁 Файловый менеджер - MyOS", 2, 1, MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW));
    print_at("Создать [N] | Удалить [D] | Редактировать [E] | Назад [ESC]", 2, 3, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    
    draw_window(2, 4, 76, 16, "Файлы проекта", MAKE_COLOR(COLOR_CYAN, COLOR_WHITE));
    
    // Эмуляция выбора файла
    if (current_input.mouse_click) {
        selected_file = (selected_file + 1) % file_count;
        for (int i = 0; i < file_count; i++) {
            files[i].selected = (i == selected_file);
        }
    }
    
    // Список файлов
    int y = 6;
    for (int i = 0; i < file_count; i++) {
        char line[50];
        int pos = 0;
        
        // Маркер выбора
        if (files[i].selected) {
            line[pos++] = '[';
            line[pos++] = 'x';
            line[pos++] = ']';
        } else {
            line[pos++] = '[';
            line[pos++] = ' ';
            line[pos++] = ']';
        }
        line[pos++] = ' ';
        
        line[pos++] = '0' + (i + 1) / 10;
        line[pos++] = '0' + (i + 1) % 10;
        line[pos++] = '.';
        line[pos++] = ' ';
        
        int j = 0;
        while (files[i].name[j] && pos < 25) {
            line[pos++] = files[i].name[j++];
        }
        
        while (pos < 35) line[pos++] = ' ';
        line[pos++] = '[';
        line[pos++] = ' ';
        
        int size = files[i].size;
        if (size == 0) {
            line[pos++] = '0';
        } else {
            char size_str[10];
            int temp = size;
            int digits = 0;
            while (temp > 0) {
                size_str[digits++] = '0' + (temp % 10);
                temp /= 10;
            }
            for (int k = digits - 1; k >= 0; k--) {
                line[pos++] = size_str[k];
            }
        }
        
        line[pos++] = ' ';
        line[pos++] = 'b';
        line[pos++] = ']';
        line[pos] = '\0';
        
        char text_color = files[i].selected ? 
            MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW) : 
            MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREEN);
            
        print_at(line, 4, y, text_color);
        y++;
    }
    
    if (file_count == 0) {
        print_at("Файлы не найдены. Создайте новый файл [N]", 10, 10, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_RED));
    }
    
    print_at("Всего файлов: ", 4, 21, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_MAGENTA));
    
    char count_str[4];
    count_str[0] = '0' + file_count / 10;
    count_str[1] = '0' + file_count % 10;
    count_str[2] = '\0';
    print_at(count_str, 18, 21, MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW));
    
    // Эмуляция действий
    if (current_input.key_pressed == 'e' || current_input.key_pressed == 'E') {
        print_at("Открытие файла для редактирования...", 30, 21, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_BLUE));
    }
    
    print_at("Эмуляция: Клики мышью выбирают файлы, E - редактировать", 2, 23, MAKE_COLOR(COLOR_BLACK, COLOR_CYAN));
    
    // Отрисовка мыши
    draw_mouse();
}

// Терминал разработчика
void terminal_app() {
    clear_screen(COLOR_BLACK, COLOR_LIGHT_GREEN);
    
    print_at("💻 Терминал разработчика - MyOS", 2, 1, MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW));
    draw_window(2, 2, 76, 18, "Командная строка", MAKE_COLOR(COLOR_GREEN, COLOR_WHITE));
    
    // Эмуляция ввода в терминале
    static int term_cycle = 0;
    term_cycle++;
    
    if (term_cycle < 50) {
        print_at("$ gcc hello.c -o hello", 4, 4, MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    } else if (term_cycle < 100) {
        print_at("$ gcc hello.c -o hello", 4, 4, MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
        print_at("Компиляция успешна!", 4, 5, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREEN));
    } else if (term_cycle < 150) {
        print_at("$ gcc hello.c -o hello", 4, 4, MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
        print_at("Компиляция успешна!", 4, 5, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREEN));
        print_at("$ ./hello", 4, 6, MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    } else {
        print_at("$ gcc hello.c -o hello", 4, 4, MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
        print_at("Компиляция успешна!", 4, 5, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREEN));
        print_at("$ ./hello", 4, 6, MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
        print_at("Hello MyOS!", 4, 7, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_CYAN));
    }
    
    print_at("Доступные команды:", 4, 11, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_MAGENTA));
    print_at("gcc <file> - компиляция C", 6, 12, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    print_at("./<app> - запуск программы", 6, 13, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    print_at("ls - список файлов", 6, 14, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    print_at("edit <file> - редактор", 6, 15, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    
    // Эмуляция мигающего курсора
    if (term_cycle % 30 < 15) {
        print_at(">>> _", 4, 17, MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW));
    } else {
        print_at(">>>  ", 4, 17, MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW));
    }
    
    print_at("Эмуляция: Автоматический ввод команд в терминале", 2, 23, MAKE_COLOR(COLOR_BLACK, COLOR_CYAN));
    
    // Отрисовка мыши
    draw_mouse();
}

// Редактор кода
char code_buffer[10][80] = {
    "#include <stdio.h>",
    "",
    "int main() {",
    "    printf(\"Hello MyOS!\\n\");",
    "    int x = 5;",
    "    for(int i = 0; i < x; i++) {",
    "        printf(\"Count: %d\\n\", i);",
    "    }",
    "    return 0;",
    "}"
};

int cursor_line = 3;
int cursor_col = 4;

void code_editor_app() {
    clear_screen(COLOR_BLUE, COLOR_LIGHT_GREY);
    
    print_at("📝 Редактор кода - hello.c", 2, 1, MAKE_COLOR(COLOR_BLUE, COLOR_YELLOW));
    print_at("[F1]Сохранить [F2]Открыть [F3]Компилировать [ESC]Назад", 2, 2, MAKE_COLOR(COLOR_BLUE, COLOR_WHITE));
    
    draw_window(2, 3, 76, 16, "hello.c", MAKE_COLOR(COLOR_LIGHT_BLUE, COLOR_BLACK));
    
    // Эмуляция редактирования
    static int edit_cycle = 0;
    edit_cycle++;
    
    // Меняем позицию курсора
    if (edit_cycle % 40 == 0) {
        cursor_col = (cursor_col + 1) % 20 + 4;
    }
    if (edit_cycle % 100 == 0) {
        cursor_line = (cursor_line + 1) % 6 + 2;
    }
    
    // Номера строк
    for (int y = 0; y < 10; y++) {
        char line_num[4];
        line_num[0] = '0' + (y + 1) / 10;
        line_num[1] = '0' + (y + 1) % 10;
        line_num[2] = '\0';
        print_at(line_num, 3, y + 5, MAKE_COLOR(COLOR_BLUE, COLOR_LIGHT_MAGENTA));
    }
    
    // Код программы
    for (int y = 0; y < 10; y++) {
        print_at(code_buffer[y], 7, y + 5, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREEN));
    }
    
    // Мигающий курсор
    if (edit_cycle % 30 < 15) {
        char cursor_str[2] = { '_', '\0' };
        print_at(cursor_str, 7 + cursor_col, 5 + cursor_line, MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW));
    }
    
    print_at("Строка 5, Столбец 5 | C | READY", 2, 21, MAKE_COLOR(COLOR_BLUE, COLOR_GREEN));
    
    // Эмуляция сохранения
    if (edit_cycle == 120) {
        print_at("💾 Файл сохранен: hello.c", 40, 21, MAKE_COLOR(COLOR_BLUE, COLOR_LIGHT_CYAN));
    }
    
    print_at("Эмуляция: Автоматическое движение курсора и редактирование", 2, 23, MAKE_COLOR(COLOR_BLUE, COLOR_CYAN));
    
    // Отрисовка мыши
    draw_mouse();
}

// Системная информация
void system_info_app() {
    clear_screen(COLOR_BLACK, COLOR_LIGHT_MAGENTA);
    
    print_at("🔧 Системная информация - MyOS", 2, 1, MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW));
    draw_window(2, 2, 76, 18, "Статус системы", MAKE_COLOR(COLOR_MAGENTA, COLOR_WHITE));
    
    // Эмуляция изменяющихся системных показателей
    static int sys_cycle = 0;
    sys_cycle++;
    
    int memory_usage = 30 + (sys_cycle / 10) % 40;
    int cpu_usage = 20 + (sys_cycle / 5) % 60;
    
    print_at("MyOS Developer Edition v2.0", 10, 4, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_CYAN));
    print_at("ОС для разработчиков", 15, 5, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    
    print_at("Системные ресурсы:", 10, 7, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_MAGENTA));
    
    char memory_str[30];
    char cpu_str[30];
    
    // Создаем строки с показателями
    strcpy(memory_str, "Память: ");
    memory_str[8] = '0' + memory_usage / 10;
    memory_str[9] = '0' + memory_usage % 10;
    strcpy(memory_str + 10, "% использовано");
    
    strcpy(cpu_str, "ЦПУ: ");
    cpu_str[5] = '0' + cpu_usage / 10;
    cpu_str[6] = '0' + cpu_usage % 10;
    strcpy(cpu_str + 7, "% загруженности");
    
    print_at(memory_str, 12, 8, MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    print_at(cpu_str, 12, 9, MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    print_at("Файлы: 15 .c, 8 .h, 3 .py", 12, 10, MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    print_at("Сборка: Debug x86", 12, 11, MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    
    print_at("Активные приложения:", 10, 13, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_MAGENTA));
    print_at("• Редактор кода", 12, 14, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    print_at("• Терминал", 12, 15, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    print_at("• Файловый менеджер", 12, 16, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    
    print_at("Статус: СИСТЕМА РАБОТАЕТ НОРМАЛЬНО", 10, 18, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREEN));
    
    print_at("Эмуляция: Динамическое обновление системных показателей", 2, 23, MAKE_COLOR(COLOR_BLACK, COLOR_CYAN));
    
    // Отрисовка мыши
    draw_mouse();
}

// Демонстрация ввода
void input_demo_app() {
    clear_screen(COLOR_BLACK, COLOR_LIGHT_RED);
    
    print_at("🎮 Демонстрация ввода - MyOS", 2, 1, MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW));
    draw_window(2, 2, 76, 18, "Эмуляция клавиатуры и мыши", MAKE_COLOR(COLOR_RED, COLOR_WHITE));
    
    // Отображение состояния ввода
    print_at("Состояние ввода:", 10, 4, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_MAGENTA));
    
    char key_str[30];
    char mouse_str[30];
    char click_str[30];
    
    if (current_input.key_pressed) {
        strcpy(key_str, "Клавиша нажата: ");
        if (current_input.key_pressed == 1) key_str[15] = '↓'; // Стрелка вниз
        else if (current_input.key_pressed == 2) strcpy(key_str + 15, "ENTER");
        else if (current_input.key_pressed == 27) strcpy(key_str + 15, "ESC");
        else {
            key_str[15] = (char)current_input.key_pressed;
            key_str[16] = '\0';
        }
    } else {
        strcpy(key_str, "Клавиша: не нажата");
    }
    
    strcpy(mouse_str, "Мышь: X=");
    mouse_str[8] = '0' + current_input.mouse_x / 10;
    mouse_str[9] = '0' + current_input.mouse_x % 10;
    strcpy(mouse_str + 10, " Y=");
    mouse_str[13] = '0' + current_input.mouse_y / 10;
    mouse_str[14] = '0' + current_input.mouse_y % 10;
    mouse_str[15] = '\0';
    
    if (current_input.mouse_click) {
        strcpy(click_str, "Клик мыши: ДА");
    } else {
        strcpy(click_str, "Клик мыши: нет");
    }
    
    print_at(key_str, 12, 6, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREEN));
    print_at(mouse_str, 12, 7, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREEN));
    print_at(click_str, 12, 8, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREEN));
    
    // Визуализация области кликов
    print_at("Области для кликов:", 10, 10, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_MAGENTA));
    
    draw_button(12, 12, 15, 3, "Кнопка 1", MAKE_COLOR(COLOR_GREEN, COLOR_BLACK), 
                current_input.mouse_x >= 12 && current_input.mouse_x <= 27 && 
                current_input.mouse_y >= 12 && current_input.mouse_y <= 15);
    
    draw_button(30, 12, 15, 3, "Кнопка 2", MAKE_COLOR(COLOR_BLUE, COLOR_BLACK),
                current_input.mouse_x >= 30 && current_input.mouse_x <= 45 && 
                current_input.mouse_y >= 12 && current_input.mouse_y <= 15);
    
    draw_button(48, 12, 15, 3, "Кнопка 3", MAKE_COLOR(COLOR_MAGENTA, COLOR_BLACK),
                current_input.mouse_x >= 48 && current_input.mouse_x <= 63 && 
                current_input.mouse_y >= 12 && current_input.mouse_y <= 15);
    
    // Эмуляция текстового ввода
    print_at("Текстовый ввод:", 10, 16, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_MAGENTA));
    
    static char demo_text[20] = "Hello MyOS!";
    static int text_pos = 11;
    
    if (current_input.key_pressed && current_input.key_pressed != 27 && 
        current_input.key_pressed != 1 && current_input.key_pressed != 2) {
        if (text_pos < 19) {
            demo_text[text_pos++] = (char)current_input.key_pressed;
            demo_text[text_pos] = '\0';
        }
    }
    
    print_at(demo_text, 12, 17, MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW));
    
    // Мигающий курсор ввода
    static int cursor_blink = 0;
    cursor_blink++;
    if (cursor_blink % 30 < 15) {
        char cursor[2] = { '_', '\0' };
        print_at(cursor, 12 + text_pos, 17, MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW));
    }
    
    print_at("Эмуляция: Движение мыши, клики и ввод с клавиатуры", 2, 23, MAKE_COLOR(COLOR_BLACK, COLOR_CYAN));
    
    // Отрисовка мыши
    draw_mouse();
}

// Главный цикл ОС
void kernel_main() {
    init_file_manager();
    
    // Показываем главное меню при запуске
    main_menu();
    
    int app_timer = 0;
    int current_app = 0;
    
    while (1) {
        // Обновление ввода
        update_input();
        
        // Обработка возврата в меню по ESC
        if (current_input.key_pressed == 27) { // ESC
            in_menu = 1;
            current_input.key_pressed = 0;
            main_menu();
            app_timer = 0;
        }
        
        // Обработка выбора в меню
        if (in_menu && current_input.key_pressed == 2) { // ENTER
            in_menu = 0;
            current_app = current_selection;
            app_timer = 0;
            current_input.key_pressed = 0;
        }
        
        // Автоматическое переключение приложений через время
        if (!in_menu) {
            app_timer++;
            if (app_timer > 300) { // ~15 секунд
                in_menu = 1;
                current_selection = (current_selection + 1) % 6;
                main_menu();
                app_timer = 0;
            }
        }
        
        // Отрисовка текущего приложения
        if (!in_menu) {
            switch (current_app) {
                case 0: code_editor_app(); break;
                case 1: terminal_app(); break;
                case 2: file_manager_app(); break;
                case 3: system_info_app(); break;
                case 4: input_demo_app(); break;
                case 5: 
                    // Выход - перезагрузка
                    print_at("Перезагрузка...", 35, 12, MAKE_COLOR(COLOR_BLUE, COLOR_YELLOW));
                    for (volatile int i = 0; i < 10000000; i++);
                    in_menu = 1;
                    main_menu();
                    app_timer = 0;
                    break;
            }
        }
        
        // Сброс флагов ввода
        current_input.mouse_click = 0;
        current_input.key_pressed = 0;
        
        // Задержка для стабильности
        for (volatile int i = 0; i < 10000; i++);
    }
}