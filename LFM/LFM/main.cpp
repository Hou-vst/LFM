#include<iostream>
#include"LFM.h"

extern vector<string> item_pool;

//LFM的最终目的是要求的矩阵P（表述用户和各个物品类型之间的感兴趣程度）和矩阵Q（表述物品属于各个物品类型的程度）
int main()
{
	// 定义原始的user-item数据集
	data_base UserToItem_ori;
	Create_UserToItem_Table(UserToItem_ori);

	// 定义扩充后的user-item数据集，负样本采样
	data_base&& UserToItem=CreateUserToItemAppend(UserToItem_ori, item_pool);

	//随机生成P Q前找到用户列表和物品列表
	set<string> users;
	set<string> items;
	GetUsersAndItems(UserToItem, users, items);

	// 根据扩充前后两个user-item数据集生成一个R
	matrix R;
	CreateR(UserToItem_ori, UserToItem,R);

	// 定义用户对某类型物品的感兴趣程度的矩阵 用户-物品类型 随机生成一个 
	matrix P;

	// 定义物品在某类型物品中的比重矩阵 物品类型-物品 随机生成一个 
	matrix Q;

	//关于P Q的初始化，可以在作者书上第189页找到，用随机数填充即可，但是根据经验，随机数需要和1/sqrt(F）成正比


	//隐特征个数定为3个
	int F = 3;

	//初始化P Q
	GetRandom_P_Q(users, items, F, P, Q);

	// 计算得到预测的R
	matrix Predict_R;
	int N = 2;
	float alpha = static_cast<float>(0.9);
	float lambda = static_cast<float>(0.01);
	LFM(R,P,Q,F,N,alpha,lambda,Predict_R);

	Print_R(R,users,items);
	cout << "================================" << endl;
	Print_R(Predict_R, users, items);
	return 0;
} 