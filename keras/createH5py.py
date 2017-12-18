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

def detecte_edges(frame):

    ddepth = cv2.CV_8U
    ksize = 3
    scale = 0.1
    delta = 0

    # frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    cv2.GaussianBlur(frame, (7,7), 1 , frame, 1 )
    cv2.GaussianBlur(frame, (7,7), 1 , frame, 1 )
    cv2.GaussianBlur(frame, (7,7), 1 , frame, 1 )

    cv2.Laplacian(frame, ddepth, frame, ksize, scale, delta, cv2.BORDER_DEFAULT)

    # minVal, maxVal, _, _ = cv2.minMaxLoc(frame)

    for i in range(1,3):
        frame = cv2.medianBlur(frame,5)

    # frame = cv2.adaptiveThreshold(frame, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 7, 0)

    return frame

def get_class(img_path):
    classe = img_path.split('/')[-2]
    return CLASSES[classe]

path = sys.argv[1]
shuffle_data = True  # shuffle the addresses before saving
hdf5_path = 'dataset.hdf5'  # address to where you want to save the hdf5 file
# read addresses and labels from the 'train' folder

train_addrs = glob.glob(os.path.join(path, '*/*.*'))
# to shuffle data
if shuffle_data:
    shuffle(train_addrs)
train_labels = [get_class(addr) for addr in train_addrs]
# Divide the hata into 60% train, 20% validation, and 20% test
# train_addrs = addrs[0:int(0.6*len(addrs))]
# train_labels = labels[0:int(0.6*len(labels))]
# val_addrs = addrs[int(0.6*len(addrs)):int(0.8*len(addrs))]
# val_labels = labels[int(0.6*len(addrs)):int(0.8*len(addrs))]
# test_addrs = addrs[int(0.8*len(addrs)):]
# test_labels = labels[int(0.8*len(labels)):]

train_shape = (len(train_addrs), IMG_SIZE_HEIGHT, IMG_SIZE_WIDHT, 3)

hdf5_file = h5py.File(hdf5_path, mode='w')
hdf5_file.create_dataset("size_image", [3], np.int16)
hdf5_file.create_dataset("train_img", train_shape, np.float32)
hdf5_file.create_dataset("train_mean", train_shape[1:], np.float32)
hdf5_file.create_dataset("train_labels", (len(train_addrs),), np.int8)
hdf5_file["train_labels"][...] = train_labels
hdf5_file["size_image"][...] = train_shape[1:4]


mean = np.zeros(train_shape[1:], np.float32)
# loop over train addresses
for i in range(len(train_addrs)):
    # print how many images are saved every 1000 images
    # read an image and resize to (224, 224)
    # cv2 load images as BGR, convert it to RGB
    addr = train_addrs[i]
    img = cv2.imread(addr)
    # img = detecte_edges(img)
    img = cv2.resize(img, (IMG_SIZE_WIDHT, IMG_SIZE_HEIGHT), interpolation=cv2.INTER_CUBIC)
    img = (img / 127.5) -1.
    # img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # img = img.reshape((IMG_SIZE_HEIGHT, IMG_SIZE_WIDHT, 1))
    # img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    # print img.shape
    if i % 100 == 0 and i > 1:
        print 'Train data: {}/{}'.format(i, len(train_addrs))
    # add any image pre-processing here
    # save the image and calculate the mean so far
    hdf5_file["train_img"][i, ...] = img[None]
    mean += img / float(len(train_labels))

# save the mean and close the hdf5 file
hdf5_file["train_mean"][...] = mean
hdf5_file.close()
