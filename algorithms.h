#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include<iostream>
#include<vector>

using namespace std;

// 基于回溯解决数独问题 
class Solved_by_backtrack
{
public:
	void  solved_by_backtrack(vector< vector<int> >& sudoku);

private:
	//相关函数
	bool backtrack(int num, vector<vector<int>>& blank_point, vector<vector<int>>& sudoku);
	bool check(vector<int>point, int num, const vector<vector<int>> sudoku);

};


//基于前向检测+CSP
class Solved_by_CSP
{
public:
	void solved_by_CSP(vector< vector<int> >& sudoku);

	Solved_by_CSP()
	{
		init_domain();
	}

private:
	int domain[9][9][10];
	void init_domain();
	void edit_domain(const vector<vector<int> >& sudoku);
	bool solve(vector< vector<int> >& sudoku);
	void change_neighbors(int x, int y, const vector< vector<int> >& sudoku);
	pair<int, int> find_point(const vector< vector<int> >&soduku);

	void print_domain();
};


 
//基于遗传算法
class Solved_by_GA
{
public:
	void solved_by_GA(vector< vector<int> >& sudoku);

	Solved_by_GA(int set_group_rate, float set_keep_rate, float set_cross_rate = 0.7, float set_muta_rate = 0.01)
	{
		this->group_rate = set_group_rate;
		this->keep_rate = set_keep_rate;
		this->cross_rate = set_cross_rate;
		this->muta_rate = set_muta_rate;
		this->symbol_change = false;
	}
	
private:
	int group_rate;
	float keep_rate;
	float cross_rate;
	float muta_rate;
	bool symbol_change;
	
	//获得染色体
	vector<vector<int> > get_chromo(vector<vector<int>>& sudoku);
	vector<int> check_block(int position, const  vector< vector<int> >& sudoku);
	int get_chromo_size(const vector< vector<int> >& chromo);
	vector<vector<int> > shuffle_chromo(const vector< vector<int> > chromo);
	void print_chromes(const vector<vector<int> >& chromes);

	//获取新种群
	int get_group_size(const vector< vector<int> >& chromo);
	vector<pair<vector< vector<int>>, int> > generate_new_group(vector<pair<vector< vector<int>>, int> >&group, int group_size, vector< vector<int> >sudoku);
	void select_group_mem(vector<pair<vector< vector<int>>, int> >& new_group, vector<pair<vector< vector<int>>, int> >group, int select_size);
	vector<float> get_roulette(const vector < pair<vector< vector<int>>, int>>& group);
	int get_total_score(const vector<pair<vector< vector<int>>, int> >& group);
	int get_keep_chromos(vector<float>roulette);
	void gene_new_by_cross(vector < pair<vector< vector<int>>, int>>& group, vector< vector<int> >sudoku);

	//关于适应分数的计算
	int fitting_function(vector< vector<int> >& chromos, vector< vector<int> >& sudoku);
	vector< vector<int> > concate_function(const vector< vector<int> >& chromos, const vector< vector<int> >& sudoku);
	int compute_fitting_score(const vector< vector<int> >&concate);

	//染色体交叉产生后代，变异等
	pair< vector< vector<int>>, vector< vector<int>>> chromos_cross(const vector< vector<int> > first_chromo, const vector< vector<int> > second_chromo);
	pair< vector< vector<int> >, vector< vector<int> > > chromos_cross_B(const vector<vector<int> >first_chromo, const vector< vector<int> >second_chromo);
	void chromos_muta(vector < vector<int>>& chromos);

	//调整交叉后的染色体，防止重复
	void adjust_chromo(vector< vector<int>>& first_chromo, vector< vector<int> >& second_chromo);

	
};



bool cmp(pair<vector< vector<int> >, int>a, pair< vector< vector<int>>, int>b);
int get_rand_num();

//打印/展示函数
void print_sudoku(const vector<vector<int> >& sudoku);

//交互函数
void interactive_function();
void print_outline();

#endif // !_ALGORITHMS_H_

