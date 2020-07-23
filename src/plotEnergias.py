#!/bin/env python3
'''plotEnergias.py
Programa para graficar las energías de la simulación de granujas
'''

import argparse
import matplotlib.pyplot as plt
import numpy as np

def plot_datos(t, k, u, T):
    plt.plot(t, k, 'o-', label='E. cinética')
    plt.plot(t, u, 'o-', label='E. potencial')
    plt.plot(t, T, 'o-', label='E. total')
    plt.axhline(linewidth=1, color='k', alpha=0.5)
    plt.xlabel('t [s]')
    plt.ylabel('E [J]')
    plt.legend()
    plt.show()

def main():
    '''Programa principal: lee los datos desde el archivo de entrada que se
    pasa como parámetro.
    '''
    parser = argparse.ArgumentParser(description='''Programa para graficar las
    energías cinética, potencial y total en función del tiempo, para una
    simulación de los granujas magnéticos.''')
    parser.add_argument('-f', '--file', help='Archivo con las energías',
                        required=True, action='store')
    args = parser.parse_args()
    fileIn = args.file
    t, k, u = np.loadtxt(fileIn, unpack=True)
    T = k + u
    plot_datos(t, k, u, T)

if __name__ == '__main__':
    main()
