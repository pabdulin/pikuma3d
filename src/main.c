#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"

triangle_t* triangles_to_render = NULL;

vec3_t camera_position = { .x = 0, .y = 0, .z = 0 };
float fov_factor = 640;

bool is_running = false;
int previous_frame_time = 0;

extern cull_method_t cull_method;
extern render_method_t render_method;

void setup(void) {
    render_method = RENDER_WIRE;
    cull_method = CULL_BACKFACE;

    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height);
    
    load_cube_mesh_data();
    // load_obj_file_data_reference("./assets/cube.obj");
    // load_obj_file_data_reference("./assets/f22.obj");
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
                if (event.key.keysym.sym == SDLK_1)
                    render_method = RENDER_WIRE_VERTEX;
                if (event.key.keysym.sym == SDLK_2)
                    render_method = RENDER_WIRE;
                if (event.key.keysym.sym == SDLK_3)
                    render_method = RENDER_FILL_TRIANGLE;
                if (event.key.keysym.sym == SDLK_4)
                    render_method = RENDER_FILL_TRIANGLE_WIRE;
                if (event.key.keysym.sym == SDLK_c)
                    cull_method = CULL_BACKFACE;
                if (event.key.keysym.sym == SDLK_d)
                    cull_method = CULL_NONE;
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

    triangles_to_render = NULL;

    mesh.rotation.x += 0.01f;
    mesh.rotation.y += 0.01f;
    mesh.rotation.z += 0.01f;
    
    int num_faces = array_length(mesh.faces);
    for(int i = 0; i < num_faces; i++) {
        face_t mesh_face = mesh.faces[i];
        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];
        
        
        vec3_t transformed_vertices[3];
        
        for(int j = 0; j < 3; j++) {
            vec3_t transformed_vertex = face_vertices[j];
            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);
            // translate points away from camera
            transformed_vertex.z += 5;
            transformed_vertices[j] = transformed_vertex;
        }

        if(cull_method == CULL_BACKFACE) {
            // check backface culling
            vec3_t vector_a = transformed_vertices[0];
            vec3_t vector_b = transformed_vertices[1];
            vec3_t vector_c = transformed_vertices[2];

            // B-A
            vec3_t vector_ab = vec3_sub(vector_b, vector_a);
            // C-A
            vec3_t vector_ac = vec3_sub(vector_c, vector_a);
            // N - normal
            vec3_t normal = vec3_cross(vector_ab, vector_ac);

            // Normalize the face normal vector
            vec3_normalize(&normal);

            // calculate ray to the camera
            vec3_t camera_ray = vec3_sub(camera_position, vector_a);
            // check our dot product (vector aligment)
            float dot = vec3_dot(normal, camera_ray);

            if (dot < 0) {
                // cull faces away from us
                continue;
            }
        }
        
        triangle_t projected_triangle;
        projected_triangle.color = mesh_face.color;
        int vertex_count = 3;
        float total_z = 0;
        for(int j = 0; j < vertex_count; j++) {
            // projecting to 2D
            vec2_t projected_point = project(transformed_vertices[j]);
            projected_point.x += (window_width / 2);
            projected_point.y += (window_height / 2);
            
            projected_triangle.points[j] = projected_point;
            total_z += transformed_vertices[j].z;
        }
        projected_triangle.avg_depth = total_z / (float)vertex_count;

        array_push(triangles_to_render, projected_triangle);
    }

    // TODO: sort the faces by avg_depth from max to min
    int num_tri = array_length(triangles_to_render);
    bool exchange;
    // bubble sort (reference)
    for (int i = 0; i < num_tri; i++) {
        for (int j = i; j < num_tri; j++) {
            if (triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth) {
                triangle_t temp = triangles_to_render[i];
                triangles_to_render[i] = triangles_to_render[j];
                triangles_to_render[j] = temp;
            }
        }
    }
}

void render(void) {
    uint32_t grid_color = 0xFF808080;
    uint32_t clear_color = 0xFF000000;
    uint32_t color_white = 0xFFffffff;
    uint32_t color_gray = 0xFF555555;
    uint32_t color_dark_gray = 0xFF303030;
    uint32_t color_green = 0xFF00ff00;
    uint32_t color_black = 0xFF000000;

    clear_color_buffer(clear_color);
    draw_grid(grid_color, 100);

    int num_triangles = array_length(triangles_to_render);
    for(int i = 0; i < num_triangles; i++) {
        triangle_t t = triangles_to_render[i];

        if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE) {
            draw_filled_triangle(
                t.points[0].x,
                t.points[0].y,
                t.points[1].x,
                t.points[1].y,
                t.points[2].x,
                t.points[2].y,
                t.color);
        }

        if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE) {
            draw_triangle(
                t.points[0].x,
                t.points[0].y,
                t.points[1].x,
                t.points[1].y,
                t.points[2].x,
                t.points[2].y,
                color_dark_gray);
        }

        if (render_method == RENDER_WIRE_VERTEX) {
            draw_rect(t.points[0].x - 3, t.points[0].y - 3, 6, 6, color_green);
            draw_rect(t.points[1].x - 3, t.points[1].y - 3, 6, 6, color_green);
            draw_rect(t.points[2].x - 3, t.points[2].y - 3, 6, 6, color_green);
        }
    }

    // TODO: remove gameloop alloc/free
    array_free(triangles_to_render);

    render_color_buffer();
    SDL_RenderPresent(renderer);
}

void free_resources(void) {
    free(color_buffer);
    array_free(mesh.faces);
    array_free(mesh.vertices);
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
    free_resources();

    return 0;
}
