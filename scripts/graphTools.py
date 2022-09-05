#!/bin/env python3
'''Programa de herramientas de análisis usando grafos.'''

import networkx as nx
import numpy as np

def make_graph(f):
    '''Devuelve el grafo de contactos a partir del archivo de entrada
    xvc.'''
    fin = open(f, 'r')
    data = fin.readlines()
    fin.close()
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
    return G

def coord_number(G, t):
    '''Devuelve el número medio de coordinación por tipo. Si tipo == 0, es el
    número medio de todos los granos, en otro caso solo el correspondiente al
    tipo establecido.'''
    if t == 0:
        nc = []
        for n in G.nodes():
            nc.append(G.degree[n])
        a_nc = np.array(nc)
        return a_nc.mean(), a_nc.std()
    else:
        nc = []
        for n in G.nodes():
            if G.nodes[n]['tipo'] != t:
                continue
            nc.append(G.degree[n])
        a_nc = np.array(nc)
        return a_nc.mean(), a_nc.std()

def strenght_number(G, t):
    '''Devuelve el número medio de strenght por tipo. Si tipo == 0, es el
    número medio de todos los granos, en otro caso solo el correspondiente al
    tipo establecido.'''
    if t == 0:
        ns = np.array([G.degree(n, 'weight') for n in G.nodes])
        return ns.mean(), ns.std()
    else:
        nc = []
        for n in G.nodes():
            if G.nodes[n]['tipo'] != t:
                continue
            nc.append(G.degree(n, 'weight'))
        a_nc = np.array(nc)
        return a_nc.mean(), a_nc.std()
