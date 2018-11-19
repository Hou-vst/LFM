#include"LFM.h"
#include<iostream>

string A[] = { "a","b","d" };
string B[] = { "b","c","e" };
string C[] = { "c","d" };
string D[] = { "b","c","d" };
string E[] = { "a","d" };

string item[] = { "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z" };
vector<string> item_pool(item, item+sizeof(item)/sizeof(string));

//生成 用户-物品的对应表
void Create_UserToItem_Table(data_base& user_to_item)
{
	set<string> v;
	for (int i = 0; i < sizeof(A) / sizeof(string); i++)
	{
		v.insert(A[i]);
	}
	user_to_item["A"] = v;

	v.clear();
	for (int i = 0; i < sizeof(B) / sizeof(string); i++)
	{
		v.insert(B[i]);
	}
	user_to_item["B"] = v;

	v.clear();
	for (int i = 0; i < sizeof(C) / sizeof(string); i++)
	{
		v.insert(C[i]);
	}
	user_to_item["C"] = v;

	v.clear();
	for (int i = 0; i < sizeof(D) / sizeof(string); i++)
	{
		v.insert(D[i]);
	}
	user_to_item["D"] = v;

	v.clear();
	for (int i = 0; i < sizeof(E) / sizeof(string); i++)
	{
		v.insert(E[i]);
	}
	user_to_item["E"] = v;
}

//扩展用户-商品集，选取与正样本数量相当的样本作为负样本
data_base CreateUserToItemAppend(const data_base& UserToItem_ori, const vector<string>& items)
{
	data_base result= UserToItem_ori;
	data_base::const_iterator iter_begin = result.begin();
	for (;iter_begin != result.end(); iter_begin++)
	{
		const string& user_name = iter_begin->first;
		const set<string>& item_set = iter_begin->second;
		int size = static_cast<int>(item_set.size());
		int i = 0;
		while (i < size)
		{
			int idx = rand() % items.size();
			set<string>& item_set1 = result[user_name];
			if (item_set1.find(items[idx]) == item_set1.end())
			{
				item_set1.insert(items[idx]);
				i++;
			}
		}

	}

	return result;
}

void CreateR(const data_base& user_to_item_ori,const data_base& user_to_item,matrix& R)
{
	R.clear();

	//根据扩展后的用户-商品表，初始化R矩阵
	data_base::const_iterator iter_begin = user_to_item.begin();
	data_base::const_iterator iter_end = user_to_item.end();
	for (; iter_begin != iter_end; iter_begin++)
	{
		const string& user_name = iter_begin->first;
		const set<string>& item_set = iter_begin->second;
		set<string>::const_iterator iter_begin1 = item_set.begin();
		set<string>::const_iterator iter_end1 = item_set.end();
		for (; iter_begin1 != iter_end1; iter_begin1++)
		{
			const string& item_name = *iter_begin1;
			R[user_name][item_name] = 0;
		}
	}

	//根据原始的用户-商品表，给R矩阵赋值
	iter_begin = user_to_item_ori.begin();
	iter_end = user_to_item_ori.end();
	for (; iter_begin != iter_end; iter_begin++)
	{
		const string& user_name = iter_begin->first;
		const set<string>& item_set = iter_begin->second;
		set<string>::const_iterator iter_begin1 = item_set.begin();
		set<string>::const_iterator iter_end1 = item_set.end();
		for (; iter_begin1 != iter_end1; iter_begin1++)
		{
			const string& item_name = *iter_begin1;
			R[user_name][item_name] = 1;
		}
	}
}

void GetUsersAndItems(const data_base& user_to_item,set<string>& users,set<string>& items)
{
	users.clear();
	items.clear();

	data_base::const_iterator iter_begin = user_to_item.begin();
	data_base::const_iterator iter_end = user_to_item.end();
	for (; iter_begin != iter_end; iter_begin++)
	{
		const string& user_name = iter_begin->first;
		users.insert(user_name);
		const set<string>& item_set = iter_begin->second;
		set<string>::const_iterator iter_begin1 = item_set.begin();
		set<string>::const_iterator iter_end1 = item_set.end();
		for (;iter_begin1 != iter_end1;iter_begin1++)
		{
			const string& item_name= *iter_begin1;
			items.insert(item_name);
		}
	}
}


void GetRandom_P_Q(const set<string>& users,const set<string>& items,int F,matrix& P,matrix& Q)
{
	P.clear();
	Q.clear();
	//关于P Q的初始化，可以在作者书上第189页找到，用随机数填充即可，但是根据经验，随机数需要和1/sqrt(F）成正比
	set<string>::const_iterator iter_begin = users.begin();
	set<string>::const_iterator iter_end = users.end();

	//构造P[user][F]
	for (;iter_begin!=iter_end;iter_begin++)
	{
		const string& user_name = *iter_begin;
		for (int i = 0; i < F; i++)
		{
			char buff[256];
			memset(buff, 0, 256);
			sprintf_s(buff,"%d",i);
			string F_str = buff;

			int temp = static_cast<int>(rand()/sqrt(F));
			float score = static_cast<float>(temp * sqrt(F));
			P[user_name][F_str] = score;
		}
	}

	//构造Q[F][item]
	for (int i = 0; i < F; i++)
	{
		char buff[256];
		memset(buff, 0, 256);
		sprintf_s(buff, "%d", i);
		string F_str = buff;

		iter_begin = items.begin();
		iter_end = items.end();
		for (; iter_begin != iter_end; iter_begin++)
		{
			const string& item_name = *iter_begin;

			int temp = static_cast<int>(rand() / sqrt(F));
			float score = static_cast<float>(temp * sqrt(F));
			Q[F_str][item_name] = score;
		}
	}

}

void GetPredictR(matrix& P, matrix& Q, matrix& Predict_R)
{
	matrix::const_iterator P_begin = P.begin();
	matrix::const_iterator P_end = P.end();

	vector<string> users;
	vector<string> items;
	vector<string> classes;
	for (;P_begin != P_end;P_begin++)
	{
		const string& user_name = P_begin->first;
		users.push_back(user_name);
		const map<string, float>& c = P_begin->second;
		map<string, float>::const_iterator c_begin = c.begin();
		map<string, float>::const_iterator c_end = c.end();
		for (; c_begin != c_end; c_begin++)
		{
			const string& c_name = c_begin->first;
			classes.push_back(c_name);
		}
	}

	if (classes.empty() || Q.find(classes[0]) == Q.end())
	{
		return;
	}

	const map<string, float>& item = Q.find(classes[0])->second;
	map<string, float>::const_iterator item_begin = item.begin();
	map<string, float>::const_iterator item_end = item.end();
	for (; item_begin != item_end; item_begin++)
	{
		const string& item_name = item_begin->first;
		items.push_back(item_name);
	}

	for (int i = 0; i < users.size(); i++)
	{
		for (int j = 0; j < items.size(); j++)
		{
			Predict_R[users[i]][items[j]] = 0;
			for (int z = 0; z < classes.size(); z++)
			{
				Predict_R[users[i]][items[j]] += P[users[i]][classes[z]] * Q[classes[z]][items[j]];
			}
			
		}
	}
}
/*
R:采集负样本后生成的user-item兴趣度矩阵 
P:user-隐特征矩阵，要优化的结果
Q:隐特征-item矩阵，要优化的结果
F：隐特征个数
N：迭代次数
alfha：学习速率
lambda：正则化参数
*/
void LFM(const matrix& R, matrix& P, matrix& Q, const int F,const int N,const float alpha,const float lambda, matrix& R_Result)
{
	for (int i = 0; i < N; i++)
	{
		matrix Predict_R;
		GetPredictR(P,Q,Predict_R);

		matrix::const_iterator user_begin = Predict_R.begin();
		matrix::const_iterator user_end = Predict_R.end();
		for (; user_begin != user_end; user_begin++)
		{
			const string& user_name = user_begin->first;
			const map<string, float>& items = user_begin->second;

			map<string, float>::const_iterator item_begin = items.begin();
			map<string, float>::const_iterator item_end = items.end();
			for (; item_begin != item_end; item_begin++)
			{
				const string& item_name = item_begin->first;

				matrix::const_iterator iter = R.find(user_name);
				float r = 0;
				if (iter != R.end() && iter->second.find(item_name) != iter->second.end())
				{
					r = iter->second.find(item_name)->second;
				}
				float eui = r - Predict_R[user_name][item_name];
				for (int j = 0; j < F; j++)
				{
					char buff[256];
					memset(buff, 0, 256);
					sprintf_s(buff, "%d", i);
					string F_str = buff;
					P[user_name][F_str] += alpha * (eui*Q[F_str][item_name] - lambda * P[user_name][F_str]);
					Q[F_str][item_name] += alpha * (eui*P[user_name][F_str]-lambda*Q[F_str][item_name]);
				}
			}
		}
	}

	R_Result.clear();
	GetPredictR(P, Q, R_Result);
}

void Print_R(const matrix& R_Result,const set<string>& users, const set<string>& items)
{
	set<string>::const_iterator item_begin1 = items.begin();
	set<string>::const_iterator item_end1 = items.end();
	cout << "    ";
	for (; item_begin1 != item_end1; item_begin1++)
	{
		cout << (*item_begin1).c_str() << "    ";
	}
	cout << endl;
	set<string>::const_iterator user_begin = users.begin();
	set<string>::const_iterator user_end = users.end();
	for (; user_begin != user_end; user_begin++)
	{
		const string& user_name = *user_begin;
		cout << user_name.c_str() << "    ";
		set<string>::const_iterator item_begin = items.begin();
		set<string>::const_iterator item_end = items.end();
		for (; item_begin != item_end; item_begin++)
		{
			const string& item_name = *item_begin;
			float score = 0;
			if (R_Result.find(user_name) != R_Result.end() && R_Result.find(user_name)->second.find(item_name) != R_Result.find(user_name)->second.end())
			{
				score = R_Result.find(user_name)->second.find(item_name)->second;
			}
			cout << score << "    ";
		}
		cout << endl;
	}
}


