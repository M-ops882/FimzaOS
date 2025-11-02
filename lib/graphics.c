#include "common.h"

void draw_window(int x, int y, int w, int h, char *title, char border_color) {
    // Простая рамка - только углы
    if (x >= 0 && y >= 0 && x + w < VGA_WIDTH && y + h < VGA_HEIGHT) {
        // Верхняя и нижняя границы
        for (int i = x; i < x + w; i++) {
            if (i < VGA_WIDTH) {
                int top_idx = (y * VGA_WIDTH + i) * 2;
                int bottom_idx = ((y + h - 1) * VGA_WIDTH + i) * 2;
                video[top_idx + 1] = border_color;
                video[bottom_idx + 1] = border_color;
            }
        }
        
        // Левая и правая границы
        for (int j = y; j < y + h; j++) {
            if (j < VGA_HEIGHT) {
                int left_idx = (j * VGA_WIDTH + x) * 2;
                int right_idx = (j * VGA_WIDTH + (x + w - 1)) * 2;
                video[left_idx + 1] = border_color;
                video[right_idx + 1] = border_color;
            }
        }
    }
    
    // Заголовок
    if (title) {
        print_at(title, x + 1, y, border_color);
    }
}

void draw_button(int x, int y, int w, char *text, char color) {
    // Простая кнопка - инвертированный цвет текста
    print_at(text, x, y, color);
}