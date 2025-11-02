#ifndef KERNEL_H
#define KERNEL_H

// Базовые функции
void clear_screen(char bg, char fg);
void print_at(const char *str, int x, int y, char color);
int strlen(const char *str);
void strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);

// Приложения
void code_editor();
void terminal_app();
void file_manager();
void developer_dashboard();

#endif