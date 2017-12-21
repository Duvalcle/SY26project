#include "opencv2/opencv.hpp"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;
using namespace cv;

//fonction de detection de contours par un kernel de laplace
void detecte_edges_laplace(Mat & img) {
	Mat ep2, ep3;

	//definition du paramettre du filtre de laplace
	int ddepth = CV_8U, ksize = 3;
	double scale = 0.1, delta = 0;

	//couleur en niveaux de gris
	cvtColor(img,img,COLOR_BGR2GRAY);

	double minVal, maxVal;

	//reduction du bruit par plusieus filtre gaussien
	GaussianBlur(img,img, Size(7,7), 1, 1);
	GaussianBlur(img,img, Size(7,7), 1, 1);
	GaussianBlur(img,img, Size(7,7), 1, 1);

	//filtre de laplace
	Laplacian(img, ep2, ddepth, ksize, scale, delta, BORDER_DEFAULT );

	//normalisation de l'image
	minMaxLoc(ep2, &minVal, &maxVal); //find minimum and maximum intensities
	ep2.convertTo(ep3, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

	//réduction du bruit de detection
	for(int i = 0; i < 2; i++){
		medianBlur(ep3,ep3, 3);
	}

	//un seuil adaptatif pour souligné les contours
	adaptiveThreshold(ep3, img, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 0);
}


int main(int narg, char *argv[]) {

  VideoCapture Camera(0);
	Camera.set ( CV_CAP_PROP_FORMAT, CV_8UC3);
	Camera.set ( CV_CAP_PROP_FRAME_WIDTH,  720 );
  Camera.set ( CV_CAP_PROP_FRAME_HEIGHT, 576);
	Camera.set ( CV_CAP_PROP_BRIGHTNESS, 0.55 );
	Camera.set ( CV_CAP_PROP_CONTRAST , 0.75);
	Camera.set ( CV_CAP_PROP_GAIN, 0 );
	Camera.set ( CV_CAP_PROP_EXPOSURE, 0.2 );
  Camera.set ( CV_CAP_PROP_SATURATION, 0.6);

	if (!Camera.isOpened()) {
		cerr<<"Error opening the camera"<<endl;
		return -1;
}
    Mat frame;

    cout << "on filme" << endl;

    for(;;) {
		Camera.grab();
    Camera.retrieve ( frame);
		//afficher la photo initiale
		imshow("camera init", frame);

		//detections de contours
		detecte_edges(frame);

		//afficher la détéction de contours
    imshow("frame", frame);

        if(waitKey(10) >= 0) break;
    }
    Camera.release();
    cout << "that's all folk" << endl;

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
