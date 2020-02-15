#include <bits/stdc++.h>

using namespace std;


vector<int> getPossibleValues(vector<vector<int>> sudoku, int row, int col)
{
	set<int> other_values;

	int local_square_size = sqrt(sudoku.size());

	// adding surrounding values at the row
	for(int j=0;j<sudoku[row].size();j++)
	{
		if(sudoku[row][j]!=0)
			other_values.insert(sudoku[row][j]);
	}

	// adding surrounding values at the column
	for(int i=0;i<sudoku.size();i++)
	{
		if(sudoku[i][col]!=0)
			other_values.insert(sudoku[i][col]);
	}

	// adding surrounding local square values
	int start_row = (row/local_square_size) * local_square_size;
	int start_col = (col/local_square_size) * local_square_size;
	for(int i=start_row; i<start_row+2;i++)
	{
		for(int j=start_col; j<start_col+2;j++)
		{
			if(sudoku[i][j]!=0)
			other_values.insert(sudoku[i][j]);
		}
	}

	//now choosing the range of possible values
	vector<int> possible_values;
	for(int i=1;i<=sudoku.size();i++)
	{
		if(find(other_values.begin(), other_values.end(), i)==other_values.end())
		{
			possible_values.push_back(i);
		}
	}

	return possible_values;

}

bool isSudokuSolved(vector<vector<int>> sudoku)
{
	for(int i=0;i<sudoku.size();i++)
	{
		for(int j=0;j<sudoku[i].size();j++)
		{
			if(sudoku[i][j]==0)
				return false;	
		}
	}
	return true;
}
void printSudoku(vector<vector<int>>& sudoku)
{
	for(int i=0;i<sudoku.size();i++)
	{
		for(int j=0;j<sudoku.size();j++)
		{
			cout<<sudoku[i][j]<<" ";
		}
	cout<<endl;
	}
}	



bool solveSudoku(vector<vector<int>>& sudoku)
{

	if(isSudokuSolved(sudoku))
		return true;

	for(int i=0;i<sudoku.size();i++)
	{
		for(int j=0;j<sudoku[i].size();j++)
		{
			if(sudoku[i][j]==0)
			{
				vector<int> possible_values = getPossibleValues(sudoku, i, j);
				// cout<<i<<", "<<j<<endl;
				// for(int k=0;k<possible_values.size();k++)
				// {
				// 	cout<<possible_values[k]<<" ";
				// }
				// cout<<"\n**************\n";

				if(possible_values.size()==0)
				 	return false;

				for(int k=0;k<possible_values.size();k++)
				{
					sudoku[i][j] = possible_values[k];
					// printSudoku(sudoku);
					// cout<<"________________________________\n";
					if(solveSudoku(sudoku))
						return true;
					sudoku[i][j] = 0;
				}

				if(sudoku[i][j]==0)
					return false;
			}
		}
	}
	return false;
}



int main()
{
	vector<vector<int>> sudoku;

	int n;
	cin>>n;

	for(int i=0;i<n;i++)
	{
		int element;
		vector<int> row;
		row.clear();
		for(int j=0;j<n;j++)
		{
			cin>>element;
			row.push_back(element);
		}
		sudoku.push_back(row);
	}

	if(solveSudoku(sudoku))
		cout<<"Solved!"<<endl;
	else
		cout<<"Couldn't be solved, kindly check your input."<<endl;

	printSudoku(sudoku);
}
/*
9
3 0 6 5 0 8 4 0 0 
5 2 0 0 0 0 0 0 0 
0 8 7 0 0 0 0 3 1
0 0 3 0 1 0 0 8 0 
9 0 0 8 6 3 0 0 5
0 5 0 0 9 0 6 0 0 
1 3 0 0 0 0 2 5 0 
0 0 0 0 0 0 0 7 4
0 0 5 2 0 6 3 0 0 

9
0 0 0 6 0 4 7 0 0 
7 0 6 0 0 0 0 0 9
0 0 0 0 0 5 0 8 0 
0 7 0 0 2 0 0 9 3
8 0 0 0 0 0 0 0 5
4 3 0 0 1 0 0 7 0 
0 5 0 2 0 0 0 0 0 
3 0 0 0 0 0 2 0 8
0 0 2 3 0 1 0 0 0 

*/