#!/usr/bin/python3
# -*- coding: utf-8 -*-
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import Circle, Polygon
from matplotlib.collections import PatchCollection
from matplotlib import collections  as mc
import pylab
# import numpy as np
import argparse
import os
import glob
import sys

###
# VER http://jakevdp.github.io/blog/2012/08/18/matplotlib-animation-tutorial/
###

def progressbar(it, prefix="", size=60, file=sys.stdout):
    '''Genera una barra de progreso de archivos procesados'''
    count = len(it)
    def show(j):
        x = int(size*j/count)
        file.write("%s[%s%s] %i/%i\r" % (prefix, "#"*x, "."*(size-x), j, count))
        file.flush()
    show(0)
    for i, item in enumerate(it):
        yield item
        show(i+1)
    file.write("\n")
    file.flush()

parser = argparse.ArgumentParser(description='Programa para graficar frames.')
parser.add_argument('-f','--pfile',help='Input data file', required=True, action="store")

args =  parser.parse_args()
preName = args.pfile + '_'

fileFrames = []
frames = []
for f in glob.glob(preName + '*.xy'):
    fileFrames.append(f)
fileFrames.sort()

params = {'backend': 'pdf',
        'interactive': False,
        'lines.linewidth': 2.0,
        'axes.labelsize': 10,
        'font.size': 10,
        'legend.fontsize': 10,
        'xtick.labelsize': 15,
        'ytick.labelsize': 15,
        'figure.subplot.left':0.05,
        'figure.subplot.right':0.99,
        'figure.subplot.top':0.99,
        'figure.subplot.bottom':0.05,
        'text.usetex': True}
matplotlib.rcParams.update(params)

nTotFiles = len(fileFrames)
nActualFile = 1
colorG = []
yMin = yMax = xMin = xMax = 0.0
for f in progressbar(fileFrames):
    fig = pylab.figure(figsize=(10,10))
    fin = open(f,'r')
    data = fin.readlines()
    fin.close()
    ax=fig.add_subplot(111, aspect='equal')
    patches = []
    caja = []
    lines = []
    fout = preName + '{:06d}.png'.format(nActualFile)
    for linea in data:
        l = linea.split()
        gid = int(l[0])
        if gid < 0:
            bverts = []
            nv = int(l[1])
            for i in range(nv):
                bverts.append((float(l[2+2*i]), float(l[2+2*i+1])))
                if bverts[i][0] < xMin:
                    xMin = bverts[i][0]
                if bverts[i][0] > xMax:
                    xMax = bverts[i][0]
                if bverts[i][1] < yMin:
                    yMin = bverts[i][1]
                if bverts[i][1] > yMax:
                    yMax = bverts[i][1]
            for i in range(nv - 1):
                lines.append([bverts[i], bverts[i+1]])
            continue
        nvert = int(l[1])
        conts = []
        verts = []
        if nvert > 1:
            for i in range(nvert):
                verts.append([float(l[2+2*i]), float(l[2+2*i+1])])
            patches.append(Polygon(verts, closed=True, fill=True))
            colorG.append(int(l[-1]))
        else:
            radio = float(l[4])
            patches.append(Circle((float(l[2]),float(l[3])), radio, fill=True))
            colorG.append(int(l[-1]))

    cmap = plt.get_cmap('Set1')
    # cmap = plt.get_cmap('Paired')
    colors = cmap(colorG)
    p = PatchCollection(patches, color=colors, alpha=0.9)
    lc = mc.LineCollection(lines, color='black', linewidths=1)
    pylab.axes().set_aspect('equal')
    ax1 = pylab.gca()
    ax1.add_collection(p)
    ax1.add_collection(lc)
    ax1.set_axis_off()
    alturaSilo = yMax - yMin
    pylab.xlim([1.1 * xMin, 1.1 * xMax])
    pylab.ylim([yMin - 0.05 * alturaSilo, yMax + 0.05 * alturaSilo])
    pylab.savefig(fout)
    pylab.cla()
    nActualFile += 1
    pylab.close(fig)
