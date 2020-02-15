#include <cv.h>
#include <highgui.h>


int main()
{
	IplImage* img = cvLoadImage("./sudoku.jpg");
	cvNamedWindow("myfirstwindow");
    cvShowImage("myfirstwindow", img);
    cvWaitKey(0);
    cvReleaseImage(&img);

    return 0;
}