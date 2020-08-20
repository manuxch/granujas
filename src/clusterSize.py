#!/bin/env python3
'''Programa para el análisis del tamaño de clusters utilizando grafos.'''

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
    description='Calcula la distribución de tamaños de clusters.')
parser.add_argument('-o', '--output', default='cluster_sizes.dat',
            help='Archivo de guardado de histograma.')
parser.add_argument('-s', '--skip', type=int, default=0,
            help='Cantidad de frames inicales que se ignoran en el promedio.')
parser.add_argument('-p', '--prefix', required = True,
            help='Prefijo del nombre de archivos xvc.')
args = parser.parse_args()

# Levanto todos los archivos con datos del grafo (*xvc)
file_list = []
for f in glob.glob(args.prefix + '*.xvc'):
    file_list.append(f)
n_files = len(file_list)
file_list.sort()

# Detecto la cantidad de granos (nodos) en el sistema
fin = open(file_list[0], 'r')
data = fin.readlines()
fin.close()
n_granos = len(data) - 1 # resto 1 porque la primera línea es encabezado
print(f'Número total de granos: {n_granos}')
size_clust = np.zeros(n_granos + 1)

# Proceso los archivos
n_tot_files_anal = len(file_list[args.skip:])
for f in progressbar(file_list[args.skip:]):
    fin = open(f, 'r')
    data = fin.readlines()
    fin.close()
    G = nx.Graph()
    edges = []
    for linea in data[1:]:
        l = linea.split()
        gid = int(l[0])
        if gid == -100:
            continue
        G.add_node(gid)
        for sid in l[9:]:
            if int(sid) == -100:
                continue
            edges.append((gid, int(sid)))
    G.add_edges_from(edges)
    cc = [len(c) for c in sorted(nx.connected_components(G), key=len,
                                 reverse=True)]
    for i in cc:
        size_clust[i] += 1
size_clust = np.array(size_clust) / float(n_tot_files_anal)
no_zero = np.nonzero(size_clust)
max_clus_size = no_zero[0][-1]
xbins = np.arange(0, max_clus_size + 1)
print(f'Tamaño máximo de cluster: {max_clus_size}')
print(xbins.shape, size_clust.size)
np.savetxt(args.output, list(zip(xbins[1:max_clus_size + 1],
                                     size_clust[1:max_clus_size + 1])))
plt.bar(xbins[1:max_clus_size + 1], size_clust[1:max_clus_size + 1])
plt.xlabel('Cluster size')
plt.ylabel('<n>')
plt.show()
