#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define N_MESH_VERTICES 8
extern vec3_t mesh_vertices[N_MESH_VERTICES];

// vertices indices of `cube_vertices` array (clockwise)
#define N_MESH_FACES (6 * 2)
extern face_t mesh_faces[N_MESH_FACES];

#endif