#include "display.h"
#include "vector.h"

#define N_POINTS (9 * 9 * 9)
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];
vec3_t camera_position = { .x = 0, .y = 0, .z = -5 };
vec3_t cube_rotation = { .x = 0, .y = 0, .z = 0 };

float fov_factor = 640;

bool is_running = false;
int previous_frame_time = 0;

void setup(void) {
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height);

    int point_count = 0;
    for(float x = -1; x <= 1; x += 0.25f) {
        for(float y = -1; y <= 1; y += 0.25f) {
            for(float z = -1; z <= 1; z += 0.25f) {
                vec3_t new_point = { .x = x, .y = y, .z = z };
                cube_points[point_count++] = new_point;
            }
        }
    }
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

vec2_t project(vec3_t point) {
    vec2_t projected_point = { 
        .x = (fov_factor * point.x) / point.z, 
        .y = (fov_factor * point.y) / point.z 
    };
    return projected_point;
}

void update(void) {
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }
    previous_frame_time = SDL_GetTicks();

    cube_rotation.x += 0.01f;
    cube_rotation.y += 0.01f;
    cube_rotation.z += 0.01f;
    for(int i = 0; i < N_POINTS; i++) {
        vec3_t point = cube_points[i];
        vec3_t transformed_point = point;
        transformed_point = vec3_rotate_x(transformed_point, cube_rotation.x);
        transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
        transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);
        // translate points away from camera
        transformed_point.z -= camera_position.z;
        // projecting to 2D
        vec2_t projected_point = project(transformed_point);
        projected_points[i] = projected_point;
    }
}

void render(void) {
    uint32_t grid_color = 0xFF808080;
    uint32_t clear_color = 0xFF000000;

    clear_color_buffer(clear_color);
    draw_grid(grid_color, 128);

    for(int i = 0; i < N_POINTS; i++) {
        vec2_t projected_point = projected_points[i];
        draw_rect(
            projected_point.x + (window_width / 2),
            projected_point.y + (window_height / 2),
            4,
            4,
            0xFFffff00);
    }

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
