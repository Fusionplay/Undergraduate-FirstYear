//
//  Reversi.h
//  ReversiClient
//
//  Created by ganjun on 2018/3/6.
//  Copyright © 2018年 ganjun. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS
#ifndef Reversi_h
#define Reversi_h
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include "ClientSocket.h"
using namespace std;


void iniloc(); //初始化禁手位置数组，-1代表这个位置不存在
bool Is_banned(int &row, int &col); //判断是否是禁手位置
bool banned(const int &row, const int &col, const pair<int, int> &lastmove); //判断是否是禁手位置。用于博弈树
void valid_loc(const int &color); //搜索出所有可落子的位置
bool judge(int chess[][8], const int &row, const int &col, const int &color, const pair<int, int> &lastmove); //判断某个位置在某个棋盘上是否可以落子。用于博弈树
pair<int, int> handle(int chess[][8], const int row, const int col, const int &color); //翻转棋盘。用于博弈树
int tree(int inichess[][8], int depth, int alpha, int beta, int color, pair<int, int> lastmove); //博弈树
int assess(int chess[][8], const int &color, int turns, const pair<int, int> &lastmove); //估值函数
int get_motive(int chess[][8], const int &color, const pair<int, int> &lastmove); //计算行动力
int get_pawnnum(int chess[][8], const int &color); //计算现存的子的个数
int get_locweight(int chess[][8], const int &color); //计算己方的位置权重估值
int get_locweight_2(int chess[][8], const int &color); //计算己方的位置权重估值--2
int Max(int *t); 


class Reversi{
private:
    ClientSocket client_socket;
    int ownColor;
    int oppositeColor;

	//function 
	void handleMessage(int row, int col, int color);

	// according to chessman position (row , col) , generate one step message in order to send to server
    void generateOneStepMessage(int row , int col);
    
    pair<int,int> step(const int &color);
    
    void saveChessBoard(const int &color);

	void initChessBoard();

    void flip(int row, int col, int color); //翻转对方的棋子

    void setbanned(int row, int col, int color); //设置禁手位置

    void dir_up(const int &row, const int &col, int &rowtemp, int &coltemp, int &rowend, int &color, int &colortemp, bool &flippable); //上方向
    void dir_down(const int &row, const int &col, int &rowtemp, int &coltemp, int &rowend, int &color, int &colortemp, bool &flippable); //下方向
    void dir_left(const int &row, const int &col, int &rowtemp, int &coltemp, int &colend, int &color, int &colortemp, bool &flippable); //左方向
    void dir_right(const int &row, const int &col, int &rowtemp, int &coltemp, int &colend, int &color, int &colortemp, bool &flippable); //右方向
    void dir_leftup(const int &row, const int &col, int &rowtemp, int &coltemp, int &rowend, int &colend, int &color, int &colortemp, bool &flippable); //左上方向
    void dir_rightup(const int &row, const int &col, int &rowtemp, int &coltemp, int &rowend, int &colend, int &color, int &colortemp, bool &flippable); //右上方向
    void dir_leftdown(const int &row, const int &col, int &rowtemp, int &coltemp, int &rowend, int &colend, int &color, int &colortemp, bool &flippable); //左下方向
    void dir_rightdown(const int &row, const int &col, int &rowtemp, int &coltemp, int &rowend, int &colend, int &color, int &colortemp, bool &flippable); //右下方向


public:
    Reversi();
    ~Reversi();
    
    void authorize(const char *id , const char *pass);
    
    void gameStart();
    
    void gameOver();
    
    void roundStart(int round);
    
    void oneRound();
    
    void roundOver(int round);
    
    int observe();
    
};

#endif /* Reversi_h */