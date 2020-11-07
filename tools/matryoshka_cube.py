#!/usr/bin/python3
import argparse as ap
import numpy as np

parser = ap.ArgumentParser()
parser.add_argument('-N', type=int, default=1)
N = parser.parse_args().N

objectname = 'matryoshka_cube_' + str(N)
filename = objectname + '.obj'

base_vertices = np.array([
    ( 1.0, 1.0,-1.0 ), ( 1.0,-1.0,-1.0 ), (-1.0,-1.0,-1.0 ), (-1.0, 1.0,-1.0),
    ( 1.0, 1.0, 1.0 ), ( 1.0,-1.0, 1.0 ), (-1.0,-1.0, 1.0 ) ,(-1.0, 1.0, 1.0)
])

vertices_indexes = np.array([
    (1, 1, 2, 1, 3, 1, 4, 1),
    (5, 2, 8, 2, 7, 2, 6, 2),
    (1, 3, 5, 3, 6, 3, 2, 3),
    (2, 4, 6, 4, 7, 4, 3, 4),
    (3, 5, 7, 5, 8, 5, 4, 5),
    (5, 6, 1, 6, 4, 6, 8, 6)
])

number_of_faces = base_vertices.shape[0]

vertex_line = 'v {0} {1} {2}\n'
face_line = 'f {0}//{1} {2}//{3} {4}//{5} {6}//{7}\n'

with open(filename, 'w+') as env:
    env.write('o ' + objectname + '\n')

    for i in range(N):
        for vertex in base_vertices:
            env.write((vertex_line.format(*((i+1)*vertex))))

    env.writelines('s off\n')
    for i in range(N):
        for vertex_index in vertices_indexes:
            env.write(face_line.format(*vertex_index))

        vertices_indexes[:, 0] += number_of_faces
        vertices_indexes[:, 2] += number_of_faces
        vertices_indexes[:, 4] += number_of_faces
        vertices_indexes[:, 6] += number_of_faces
