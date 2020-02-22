#include <bits/stdc++.h>

using namespace std;

int main()
{
    fstream fin; 
  
    // Open an existing file 
    fin.open("./MNIST/mnist_train.csv", ios::in); 
  
    // Read the Data from the file 
    // as String Vector 
    vector<vector<int>> training_vectors;
    vector<int> training_classes; 
    string line, word, temp; 
  
    while (fin >> temp) { 

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
} 