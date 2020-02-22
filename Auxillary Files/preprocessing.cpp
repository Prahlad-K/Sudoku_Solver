#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <vector>

#include "digit_recognizer.cpp"

using namespace cv;
using namespace std;

Mat removeBorders(Mat image)
{
	Mat inv;
	bitwise_not(image, inv);

	Mat horizontal_image = image.clone();
	Mat vertical_image = image.clone();

	int scale = 10;

	int horizontal_size = horizontal_image.cols/scale;
	int vertical_size = vertical_image.rows/scale;

	Mat element;
	element = getStructuringElement(MORPH_RECT, Size(horizontal_size, 1), Point(-1,-1) );
	erode(horizontal_image, horizontal_image, element);
	dilate(horizontal_image, horizontal_image, element);

	element = getStructuringElement(MORPH_RECT, Size(1, vertical_size), Point(-1,-1) );
	erode(vertical_image, vertical_image, element);
	dilate(vertical_image, vertical_image, element);

	Mat mask_img;
	add(horizontal_image, vertical_image, mask_img);

	Mat result;
//	bitwise_or(image, mask_img, result);
	subtract(image, mask_img, result);

	// element = getStructuringElement(MORPH_RECT, Size(2, 2), Point(-1,-1) );
	// erode(result, result, element);

	namedWindow("result", CV_WINDOW_AUTOSIZE);
	imshow("result", result);
	waitKey(0);

	return result;

}

int main()
{
	Mat full_image= imread("undistorted_th_sudoku.jpg", 0);

	namedWindow("full_image", CV_WINDOW_AUTOSIZE);
	imshow("full_image", full_image);
	waitKey(0);

	removeBorders(full_image);

	int dist = ceil((double)full_image.rows/9);
    Mat currentCell = Mat(dist, dist, CV_8UC1);

    for(int j=0;j<9;j++)
    {
        for(int i=0;i<9;i++)
        {
            for(int y=0;y<dist && j*dist+y<full_image.cols;y++)
            {

                uchar* ptr = currentCell.ptr(y);

                for(int x=0;x<dist && i*dist+x<full_image.rows;x++)
                {
                    ptr[x] = full_image.at<uchar>(j*dist+y, i*dist+x);
                }
            }            

            namedWindow("currentCell", CV_WINDOW_AUTOSIZE);
			imshow("currentCell", currentCell);
			waitKey(0);

			currentCell = removeBorders(currentCell);
        }
    }

    return 0;

}