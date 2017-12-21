#!/usr/bin/env python


from random import shuffle
import glob
import os
import sys
import numpy as np
import h5py
import cv2
import matplotlib.pyplot as plt

#taille de l'image en entree
IMG_SIZE_HEIGHT = 24
IMG_SIZE_WIDHT = 32
#association classe numero
CLASSES = {"Carre": 0, "Cercle": 1, "Losange": 2, "Croix": 3, "Triangle": 4, "Octogone": 5}
NUM_CLASSES = len(CLASSES)

#fonction pour labeliser l'image en fonction du chemin
def get_class(img_path):
    classe = img_path.split('/')[-2]
    return CLASSES[classe]

path = sys.argv[1]
shuffle_data = True
hdf5_path = 'dataset.hdf5'
#recuperation de tout les chemin vers les images
train_addrs = glob.glob(os.path.join(path, '*/*.*'))
train_shape = (len(train_addrs), IMG_SIZE_HEIGHT, IMG_SIZE_WIDHT, 3)

# melange des chemins
if shuffle_data:
    shuffle(train_addrs)
# labelisation
train_labels = [get_class(addr) for addr in train_addrs]

#creation du fichier hdf5
hdf5_file = h5py.File(hdf5_path, mode='w')
hdf5_file.create_dataset("size_image", [3], np.int16)
hdf5_file.create_dataset("train_img", train_shape, np.float32)
hdf5_file.create_dataset("train_labels", (len(train_addrs),), np.int8)
# ecriture des label et la taille de l'entree du reseau de neurone
hdf5_file["train_labels"][...] = train_labels
hdf5_file["size_image"][...] = train_shape[1:4]

# pour tout les chemins
for i in range(len(train_addrs)):
    # lecrure de l'image
    addr = train_addrs[i]
    img = cv2.imread(addr)
    # resize de l'image
    img = cv2.resize(img, (IMG_SIZE_WIDHT, IMG_SIZE_HEIGHT), interpolation=cv2.INTER_CUBIC)
    if i % 100 == 0 and i > 1:
        print 'Train data: {}/{}'.format(i, len(train_addrs))
    # ecriture dans le fichier de l'image
    hdf5_file["train_img"][i, ...] = img[None]

#fermeture du fichier
hdf5_file.close()
