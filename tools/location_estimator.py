#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

anchors = np.array([
    [0, 0, 0],
    [1, 0, 0],
    [0, 1, 0],
    [0, 0, 1],
    [1, 1, 1]
])

N = 5
x = np.linspace(-5, 5, N)
y = np.linspace(-5, 5, N)
z = np.linspace(-5, 5, N)

X = np.zeros((3,np.power(N, 3)))

index = 0
while index < X.shape[1]:
    x_index = index % N
    y_index = int(index / N) % N
    z_index = int(index / (N*N))
    X[:, index] = [x[x_index], y[y_index], z[z_index]]
    index += 1

noise = np.random.normal(size=X.shape)
x_noisy = X + 0.1*noise

A = 2*np.array([
    [anchors[0, 0] - anchors[1, 0], anchors[0, 1] -
        anchors[1, 1], anchors[0, 2] - anchors[1, 2]],
    [anchors[0, 0] - anchors[2, 0], anchors[0, 1] -
        anchors[2, 1], anchors[0, 2] - anchors[2, 2]],
    [anchors[0, 0] - anchors[3, 0], anchors[0, 1] -
        anchors[3, 1], anchors[0, 2] - anchors[3, 2]],
    [anchors[0, 0] - anchors[4, 0], anchors[0, 1] -
        anchors[4, 1], anchors[0, 2] - anchors[4, 2]]
])

err = np.array([])
print(x_noisy.shape)

index = 0
while index < x_noisy.shape[1]:

    b = np.array([
    [
        np.square(np.linalg.norm(anchors[1] - x_noisy[:,index])) - np.square(np.linalg.norm(anchors[0] - x_noisy[:,index])) -
        np.square(anchors[1, 0]) + np.square(anchors[0, 0]) -
        np.square(anchors[1, 1]) + np.square(anchors[0, 1]) -
        np.square(anchors[1, 2]) + np.square(anchors[0, 2])
    ],
    [
        np.square(np.linalg.norm(anchors[2] - x_noisy[:,index])) - np.square(np.linalg.norm(anchors[0] - x_noisy[:,index])) -
        np.square(anchors[2, 0]) + np.square(anchors[0, 0]) -
        np.square(anchors[2, 1]) + np.square(anchors[0, 1]) -
        np.square(anchors[2, 2]) + np.square(anchors[0, 2])
    ],
    [
        np.square(np.linalg.norm(anchors[3] - x_noisy[:,index])) - np.square(np.linalg.norm(anchors[0] - x_noisy[:,index])) -
        np.square(anchors[3, 0]) + np.square(anchors[0, 0]) -
        np.square(anchors[3, 1]) + np.square(anchors[0, 1]) -
        np.square(anchors[3, 2]) + np.square(anchors[0, 2])
    ],
    [
    np.square(np.linalg.norm(anchors[4] - x_noisy[:,index])) - np.square(np.linalg.norm(anchors[0] - x_noisy[:,index])) -
        np.square(anchors[4, 0]) + np.square(anchors[0, 0]) -
        np.square(anchors[4, 1]) + np.square(anchors[0, 1]) -
        np.square(anchors[4, 2]) + np.square(anchors[0, 2])
    ]
    ])

    print(x_noisy[:,index])
    x_estimated = np.linalg.lstsq(A, b, rcond=None)[0]
    print(x_estimated.T)
    err = np.append(err, np.linalg.norm(X[:,index]-x_estimated.T))
    print(err[index])
    index += 1


print("avg: ", np.average(err))
print("min: ", np.min(err))
print("max: ", np.max(err))
