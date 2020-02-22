# Sudoku_Solver
A simple image processing application that uses OpenCV and C++ to solve any given 10x10 sudoku puzzles, by reading images of them taken from a newspaper!

## Input
An arbitrary sized image of any newspaper sudoku puzzle. 

## Output - Aim
The solved sudoku, displayed to the user 

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

## Progress So Far
The accuracy of sudoku formation is only at 92.3 %. 
To solve a sudoku, we would require a 100 % accurate sudoku formation! 

