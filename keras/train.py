#!/usr/bin/env python

import numpy as np
import os
import sys
import cv2
import glob
import h5py

import keras
from keras.utils import HDF5Matrix
from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Activation, Flatten
from keras.layers.convolutional import Conv2D
from keras.layers.pooling import MaxPooling2D
from keras.optimizers import Adam
from keras.callbacks import ModelCheckpoint
from keras import backend as K


# IMG_SIZE_HEIGHT = 96
# IMG_SIZE_WIDHT = 128
CLASSES = {"Carre": 0, "Cercle": 1, "Losange": 2, "Croix": 3, "Triangle": 4, "Octogone": 5}
NUM_CLASSES = len(CLASSES)

# fonction pour définir le réseau de neurone
def cnn_model(input_shape):
    model = Sequential()

    model.add(Conv2D(32, kernel_size=(5, 5),
                     activation='relu',
                     input_shape=input_shape))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2))) #Test Clement
    model.add(Conv2D(64, (3, 3), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2,2))) #Test Clement
    model.add(Conv2D(64, (3,3), activation='relu'))

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

    # récuparition des données du fichier
    x_train = HDF5Matrix(path, 'train_img')
    y_train = HDF5Matrix(path, 'train_labels')
    input_shape = HDF5Matrix(path, 'size_image')

    #debuf print
    print "X shape:", x_train.shape
    print "Y shape:", y_train.shape
    print "input_shape:", input_shape[0]
    print "input_shape:", input_shape[1]
    print "input_shape:", input_shape[2]

    # transtypage
    input_shape = np.array(input_shape, dtype='int')
    y_train = keras.utils.to_categorical(y_train, NUM_CLASSES)

    # création du réseau de neurone
    model = cnn_model(input_shape)

    # règle d'aprentissage Adam
    sgd = Adam(lr=0.001)

    # compilation du modéle
    model.compile(loss='categorical_crossentropy',
                  optimizer=sgd,
                  metrics=['accuracy'])

    # parametre d'apprentissage
    batch_size = 30
    epochs = 20

    # entrainement du model
    model.fit(x_train, y_train,
              batch_size=batch_size,
              epochs=epochs,
              validation_split=0.2,
              callbacks=[ModelCheckpoint('model.h5', save_best_only=True)]
              )

if __name__ == '__main__':
    main(sys.argv[1])
