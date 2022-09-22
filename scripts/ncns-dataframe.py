#!/bin/env python3
'''Programa para analizar el número de coordinación y strength.'''

import glob
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import sys
from graphTools import *
import pandas as pd

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

# Levanto todos los archivos con datos del grafo (*xvc)
file_list = []
for f in glob.glob('*.xvc'):
#  for f in glob.glob('../result*/*/*.xvc'):
    file_list.append(f)
n_files = len(file_list)
file_list.sort()
print('Número de archivos', n_files)
data = {}
row_list = []
for f in progressbar(file_list):
    fl = f.split('/')
    if 'Disk' in fl[1]:
        tipo = 'D'
    elif 'Triang' in fl[1]:
        tipo = 'T'
    elif 'MixDT' in fl[1]:
        tipo = 'DT'
    elif 'MixTD' in fl[1]:
        tipo = 'TD'
    else:
        print('Tipo no determinado', f)
        quit()
    flm = fl[1].split('-')
    m = int(flm[-1])
    fln = fl[2].split('-')
    nt1 = int(fln[0])
    clave = f'{tipo}-{m}-{nt1}'
    gf = make_graph(f)
    dict_row = {}
    dict_row['tipo'] = tipo
    dict_row['m'] = m
    dict_row['nt1'] = nt1
    nc, stdnc = coord_number(gf, 0)
    dict_row['mean-nc'] = nc
    dict_row['std-nc'] = stdnc
    nc1, stdnc1 = coord_number(gf, 1)
    dict_row['mean-nc1'] = nc1
    dict_row['std-nc1'] = stdnc1
    row_list.append(dict_row)
    s, std_s = strenght_number(gf, 0)
    dict_row['mean-s'] = s
    dict_row['std-s'] = std_s
    s1, std_s1 = strenght_number(gf, 1)
    dict_row['mean-s1'] = s1
    dict_row['std-s1'] = std_s1


df = pd.DataFrame(row_list, columns=['tipo', 'm', 'nt1', 'mean-nc', 'std-nc',
        'mean-nc1', 'std-nc1', 'mean-s', 'std-s', 'mean-s1', 'std-s1'])
print(df.info())
print(df.head())
df.to_csv('nc-ns.dat', index=False)
