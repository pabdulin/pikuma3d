#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include "SDL.h"

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

typedef enum {
    CULL_NONE,
    CULL_BACKFACE
} cull_method_t;
static cull_method_t cull_method;

typedef enum {
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE
} render_method_t;
static render_method_t render_method;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;
extern int window_width;
extern int window_height;

bool initialize_window(void);
void draw_grid(uint32_t color, int step);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int left, int top, int width, int height, uint32_t color);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void destroy_window(void);

#endif