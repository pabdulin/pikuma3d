#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2)
extern vec3_t cube_vertices[N_CUBE_VERTICES];
// vertices indices of `cube_vertices` array (clockwise)
extern face_t cube_faces[N_CUBE_FACES];

typedef struct {
    vec3_t* vertices;   // dyn array
    face_t* faces;      // dyn array
    vec3_t rotation;
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh_data(void);
void load_obj_file_data(char* filename);
void load_obj_file_data_reference(char* filename);

#endif