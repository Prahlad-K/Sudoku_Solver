#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <ml.h>
#include <cerrno>
#include <string>

#include "digit_recognizer.h"
using namespace cv;
using namespace ml;
using namespace std;
#define MAX_NUM_IMAGES    60000
#define PIXELS_IN_IMAGE 28*28

#define ENABLE_TRAIN 1

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

int DigitRecognizer::loadMNIST(char* csv_filename, Mat& training_data, Mat& label_data) {

	fstream fin; 
  
    // Open an existing file 
    fin.open(csv_filename, ios::in); 
  
    // Read the Data from the file 
    // as String Vector 
    vector<vector<int>> training_vectors;
    vector<int> training_classes; 
    string line, word, temp; 
  
    while (fin >> temp) 
    { 

        vector<int> training_vector;
        int training_class;

        stringstream s(temp); 
  
        // read every column data of a row and 
        // store it in a string variable, 'word' 
        int count = 0;
        while (getline(s, word, ',')) { 
  
            // add all the column data 
            // of a row to a vector 
            if(count ==0)
            {
                training_class = stoi(word);
                count++;
            }
            else
            {
                training_vector.push_back(stoi(word)); 
            }
        }

        training_vectors.push_back(training_vector);
        training_classes.push_back(training_class);
    }

    cout<<training_classes.size()<<" "<<training_vectors[0].size()<<endl;

    training_data = Mat(training_classes.size(), training_vectors[0].size(), CV_8U);
    label_data =  Mat(training_classes.size(), 1, CV_8U);

    Mat one_cell = Mat(28, 28, CV_8U);

    for(int i=0;i<training_data.rows;i++)
    {
    	for(int j=0;j<training_data.cols;j++)
    	{
    		training_data.at<uchar>(i, j) = (uchar)training_vectors[i][j];
    	}
    }

    int count = 0;
    for(int i=0;i<28;i++)
    {
    	for(int j=0;j<28;j++)
    	{
    		one_cell.at<uchar>(i, j) = (uchar)training_vectors[0][count++];
    	}
    }

    // cvNamedWindow("Onecell");
    // imshow("Onecell", one_cell);
    // waitKey(0);

    for(int i=0;i<label_data.rows;i++)
    {
    	label_data.at<uchar>(i, 0) = (uchar)training_classes[i];
    }

    return 1;
}

bool DigitRecognizer::train(char *path)
{
	try
	{
		Mat train_data_mat, train_label_mat;
		
		loadMNIST(path, train_data_mat, train_label_mat);
		
		train_data_mat.convertTo(train_data_mat, CV_32FC1);
		train_label_mat.convertTo(train_label_mat, CV_32SC1);

		numImages = train_data_mat.rows;
		numRows = 28;
		numCols = 28;

		knn->setDefaultK(10);
		knn->train(train_data_mat, ml::SampleTypes::ROW_SAMPLE, train_label_mat);
		cout<<"Training done!\n";
	}
	catch (const Exception& ex)
	{
		cout << "Error: " << ex.what() << endl;
	}
    return true;
}

int DigitRecognizer::classify(cv::Mat img)
{    
    Mat cloneImg = preprocessImage(img);

    Moments m = cv::moments(cloneImg, true);
    int area = m.m00;
    if(area > 0)
    {
        Mat result_mat;
        int response = knn->findNearest(cloneImg, knn->getDefaultK(), result_mat);
        return response;
    }
    else
        return 0;
}

Mat DigitRecognizer::preprocessImage(Mat img)
{

	namedWindow("img", CV_WINDOW_AUTOSIZE);
	imshow("img", img);
	waitKey(0);
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

    namedWindow("cloneImg1", CV_WINDOW_AUTOSIZE);
	imshow("cloneImg1", cloneImg);
	waitKey(0);

    // Now fill along the borders
    for(int i=0;i<cloneImg.rows;i++)
    {
        floodFill(cloneImg, cvPoint(0, i), cvScalar(0,0,0));

        floodFill(cloneImg, cvPoint(cloneImg.cols-1, i), cvScalar(0,0,0));

        floodFill(cloneImg, cvPoint(i, 0), cvScalar(0));
        floodFill(cloneImg, cvPoint(i, cloneImg.rows-1), cvScalar(0));
    }

 //    namedWindow("cloneImg", CV_WINDOW_AUTOSIZE);
	// imshow("cloneImg", cloneImg);
	// waitKey(0);
    cloneImg.convertTo(cloneImg, CV_32FC1);
    cloneImg = cloneImg.reshape(0, 1);

	


    //cout<<"DigitRecognizer preproccessed!"<<endl;

    return cloneImg;
}
