import io
import time
import cv2
import numpy as np
import h5py
import keras
import glob
from keras.optimizers import SGD, Adam
#import matplotlib.pyplot as plt
from picamera import PiCamera
from picamera.array import PiRGBArray
import picamera.array
from keras.models import load_model

IMG_SIZE_HEIGHT = 24
IMG_SIZE_WIDHT = 32
sgd = Adam(lr=0.001)

CLASSES = {0:"Carre", 1:"Cercle", 2:"Losange", 3:"Croix", 4:"Triangle", 5:"Octogone"}

#Fonction which predict the form seen from model previously load and compiled
def test_img(img):
	imarr = np.array(img).astype(np.float32)
	imarr = np.expand_dims(imarr, axis=0)
	# prediction probability vector :
	out = model.predict_classes(imarr, batch_size=1, verbose=0)
	print CLASSES.get(out[0], "none")
	print "_______"

camera = PiCamera()
camera.resolution = (640,480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640,480))
time.sleep(0.1)

#Load model from learning
model = load_model('model.h5')
#Compile the model
model.compile(loss='categorical_crossentropy', optimizer=sgd, metrics=['accuracy'])

#Streaming
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
		#grab the raw NumPy array representing the image - this array
		#will be 3D, representing the width, height and # of channels
		image = frame.array
		cv2.imshow("Frame", image)
		key = cv2.waitKey(1) & 0xFF
		image = cv2.resize(image,(IMG_SIZE_WIDHT,IMG_SIZE_HEIGHT))
		test_img(image)

		# clear the stream in preparation or the next frame
		rawCapture.truncate(0)

		# Quit if "q" is hit
		if key == ord("q"):
			break
print("thanks")
