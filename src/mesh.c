#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

#define BUF_SIZE 1024
void load_obj_file_data(char* filename) {
    char buf1[BUF_SIZE] = {0};
    char buf_copy[BUF_SIZE] = {0};
    
    int vIndex = 0;
    int fIndex = 0;
    FILE* file = fopen(filename, "rt");
    while(fgets(buf1, BUF_SIZE - 1, file)) {
        char* tok;
        strcpy(buf_copy, buf1); // DEBUG
        tok = strtok(buf1, " ");
        if(tok) {
            // vertex
            if(tok[0] == 'v' && strlen(tok) == 1) {
                printf("Found vertex: %s", buf_copy); // OK
                float v1 = (float)atof(strtok(NULL, " "));
                float v2 = (float)atof(strtok(NULL, " "));
                float v3 = (float)atof(strtok(NULL, " "));

                printf("Parsed vertex: %f, %f, %f\n", v1, v2, v3); // OK
                vec3_t vtx = { v1, v2, v3 };
                //mesh.vertices[vIndex++] = vtx;
                array_push(mesh.vertices, vtx);
            }

            // face
            if(tok[0] == 'f' && strlen(tok) == 1) {
                printf("Found face: %s", buf_copy); // OK
                
                int  vIndex1 = atoi(strtok(NULL, "/"));
                int vtIndex1 = atoi(strtok(NULL, "/"));
                int vnIndex1 = atoi(strtok(NULL, " "));
                int  vIndex2 = atoi(strtok(NULL, "/"));
                int vtIndex2 = atoi(strtok(NULL, "/"));
                int vnIndex2 = atoi(strtok(NULL, " "));
                int  vIndex3 = atoi(strtok(NULL, "/"));
                int vtIndex3 = atoi(strtok(NULL, "/"));
                int vnIndex3 = atoi(strtok(NULL, " "));

                printf("Parsed face: %d, %d, %d\n", vIndex1, vIndex2, vIndex3); // OK

                face_t face = { .a = vIndex1, .b = vIndex2, .c = vIndex3 };
                // mesh.faces[fIndex++] = face;
                array_push(mesh.faces, face);
            }
        }
    }

    fclose(file);
}