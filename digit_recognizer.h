#include <cv.h>
#include <highgui.h>

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
	bool train(char* trainPath, char* labelsPath);
	int classify(Mat img);
private:
	Mat preprocessImage(Mat img);
	int readFlippedInteger(int i);
private:
	Ptr<KNearest> knn;
	int numRows, numCols, numImages;
};