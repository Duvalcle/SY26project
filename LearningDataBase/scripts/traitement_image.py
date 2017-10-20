from PIL import Image
from resizeimage import resizeimage


with open('../Brut/+.png', 'r+b') as f:
    with Image.open(f) as image:
        cover = resizeimage.resize_crop(image, [800, 800])
        cover.save('../Rogne/+test.png', image.format)
