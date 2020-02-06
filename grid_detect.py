import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt


sudoku = cv.imread('sudoku.jpg', 0)
outerBox =  cv.CreateMat(sudoku.size(), CV_8UC1)