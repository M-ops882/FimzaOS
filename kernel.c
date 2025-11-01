#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile char *video = (volatile char*)0xB8000;
int cursor_x = 0;
int cursor_y = 0;

// Простые замены стандартных функций
int my_strlen(const char *str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

int my_strncmp(const char *s1, const char *s2, int n) {
    for (int i = 0; i < n; i++) {
        if (s1[i] != s2[i]) return 1;
        if (s1[i] == '\0') return 0;
    }
    return 0;
}

char* my_strstr(const char *haystack, const char *needle) {
    if (!*needle) return (char*)haystack;
    
    for (int i = 0; haystack[i]; i++) {
        int j = 0;
        while (needle[j] && haystack[i + j] == needle[j]) {
            j++;
        }
        if (!needle[j]) return (char*)(haystack + i);
    }
    return 0;
}

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = 0x07;
    }
    cursor_x = 0;
    cursor_y = 0;
}

void putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        int idx = (cursor_y * VGA_WIDTH + cursor_x) * 2;
        video[idx] = c;
        video[idx + 1] = 0x07;
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT) {
        // Простая прокрутка
        for (int y = 0; y < VGA_HEIGHT - 1; y++) {
            for (int x = 0; x < VGA_WIDTH; x++) {
                int src_idx = ((y + 1) * VGA_WIDTH + x) * 2;
                int dst_idx = (y * VGA_WIDTH + x) * 2;
                video[dst_idx] = video[src_idx];
                video[dst_idx + 1] = video[src_idx + 1];
            }
        }
        // Очистить последнюю строку
        for (int x = 0; x < VGA_WIDTH; x++) {
            int idx = ((VGA_HEIGHT - 1) * VGA_WIDTH + x) * 2;
            video[idx] = ' ';
            video[idx + 1] = 0x07;
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}

void print(const char *str) {
    while (*str) {
        putchar(*str);
        str++;
    }
}
// Простой Python-интерпретатор
char input_buffer[100];
int buf_pos = 0;
int vars[26] = {0}; // переменные a-z

void print_number(int num) {
    if (num == 0) {
        putchar('0');
        return;
    }
    
    char buffer[10];
    int i = 0;
    int is_negative = 0;
    
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    if (is_negative) {
        putchar('-');
    }
    
    while (i > 0) {
        putchar(buffer[--i]);
    }
}

void execute_command() {
    input_buffer[buf_pos] = '\0';
    
    // Команда help
    if (my_strcmp(input_buffer, "help") == 0) {
        print("\nДоступные команды:\n");
        print("help - справка\n");
        print("clear - очистка экрана\n");
        print("a=5 - присвоить значение\n");
        print("print a - вывести переменную\n");
        print("demo - демонстрация\n");
    }
    // Команда clear
    else if (my_strcmp(input_buffer, "clear") == 0) {
        clear_screen();
    }
    // Присвоение переменной (формат: a=5)
    else if (my_strstr(input_buffer, "=") != 0) {
        char var_name = input_buffer[0];
        if (var_name >= 'a' && var_name <= 'z') {
            char *num_str = input_buffer + 2;
            int value = 0;
            int i = 0;
            
            while (num_str[i] >= '0' && num_str[i] <= '9') {
                value = value * 10 + (num_str[i] - '0');
                i++;
            }
            
            vars[var_name - 'a'] = value;
            print("\nПрисвоено ");
            putchar(var_name);
            print(" = ");
            print_number(value);
        }
    }
    // Вывод переменной (формат: print a)
    else if (my_strncmp(input_buffer, "print ", 6) == 0) {
        char var_name = input_buffer[6];
        if (var_name >= 'a' && var_name <= 'z') {
            int value = vars[var_name - 'a'];
            print("\n");
            putchar(var_name);
            print(" = ");
            print_number(value);
        }
    }
    // Демонстрация
    else if (my_strcmp(input_buffer, "demo") == 0) {
        print("\nДемонстрация Python-OS:\n");
        print("a = 10\n");
        print("b = 20\n");
        print("a + b = 30\n");
        vars[0] = 10; // a
        vars[1] = 20; // b
    }
    else {
        print("\nНеизвестная команда. Введите 'help'");
    }
    
    buf_pos = 0;
    print("\n>>> ");
}
void python_shell() {
    clear_screen();
    print("🐍 Python Shell OS v1.0\n");
    print("Введите 'help' для списка команд\n");
    print(">>> ");
    
    // Демонстрационные команды
    const char *demo_cmds[] = {
        "help",
        "a=10", 
        "print a",
        "b=25",
        "print b",
        "demo",
        "clear"
    };
    int demo_index = 0;
    int delay = 0;
    
    while (1) {
        // Имитация ввода для демонстрации
        delay++;
        if (delay > 5000000) {
            if (demo_index < 7) {
                const char *cmd = demo_cmds[demo_index];
                print("\n");
                print(cmd);
                
                // Копируем команду в буфер и выполняем
                int i = 0;
                while (cmd[i]) {
                    if (buf_pos < 99) {
                        input_buffer[buf_pos++] = cmd[i];
                    }
                    i++;
                }
                execute_command();
                demo_index++;
            }
            delay = 0;
        }
    }
}

void kernel_main(void) {
    python_shell();
}