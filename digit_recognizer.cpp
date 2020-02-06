#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <ml.h>

#include "digit_recognizer.h"
using namespace cv;
using namespace ml;
using namespace std;
#define MAX_NUM_IMAGES    60000
typedef bitset<8> BYTE;

DigitRecognizer::DigitRecognizer()
{
    knn = KNearest::create();
}

DigitRecognizer::~DigitRecognizer()
{
    delete knn;
}

int DigitRecognizer::readFlippedInteger(FILE *fp)
{
    int ret = 0;

    BYTE *temp;

    temp = (BYTE*)(&ret);
    fread(&temp[3], sizeof(BYTE), 1, fp);
    fread(&temp[2], sizeof(BYTE), 1, fp);
    fread(&temp[1], sizeof(BYTE), 1, fp);

    fread(&temp[0], sizeof(BYTE), 1, fp);

    return ret;

}

bool DigitRecognizer::train(char *trainPath, char *labelsPath)
{
	//trainPath: path at which the training images file is present
    FILE *fp = fopen(trainPath, "rb");

    // labelsPath: path at which the traning image labels are present 
    FILE *fp2 = fopen(labelsPath, "rb");

    if(!fp || !fp2)

        return false;

    // Read BYTEs in flipped order
    int magicNumber = readFlippedInteger(fp);
    numImages = readFlippedInteger(fp);
    numRows = readFlippedInteger(fp);

    numCols = readFlippedInteger(fp);

    fseek(fp2, 0x08, SEEK_SET);


    if(numImages > MAX_NUM_IMAGES) numImages = MAX_NUM_IMAGES;

    //////////////////////////////////////////////////////////////////
    // Go through each training data entry and save a

    // label for each digit

    int size = numRows*numCols; // size of each image

    // trainingVectors and trainingClasses have the shape (numImages, size)
    Mat trainingVectors = Mat(numImages, size, CV_32FC1);

    Mat trainingClasses = Mat(numImages, 1, CV_32FC1);

    BYTE *temp = new BYTE[size];
    BYTE tempClass=0;
    for(int i=0;i<numImages;i++)
    {

        fread((void*)temp, size, 1, fp);

        fread((void*)(&tempClass), sizeof(BYTE), 1, fp2);

	    const auto val = tempClass.to_ulong();
	    float result;
	    memcpy(&result, &val, sizeof(float));
        trainingClasses.at<float>(i) = result;

        for(int k=0;k<size;k++)
        {
        	const auto val = temp[k].to_ulong();
		    float result;
		    memcpy(&result, &val, sizeof(float));
            trainingVectors.at<float>(i,k) = result; ///sumofsquares;
        }

    }
    // filling up trainingVectors and trainingClasses with actual data from the MNIST files. 

    knn->train(_InputArray(trainingVectors), ROW_SAMPLE, _InputArray(trainingClasses));
    fclose(fp);

    fclose(fp2);

    return true;
}

int DigitRecognizer::classify(cv::Mat img)
{
    Mat cloneImg = preprocessImage(img);
    return knn->predict(_InputArray(cloneImg));
}

Mat DigitRecognizer::preprocessImage(Mat img)
{

    int rowTop=-1, rowBottom=-1, colLeft=-1, colRight=-1;

    Mat temp;
    int thresholdBottom = 50;
    int thresholdTop = 50;
    int thresholdLeft = 50;
    int thresholdRight = 50;
    int center = img.rows/2;
    for(int i=center;i<img.rows;i++)
    {
        if(rowBottom==-1)
        {
            temp = img.row(i);
            IplImage stub = temp;
            if(cvSum(&stub).val[0] < thresholdBottom || i==img.rows-1)
                rowBottom = i;

        }

        if(rowTop==-1)
        {
            temp = img.row(img.rows-i);
            IplImage stub = temp;
            if(cvSum(&stub).val[0] < thresholdTop || i==img.rows-1)
                rowTop = img.rows-i;

        }

        if(colRight==-1)
        {
            temp = img.col(i);
            IplImage stub = temp;
            if(cvSum(&stub).val[0] < thresholdRight|| i==img.cols-1)
                colRight = i;

        }

        if(colLeft==-1)
        {
            temp = img.col(img.cols-i);
            IplImage stub = temp;
            if(cvSum(&stub).val[0] < thresholdLeft|| i==img.cols-1)
                colLeft = img.cols-i;
        }
    }

    Mat newImg;

    newImg = newImg.zeros(img.rows, img.cols, CV_8UC1);

    int startAtX = (newImg.cols/2)-(colRight-colLeft)/2;

    int startAtY = (newImg.rows/2)-(rowBottom-rowTop)/2;

    for(int y=startAtY;y<(newImg.rows/2)+(rowBottom-rowTop)/2;y++)
    {
        uchar *ptr = newImg.ptr<uchar>(y);
        for(int x=startAtX;x<(newImg.cols/2)+(colRight-colLeft)/2;x++)
        {
            ptr[x] = img.at<uchar>(rowTop+(y-startAtY),colLeft+(x-startAtX));
        }
    }

    Mat cloneImg = Mat(numRows, numCols, CV_8UC1);

    resize(newImg, cloneImg, Size(numCols, numRows));

    // Now fill along the borders
    for(int i=0;i<cloneImg.rows;i++)
    {
        floodFill(cloneImg, cvPoint(0, i), cvScalar(0,0,0));

        floodFill(cloneImg, cvPoint(cloneImg.cols-1, i), cvScalar(0,0,0));

        floodFill(cloneImg, cvPoint(i, 0), cvScalar(0));
        floodFill(cloneImg, cvPoint(i, cloneImg.rows-1), cvScalar(0));
    }

    cloneImg = cloneImg.reshape(1, 1);

    return cloneImg;
}
