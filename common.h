#ifndef COMMON_H
#define COMMON_H

#include "config.h"

// Объявляем video как extern
extern volatile char *video;

// Базовые функции
void clear_screen(char bg, char fg);
void print_at(const char *str, int x, int y, char color);
int strlen(const char *str);
void strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);

#endif