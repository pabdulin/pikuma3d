#include <stdio.h>
#include <math.h>
#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 128;
int window_height = 128;

bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    SDL_DisplayMode display_mode;
    int display_index = 0;
    SDL_GetCurrentDisplayMode(display_index, &display_mode);
    fprintf(stdout, "Display `%d`=`%dx%d@%d`\n", display_index, display_mode.w, display_mode.h, display_mode.refresh_rate);
    window_width = display_mode.w / 2;
    window_height = display_mode.h / 2;
    fprintf(stdout, "Using window_width=`%d`, window_height=`%d`\n", window_width, window_height);

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    return true;
}

void draw_grid(uint32_t color, int step) {
    for(int y = 0; y < window_height; y++) {
        for(int x = 0; x < window_width; x++) {
            if(((x + 1) % step == 0) || ((y + 1) % step == 0)) {
                draw_pixel(x, y, color);
            }
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;

    int abs_dx = abs(delta_x);
    int abs_dy = abs(delta_y);
    int side_lenght = abs_dx >= abs_dy ? abs_dx : abs_dy;

    float x_inc = (float)delta_x / (float)side_lenght;
    float y_inc = (float)delta_y / (float)side_lenght;

    float current_x = x0;
    float current_y = y0;

    for(int i = 0; i <= side_lenght; i++) {
        draw_pixel((int)roundf(current_x), (int)roundf(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

void draw_pixel(int x, int y, uint32_t color) {
    if(x >= 0 && x < window_width && y >= 0 && y < window_height) {
        color_buffer[(window_width * y) + x] = color;
    } else {
        // fprintf(stderr, "Attempt to draw at x=`%d`, y=`%d`\n", x, y);
        // SDL_assert(false);
    }
}

void draw_rect(int left, int top, int width, int height, uint32_t color) {
    for(int y = top; y < top + height; y++) {
        for(int x = left; x < left + width; x++) {
            draw_pixel(x, y, color);
        }
    }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void render_color_buffer(void) {
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)(window_width * sizeof(uint32_t))
    );
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
    for(int y = 0; y < window_height; y++) {
        for(int x = 0; x < window_width; x++) {
            color_buffer[(window_width * y) + x] = color;
        }
    }
}

void destroy_window(void) {
    SDL_DestroyTexture(color_buffer_texture);
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
