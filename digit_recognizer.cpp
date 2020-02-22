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

void computeHOGs( const Size wsize, const vector< Mat > & img_lst, vector< Mat > & gradient_lst, bool use_flip )
{
    Size blockSize = Size(14,14);
    Size blockStride = Size(7,7);
    Size cellSize = Size(14,14);
    int nbins = 9;

    HOGDescriptor hog(wsize, blockSize, blockStride, cellSize, nbins);

    Mat gray;
    vector< float > descriptors;

    for( size_t i = 0 ; i < img_lst.size(); i++ )
    {
        if ( img_lst[i].cols >= wsize.width && img_lst[i].rows >= wsize.height )
        {
            // Rect r = Rect(( img_lst[i].cols - wsize.width ) / 2,
            //               ( img_lst[i].rows - wsize.height ) / 2,
            //               wsize.width,
            //               wsize.height);
            // //cvtColor( img_lst[i](r), gray, COLOR_BGR2GRAY );
            // Mat gray = img_lst[i](r);

            // cvNamedWindow("gray");
            // imshow("gray",  img_lst[i]);
            // waitKey(0);

            hog.compute( img_lst[i], descriptors, Size( 7, 7 ), Size( 0, 0 ) );
            gradient_lst.push_back( Mat( descriptors ).clone() );
            //cout<<descriptors.size()<<endl;
            if ( use_flip )
            {
                flip( img_lst[i], img_lst[i], 1 );
                hog.compute( img_lst[i], descriptors, Size( 7, 7 ), Size( 0, 0 ) );
                gradient_lst.push_back( Mat( descriptors ).clone() );
            }
        }
    }
}

void convert_to_ml( const vector< Mat > & train_samples, Mat& trainData )
{
    //--Convert data
    const int rows = (int)train_samples.size();
    const int cols = (int)std::max( train_samples[0].cols, train_samples[0].rows );
    Mat tmp( 1, cols, CV_32FC1 ); //< used for transposition if needed
    trainData = Mat( rows, cols, CV_32FC1 );

    for( size_t i = 0 ; i < train_samples.size(); ++i )
    {
        CV_Assert( train_samples[i].cols == 1 || train_samples[i].rows == 1 );

        if( train_samples[i].cols == 1 )
        {
            transpose( train_samples[i], tmp );
            tmp.copyTo( trainData.row( (int)i ) );
        }
        else if( train_samples[i].rows == 1 )
        {
            train_samples[i].copyTo( trainData.row( (int)i ) );
        }
    }
}


int DigitRecognizer::loadMNIST(char* csv_filename, Mat& training_data, Mat& label_data) 
{
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

    //training_data = Mat(training_classes.size(), training_vectors[0].size(), CV_8U);
    label_data =  Mat(training_classes.size(), 1, CV_8U);

    

    // for(int i=0;i<training_data.rows;i++)
    // {
    //  for(int j=0;j<training_data.cols;j++)
    //  {
    //      training_data.at<uchar>(i, j) = (uchar)training_vectors[i][j];
    //  }
    // }

    for(int i=0;i<label_data.rows;i++)
    {
        label_data.at<uchar>(i, 0) = (uchar)training_classes[i];
    }

    vector<Mat> list_of_images;

    for(int k=0;k<training_vectors.size();k++)
    {
        int count = 0;
        Mat one_cell = Mat(28, 28, CV_8U);
        for(int i=0;i<28;i++)
        {
            for(int j=0;j<28;j++)
            {
                one_cell.at<uchar>(i, j) = (uchar)training_vectors[k][count++];
            }
        }
        list_of_images.push_back(one_cell);

        // if(k==2)
        // {
        //     cvNamedWindow("Onecell");
        //     imshow("Onecell", one_cell);
        //     waitKey(0);
        // }
    }

    vector<Mat> gradient_lst;
    computeHOGs(Size(28, 28), list_of_images, gradient_lst, false);

    convert_to_ml(gradient_lst, training_data);

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
        Mat descriptor;
        vector<Mat> list_of_images;
        list_of_images.push_back(cloneImg);

        vector<Mat> gradient_lst;
        computeHOGs(Size(28, 28), list_of_images, gradient_lst, false);
        convert_to_ml(gradient_lst, descriptor);

        Mat result_mat;
        int response = knn->findNearest(descriptor, knn->getDefaultK(), result_mat);
        return response;
    }
    else
        return 0;
}

Mat DigitRecognizer::preprocessImage(Mat img)
{

	// namedWindow("img", CV_WINDOW_AUTOSIZE);
	// imshow("img", img);
	// waitKey(0);
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

 //    namedWindow("cloneImg1", CV_WINDOW_AUTOSIZE);
	// imshow("cloneImg1", cloneImg);
	// waitKey(0);

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
    //cloneImg.convertTo(cloneImg, CV_32FC1);
    //cloneImg = cloneImg.reshape(0, 1);

    //cout<<"DigitRecognizer preproccessed!"<<endl;

    return cloneImg;
}
