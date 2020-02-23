

# Sudoku_Solver
A simple image processing application that uses OpenCV and C++ to solve any given 10x10 sudoku puzzles, by reading images of them taken from a newspaper!

## Input
An arbitrary sized image of any newspaper sudoku puzzle. 

## Output - Aim
The solved sudoku, displayed to the user. 

## Assumptions
The input image:
 1.  Primarily contains the sudoku (i.e. is focused on the sudoku)
 2.  Is clear, and the grid lines of the sudoku are visible and distinct from background
 3.  Contains only one sudoku that isn't folded (but angle of capture can vary)
 4.  Doesn't have shadows
 5.  Doesn't have complicated noise (i.e. apart from noise that can be removed by smoothing)
 6.  Contains an unsolved sudoku
 7.  Is captured in atleast dimly lit environment

## Major Steps Involved

#### Getting the Sudoku
We first remove the unnecessary detail of the given newspaper image, by cropping out only the sudoku. 
 1.  Gaussian blurring
 2.  Adaptive thresholding and inverting
 3.  Dilation (using a plus shaped kernel) to connect broken components
 4.  Flood filling for identifying the largest blob (the main sudoku grid)
 5.  Obtaining the coordinates of the grid by detecting lines using Hough transforms.
 6.  Warping perspective of the original image using the above points to focus on the sudoku

#### Extracting digits from the sudoku
We now split the sudoku into it's individual "cells", where a cell would be the smallest grid containing a number in the sudoku. Thus, we expect there to be 100 such cells according to our consideration. 

 1. On the warped image, we apply adaptive thresholding.
 2. We now obtain an image containing all horizontal grid lines only, and similarly an image containing all vertical grid lines only. 
 3. We add the above two images to get a mask. 
 4. All the grid lines are eliminated from the image by subtracting it with the above mask.
 5. We now iterate through each cell and crop it by cell size to obtain either a blank image, or an image containing a digit.

#### Classifying digits obtained
Each cell image would need to be classified to get the number present within the cell, and 0 otherwise (a sudoku cell varies from 1 to 9, and thus 0 is chosen to represent no digit.)

 1. We center each of the cell images, by calculating the bound lines where the digit starts and ends.
 2. Then, we crop and resize the cell image to 28x28 pixel size.
 3. A decision is made whether the image contains a number or not by calculating the area of moments. If above a threshold, we assume a number is present. Else, we return a zero for that cell. 
 4. We now use the K-Nearest Neighbours algorithm on the MNIST dataset that is trained, to predict the number in the cell image, and return it. 

#### Solving the sudoku
A straightforward backtracking based algorithm is implemented that can solve any solvable sudoku. 

## Executing this application
#### Requirements:

    OpenCV>=3.4.0
    gcc version 7.4.0
    MNIST database in .csv files
#### Running the application:

	> git clone .
    > cd Sudoku_Solver
    Edit paths to the MNIST database in sudoku_former.cpp
    > make
    > ./sudoku_former sudoku.jpg
    Press <Enter> upon viewing the Undistorted image
    Paste the reference sudoku present in Reference_Sudokus.txt

## Progress So Far
The accuracy of sudoku formation is only at 92.6 %.
Accuracy was calculated by the total number of correct grid cells classified.  

For example, the digit "1" is being classified as digit "7". Or the digit "9" as "0", and so on. 
To solve a sudoku, we would require a 100 % accurate sudoku formation, as even one misclassified digit could make the sudoku unsolvable. 

Possible reasons behind this issue: KNN was trained on MNIST database of handwritten digits, but it's used on printed digits.
Another reason could be the accuracy of the KNN itself - which is around 96.65 % (for a test set of 10,000 images).

## References
[AI Shack| Sudoku Grabber using OpenCV](https://aishack.in/tutorials/sudoku-grabber-opencv-plot/) 

[MNIST database in csv](https://pjreddie.com/projects/mnist-in-csv/)
