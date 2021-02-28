#include"algorithms.h"
#include"sudoku.h"
#include<algorithm>
#include<random>
#include<time.h>
#include<math.h>
#include<cstring>
#include<Windows.h>

using namespace std;


//���ڻ����㷨
void Solved_by_backtrack::solved_by_backtrack(vector< vector<int> >& sudoku)
{
	vector<int>temp = { 0, 0 };
	vector <vector<int>>blank_point;
	for (int i = 0; i < sudoku.size(); i++)
		for (int j = 0; j < sudoku[i].size(); j++)
			if (sudoku[i][j] == 0)
			{
				temp[0] = i; 
				temp[1] = j;
				blank_point.push_back(temp);
			}
				
	int queue_size = blank_point.size();
	if (backtrack(0, blank_point, sudoku))
		cout << "�ɹ�" << endl;
	else
		cout << "ʧ��" << endl;
}

//����
bool Solved_by_backtrack::backtrack(int num, vector<vector<int>>& blank_point, vector<vector<int>>& sudoku)
{
	if (num == blank_point.size())
		return true;

	for (int i = 1; i < 10; i++)
	{
		if (check(blank_point[num], i, sudoku))
		{
			sudoku[blank_point[num][0]][blank_point[num][1]] = i;
			if (backtrack(num + 1, blank_point, sudoku))
				return true;

			sudoku[blank_point[num][0]][blank_point[num][1]] = 0;
		}
	}
	return false;
}

//�������������Ƿ�Ϸ�
bool Solved_by_backtrack::check(vector<int>point, int num, const vector<vector<int>> sudoku)
{

	int x = point[0];
	int y = point[1];
	
	//�����
	for (int i = 0; i < 9; i++)
	{
		if (i != y && sudoku[x][i] == num )
			return false;
	}

	//�����
	for (int j = 0; j < 9; j++)
	{
		if (sudoku[j][y] == num  && j != x)
			return false;
	}

	/*
	//���б��
	for (int k = 1; x - k >= 0; k++)
	{
		if((y + k < sudoku.size() && sudoku[x - k][y + k] == num) || (y - k >= 0 && sudoku[x - k][y - k] == num))
			return false;
	}

	for (int l = 1; x + l < sudoku[y].size(); l++)
	{
		if ((y + l < sudoku.size() && sudoku[x + l][y + l] == num) || (y - l >= 0 && sudoku[x + l][y - l] == num))
			return false;
	}
	*/

	// ����ڲ��Ź���
	int pose_array = x / 3;     //λ�ڵڼ����Ź�����;
	int pose_column = y / 3;    //λ�ڵڼ����Ź�����; 

	for (int i = pose_array * 3; i < (pose_array + 1) * 3; i++)
	{
		for (int j = pose_column * 3; j < (pose_column + 1) * 3; j++)
		{
			if (i != x && j != y && sudoku[i][j] == num)
				return false;
		}
	}

	return true;
}



//CSP::����ǰ����+����ʽ�㷨
void Solved_by_CSP::solved_by_CSP(vector< vector<int> >& sudoku)
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (sudoku[i][j] != 0)
				change_neighbors(i, j, sudoku);
	if (solve(sudoku))
		cout << "�ɹ�" << endl;
	else
		cout << "ʧ��" << endl;
			
}

//��ʼ��Լ����
void Solved_by_CSP::init_domain()
{
	for(int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			domain[i][j][0] = 9;
			for (int k = 1; k < 10; k++)
				domain[i][j][k] = 1;
		}
}

//�޸�Լ����
void Solved_by_CSP::edit_domain(const vector< vector<int> >& sudoku)
{
	for(int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (sudoku[i][j] != 0)
				change_neighbors(i, j, sudoku);
 		}
}

//�ı�ĳ�������Χ�ĵ��Լ����
void Solved_by_CSP::change_neighbors(int x, int y, const vector< vector<int> >& sudoku)
{
	int point = sudoku[x][y];
	//�޸���&���е�domain��Ϣ
	for (int i = 0; i < 9; i++)
	{
		//��
		domain[x][i][0] -= domain[x][i][point];
		domain[x][i][point] = 0;

		//��
		domain[i][y][0] -= domain[i][y][point];
		domain[i][y][point] = 0;
     }

	//�ֲ��Ź�����Ϣ��
	int pose_array = x / 3;     //λ�ڵڼ����Ź�����;
	int pose_column = y / 3;    //λ�ڵڼ����Ź�����; 

	for (int i = pose_array * 3; i < (pose_array + 1) * 3; i++)
	{
		for (int j = pose_column * 3; j < (pose_column + 1) * 3; j++)
		{
			domain[i][j][0] -= domain[i][j][point];
			domain[i][j][point] = 0;
		}
	}
}

//Ѱ��Լ������С�ĵ���Ϊ��ʼ��
pair<int, int>  Solved_by_CSP::find_point(const vector< vector<int> > &sudoku)
{
	int x = -1, y = -1;
	int max_choice = 10;
	for(int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (sudoku[i][j] == 0 && domain[i][j][0] < max_choice)
			{
				x = i;
				y = j;
				max_choice = domain[i][j][0];
			}
		} 
	return { x, y };
}

//��Ҫ����⺯��
bool Solved_by_CSP::solve(vector< vector<int> >& sudoku)
{
	// ѡȡ��ѡ����С�ĵ�
	pair<int, int> point = find_point(sudoku);;

	//û��ѡ��������ʾ����������;
	if (point.first == -1 && point.second == -1) 
		return true;

	//���صĵ�Ŀ�ѡ�㼯Ϊ�գ��޵��ѡ
	if (domain[point.first][point.second][0] == 0) 
		return false;

	// ���浱ǰ״̬, ��Ҫ��domain 
	int temp_domain[9][9][10];

	// �������ӵ�domain
	for (int num = 1; num <= 9; num++)
			if (domain[point.first][point.second][num] == 1)
			{
				//�����¼
				memcpy(temp_domain, domain, sizeof(domain));

				// ��������õ�
				sudoku[point.first][point.second] = num;
				change_neighbors(point.first, point.second, sudoku);

				if (solve(sudoku))
					return true;

				//�������,�ָ�������
				sudoku[point.first][point.second] = 0;
				memcpy(domain, temp_domain, sizeof(domain));
			}

	return false;
}

//��ӡԼ����
void Solved_by_CSP::print_domain()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			cout << domain[i][j][0] << "  ";
		cout << endl;
	 }
}



//�����Ŵ��㷨
void Solved_by_GA::solved_by_GA(vector< vector<int> >& sudoku)
{
	int num_generate = 1;
	vector< vector<int> >chromos = get_chromo(sudoku);
	vector< vector<int> >shuffle_chromos;
	int group_size = get_group_size(chromos);

	vector<pair<vector< vector<int>>, int> >group;
	for (int i = 0; i < group_size; i++)
	{
		if (i == 0)
			group.push_back({ chromos, fitting_function(chromos, sudoku) });
		else
		{
			shuffle_chromos.clear();
			shuffle_chromos = shuffle_chromo(chromos);
			group.push_back({ shuffle_chromos, fitting_function(shuffle_chromos, sudoku) });
		}

	}
	sort(group.begin(), group.end(), cmp);  //����Ӧ���������Ӵ�С����
	cout << "��Ⱥ��С�� " << group.size() << endl;
	while (num_generate < 10000)
	{
		if (group[0].second == 144)
		{
			cout << "��" << num_generate <<"���ɹ�" << endl;
			sudoku = concate_function(group[0].first, sudoku);
			break;
		}
		else
		{
			cout << "��" << num_generate << "����" << endl;
			cout << "��߷�Ϊ�� " << group[0].second << endl;
			cout << "��ͷ�Ϊ   " << group[group.size() - 1].second << endl;

			/*
			if (group[0].second == group[group.size() - 1].second)
			{
				sudoku = concate_function(group[0].first, sudoku);
				break;
			}
			*/

			group = generate_new_group(group, group_size, sudoku);
			num_generate++;
			
			//for (int i = 0; i < 10; i++)
			//	print_chromes(group[i].first);

		}
	}	
}

//��ó�ʼȾɫ��
vector< vector<int> > Solved_by_GA::get_chromo(vector< vector<int> >& sudoku)
{
	vector<vector<int> >chromos;
	for (int i = 0; i < 9; i++)
	{
		chromos.push_back(check_block(i, sudoku));
	}
	return chromos;
}

//��������пո�λ����ô��ϵ��£������ҵ�9��block��״̬���������ݣ�
vector<int> Solved_by_GA::check_block(int position, const vector< vector<int> >& sudoku)
{
	bool pos[10] = { false };
	if (position < 3)
	{
		for (int i = position * 3; i < position * 3 + 3; i++)
			for (int j = 0; j < 3; j++)
				pos[sudoku[j][i]] = true;
	}
	else if (position < 6)
	{
		for (int i = (position - 3) * 3; i < position * 3 - 6; i++)
			for (int j = 3; j < 6; j++)
				pos[sudoku[j][i]] = true;
	}
	else
	{
		for (int i = (position - 6) * 3; i < position * 3 - 15; i++)
			for (int j = 6; j < 9; j++)
				pos[sudoku[j][i]] = true;
	}
	vector<int> return_dna;
	for (int i = 1; i < 10; i++)
		if (!pos[i])
			return_dna.push_back(i);
	return return_dna;
}

//����Ⱦɫ�壨�����ڲ���ÿһ��vector��
vector<vector<int> > Solved_by_GA::shuffle_chromo(const vector< vector<int> > chromo)
{
	vector<vector<int> >shuffle_chromo(chromo);
	for (int i = 0; i < shuffle_chromo.size(); i++)
	{
		LARGE_INTEGER seed;
		QueryPerformanceFrequency(&seed);
		QueryPerformanceCounter(&seed);
		srand(seed.QuadPart);
		random_shuffle(shuffle_chromo[i].begin(), shuffle_chromo[i].end());
	}
	return shuffle_chromo;
}

//���Ⱦɫ��ĳ���
int Solved_by_GA::get_chromo_size(const vector< vector<int> >& chromes)
{
	int return_size = 0;
	for (int i = 0; i < chromes.size(); i++)
		return_size += chromes[i].size();
	return return_size;
}

//��ӡȾɫ�壨��άvector��
void Solved_by_GA::print_chromes(const vector< vector<int> >& chromes)
{
	for (int i = 0; i < chromes.size(); i++)
	{
		for (int j = 0; j < chromes[i].size(); j++)
			cout << chromes[i][j];
		cout << "   ";
	}
	cout << endl;
}


//�����Ⱥ�Ĵ�С��Ⱦɫ������� * ��Ⱥ�Ŵ����
int Solved_by_GA::get_group_size(const vector< vector<int> >& chromos)
{
	int group_size = 0;
	for (int i = 0; i < chromos.size(); i++)
		group_size += int(chromos[i].size());
	return group_size * group_rate;
}

//����������Ⱥͨ��ɸѡ�����桢����Ȳ����µ���Ⱥ
vector<pair<vector< vector<int>>, int> > Solved_by_GA::generate_new_group(vector<pair<vector< vector<int>>, int> >&group, int group_size, vector< vector<int> >sudoku)
{
	vector<pair<vector< vector<int>>, int> >new_group;
	//����������Ĳ��֣�keep_rate = 0.05;
	for (int i = 0; i < int(group_size * keep_rate); i++)
		new_group.push_back(group[i]);

	int select_size = ceil(group_size * (0.5 - keep_rate));

	//ʣ��ٷ�֮��ʮ��ʹ�����̶��㷨ɸѡ��
	group.erase(group.begin(), group.begin() + int(group.size() * keep_rate));
	select_group_mem(new_group, group, select_size);

	//����ѡ�������µ���Ⱥ,�ٷ�֮��ʮ��
	gene_new_by_cross(new_group, sudoku);

	//�������ֲ����ţ�������ʼӱ�
	if (group[0].second == group[group.size() - 1].second && symbol_change == false)
	{
		muta_rate *= 2;
		symbol_change = true;
	}
	

	sort(group.begin(), group.end(), cmp);
	return new_group;

}

//�������̶��㷨ɸѡȾɫ�壻
void Solved_by_GA::select_group_mem(vector<pair<vector< vector<int>>, int> >& new_group, vector<pair<vector< vector<int>>, int> >group, int select_size)
{
	vector<float> roulette = get_roulette(group);

	int group_size = int(group.size());
	bool select[5000] = { false };
	
 	int order = 0;
	for (int i = 0; i < select_size; i++)
	{
		order = get_keep_chromos(roulette);
		while (select[order])
			order = get_keep_chromos(roulette);
		select[order] = true;	
		new_group.push_back(group[order]);
	}

}

//��ȡ����
vector<float> Solved_by_GA::get_roulette(const vector < pair<vector< vector<int>>, int>>& group)
{
	vector<float>roulette;
	int total_score = get_total_score(group);
	roulette.push_back(0.0);
	for (int i = 0; i < group.size(); i++)
	{
		roulette.push_back(roulette[i] + (group[i].second * 1.0) / total_score);
	}

	/*
	cout << "����" << endl;
	for (int i = 0; i < roulette.size(); i++)
		cout << roulette[i] << " ";
	cout << endl;
	*/

	return roulette;
}

//��ȡ��Ⱥ�����и������Ӧ�������ܺͣ�����ȷ���������̡�
int Solved_by_GA::get_total_score(const vector<pair<vector< vector<int>>, int> >& group)
{
	int return_score = 0;
	for (int i = 0; i < group.size(); i++)
		return_score += group[i].second;
	return return_score;
}

//���ؾ�����ɸѡ�õ���Ⱦɫ�����
int Solved_by_GA::get_keep_chromos(vector<float> roulette)
{
	int rand_num_int = get_rand_num();
	float rand_num = (rand_num_int % 999) / 1000.0;
	int return_position = 0;
	while (rand_num >= roulette[return_position])
		return_position++;
	return return_position - 1;
}

//��ʤ��̭���õĺͺõ���Ի���1-50�� 2-49�Դ����ƣ����ַ���
void Solved_by_GA::gene_new_by_cross(vector < pair<vector< vector<int>>, int>>& group, vector< vector<int> >sudoku)
{
	pair<  vector< vector<int>>,  vector< vector<int>>>new_gene;
	int group_size = int(group.size());
	sort(group.begin(), group.end(), cmp);
	//��β����
	/*
	for (int i = 0; i < group_size / 2; i ++)
	{

		new_gene = chromos_cross(group[i].first, group[group_size - i - 1].first);
		group.push_back({ new_gene.first, fitting_function(new_gene.first, sudoku) });
		group.push_back({ new_gene.second, fitting_function(new_gene.second, sudoku) });
	}
	*/
	
	//��˳����������
	for (int i = 0; i < group_size ; i += 2)
	{

		new_gene = chromos_cross(group[i].first, group[ i + 1].first);
		group.push_back({ new_gene.first, fitting_function(new_gene.first, sudoku) });
		group.push_back({ new_gene.second, fitting_function(new_gene.second, sudoku) });
	}
	
}


//��Ӧ�Է�������
int Solved_by_GA::fitting_function(vector< vector<int>>& chromo, vector< vector<int> >& sudoku)
{
	vector< vector<int> >concate;
	concate = concate_function(chromo, sudoku);
	return compute_fitting_score(concate);
}

//����Ӧ�����ݣ�Ⱦɫ�壩����Ŀ������
vector< vector<int> > Solved_by_GA::concate_function(const vector< vector<int> >& chromos, const vector< vector<int> >& sudoku)
{
	vector< vector<int> > result(sudoku);
	int pos = 0;
	for (int i = 0; i < chromos.size(); i++)
	{
		pos = 0;
		if (i < 3)
		{
			for (int j = i * 3; j < i * 3 + 3; j++)
				for (int k = 0; k < 3; k++)
					if (pos < chromos[i].size() && result[j][k] == 0)
					{
 						result[j][k] = chromos[i][pos];
						pos++;
					}
		}
		else if (i < 6)
		{
			for (int j = (i - 3) * 3; j < i * 3 - 6; j++)
				for (int k = 3; k < 6; k++)
					if (pos < chromos[i].size() && result[j][k] == 0)
					{
						result[j][k] = chromos[i][pos];
						pos++;
					}
		}
		else
		{
			for (int j = (i - 6) * 3; j < i * 3 - 15; j++)
				for (int k = 6; k < 9; k++)
				{
					if (pos < chromos[i].size() && result[j][k] == 0)
					{
						result[j][k] = chromos[i][pos];
						pos++;
					}
				}
		}
	}
	return result;
}

//�������֣���Ӧ��������chrmosģ����������� ��/�е��ظ�ֵ������ÿ�ظ�һ�μ�һ�֣��ܷ�Ϊ144�֣�8 * ��9 + 9����
int Solved_by_GA::compute_fitting_score(const vector< vector<int> >& concate)
{
	int init_score = 144;
	

	for (int i = 0; i < 9; i++)
	{
		bool repete_array[10] = { false };
		bool repete_column[10] = { false };
		
		for (int j = 0; j < 9; j++)
		{
			//����ÿ�е��ظ�����
			if (repete_array[concate[i][j]])
			{
				init_score--;
			}
			else
				repete_array[concate[i][j]] = true;

			//����ÿ�е��ظ�����
			if (repete_column[concate[j][i]])
			{
				init_score--;
			}
			else
				repete_column[concate[j][i]] = true;
		}
	}
	return init_score;
}

//���� Ⱦɫ�� + ��Ӧ�����ṹ��ʹ����Ӧ�������бȽϵıȽϺ���
bool cmp(pair<vector< vector<int> >, int>a, pair< vector< vector<int>>, int>b)
{
	return a.second > b.second;
}


//Ⱦɫ�彻�桢����

//������淽���д���֤�����ܴ����������ÿһ��Ⱦɫ���ڲ����Ƭ�ν���
pair< vector< vector<int> >, vector< vector<int> > > Solved_by_GA::chromos_cross(const vector<vector<int> >first_chromo, const vector< vector<int> >second_chromo)
{
	
	vector< vector<int> > new_chromo[2] = { first_chromo, second_chromo };
 	int size = first_chromo.size();

 	for (int i = 0; i < size; i++)
	{
		int n = first_chromo[i].size();
		int left = get_rand_num() % n;
		int right = get_rand_num() % n;
		if (left > right)
		{
			left = left ^ right;
			right = left ^ right;
			left = left ^ right;
		}
		for (int j = left; j < right; j++)
		{
			new_chromo[0][i][j] = new_chromo[0][i][j] ^ new_chromo[1][i][j];
			new_chromo[1][i][j] = new_chromo[0][i][j] ^ new_chromo[1][i][j];
			new_chromo[0][i][j] = new_chromo[0][i][j] ^ new_chromo[1][i][j];
		}
	}
	adjust_chromo(new_chromo[0], new_chromo[1]);

	//�����������
	for (int i = 0; i < 2; i++)
	{
		if (get_rand_num() % 1000 < muta_rate)
		{
			chromos_muta(new_chromo[i]);
			cout << "�¸��巢������" << endl;
		}
	}
;
	return { new_chromo[0], new_chromo[1] };
	
}

//��һ�ֽ��淽�������ڱ�������
pair< vector< vector<int> >, vector< vector<int> > > Solved_by_GA::chromos_cross_B(const vector<vector<int> >first_chromo, const vector< vector<int> >second_chromo)
{
	vector< vector<int> > new_chromo[2] = { first_chromo, second_chromo };
	int size = get_chromo_size(first_chromo);
	int cut = int(size * (1 - cross_rate));
	int start = get_rand_num() % cut;
	int sum_size = int(size * cross_rate) ;
	int count_gene = 0;
	int pose = 0, i = 0, last_pose = 0;
	while(pose < start)
	{
		last_pose = pose;
		pose += first_chromo[i].size();
		i++;
	}
	if (pose > start)
	{
		pose = start - last_pose;
		pose--;
		i--;
	}
	else
	{
		if (start != 0)
		{
			i--;
			pose = start - 1;
		}
		else
		{
			i = 0;
			pose = 0;
		}
	}
	while (count_gene < sum_size)
	{
		if (pose >= first_chromo[i].size())
		{
			pose = 0;
			i++;
		}
		new_chromo[0][i][pose] = new_chromo[0][i][pose] ^ new_chromo[1][i][pose];
		new_chromo[1][i][pose] = new_chromo[0][i][pose] ^ new_chromo[1][i][pose];
		new_chromo[0][i][pose] = new_chromo[0][i][pose] ^ new_chromo[1][i][pose];
		count_gene++;
		pose++;
	}
	adjust_chromo(new_chromo[0], new_chromo[1]);


	//�����������
	for (int i = 0; i < 2; i++)
	{
		if (get_rand_num() % 1000 < muta_rate)
		{
			chromos_muta(new_chromo[i]);
			cout << "�¸��巢������" << endl;
		}
	}

	return { new_chromo[0], new_chromo[1] };
}

//����,���ĳ��������������
void Solved_by_GA::chromos_muta(vector< vector<int> >& chromo)
{
	srand((unsigned int)time(0));
	int pose = rand() % chromo.size();
	int x = rand() % chromo[pose].size();
	int y = rand() % chromo[pose].size();

	while(y == x)
		y = rand() % chromo[pose].size();

	chromo[pose][x] = chromo[pose][x] ^ chromo[pose][y];
	chromo[pose][y] = chromo[pose][x] ^ chromo[pose][y];
	chromo[pose][x] = chromo[pose][x] ^ chromo[pose][y];
}

//���ɵ�Ⱦɫ����ܷ��������ظ�����Ҫ����
void Solved_by_GA::adjust_chromo(vector< vector<int>>& first_chromo, vector< vector<int> >& second_chromo)
{
	int size = first_chromo.size();
	int size_in = 0, pose = 0, last_pose = -1;
	for (int i = 0; i < size; i++)
	{
		bool list[10] = { false };
		
		pose = 0;
		size_in = first_chromo[i].size();
		while (pose < size_in)
		{
			//��ʾ��һ���ظ������Ը���
			if (list[first_chromo[i][pose]] && pose != last_pose)
			{
				first_chromo[i][pose] = first_chromo[i][pose] ^ second_chromo[i][pose];
				second_chromo[i][pose] = first_chromo[i][pose] ^ second_chromo[i][pose];
				first_chromo[i][pose] = first_chromo[i][pose] ^ second_chromo[i][pose];
				last_pose = pose;
				pose = 0;
				memset(list, 0, sizeof(list));
			}
			//���ҵ�����һ�θĵ�λ�ã���β��ܸĻ�ȥ�ˣ���ǰ����ظ�λ��
			else if(list[first_chromo[i][pose]] && pose == last_pose)
			{
				for (int j = 0; j < size_in; j++)
				{
					if (first_chromo[i][last_pose] == first_chromo[i][j] && j != last_pose)
					{
						first_chromo[i][j] = first_chromo[i][j] ^ second_chromo[i][j];
						second_chromo[i][j] = first_chromo[i][j] ^ second_chromo[i][j];
						first_chromo[i][j] = first_chromo[i][j] ^ second_chromo[i][j];
						last_pose = j;
						pose = 0;
						memset(list, 0, sizeof(list));
						break;
					}
				}
			}
			else
			{
				list[first_chromo[i][pose]] = true;
				pose++;
             }
		}
	}
}




//��ӡ/չʾ����
void print_sudoku(const vector<vector<int> > &sudoku)
{
	for (int i = 0; i < sudoku.size(); i++)
	{
		for (int j = 0; j < sudoku[i].size(); j++)
			cout << sudoku[i][j] << "   ";
		cout << endl << endl;
	}
}

//��������
void interactive_function()
{
	int sudoku_select;
	int algorithm_select;
	vector<vector<int>>test;
	print_outline();
	cin >> sudoku_select >> algorithm_select;

	switch (sudoku_select)
	{
	case 1:   test = sudoku;        break;
	case 2:   test = easy_sudoku;   break;
	case 3:   test = medium_sudoku; break;
	default:  test = hard_sudoku;   break;
	}

	cout << "Ҫ��������Ϊ�� " << endl;
	print_sudoku(test);
	cout << "ʹ�õ��㷨Ϊ��";
	
	DWORD k = ::GetTickCount64();
	switch (algorithm_select)
	{
	case 1: {
		cout << "�����㷨" << endl;
		Solved_by_backtrack solver = Solved_by_backtrack();
		solver.solved_by_backtrack(test);
		break;
	}

	case 2: {
		cout << "����ǰ���������ʽ�㷨��CSP��" << endl;
		Solved_by_CSP solver = Solved_by_CSP();
		solver.solved_by_CSP(test);
		break;
	}
	default: {
		cout << "�Ŵ��㷨��GA��" << endl;
		int group_rate;
		float keep_rate, cross_rate, muta_rate;
		cout << "�������Ӧ����Ⱥ���ʣ�2 -20����������屣���ʣ�0.01 -0.05���������ʣ�0 - 1���� �����ʣ�0 - 0.5��: " << endl;
		cin >> group_rate >> keep_rate >> cross_rate >> muta_rate;
		Solved_by_GA solver = Solved_by_GA(group_rate, keep_rate, cross_rate, muta_rate);
		solver.solved_by_GA(test);
		break;
	}
	}
	cout << "��������ʱ��Ϊ�� " << (::GetTickCount64() - k) << "ms" << endl;
	print_sudoku(test);
}

//��ӡ��٣�Ŀ¼
void print_outline()
{
	cout << "���ȣ���ѡ����Ҫ���в��Ե������Լ���Ӧ���������������㷨:" << endl;
	cout << "1: project     2�� easy      3:    medium      4:    hard" <<  endl;
	cout << "1: backtrack   2:  CSP       3��   GA" << endl;
	cout << "����ѡ��ʹ�ÿո������enter����" << endl;
 }

//��ȡ�����
int get_rand_num()
{
		LARGE_INTEGER seed;
		QueryPerformanceFrequency(&seed);
		QueryPerformanceCounter(&seed);
		srand(seed.QuadPart);

		return rand();
}