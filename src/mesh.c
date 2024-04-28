#include <stdio.h>
#include "mesh.h"
#include "array.h"

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = { 0, 0, 0 }
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    { .x = -1, .y = -1, .z = -1 }, // 1 - left bottom front
    { .x = -1, .y =  1, .z = -1 }, // 2 - left top front
    { .x =  1, .y =  1, .z = -1 }, // 3 - right top front
    { .x =  1, .y = -1, .z = -1 }, // 4 - right bottom front
    { .x =  1, .y =  1, .z =  1 }, // 5 - right top back 
    { .x =  1, .y = -1, .z =  1 }, // 6 - right bottom back
    { .x = -1, .y =  1, .z =  1 }, // 7 - left top back
    { .x = -1, .y = -1, .z =  1 }  // 8 - left bottom back
};

// vertices indices of `cube_vertices` array (clockwise?)
face_t cube_faces[N_CUBE_FACES] = {
    // front
    { .a = 1, .b = 2, .c = 3 },
    { .a = 1, .b = 3, .c = 4 },
    // right 
    { .a = 4, .b = 3, .c = 5 },
    { .a = 4, .b = 5, .c = 6 },
    // back 
    { .a = 6, .b = 5, .c = 7 },
    { .a = 6, .b = 7, .c = 8 },
    // left 
    { .a = 8, .b = 7, .c = 2 },
    { .a = 8, .b = 2, .c = 1 },
    // top 
    { .a = 2, .b = 7, .c = 5 },
    { .a = 2, .b = 5, .c = 3 },
    // bottom 
    { .a = 6, .b = 8, .c = 1 },
    { .a = 6, .b = 1, .c = 4 }
};

void load_cube_mesh_data(void) {
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        vec3_t v = cube_vertices[i];
        array_push(mesh.vertices, v);
    }

    for (int i = 0; i < N_CUBE_FACES; i++) {
        face_t f = cube_faces[i];
        array_push(mesh.faces, f);
    }
}

void load_obj_file_data(char* filename) {
    
}