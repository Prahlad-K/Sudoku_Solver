#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define PIXELS_IN_IMAGE 28*28

#define ENABLE_TRAIN 1

int reverseInt(int i);
int loadMNIST(const string pic_filename, Mat& training_data, Mat& label_data);

// void computeHOGs( const Size wsize, const vector< Mat > & img_lst, vector< Mat > & gradient_lst, bool use_flip )
// {
//     Size blockSize = Size(14,14);
//     Size blockStride = Size(7,7);
//     Size cellSize = Size(14,14);
//     int nbins = 9;

//     HOGDescriptor hog(wsize, blockSize, blockStride, cellSize, nbins);

//     Mat gray;
//     vector< float > descriptors;

//     for( size_t i = 0 ; i < img_lst.size(); i++ )
//     {
//         if ( img_lst[i].cols >= wsize.width && img_lst[i].rows >= wsize.height )
//         {
//             // Rect r = Rect(( img_lst[i].cols - wsize.width ) / 2,
//             //               ( img_lst[i].rows - wsize.height ) / 2,
//             //               wsize.width,
//             //               wsize.height);
//             // //cvtColor( img_lst[i](r), gray, COLOR_BGR2GRAY );
//             // Mat gray = img_lst[i](r);

//             // cvNamedWindow("gray");
//             // imshow("gray",  img_lst[i]);
//             // waitKey(0);

//             hog.compute( img_lst[i], descriptors, Size( 7, 7 ), Size( 0, 0 ) );
//             gradient_lst.push_back( Mat( descriptors ).clone() );
//             //cout<<descriptors.size()<<endl;
//             if ( use_flip )
//             {
//                 flip( img_lst[i], img_lst[i], 1 );
//                 hog.compute( img_lst[i], descriptors, Size( 7, 7 ), Size( 0, 0 ) );
//                 gradient_lst.push_back( Mat( descriptors ).clone() );
//             }
//         }
//     }
// }

// void convert_to_ml( const vector< Mat > & train_samples, Mat& trainData )
// {
//     //--Convert data
//     const int rows = (int)train_samples.size();
//     const int cols = (int)std::max( train_samples[0].cols, train_samples[0].rows );
//     Mat tmp( 1, cols, CV_32FC1 ); //< used for transposition if needed
//     trainData = Mat( rows, cols, CV_32FC1 );

//     for( size_t i = 0 ; i < train_samples.size(); ++i )
//     {
//         CV_Assert( train_samples[i].cols == 1 || train_samples[i].rows == 1 );

//         if( train_samples[i].cols == 1 )
//         {
//             transpose( train_samples[i], tmp );
//             tmp.copyTo( trainData.row( (int)i ) );
//         }
//         else if( train_samples[i].rows == 1 )
//         {
//             train_samples[i].copyTo( trainData.row( (int)i ) );
//         }
//     }
// }


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

    

    for(int i=0;i<training_data.rows;i++)
    {
    	for(int j=0;j<training_data.cols;j++)
    	{
    		training_data.at<uchar>(i, j) = (uchar)training_vectors[i][j];
    	}
    }

    for(int i=0;i<label_data.rows;i++)
    {
        label_data.at<uchar>(i, 0) = (uchar)training_classes[i];
    }

    // vector<Mat> list_of_images;

    // for(int k=0;k<training_vectors.size();k++)
    // {
    //     int count = 0;
    //     Mat one_cell = Mat(28, 28, CV_8U);
    //     for(int i=0;i<28;i++)
    //     {
    //     	for(int j=0;j<28;j++)
    //     	{
    //     		one_cell.at<uchar>(i, j) = (uchar)training_vectors[k][count++];
    //     	}
    //     }
    //     list_of_images.push_back(one_cell);

    //     // if(k==1)
    //     // {
    //     //     cvNamedWindow("Onecell");
    //     //     imshow("Onecell", one_cell);
    //     //     waitKey(0);
    //     // }
    // }

    // vector<Mat> gradient_lst;
    // computeHOGs(Size(28, 28), list_of_images, gradient_lst, false);

    // convert_to_ml(gradient_lst, training_data);

    return 1;

}

/*

Accuracy of the KNN is 96.83 percent with HOG descriptors. 
Accuracy of the KNN is 96.65 percent without HOG descriptors. 

*/