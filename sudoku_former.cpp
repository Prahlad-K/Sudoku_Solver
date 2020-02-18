#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <vector>

#include "digit_recognizer.cpp"
#include "sudoku_solver.cpp"

using namespace cv;
using namespace std;

void drawLine(Vec2f line, Mat &img, Scalar rgb = CV_RGB(0, 0, 255))
{
	// rgb color value is white
	if(line[1]!=0)
	{
		// non-zero angle
		// calculate the slope and intercept from the radius and angle
		float m = -1/tan(line[1]);
		float c = line[0]/sin(line[1]);

		// draw line based on above values onto the image
		// connects p1 (from y-intercept) to p2 (last point of the line in the image)
		cv::line(img, Point(0, c), Point(img.size().width, m*img.size().width+c), rgb);
	}
	else
    {
    	// if angle is 0
    	// vertical line - special case
        cv::line(img, Point(line[0], 0), Point(line[0], img.size().height), rgb);
    }
}

Mat removeBorders(Mat image)
{
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
//  bitwise_or(image, mask_img, result);
    subtract(image, mask_img, result);

    element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1,-1) );
    erode(result, result, element);
    dilate(result, result, element);

    element = getStructuringElement(MORPH_RECT, Size(2, 2), Point(-1,-1) );
    erode(result, result, element);

    //blur(result, result, Size(3, 3), Point(-1,-1));
    //GaussianBlur(result, result, Size(3, 3),0,0);

    // namedWindow("result", CV_WINDOW_AUTOSIZE);
    // imshow("result", result);
    // waitKey(0);

    return result;

}

void mergeRelatedLines(vector<Vec2f>*lines, Mat &img)
{
	vector<Vec2f>::iterator current;
	for(current=lines->begin(); current!=lines->end();current++)
	{
		// iterate through all lines, "fuse" (average) them together.
		// to distinguish already fused lines, use radius = 0, angle = -100 (impossible values)
		if((*current)[0]==0 && (*current)[1]==-100) continue;

		float p1 = (*current)[0];
        float theta1 = (*current)[1];

        //using these two values, find two points on this line

        Point pt1current, pt2current;
        if(theta1>CV_PI*45/180 && theta1<CV_PI*135/180)
        {
        	// line is approx horizontal 
        	// pt1current is the left extreme
            pt1current.x=0;
            pt1current.y = p1/sin(theta1);

            // pt2current is the right extreme
            pt2current.x=img.size().width;
            pt2current.y=-pt2current.x/tan(theta1) + p1/sin(theta1);
        }
        else
        {
        	// line is approx vertical
        	// pt1current is top extreme
            pt1current.y=0;
            pt1current.x=p1/cos(theta1);

            // pt2current is bottom extreme
            pt2current.y=img.size().height;
            pt2current.x=-pt2current.y/tan(theta1) + p1/cos(theta1);
        }

        vector<Vec2f>::iterator pos;
        for(pos=lines->begin();pos!=lines->end();pos++)
        {
        	// ignore if we come across the currently considered line
            if(*current==*pos) continue;

            // check if this line is close enough to our considered line
            if(fabs((*pos)[0]-(*current)[0])<20 && fabs((*pos)[1]-(*current)[1])<CV_PI*10/180)
            {
                float p = (*pos)[0];
                float theta = (*pos)[1];

                // calculate the extreme points for this line as well
                Point pt1, pt2;
                if((*pos)[1]>CV_PI*45/180 && (*pos)[1]<CV_PI*135/180)
                {
                    pt1.x=0;
                    pt1.y = p/sin(theta);
                    pt2.x=img.size().width;
                    pt2.y=-pt2.x/tan(theta) + p/sin(theta);
                }
                else
                {
                    pt1.y=0;
                    pt1.x=p/cos(theta);
                    pt2.y=img.size().height;
                    pt2.x=-pt2.y/tan(theta) + p/cos(theta);
                }

                // merge both lines if the two points are close enough  
                if(((double)(pt1.x-pt1current.x)*(pt1.x-pt1current.x) + (pt1.y-pt1current.y)*(pt1.y-pt1current.y)<64*64) &&
((double)(pt2.x-pt2current.x)*(pt2.x-pt2current.x) + (pt2.y-pt2current.y)*(pt2.y-pt2current.y)<64*64))
                {
                    (*current)[0] = ((*current)[0]+(*pos)[0])/2;

                    (*current)[1] = ((*current)[1]+(*pos)[1])/2;

                    (*pos)[0]=0;
                    (*pos)[1]=-100;
                }
            }
        }
	}
}

int main(int argc, char** argv)
{
	//reading and storing the sudoku image
	//assuming the sudoku grid forms the major part of the image

    if(argc<2)
    {    
        cout<<"Format: ./sudoku_former <image_path>"<<endl;
        exit(0);
    }

    char* path = argv[1];

	Mat sudoku = imread(path, 0);
	//0 for grayscale 

	//create a same sized empty image container
	Mat outerBox = Mat(sudoku.size(), CV_8UC1);

    // apply a Gaussian Blur to this sudoku 
	GaussianBlur(sudoku, sudoku, Size(11, 11), 0);

	//Printing the blurred image
	/*namedWindow("Blur", CV_WINDOW_AUTOSIZE );
	imshow("Blur", sudoku);
	waitKey(0);*/

	//Performing adaptive thresholding that is independent of illumination levels
	adaptiveThreshold(sudoku, outerBox, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
	//calculates mean over a 5*5 window and subtracts 2 from this mean (threshold level for every pixel)

	bitwise_not(outerBox, outerBox);
	//invert the image cause we're interested in the borders, make em white!

	//namedWindow("Invert", CV_WINDOW_AUTOSIZE);
    //imshow("Invert", outerBox);

	//adaptive thresholding could have broken certain connected parts. 
	//let's dilate it to get back those lines

	Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
	//plus shaped kernel element 
    dilate(outerBox, outerBox, kernel);

    //namedWindow("Invert+Dilate", CV_WINDOW_AUTOSIZE);
    //imshow("Invert+Dilate", outerBox);
    //now, we must find the biggest blob

    int count = 0;
    int max = -1;

    Point maxPt;

    for(int y=0;y<outerBox.size().height;y++)
    {
    	uchar *row = outerBox.ptr(y);
    	for(int x=0;x<outerBox.size().width;x++)
    	{
    		//making sure the pixel is white
    		if(row[x]>=128)
    		{
    			//flood fill this blob with gray and check it's area
    			int area = floodFill(outerBox, Point(x,y), CV_RGB(0, 0, 64));
                // floodFill fills the area under the outerBox starting at point(x, y) 
                // along the lines of the edges it's a part of, with color grey (0, 0, 64)
    			if(area>max)
    			{
    				maxPt = Point(x, y);
    				max = area;
    				//remember maximum such blob
    			}

    		}
    	}
    }

    //basically only the blue one stands, because this is grayscale!
    floodFill(outerBox, maxPt, CV_RGB(0,0,255));
    //so the last 255 indicates full white
    //fills the largest blob white

    for(int y=0;y<outerBox.size().height;y++)
    {
        uchar *row = outerBox.ptr(y);
        for(int x=0;x<outerBox.size().width;x++)
        {
        	//as long as the points aren't maxPts(i.e. the large blob)
        	//and the value of pixel is gray
            if(row[x]==64 && x!=maxPt.x && y!=maxPt.y)
            {
                //flood fill other minor blobs to black, not required!
                int area = floodFill(outerBox, Point(x,y), CV_RGB(0,0,0));
            }
        }
    }

/*    cvNamedWindow("Isolate blob");
    imshow("Isolate blob", outerBox);
   	waitKey(0);*/

    //perform erosion after this, so that we "restore" the dilation done before
    erode(outerBox, outerBox, kernel);
    //imshow("thresholded", outerBox);

    //cvNamedWindow("Grid");
    //imshow("Grid", outerBox);
   	//waitKey(0);

   	// Let's detect the lines in the image!
    vector<Vec2f> lines;
    // Hough line transformations 
    // get all lines drawn from points on outerBox (contains edges only!) based on threshold on number of votes (200) and a 
   	// resolution on the radius and angle of the line
    HoughLines(outerBox, lines, 1, CV_PI/180, 200);

    // we merge some of these lines
    mergeRelatedLines(&lines, outerBox);

    // Now, we detect extreme lines
    // Set random initial values
    Vec2f topEdge = Vec2f(1000,1000);    double topYIntercept=100000, topXIntercept=0;
    Vec2f bottomEdge = Vec2f(-1000,-1000);        double bottomYIntercept=0, bottomXIntercept=0;
    Vec2f leftEdge = Vec2f(1000,1000);    double leftXIntercept=100000, leftYIntercept=0;
    Vec2f rightEdge = Vec2f(-1000,-1000);        double rightXIntercept=0, rightYIntercept=0;

    for(int i=0;i<lines.size();i++)
    {
    	Vec2f current = lines[i];
    	float p = current[0];
    	float theta = current[1];

    	// ignore if we encounter a merged line
    	if(p==0 && theta==-100)
    		continue;

    	double xIntercept, yIntercept;
        xIntercept = p/cos(theta);
        yIntercept = p/(cos(theta)*sin(theta));

        //if the line is nearly horizontal
        if(theta>CV_PI*80/180 && theta<CV_PI*100/180)
        {
            if(p<topEdge[0])
                topEdge = current;

            if(p>bottomEdge[0])
                bottomEdge = current;
        }
        //if the line is nearly vertical
        else if(theta<CV_PI*10/180 || theta>CV_PI*170/180)
        {
            if(xIntercept>rightXIntercept)
            {
                rightEdge = current;
                rightXIntercept = xIntercept;
            }
            else if(xIntercept<=leftXIntercept)
            {
                leftEdge = current;
                leftXIntercept = xIntercept;
            }
        }
        // ignore lines of any other slope
    } // done detecting extreme lines!

    // find two points on each of these lines
    Point left1, left2, right1, right2, bottom1, bottom2, top1, top2;

    int height=outerBox.size().height;
    int width=outerBox.size().width;

    if(leftEdge[1]!=0)
    {
    	// infinite slope?
        left1.x=0;        left1.y=leftEdge[0]/sin(leftEdge[1]);
        left2.x=width;    left2.y=-left2.x/tan(leftEdge[1]) + left1.y;
    }
    else
    {
        left1.y=0;        left1.x=leftEdge[0]/cos(leftEdge[1]);
        left2.y=height;    left2.x=left1.x - height*tan(leftEdge[1]);

    }

    if(rightEdge[1]!=0)
    {
    	// infinite slope?
        right1.x=0;        right1.y=rightEdge[0]/sin(rightEdge[1]);
        right2.x=width;    right2.y=-right2.x/tan(rightEdge[1]) + right1.y;
    }
    else
    {
        right1.y=0;        right1.x=rightEdge[0]/cos(rightEdge[1]);
        right2.y=height;    right2.x=right1.x - height*tan(rightEdge[1]);

    }

    // no issues with infinite slopes here!
    bottom1.x=0;    bottom1.y=bottomEdge[0]/sin(bottomEdge[1]);
    bottom2.x=width;bottom2.y=-bottom2.x/tan(bottomEdge[1]) + bottom1.y;

    top1.x=0;        top1.y=topEdge[0]/sin(topEdge[1]);
    top2.x=width;    top2.y=-top2.x/tan(topEdge[1]) + top1.y;

    // we now find the intersection of these four lines
	double leftA = left2.y-left1.y;
    double leftB = left1.x-left2.x;

    double leftC = leftA*left1.x + leftB*left1.y;

    double rightA = right2.y-right1.y;
    double rightB = right1.x-right2.x;

    double rightC = rightA*right1.x + rightB*right1.y;

    double topA = top2.y-top1.y;
    double topB = top1.x-top2.x;

    double topC = topA*top1.x + topB*top1.y;

    double bottomA = bottom2.y-bottom1.y;
    double bottomB = bottom1.x-bottom2.x;

    double bottomC = bottomA*bottom1.x + bottomB*bottom1.y;

    // Intersection of left and top
    double detTopLeft = leftA*topB - leftB*topA;
    CvPoint ptTopLeft = cvPoint((topB*leftC - leftB*topC)/detTopLeft, (leftA*topC - topA*leftC)/detTopLeft);

    // Intersection of top and right
    double detTopRight = rightA*topB - rightB*topA;
    CvPoint ptTopRight = cvPoint((topB*rightC-rightB*topC)/detTopRight, (rightA*topC-topA*rightC)/detTopRight);

    // Intersection of right and bottom
    double detBottomRight = rightA*bottomB - rightB*bottomA;
    CvPoint ptBottomRight = cvPoint((bottomB*rightC-rightB*bottomC)/detBottomRight, (rightA*bottomC-bottomA*rightC)/detBottomRight);

    // Intersection of bottom and left
    double detBottomLeft = leftA*bottomB-leftB*bottomA;
    CvPoint ptBottomLeft = cvPoint((bottomB*leftC-leftB*bottomC)/detBottomLeft, (leftA*bottomC-bottomA*leftC)/detBottomLeft);

    // we now have the points that can correct the skewed perspective!
    // the image will now be the square of the length of the longest edge

    int maxLength = (ptBottomLeft.x-ptBottomRight.x)*(ptBottomLeft.x-ptBottomRight.x) + (ptBottomLeft.y-ptBottomRight.y)*(ptBottomLeft.y-ptBottomRight.y);
    int temp = (ptTopRight.x-ptBottomRight.x)*(ptTopRight.x-ptBottomRight.x) + (ptTopRight.y-ptBottomRight.y)*(ptTopRight.y-ptBottomRight.y);

    if(temp>maxLength) maxLength = temp;

    temp = (ptTopRight.x-ptTopLeft.x)*(ptTopRight.x-ptTopLeft.x) + (ptTopRight.y-ptTopLeft.y)*(ptTopRight.y-ptTopLeft.y);

    if(temp>maxLength) maxLength = temp;

    temp = (ptBottomLeft.x-ptTopLeft.x)*(ptBottomLeft.x-ptTopLeft.x) + (ptBottomLeft.y-ptTopLeft.y)*(ptBottomLeft.y-ptTopLeft.y);

    if(temp>maxLength) maxLength = temp;

    maxLength = sqrt((double)maxLength);

    // now that we have the length of the unskewed version,
    // let's distort the current image into a squared unskewed version

	Point2f src[4], dst[4];
	// top left point becomes origin and so on
	src[0] = ptTopLeft;            dst[0] = Point2f(0,0);
	src[1] = ptTopRight;        dst[1] = Point2f(maxLength-1, 0);
	src[2] = ptBottomRight;        dst[2] = Point2f(maxLength-1, maxLength-1);
	src[3] = ptBottomLeft;        dst[3] = Point2f(0, maxLength-1);

    // let's read the original again to warp it using our above derived points
	Mat original = imread(path, 0);

	Mat undistorted = Mat(Size(maxLength, maxLength), CV_8UC1);
    // warping the original
	cv::warpPerspective(original, undistorted, cv::getPerspectiveTransform(src, dst), Size(maxLength, maxLength));	

    // saving the image
    imwrite("undistorted_sudoku.jpg", undistorted);

    Mat undistortedThreshed = undistorted.clone();
    adaptiveThreshold(undistorted, undistortedThreshed, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 101, 1);

    imwrite("undistorted_th_sudoku.jpg", undistortedThreshed);

    undistortedThreshed = removeBorders(undistortedThreshed);

    cvNamedWindow("Undistorted");
    imshow("Undistorted", undistortedThreshed);
    waitKey(0);

    DigitRecognizer *dr = new DigitRecognizer();
    bool b = dr->train("/home/prahlad/MNIST/mnist_train.csv");

    int dist = ceil((double)maxLength/9);

    Mat currentCell = Mat(dist, dist, CV_8UC1);

    vector<vector<int>> sudoku_;

    for(int j=0;j<9;j++)
    {
        vector<int> row;
        row.clear();
        for(int i=0;i<9;i++)
        {
            for(int y=0;y<dist && j*dist+y<undistortedThreshed.cols;y++)
            {

                uchar* ptr = currentCell.ptr(y);

                for(int x=0;x<dist && i*dist+x<undistortedThreshed.rows;x++)
                {
                    ptr[x] = undistortedThreshed.at<uchar>(j*dist+y, i*dist+x);
                }
            }            

         
            int number = dr->classify(currentCell);
            printf("%d ", number);
            row.push_back(number);
        }
        sudoku_.push_back(row);
        printf("\n");
    }

    // SudokuSolver s = SudokuSolver(sudoku_);
    // if(s.solveSudoku())
    //     vector<vector<int>> answer = s.printSudoku();
    // else
    //     cout<<"Could not solve!\n";

    cout<<"Enter the reference sudoku: \n";
    vector<vector<int>> ref_sudoku;
    for(int i=0;i<sudoku_.size();i++)
    {
        int element;
        vector<int> row;
        row.clear();
        for(int j=0;j<sudoku_.size();j++)
        {
            cin>>element;
            row.push_back(element);
        }
        ref_sudoku.push_back(row);
    }

    float accuracy=0;
    for(int i=0;i<sudoku_.size();i++)
    {
        for(int j=0;j<sudoku_.size();j++)
        {
            if(sudoku_[i][j]==ref_sudoku[i][j])
                accuracy++;
        }
    }
    accuracy =accuracy/(sudoku_.size() * sudoku_.size());

    cout<<"Accuracy of Sudoku formation: "<<accuracy<<endl;

    return 0;
}