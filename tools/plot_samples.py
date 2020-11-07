#!/usr/bin/python3
import argparse as ap
import matplotlib.pyplot as plt
import numpy as np
import scipy.io.wavfile as wav

parser = ap.ArgumentParser()
parser.add_argument('-f', required=True)
parser.add_argument('-n', default='figure.pdf')
parser.add_argument('-t', default='Samples')

args = parser.parse_args()
filePath = args.f
imgFileName = args.n
title = args.t

samplingFrequency, samples = wav.read(filePath)
length = samples.shape[0]/samplingFrequency
time = np.linspace(0., length, samples.shape[0])

indices = (samples > 0).nonzero()[0]
samples = samples[samples > 0]
time = time[indices]

samples = np.append(samples, [0])
time = np.append(time, [0.025])

plt.stem(time, samples/32768)
plt.xlabel('t[s]')
plt.ylabel('r(t)')
plt.title(title)
axes = plt.gca()
axes.set_xlim([-0.001, 0.025])
# plt.show()
plt.savefig(imgFileName)
