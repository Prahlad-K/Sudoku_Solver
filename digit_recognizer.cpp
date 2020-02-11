#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <ml.h>
#include <cerrno>

#include "digit_recognizer.h"
using namespace cv;
using namespace ml;
using namespace std;
#define MAX_NUM_IMAGES    60000

typedef unsigned char BYTE;

DigitRecognizer::DigitRecognizer()
{
    knn = KNearest::create();
}

DigitRecognizer::~DigitRecognizer()
{
    delete knn;
}

int DigitRecognizer::readFlippedInteger(int i)
{
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;

}

bool DigitRecognizer::train(char *trainPath, char *labelsPath)
{

	ifstream f1 (trainPath);
	ifstream f2 (labelsPath);
    if (f1.is_open())
    {
        int magic_number=0;
        int number_of_images=0;
        int n_rows=0;
        int n_cols=0;
        f1.read((char*)&magic_number,sizeof(magic_number)); 
        magic_number= readFlippedInteger(magic_number);
        f1.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= readFlippedInteger(number_of_images);
        f1.read((char*)&n_rows,sizeof(n_rows));
        n_rows= readFlippedInteger(n_rows);
        f1.read((char*)&n_cols,sizeof(n_cols));
        n_cols= readFlippedInteger(n_cols);
        for(int i=0;i<number_of_images;++i)
        {
            for(int r=0;r<n_rows;++r)
            {
                for(int c=0;c<n_cols;++c)
                {
                    unsigned char temp=0;
                    f1.read((char*)&temp,sizeof(temp));

                }
            }
        }
        numImages = number_of_images;
    	numRows = n_rows;
    	numCols = n_cols;

    	f2.read((char*)&magic_number,sizeof(magic_number)); 
        magic_number= readFlippedInteger(magic_number);
        f2.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= readFlippedInteger(number_of_images);
        cout<<number_of_images<<endl;

    }

    //cout<<numImages<<" "<<numRows<<" "<<numCols<<endl;
    if(numImages > MAX_NUM_IMAGES) numImages = MAX_NUM_IMAGES;

    //////////////////////////////////////////////////////////////////
    // Go through each training data entry and save a

    // label for each digit

    int size = numRows*numCols; // size of each image

    //cout<<numRows<<" "<<numCols<<endl;
    // trainingVectors and trainingClasses have the shape (numImages, size)

    Mat trainingVectors = Mat(numImages, size, CV_32F);

    Mat trainingClasses = Mat(numImages, 1, CV_32F);


    BYTE *temp = new BYTE[size];
    BYTE tempClass=0;
    for(int i=0;i<numImages;i++)
    {

        f1.read((char*)temp, size);

        f2.read((char*)(&tempClass), 1);

	    const auto val = tempClass;
	    float result;
	    memcpy(&result, &val, sizeof(float));
	    //cout<<result<<endl;
        trainingClasses.at<float>(i) = result;

        for(int k=0;k<size;k++)
        {
        	const auto val = temp[k];
		    float result;
		    memcpy(&result, &val, sizeof(float));
            trainingVectors.at<float>(i,k) = result; ///sumofsquares;
        }

    }
    // filling up trainingVectors and trainingClasses with actual data from the MNIST files. 

    knn->train(_InputArray(trainingVectors), ROW_SAMPLE, _InputArray(trainingClasses));
    f1.close();
    f2.close();


	//cout<<"DigitRecognizer trained!"<<endl;

    return true;
}

int DigitRecognizer::classify(cv::Mat img)
{
    Mat cloneImg = preprocessImage(img);
    //cout<<"DigitRecognizer classified!"<<endl;
    //int rows = cloneImg.rows;
	//int cols = cloneImg.cols;
	//cout<<rows<<" "<<cols<<endl;
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

    newImg = newImg.zeros(img.rows, img.cols, CV_32F);

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

    Mat cloneImg = Mat(numRows, numCols, CV_32F);

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

    //cout<<"DigitRecognizer preproccessed!"<<endl;

    return cloneImg;
}
