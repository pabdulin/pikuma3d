#include "display.h"

bool is_running = false;

void setup(void) {
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height);
}

void process_input(void) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    is_running = false;
                break;
        }
    }
}

void update(void) {

}

void render(void) {
    uint32_t grid_color = 0xFF808080;
    uint32_t clear_color = 0xFF000000;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    clear_color_buffer(clear_color);
    draw_grid(grid_color, 128);
    
    draw_rect(0, 0, 128, 128, 0xFFcc0000);
    draw_rect(128, 128, 128, 128, 0xFF00cc00);
    draw_rect(256, 256, 128, 128, 0xFF0000cc);
    
    draw_pixel(20, 20, 0xFFffffff);

    render_color_buffer();
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    is_running = initialize_window();

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }
    
    destroy_window();

    return 0;
}
