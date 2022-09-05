#!/usr/bin/env python3
import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

df = pd.read_csv('nc-ns.dat')
print(df.info())
c1 = (df['nt1'] <= 150)
# cdm1 = (df['tipo'] == 'D') & (df['m'] == 1)
df1 = df[c1]
print(df1.info())
print(df1['m'].unique())
print(df1['tipo'].unique())
gnc= sns.relplot(x='nt1', y='mean-nc', kind='line', hue='m', style='tipo',
                data=df1)
gnc1 = sns.relplot(x='nt1', y='mean-nc1', kind='line', hue='m', style='tipo',
                data=df1)
gns= sns.relplot(x='nt1', y='mean-s', kind='line', hue='m', style='tipo',
                data=df1)
gns1 = sns.relplot(x='nt1', y='mean-s1', kind='line', hue='m', style='tipo',
                data=df1)
plt.show()
