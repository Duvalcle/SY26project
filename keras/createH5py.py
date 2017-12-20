#!/usr/bin/env python


from random import shuffle
import glob
import os
import sys
import numpy as np
import h5py
import cv2
import matplotlib.pyplot as plt

IMG_SIZE_HEIGHT = 96
IMG_SIZE_WIDHT = 128
CLASSES = {"Carre": 0, "Cercle": 1, "Losange": 2, "Croix": 3, "Triangle": 4, "Octogone": 5}
NUM_CLASSES = len(CLASSES)

def get_class(img_path):
    classe = img_path.split('/')[-2]
    return CLASSES[classe]

path = sys.argv[1]
shuffle_data = True  # shuffle the addresses before saving
hdf5_path = 'dataset.hdf5'  # address to where you want to save the hdf5 file
train_addrs = glob.glob(os.path.join(path, '*/*.*'))
train_shape = (len(train_addrs), IMG_SIZE_HEIGHT, IMG_SIZE_WIDHT, 3)

if shuffle_data:
    shuffle(train_addrs)
train_labels = [get_class(addr) for addr in train_addrs]

hdf5_file = h5py.File(hdf5_path, mode='w')
hdf5_file.create_dataset("size_image", [3], np.int16)
hdf5_file.create_dataset("train_img", train_shape, np.float32)
# hdf5_file.create_dataset("train_mean", train_shape[1:], np.float32)
hdf5_file.create_dataset("train_labels", (len(train_addrs),), np.int8)
hdf5_file["train_labels"][...] = train_labels
hdf5_file["size_image"][...] = train_shape[1:4]


mean = np.zeros(train_shape[1:], np.float32)
for i in range(len(train_addrs)):
    addr = train_addrs[i]
    img = cv2.imread(addr)
    img = cv2.resize(img, (IMG_SIZE_WIDHT, IMG_SIZE_HEIGHT), interpolation=cv2.INTER_CUBIC)
    img = (img / 127.5) -1.
    if i % 100 == 0 and i > 1:
        print 'Train data: {}/{}'.format(i, len(train_addrs))
    hdf5_file["train_img"][i, ...] = img[None]
    mean += img / float(len(train_labels))

# hdf5_file["train_mean"][...] = mean
hdf5_file.close()
