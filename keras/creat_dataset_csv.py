#!/usr/bin/env python

import numpy as np

# from keras.models import Sequential
# from keras.layers import Dense, Dropout, Activation, Flatten
# from keras.layers import Convolution2D, MaxPooling2D
#
# from keras.utils import np_utils


from os import listdir
from os.path import isfile, join
import glob
import sys

def main(arg):
    path = arg
    classe = -1
    string = ""
    for dire in glob.iglob(path+ "*"):
        classe += 1
        for pa in glob.iglob(dire+ "/*.*"):
            string += "{};{}\n".format(pa, classe)
    f = open('dataset.csv', 'w')
    f.write(string)
    f.close()


if __name__ == '__main__':
    main(sys.argv[1])
