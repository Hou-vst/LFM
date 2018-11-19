#pragma once
#ifndef LFM_H
#define LFM_H
#include<map>
#include<string>
#include<set>
#include<vector>

using namespace std;
// 用来存储user-item的集合还活着item-user的集合
typedef map<string, set<string> > data_base;
// 矩阵
typedef map<string, map<string, float> > matrix;

void Create_UserToItem_Table(data_base& user_to_item);
data_base CreateUserToItemAppend(const data_base& UserToItem_ori, const vector<string>& items);
void CreateR(const data_base& user_to_item_ori, const data_base& user_to_item, matrix& R);
void GetUsersAndItems(const data_base& user_to_item, set<string>& users, set<string>& items);
void GetRandom_P_Q(const set<string>& users, const set<string>& items, int F, matrix& P, matrix& Q);
void GetPredictR(matrix& P, matrix& Q, matrix& Predict_R);
void LFM(const matrix& R, matrix& P, matrix& Q, const int F, const int N, const float alpha, const float lambda, matrix& R_Result);
void Print_R(const matrix& R_Result, const set<string>& users, const set<string>& items);
#endif // !LFM_H
