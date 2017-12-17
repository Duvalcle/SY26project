#!/usr/bin/env python

import numpy as np
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
from keras.layers.pooling import MaxPooling2D
from keras.optimizers import SGD
from keras.callbacks import LearningRateScheduler, ModelCheckpoint
from keras import backend as K
# K.set_image_data_format('channels_first')

from keras.datasets import mnist

IMG_SIZE_HEIGHT = 96
IMG_SIZE_WIDHT = 128
CLASSES = {"Carre": 0, "Cercle": 1, "Losange": 2, "Croix": 3, "Triangle": 4, "Octogone": 5}
NUM_CLASSES = len(CLASSES)
lr = 0.1


def preprocess_img(img):
    # Histogram normalization in v channel
    # hsv[:, :, 2] = exposure.equalize_hist(hsv[:, :, 2])
    # img = color.hsv2rgb(hsv)

    # central square crop
    # min_side = min(img.shape[:-1])
    # centre = img.shape[0] // 2, img.shape[1] // 2
    # img = img[centre[0] - min_side // 2:centre[0] + min_side // 2,
    #           centre[1] - min_side // 2:centre[1] + min_side // 2,
    #           :]
    # print(img)
    # img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # rescale to standard size
    img = cv2.resize(img, (IMG_SIZE_HEIGHT, IMG_SIZE_WIDHT))

    # roll color axis to axis 0
    # img = np.rollaxis(img, -1)

    return img


def lr_schedule(epoch):
    return lr * (0.1 ** int(epoch / 10))

def get_class(img_path):
    classe = img_path.split('/')[-2]
    return CLASSES[classe]

def cnn_model(input_shape):
    model = Sequential()

    # model.add(Conv2D(32, (3, 3), padding='same',
    #                  input_shape=(1, IMG_SIZE_WIDHT, IMG_SIZE_HEIGHT),
    #                  activation='relu'))
    # model.add(Conv2D(32, (3, 3), activation='relu'))
    # model.add(MaxPooling2D(pool_size=(2, 2)))
    # model.add(Dropout(0.2))
    #
    # model.add(Conv2D(64, (3, 3), padding='same',
    #                  activation='relu'))
    # model.add(Conv2D(64, (3, 3), activation='relu'))
    # model.add(MaxPooling2D(pool_size=(2, 2)))
    # model.add(Dropout(0.2))
    #
    # model.add(Conv2D(128, (3, 3), padding='same',
    #                  activation='relu'))
    # model.add(Conv2D(128, (3, 3), activation='relu'))
    # model.add(MaxPooling2D(pool_size=(2, 2)))
    # model.add(Dropout(0.2))
    #
    # model.add(Flatten())
    # model.add(Dense(512, activation='relu'))
    # model.add(Dropout(0.5))
    # model.add(Dense(NUM_CLASSES, activation='softmax'))

    model.add(Conv2D(32, kernel_size=(5, 5),
                     activation='relu',
                     input_shape=input_shape))
    model.add(MaxPooling2D(pool_size=(2, 2))) #Test Clement
    model.add(Conv2D(64, (3, 3), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2))) #Test Clement
    model.add(Conv2D(64, (3, 3), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))

    # model.add(Dropout(0.25))
    model.add(Flatten())
    model.add(Dense(128, activation='relu'))
    # model.add(Dropout(0.5))
    model.add(Dense(NUM_CLASSES, activation='softmax'))
    print("\nneuronal shape:")

    for layer in model.layers:
        print(layer.get_output_at(0).get_shape().as_list())
    return model



def main(arg):
    path = arg
    imgs = []
    labels = []

    # (x_train, y_train), (x_test, y_test) = mnist.load_data()

    # all_img_paths = glob.glob(os.path.join(path, '*/*.*'))
    # np.random.shuffle(all_img_paths)
    # for img_path in all_img_paths:
    #     img = preprocess_img(cv2.imread(img_path,0))[None,:,:]
    #     label = get_class(img_path)
    #     imgs.append(img)
    #     labels.append(label)
    #
    # X = np.array(imgs, dtype='float32')
    # # Make one hot targets
    # Y = np.eye(NUM_CLASSES, dtype='uint8')[labels]

    x_train = HDF5Matrix(path, 'train_img')
    y_train = HDF5Matrix(path, 'train_labels')
    input_shape = HDF5Matrix(path, 'size_image')
    print "X shape:", x_train.shape
    print "Y shape:", y_train.shape
    print "input_shape:", input_shape[0]
    print "input_shape:", input_shape[1]
    print "input_shape:", input_shape[2]

    input_shape = np.array(input_shape, dtype='int')
    # print input_shape


    y_train = keras.utils.to_categorical(y_train, NUM_CLASSES)

    model = cnn_model(input_shape)

    sgd = SGD(lr=lr, decay=1e-6, momentum=0.9, nesterov=True)
    model.compile(loss='categorical_crossentropy',
                  optimizer=sgd,
                  metrics=['accuracy'])

    # model.compile(loss=keras.losses.categorical_crossentropy,
    #               optimizer=keras.optimizers.Adadelta(),
    #               metrics=['accuracy'])

    batch_size = 60
    epochs = 12

    # X.reshape((-1,X.shape[], X.shape, X.shape))

    model.fit(x_train, y_train,
              batch_size=batch_size,
              epochs=epochs,
              validation_split=0.2,
              callbacks=[LearningRateScheduler(lr_schedule),
                         ModelCheckpoint('model.h5', save_best_only=True)]
              )
    # model.fit(x_train, y_train,
    #           batch_size=batch_size,
    #           epochs=epochs,
    #           verbose=1
    #           )
    model.save("SY26_model.h5")

# def main():
#     X_test = []
#     y_test = []
#     with open('dataset.csv') as csvfile:
#         readCSV = csv.reader(csvfile, delimiter=';')
#         for row in readCSV:
#             # print(row)
#             # print(row[0])
#             # print(row[1])
#             X_test.append(preprocess_img(io.imread(row[0])))
#             # X_test.append(row[0])
#             y_test.append(row[1])



if __name__ == '__main__':
    main(sys.argv[1])
