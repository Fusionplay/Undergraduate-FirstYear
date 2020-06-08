#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>

struct table_column //储存一行中各列的数据
{
	vector<string> col;
	vector<string>::iterator colit;
};

struct tables //整个数据库拥有的table
{
    string tablename; //table名
    string filename; //文件名
};


void initial(); //加载原有的table
inline int coun(char *p); //计算某个单词的长度
inline void poi_mov(char *&p); //把字符指针移动到下一个“单词”处
bool In_str(char *target, char *source); //判断source是否为target的子串
inline int Max(int *arr, int len); //求int型数组中的最大元素

void func_detect(string &s); //探测功能
void tableprint(); //打印table

void create_table(char *tablename, char *filename); //从文件中打开table
void create_table(char *filename); //从文件中创建新的table。用户输入的table名必须和文件名一致，否则报错
void create_table(char *tablename, char *filename, char *start); //create table:按用户的输入来创建
void drop_table(char *name); //从数据库中删除名为name的TABLE
void table_list(); //打印当前数据库中所有TABLE
void insert_into(char *tablename, char *p); //第一种插入: 插入一整行
void insert_into(char *tablename, char *p, int num); //第二种插入：指定的行
void Delete(char *tablename); //删除若干行或所有行
void Delete(char *tablename, char *colname, char *colcont, int choice); //第一个Delete: 删除若干行
void update(char *tablename, char *p, int num, bool has_where); //更新TABLE中若干指定列的值
void select(char *tablename, char *p, int num); //select 1: 选择若干列来展示
void select(char *tablename, char *p, int num, int Is_dis); //有distinct
void select(char *tablename, char *p); //ORDER BY
void select(char *tablename, char *p, int num, bool Is_dis); //单纯有where
void select(char *tablename, char *filename, char *p, int num); //把查询结果写入文件中: 没有where
void sel_file_wh(char *tablename, char *filename, char *p); //把查询结果写入文件中: 有where
void selectspe(char *tablename, char *p); //按某一项查询
void sel_new(int num, char *tablename, char *p, char *sortby, int sort_kind); //最新出现的格式
void num_proc(char *tablename, char *colname, int choice); //数值处理:最大值，最小值，平均值
void todos(char *filename); //事务文件的读取与执行