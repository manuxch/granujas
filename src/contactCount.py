#!/bin/env python3
'''Programa para analizar el número de contactos usando grafos.'''

import glob
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import sys
import argparse

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

parser = argparse.ArgumentParser(
    description='Calcula el número de coordinación y la intensidad de los nodos.')
parser.add_argument('-o', '--output', default='ncoord.dat',
                    help='Archivo de salida por frame.')
parser.add_argument('-s', '--skip', type=int, default=0,
            help='Cantidad de frames iniciales que se ignoran en el promedio.')
parser.add_argument('-c', '--c-histo', default='nc_histo.dat',
            help='Archivo de histograma de número de coordinación.')
parser.add_argument('-i', '--i-histo', default='s_histo.dat',
            help='Archivo de histograma del "strength" del nodo.')
args = parser.parse_args()

# Levanto todos los archivos con datos del grafo (*xvc)
file_list = []
for f in glob.glob('*.xvc'):
    file_list.append(f)
n_files = len(file_list)
file_list.sort()
# Detecto la cantidad de granos (nodos) en el sistema
fin = open(file_list[0], 'r')
data = fin.readlines()
fin.close()
n_granos = len(data) - 1 # resto 1 porque la primera línea es encabezado
print(f'Número total de granos: {n_granos}')

nc_file = open(args.output, 'w')
nc_file.write('#n_frm <k> <s> \n')
nc = []
ns = []
kMean_list = []
sMean_list = []

for f in progressbar(file_list[args.skip:]):
    fin = open(f, 'r')
    data = fin.readlines()
    fin.close()
    n_frm = int(f.split('_')[1][:-4])
    G = nx.Graph()
    edges = []
    for linea in data[1:]:
        l = linea.split()
        gid = int(l[0])
        atipo = int(l[7])
        if gid == -100:
            continue
        G.add_node(gid, tipo=atipo)
        for sid in l[9:]:
            if int(sid) == -100:
                continue
            edges.append((gid, int(sid)))
    G.add_edges_from(edges)
    for u, v, d in G.edges.data():
        G[u][v]['weight'] = \
            1.0 if G.nodes[u]['tipo'] == G.nodes[v]['tipo'] else -1.0
    nc += [len(G.edges(n)) for n in G.nodes]
    ns += [G.degree(n, 'weight') for n in G.nodes]
    k_mean = 2.0 * G.number_of_edges() / G.number_of_nodes()
    s_mean = np.sum(np.array([G.degree(n, 'weight') for n in G.nodes])) \
            / G.number_of_edges()
    kMean_list.append(k_mean)
    sMean_list.append(s_mean)
    nc_file.write(f'{n_frm} {k_mean:.3f} {s_mean:.3f}\n')

nc_file.close()
aKMean = np.array(kMean_list)
akMean_m = aKMean.mean()
akMean_sd = aKMean.std()
print()
print(f'Valor medio de <k> en la trayectoria: {akMean_m:.3f} +/- {akMean_sd:.3f}')
aSMean = np.array(sMean_list)
aSMean_m = aSMean.mean()
aSMean_sd = aSMean.std()
print(f'Valor medio de <s> en la trayectoria: {aSMean_m:.3f} +/- {aSMean_sd:.3f}')
print(80 * '-')
fig, ((ax1, ax2)) = plt.subplots(1, 2)
anc = np.array(nc)
d = np.diff(np.unique(anc)).min()
leftf = anc.min() - float(d) / 2
rightl = anc.max() + float(d) / 2
abins = np.arange(leftf, rightl + d, d)
h = ax1.hist(anc, abins, rwidth=0.7, density=True)
ax1.set_xlabel('nc')
ax1.set_ylabel('PDF')

ans = np.array(ns)
ds = np.diff(np.unique(ans)).min()
leftfs = ans.min() - float(ds) / 2
rightls = ans.max() + float(ds) / 2
abinss = np.arange(leftfs, rightls + ds, ds)
hs = ax2.hist(ans, abinss, rwidth=0.7, density=True)
ax2.set_xlabel('s')
ax2.set_ylabel('PDF')
hbins = [(i + d/2) for i in abins]
hsbins = [(i + ds/2) for i in abinss]
np.savetxt(args.c_histo, list(zip(hbins, h[0])))
np.savetxt(args.i_histo, list(zip(hsbins, hs[0])))
plt.show()
