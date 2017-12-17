#!/usr/bin/env python
import sys
import cv2
import numpy as np

def detecte_edges(frame):

    ddepth = cv2.CV_8U
    ksize = 3
    scale = 0.1
    delta = 0

    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    cv2.GaussianBlur(frame, (7,7), 1 , frame, 1 )
    cv2.GaussianBlur(frame, (7,7), 1 , frame, 1 )
    cv2.GaussianBlur(frame, (7,7), 1 , frame, 1 )

    cv2.Laplacian(frame, ddepth, frame, ksize, scale, delta, cv2.BORDER_DEFAULT)

    minVal, maxVal, _, _ = cv2.minMaxLoc(frame)

    for i in range(1,4):
        frame = cv2.medianBlur(frame,5)

    frame = cv2.adaptiveThreshold(frame, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 7, 0)

    return frame

# Mat ep2, ep3;
#
#
# cvtColor(img,img,COLOR_BGR2GRAY);
#
# double minVal, maxVal;
#
# GaussianBlur(img,img, Size(7,7), 1, 1);
# GaussianBlur(img,img, Size(7,7), 1, 1);
# GaussianBlur(img,img, Size(7,7), 1, 1);
#
# Laplacian(img, ep2, ddepth, ksize, scale, delta, BORDER_DEFAULT );
#
# minMaxLoc(ep2, &minVal, &maxVal); //find minimum and maximum intensities
# ep2.convertTo(ep3, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
# for(int i = 0; i < 2; i++){
#     medianBlur(ep3,ep3, 3);
# }
#
# adaptiveThreshold(ep3, img, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 0);
# }


def main (arg):
    photo = sys.argv[1]
    img = cv2.imread(photo,6)

    img = detecte_edges(img)

    cv2.imshow('image',img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)
