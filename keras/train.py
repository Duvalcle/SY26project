#!/usr/bin/env python

import numpy as np
# np.random.seed(123)  # for reproducibility
import os
import sys
import cv2
    # import tensorflow as tf
import csv
import glob
import h5py

import keras
from keras.utils import HDF5Matrix
from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Activation, Flatten
from keras.layers.convolutional import Conv2D
from keras.layers.pooling import MaxPooling2D, AveragePooling2D
from keras.optimizers import SGD, Adam
from keras.callbacks import LearningRateScheduler, ModelCheckpoint
from keras import backend as K
# K.set_image_data_format('channels_first')

from keras.datasets import mnist

# IMG_SIZE_HEIGHT = 96
# IMG_SIZE_WIDHT = 128
CLASSES = {"Carre": 0, "Cercle": 1, "Losange": 2, "Croix": 3, "Triangle": 4, "Octogone": 5}
NUM_CLASSES = len(CLASSES)

def lr_schedule(epoch):
    return lr * (0.1 ** int(epoch / 10))

def get_class(img_path):
    classe = img_path.split('/')[-2]
    return CLASSES[classe]

def cnn_model(input_shape):
    model = Sequential()

    model.add(Conv2D(32, kernel_size=(5, 5),
                     activation='relu',
                     input_shape=input_shape))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2))) #Test Clement
    model.add(Conv2D(64, (3, 3), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2,2))) #Test Clement
    model.add(Conv2D(64, (3,3), activation='relu'))
    model.add(AveragePooling2D(pool_size=(4, 4), strides=(4,4))) #Test Clement
    model.add(Conv2D(64, (3, 3), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))

    model.add(Dropout(0.25))
    model.add(Flatten())

    model.add(Dense(500, activation='relu'))
    model.add(Dropout(0.40))
    model.add(Dense(NUM_CLASSES, activation='softmax'))


    print("\nneuronal shape:")

    for layer in model.layers:
        print(layer.name, layer.get_output_at(0).get_shape().as_list())
    return model

def main(arg):
    path = arg
    imgs = []
    labels = []

    x_train = HDF5Matrix(path, 'train_img')
    y_train = HDF5Matrix(path, 'train_labels')
    input_shape = HDF5Matrix(path, 'size_image')
    print "X shape:", x_train.shape
    print "Y shape:", y_train.shape
    print "input_shape:", input_shape[0]
    print "input_shape:", input_shape[1]
    print "input_shape:", input_shape[2]

    input_shape = np.array(input_shape, dtype='int')

    y_train = keras.utils.to_categorical(y_train, NUM_CLASSES)
    model = cnn_model(input_shape)
    sgd = Adam(lr=0.001)
    # sgd = SGD(lr=lr, decay=1e-6, momentum=0.9, nesterov=True)
    model.compile(loss='categorical_crossentropy',
                  optimizer=sgd,
                  metrics=['accuracy'])

    batch_size = 30
    epochs = 20

    model.fit(x_train, y_train,
              batch_size=batch_size,
              epochs=epochs,
              validation_split=0.2,
              callbacks=[ModelCheckpoint('model.h5', save_best_only=True)]
              )

if __name__ == '__main__':
    main(sys.argv[1])
