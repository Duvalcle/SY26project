#!/usr/bin/env python3
import tensorflow as tf
sess = tf.InteractiveSession()

# classes = ["carre", "cercle", "losange", "plus", "triangle"]
# train_path='../data'
#
# # validation split
# validation_size = 0.2
#
# # batch size
# batch_size = 16
#
# data = tf.dataset.read_train_sets(train_path, img_size, classes, validation_size=validation_size)

from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets('MNIST_data', one_hot=True)




 sess.close()
