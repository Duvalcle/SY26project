#include "opencv2/opencv.hpp"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;
using namespace cv;

//fonction pour troncer les valeurs des photos entre 0 et 1
void troncate(Mat &img) {
	threshold(img, img, 1, 1, THRESH_TRUNC);
	threshold(img, img, 0, 0, THRESH_TOZERO);
}

//fonction pour normalizer l'image
void regulate(Mat &img) {
	double min, max;
	minMaxLoc(img, &min, &max);
	img = (img - min) / (max - min);
}

void detecte_edges_gabor(Mat & img) {
	// definition des kernels
	int kernel_size = 11;
	double sig = 1.5, lm = 4, gm = 0.3, ps = 0;
	double theta[4];

	theta[0] = 0 ;
	theta[1] = CV_PI/4 ;
	theta[2] = CV_PI/2 ;
	theta[3] = 3 * CV_PI/4 ;

  Mat kernel[4], prKer[4];
  for(int i =0; i<4; i++){
		kernel[i] =getGaborKernel(Size(kernel_size,kernel_size), sig, theta[i], lm,gm, ps, CV_32F);
		resize(kernel[i], prKer[i], cv::Size(), 10, 10);
		regulate(prKer[i]);
	}
	Mat temp[4];
	Mat result;

	//couleur en niveaux de gris
  cvtColor(img,img,COLOR_BGR2GRAY);

	//detection de contours
  for(int i =0; i<4; i++){
		filter2D(img, temp[i], CV_32F, kernel[i]);
		troncate(temp[i]);
	}

	bitwise_and(temp[0], temp[1], img);
	bitwise_and(img, temp[2], img);
	bitwise_and(img, temp[3], img);

	//afficher les kernels
	// imshow("kernel0", prKer[0]);
	// imshow("kernel1", prKer[1]);
	// imshow("kernel2", prKer[2]);
	// imshow("kernel3", prKer[3]);

	//afficher les detections de contours
	// imshow("camera0", temp[0]);
	// imshow("camera1", temp[1]);
	// imshow("camera3", temp[3]);
	// imshow("camera2", temp[2]);

	//afficher les detections de contours au carré
	// imshow("camera0_2", temp_2[0]);
	// imshow("camera2_2", temp_2[2]);
	// imshow("camera1_2", temp_2[1]);
	// imshow("camera3_2", temp_2[3]);
}

int main(int narg, char *argv[]) {

	cout << "on filme" << endl;
	Mat frame;
  frame = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED );
  if(! frame.data )                              // Check for invalid input
  {
    cout <<  "Could not open or find the image" << std::endl ;
    return -1;
  }
	//afficher la photo initiale
	imshow("photo_base", frame);

	//detections de contours
	detecte_edges_gabor(frame);

  for(;;) {
		//afficher la détéction de contours
		imshow("photo", frame);

      if(waitKey(10) >= 0) break;
  }
  cout << "that's all folk" << endl;

  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
