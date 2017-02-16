#!/usr/bin/env python


#from mpl_toolkits.mplot3d import Axes3D
#from matplotlib import cm
#from matplotlib import mlab
#from matplotlib.ticker import LinearLocator, FormatStrFormatter
#import matplotlib.pyplot as plt
#import numpy as np


#f = open('better.txt', 'r')
#f = open('output_moutain_ghc.txt', 'r')
#X = []
#Y = []
#Z = []
#for l in f:
#    tmp = l.split()
#    X.append(float(tmp[0]))
#    Y.append(float(tmp[1]))
#    Z.append(float(tmp[2]))

#fig = plt.figure()
#ax = fig.gca(projection='3d')
#X2, Y2 = np.meshgrid(X, Y)
#Z2 = mlab.griddata(X, Y, Z, X2, Y2)


#print X2
#print Y2
#print Z2
#surf = ax.plot_surface(X2, Y2, Z2, rstride=1, cstride=1, cmap=cm.coolwarm,
#                               linewidth=0, antialiased=False)
#ax.set_zlim(-1.01, 1.01)

#ax.zaxis.set_major_locator(LinearLocator(10))
#ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

#fig.colorbar(surf, shrink=0.5, aspect=5)


#ax.scatter(X, Y, Z)
#ax.set_xlabel("stride (Byte)")
#ax.set_ylabel("log(# of elements accessed)")
#ax.set_zlabel("MB/s")
#plt.show()



#! /usr/bin/env python

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy
import matplotlib.cm as cm
import argparse


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("file", type=str, help="Input file")
    parser.add_argument("-o", "--out", type=str, default="mountain.png", help="Output file")
    args = parser.parse_args()

    x, y, z = numpy.loadtxt(args.file, unpack=True)


    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')


    ax.set_xlabel("Stride")
    ax.set_ylabel("log2(size) (Bytes)")
    ax.set_zlabel("MB/s")
    plt.tight_layout()

    ax.plot_trisurf(x, y, z, cmap=cm.Blues_r)
	ax.invert_yaxis()

    plt.savefig(args.out)
