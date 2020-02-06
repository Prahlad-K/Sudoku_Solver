
#include <cv.h>
#include <highgui.h>

int main()
{
	CvCapture* capture = 0;
    capture = cvCaptureFromCAM(0);

    if(!capture)
    {
        printf("Could not initialize capturing...");
        return -1;
    }

    cvNamedWindow("video");
    while(true)
    {
    	IplImage* frame = 0;
    	frame = cvQueryFrame(capture);
    	if(!frame)
    		break;
    	cvShowImage("video", frame);
    	int c = cvWaitKey(20);
    	if((char)c==27)
    		break;
    }
    cvReleaseCapture(&capture);
    return 0;
}