#pragma once
void oled_init();
void oled_clear();
void oled_text(int x, int y, const char *s, int scale);
void oled_flush();
void oled_power(bool on);