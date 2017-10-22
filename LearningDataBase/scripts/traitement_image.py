from PIL import Image
from resizeimage import resizeimage
import os
import imageio
import matplotlib.pyplot as plt

plus_lumino = 170
#Rognage des images et copie dans le dossier Rogne

#for element in os.listdir('../Brut'):
#    with open('../Brut/'+element, 'r+b') as f:
#        with Image.open(f) as image:
#            cover = resizeimage.resize_crop(image, [800, 800])
#            cover.save('../Rogne/'+element, image.format)

for element in os.listdir('../Rogne'):
    string = "../Rogne/"+element
    image = imageio.imread(string)
    #  0 : red, 1 green2 2 blue
    for i in range(800):
        for j in range(800):
            for k in range(3):
                if (image[i,j][k]<=255 - plus_lumino):
                    image[i,j][k] = image[i,j][k] + plus_lumino
                else :
                    image[i,j][k] = 255
        print(element+str(i))
    string2 = "../Degradee/plus_lumino"+element
    imageio.imsave(string2, image)
