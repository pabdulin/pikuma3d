#include <stdint.h>
#include <math.h>
#include "triangle.h"
#include "display.h"

void int_swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int Mx, int My, uint32_t color) {
    draw_triangle(x0, y0, x1, y1, Mx, My, 0xFF00ff00);
}

fill_flat_top_triangle(int x1, int y1, int Mx, int My, int x2, int y2, uint32_t color) {
    draw_triangle(x1, y1, Mx, My, x2, y2, 0xFF0000ff);
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    draw_triangle(x0, y0, x1, y1, x2, y2, 0xFFff0000);

    // using flat-botton, flat-top algo
    // sort vertices by Y
    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }
    if (y1 > y2) {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
    }
    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }

    // calc Mx, My division vertex
    int My = y1;
    int Mx = (int)roundf(((float)((x2-x0)*(y1-y0))/(float)(y2-y0)) + x0);

    // TODO: draw flat bottom
    fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);

    // TODO: draw flat top
    fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
}