#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>

struct table_column //����һ���и��е�����
{
	vector<string> col;
	vector<string>::iterator colit;
};

struct tables //�������ݿ�ӵ�е�table
{
    string tablename; //table��
    string filename; //�ļ���
};


void initial(); //����ԭ�е�table
inline int coun(char *p); //����ĳ�����ʵĳ���
inline void poi_mov(char *&p); //���ַ�ָ���ƶ�����һ�������ʡ���
bool In_str(char *target, char *source); //�ж�source�Ƿ�Ϊtarget���Ӵ�
inline int Max(int *arr, int len); //��int�������е����Ԫ��

void func_detect(string &s); //̽�⹦��
void tableprint(); //��ӡtable

void create_table(char *tablename, char *filename); //���ļ��д�table
void create_table(char *filename); //���ļ��д����µ�table���û������table��������ļ���һ�£����򱨴�
void create_table(char *tablename, char *filename, char *start); //create table:���û�������������
void drop_table(char *name); //�����ݿ���ɾ����Ϊname��TABLE
void table_list(); //��ӡ��ǰ���ݿ�������TABLE
void insert_into(char *tablename, char *p); //��һ�ֲ���: ����һ����
void insert_into(char *tablename, char *p, int num); //�ڶ��ֲ��룺ָ������
void Delete(char *tablename); //ɾ�������л�������
void Delete(char *tablename, char *colname, char *colcont, int choice); //��һ��Delete: ɾ��������
void update(char *tablename, char *p, int num, bool has_where); //����TABLE������ָ���е�ֵ
void select(char *tablename, char *p, int num); //select 1: ѡ����������չʾ
void select(char *tablename, char *p, int num, int Is_dis); //��distinct
void select(char *tablename, char *p); //ORDER BY
void select(char *tablename, char *p, int num, bool Is_dis); //������where
void select(char *tablename, char *filename, char *p, int num); //�Ѳ�ѯ���д���ļ���: û��where
void sel_file_wh(char *tablename, char *filename, char *p); //�Ѳ�ѯ���д���ļ���: ��where
void selectspe(char *tablename, char *p); //��ĳһ���ѯ
void sel_new(int num, char *tablename, char *p, char *sortby, int sort_kind); //���³��ֵĸ�ʽ
void num_proc(char *tablename, char *colname, int choice); //��ֵ����:���ֵ����Сֵ��ƽ��ֵ
void todos(char *filename); //�����ļ��Ķ�ȡ��ִ��