#!/usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt

x_alpha = np.deg2rad(0)
y_alpha = np.deg2rad(90)
z_alpha = np.deg2rad(0)
translation = np.array([
    0.5, 0.5, 0.5
])
d = 0.1

rot_x = np.array([
    [1, 0, 0],
    [0, np.cos(x_alpha), -np.sin(x_alpha)],
    [0, np.sin(x_alpha), np.cos(x_alpha)]
])

rot_y = np.array([
    [np.cos(y_alpha), 0, np.sin(y_alpha)],
    [0, 1, 0],
    [-np.sin(y_alpha), 0, np.cos(y_alpha)]
])

rot_z = np.array([
    [np.cos(z_alpha), -np.sin(z_alpha), 0],
    [np.sin(z_alpha), np.cos(z_alpha), 0],
    [0, 0, 1]
])

ildars = np.array([
    [0,0,0],
    [d,0,0],
    [d+d*np.sin(np.pi/10),d*np.cos(np.pi/10),0],
    [d/2,d/2*np.sqrt(5+2*np.sqrt(5)),0],
    [-d*np.sin(np.pi/10), d*np.cos(np.pi/10),0]
])

ildars_rot = np.zeros(ildars.shape)

index = 0
for row in ildars:
    print(row)
    ildars_rot[index, :] = rot_x @ rot_y @ rot_z @ row + translation
    print(ildars_rot[index, :])
    index += 1

np.savetxt('rsc/receiver/ildars.csv', ildars, delimiter=',')
plt.xlim(-1, 1)
plt.ylim(-1, 1)
plt.plot(ildars[:,0], ildars[:,1], 'o')
plt.plot(ildars_rot[:,0], ildars_rot[:,1], 'o')
plt.show()
