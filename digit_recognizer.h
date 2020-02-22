#include <cv.h>
#include <highgui.h>
#include<bits/stdc++.h>
#include <ml.h>

using namespace cv;
using namespace ml;
#define MAX_NUM_IMAGES    60000

class DigitRecognizer
{
public:
	//constructer
	DigitRecognizer();
	//destructer
	~DigitRecognizer();
	bool train(char* path);
	int classify(Mat img);
private:
	Mat preprocessImage(Mat img);
	int readFlippedInteger(int i);
	int loadMNIST(char* csv_filename, Mat& training_data, Mat& label_data);
private:
	Ptr<KNearest> knn;
	int numRows, numCols, numImages;
};