#!/usr/bin/python3
import matplotlib.pyplot as plt
import numpy as np

r = np.arange(1, 10, 0.1)

L1 = np.exp(-0.3*r)
L2 = 1/(1+r)
L3 = 1/r

plt.figure()
plt.plot(r, L3, 'r', r, L2, 'b')
plt.show()