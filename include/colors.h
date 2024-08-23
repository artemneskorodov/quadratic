#ifndef COLORS_H
#define COLORS_H

enum color_t {
    RED,
    GREEN,
    YELLOW,
    BLUE,
    PURPLE,
    CYAN,
    WHITE,
    DEFAULT
};

int color_printf(color_t color, const char *string, ...);

#endif
