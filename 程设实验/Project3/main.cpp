//
//  main.cpp
//  ReversiClient
//
//  Created by ganjun on 2018/3/5.
//  Copyright © 2018年 ganjun. All rights reserved.
//

#include "Reversi.h"

int main()
{
    /*for (int i = 0; i < 5; i++)
    {
        Reversi reversi = Reversi();
        reversi.gameStart();
    }*/
    Reversi reversi = Reversi();
    reversi.gameStart();

	system("pause");
    return 0;
}