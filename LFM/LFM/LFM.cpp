#include"LFM.h"
#include<vector>
using namespace std;

// 用来存储user-item的集合还活着item-user的集合
typedef map<string, set<string> > data_base;

// 矩阵
typedef map<string, map<string, float> > matrix;

string A[] = { "a","b","d" };
string B[] = { "b","c","e" };
string C[] = { "c","d" };
string D[] = { "b","c","d" };
string E[] = { "a","d" };

string item[] = { "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z" };
vector<string> item_pool(item, item+sizeof(item)/sizeof(string));

// 定义原始的user-item数据集
data_base UserToItem_ori;

// 定义扩充后的user-item数据集
data_base UserToItem;

// 定义用户对物品兴趣度的矩阵 用户-物品 R=P*Q
matrix R;

// 定义用户对某类型物品的感兴趣程度的矩阵 用户-物品类型
matrix P;

// 定义物品在某类型物品中的比重矩阵 物品类型-物品
matrix Q;

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
		int size = item_set.size();
		int i = 0;
		while (i < size)
		{
			int idx = rand() % items.size();
			set<string>& item_set1 = result[user_name];
			item_set1.insert(items[idx]);
		}

	}
}

void GetR(matrix& P, matrix& Q,matrix& Result)
{

}

void GetP(matrix& P)
{

}

void GetQ(matrix& Q)
{

}
