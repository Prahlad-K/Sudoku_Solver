#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define PIXELS_IN_IMAGE 28*28

#define ENABLE_TRAIN 1

int reverseInt(int i);
int loadMNIST(const string pic_filename, Mat& training_data, Mat& label_data);

int main(int argc, char* argv[])
{
	try
	{
		Mat train_data_mat, train_label_mat;
		Mat test_data_mat, test_label_mat;
		
		loadMNIST("/home/prahlad/MNIST/mnist_train.csv", train_data_mat, train_label_mat);
		loadMNIST("/home/prahlad/MNIST/mnist_test.csv", test_data_mat, test_label_mat);
		
		train_data_mat.convertTo(train_data_mat, CV_32FC1);
		train_label_mat.convertTo(train_label_mat, CV_32SC1);
		test_data_mat.convertTo(test_data_mat, CV_32FC1);

		Ptr<ml::KNearest> knn;
		knn = ml::KNearest::create();
		knn->setDefaultK(10);
		knn->train(train_data_mat, ml::SampleTypes::ROW_SAMPLE, train_label_mat);

		cout<<knn->getDefaultK()<<endl;

		int correct_count = 0;
		for (int idx = 0; idx < test_label_mat.rows; idx++) {
			Mat result_mat;
			Mat temp = test_data_mat.row(idx);
			float response = knn->findNearest(temp, knn->getDefaultK(), result_mat);
			if(test_label_mat.at<uchar>(idx, 0) == (uchar)response) {

				correct_count++;
				if(correct_count%1000==0)
					cout<<correct_count<<endl;
			}
		}

		double correct_ratio = (double)correct_count / (double)test_label_mat.rows;
		cout << correct_ratio << endl;
		
	}
	catch (const Exception& ex)
	{
		cout << "Error: " << ex.what() << endl;
	}


	return 0;
}

int reverseInt(int i) {
	unsigned char ch1, ch2, ch3, ch4;
	ch1 = i & 255;
	ch2 = (i >> 8) & 255;
	ch3 = (i >> 16) & 255;
	ch4 = (i >> 24) & 255;

	return ((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}

int loadMNIST(const string csv_filename, Mat& training_data, Mat& label_data) {

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

    cvNamedWindow("Onecell");
    imshow("Onecell", one_cell);
    waitKey(0);

    for(int i=0;i<label_data.rows;i++)
    {
    	label_data.at<uchar>(i, 0) = (uchar)training_classes[i];
    }

    return 1;
}