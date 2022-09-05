#!/bin/env python3
'''Programa para analizar el número de n-meros usando grafos.'''

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
    description='Calcula la distribución de n-meros.')
parser.add_argument('-o', '--output', default='dist-nmeros.dat',
                    help='Archivo de salida por frame.')
parser.add_argument('-t', '--tipo', default=1,
            help='Tipo de grano que define el n-mero.')
args = parser.parse_args()
tmero = int(args.tipo)
print('Detectando n-meros de tipo:', tmero)

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
nc1 = []
nc1_file = open(args.output, 'w')
nc1_file.write('#label n clus_size \n')

for f in progressbar(file_list):
    fin = open(f, 'r')
    data = fin.readlines()
    fin.close()
    #n_frm = int(f.split('_')[1][:-4])
    n_frm = f.split('_')[1][:-4]
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
    clusters = nx.connected_components(G)
    for nodes in clusters:
        nt1 = 0
        for node in nodes:
            # print(G.nodes[node]['tipo'])
            if G.nodes[node]['tipo'] == tmero:
                nt1 += 1
        nc1.append((n_frm, nt1, len(nodes)))

for d in nc1:
    if d[1] == 0:
        continue
    nc1_file.write('{:s} {:d} {:d}\n'.format(d[0], d[1], d[2]))


    # for c in clusters:
        # if len(c) == 1:
            # continue
        # print(c)

