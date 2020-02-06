#include <cv.h>
#include <highgui.h>


int main()
{
	IplImage* img = cvLoadImage("./flowers.jpg");
    cvNamedWindow("Original");
    cvShowImage("Original", img);

    // We add processing code here
    /* FOR ERODING
    cvErode(img, img, 0, 1);

    cvNamedWindow("Eroded");
    cvShowImage("Eroded", img); */

    /* FOR DILATION
    cvDilate(img, img, 0, 2);
    cvNamedWindow("Dilated");

    cvShowImage("Dilated", img); */

    /* FOR BRIGHTENING
    cvAddS(img, cvScalar(50,50,50), img);
    cvNamedWindow("Bright");
    cvShowImage("Bright", img); */

    /* FOR CONTRAST
    cvScale(img, img, 2);
    cvNamedWindow("Contrast");

    cvShowImage("Contrast", img); */

    /* FOR INVERTING
    cvNot(img, img);
    cvNamedWindow("Invert");
    cvShowImage("Invert", img); */

    cvWaitKey(0);
    cvReleaseImage(&img);
    return 0;
}