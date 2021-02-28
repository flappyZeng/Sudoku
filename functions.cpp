#include"algorithms.h"

using namespace std;

void solved_by_backtrack(vector< vector<int> >& sudoku)
{
	vector <vector<int>>zero_point;
	for (int i = 0; i < sudoku.size(); i++)
		for (int j = 0; j < sudoku[i].size(); j++)
			if (sudoku[i][j] == 0)
				zero_point.push_back(vector<int>(i, j));
	int queue_size = zero_point.size();

	backtrack(0, zero_point, sudoku);
}

bool backtrack(int num, vector<vector<int>>& zero_point, vector<vector<int>>& sudoku)
{
	if (num == zero_point.size() - 1)
		return true;

	for (int i = 0; i < 10; i++)
	{
		if (check(zero_point[num], i, sudoku))
		{
			sudoku[zero_point[num][0]][zero_point[num][1]] = i;
			if (backtrack(num + 1, zero_point, sudoku))
				return true;
			else
				sudoku[zero_point[num][0]][zero_point[num][1]] = 0;
		}
	}
	return false;
}

bool check(vector<int>point, int num, const vector<vector<int>> sudoku)
{
	int x = point[0];
	int y = point[1];

	//检查行
	for (int i = 0; i < sudoku[i].size(); i++)
	{
		if (i != x && sudoku[i][y] == num )
			return false;
	}

	//检查列
	for (int j = 0; j < sudoku.size(); j++)
	{
		if (sudoku[x][j] == num  && j != y)
			return false;
	}

	//检查斜线
	for (int k = 1; x - k >= 0; k++)
	{
		if((y + k < sudoku.size() && sudoku[x - k][y + k] == num) || (y - k >= 0 && sudoku[x - k][y - k] == num))
			return false;
	}

	for (int l = 1; x + l < sudoku[0].size(); l++)
	{
		if ((y + l < sudoku.size() && sudoku[x + l][y + l] == num) || (y - l >= 0 && sudoku[x + l][y - l] == num))
			return false;
	}

	// 检查内部九宫格
	int pose_array = x / 3;     //位于第几个九宫格行;
	int pose_column = y / 3;    //位于第几个九宫格列; 

	for (int i = pose_array; i < pose_array + 3; i++)
	{
		for (int j = pose_column; j < pose_column + 3; j++)
		{
			if (i != x && j != y && sudoku[i][j] == num)
				return false;
		}
	}

	return true;
}

void print_sudoku(const vector<vector<int> > sudoku)
{
	for (int i = 0; i < sudoku.size(); i++)
	{
		for (int j = 0; j < sudoku[i].size(); j++)
			cout << sudoku[i][j] << "   ";
		cout << endl;
	}
}