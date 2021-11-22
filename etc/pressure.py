#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Nov 17 11:43:41 2021

@author: noboru
"""
import numpy as np
import seaborn as sns
from matplotlib import pyplot as plt
from matplotlib import gridspec
from sys import argv
from os.path import isfile
import pandas as pd

sns.set_theme()

if len(argv)<2:
    print("python script.py logfile.csv")
    exit(0)
if not isfile(argv[1]):
    print("file %s does not exist"%argv[1])
    exit(0)

file = argv[1]
#check the file
data = pd.read_csv(file)
x = data['timestamp'].to_numpy()
y = data['pressure'].to_numpy()
v = data['voltage'].to_numpy()

gs = gridspec.GridSpec(2,1, height_ratios=[3,1]) 
ax0 = plt.subplot(gs[0])
ax1 = plt.subplot(gs[1], sharex=ax0)

d = [(x[i],float(v[i])) for i in range(len(v)) if str(v[i]).isdigit()]

ax0.plot(x,y,'o',markersize=0.5)
ax0.set(xlabel='time (second)',ylabel='Pressure',title='high-res pressure reading')
x,v = zip(*d)
ax1.plot(x,np.asarray(v)/1000.,'o',markersize=0.5)
ax1.set(xlabel='time (second)',ylabel='Voltage (V)')
plt.tight_layout()
plt.show()
