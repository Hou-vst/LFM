#include<iostream>
#include"LFM.h"

extern vector<string> item_pool;

//LFM������Ŀ����Ҫ��ľ���P�������û��͸�����Ʒ����֮��ĸ���Ȥ�̶ȣ��;���Q��������Ʒ���ڸ�����Ʒ���͵ĳ̶ȣ�
int main()
{
	// ����ԭʼ��user-item���ݼ�
	data_base UserToItem_ori;
	Create_UserToItem_Table(UserToItem_ori);

	// ����������user-item���ݼ�������������
	data_base&& UserToItem=CreateUserToItemAppend(UserToItem_ori, item_pool);

	//�������P Qǰ�ҵ��û��б����Ʒ�б�
	set<string> users;
	set<string> items;
	GetUsersAndItems(UserToItem, users, items);

	// ��������ǰ������user-item���ݼ�����һ��R
	matrix R;
	CreateR(UserToItem_ori, UserToItem,R);

	// �����û���ĳ������Ʒ�ĸ���Ȥ�̶ȵľ��� �û�-��Ʒ���� �������һ�� 
	matrix P;

	// ������Ʒ��ĳ������Ʒ�еı��ؾ��� ��Ʒ����-��Ʒ �������һ�� 
	matrix Q;

	//����P Q�ĳ�ʼ�����������������ϵ�189ҳ�ҵ������������伴�ɣ����Ǹ��ݾ��飬�������Ҫ��1/sqrt(F��������


	//������������Ϊ3��
	int F = 3;

	//��ʼ��P Q
	GetRandom_P_Q(users, items, F, P, Q);

	// ����õ�Ԥ���R
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