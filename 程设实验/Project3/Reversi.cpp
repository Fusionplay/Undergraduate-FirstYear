#include "Reversi.h"
#define random(x) (rand()%x)
#define ROWS 8
#define COLS 8
#define ROUNDS 10


ofstream outfile; //存文件
int turn = 4; //回合数
int chessboard[8][8] = { -1 }; //棋盘。0代表黑色，1代表白色，-1代表空格
int weight[8][8] =
{
    { 3000,  -1000, -20, -20, -20, -20, -1000,  3000 },
    { -1000, -2000,  1,   1,   1,   1,  -2000, -1000 },
    { -20, 1, 1, 1, 1, 1, 1, -20 },
    { -20, 1, 1, 3, 3, 1, 1, -20 },
    { -20, 1, 1, 3, 3, 1, 1, -20 },
    { -20, 1, 1, 1, 1, 1, 1, -20 },
    { -1000, -2000, 1, 1, 1, 1, -2000, -1000 },
    { 3000, -1000, -20, -20, -20, -20, -1000, 3000 }
};


int weight2[8][8] =
{
    { 3000, -1000, 1, 1, 1, 1, -1000, 3000 },
    { -1000, -2000, 1, 1, 1, 1, -2000, -1000 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { -1000, -2000, 1, 1, 1, 1, -2000, -1000 },
    { 3000, -1000, 1, 1, 1, 1, -1000, 3000 }
};

/*{\
//5, 0, 4, 4, 4, 4, 0, 5,\
//0, -5, -4, -4, -4, -4, -5, 0,\
//4, -4, 3, 3, 3, 3, -4, 4,\
//4, -4, 3, 3, 3, 3, -4, 4,\
//4, -4, 3, 3, 3, 3, -4, 4,\
//4, -4, 3, 3, 3, 3, -4, 4,\
//0, -5, -4, -4, -4, -4, -5, 0,\
//5, 0, 4, 4, 4, 4, 0, 5}; //权重表 */

pair<int, int> bannedloc[4]; //禁手位置
pair<int, int> yesloc[30]; //储存可下的位置
pair<int, int> last_mov(-1, -1); //上一个位置。用于博弈树中.

inline void iniloc() //初始化禁手位置数组，-1代表这个位置不存在
{
    for (int i = 0; i < 4; i++)
        bannedloc[i].first = bannedloc[i].second = -1;
}


inline bool banned(const int &row, const int &col, const pair<int, int> &lastmove) //判断是否是禁手位置。用于博弈树
{
    if (row == lastmove.first - 1 && col == lastmove.second || row == lastmove.first + 1 && col == lastmove.second || \
        row == lastmove.first && col == lastmove.second - 1 || row == lastmove.first && col == lastmove.second + 1)
        return true; //是禁手位置

    return false;
}


bool judge(int chess[][8], const int &i, const int &j, const int &color, const pair<int, int> &lastmove) //判断某个位置在某个棋盘上是否可以落子,用于博弈树
{
    if (chess[i][j] != -1 || banned(i, j, lastmove))
        return false;


    int ricolor = 1 - color; //对手的颜色
    int rowtemp = 0, coltemp = 0; //临时


    if (i - 1 >= 0 && chess[i - 1][j] == ricolor) //上侧
    {
        rowtemp = i - 2;
        coltemp = j;
        while (rowtemp >= 0)
        {
            if (chess[rowtemp][coltemp] == ricolor) //对手的棋
                rowtemp--;
            else if (chess[rowtemp][coltemp] == -1) //空格
                break;
            else if (chess[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                return true;
        }
    }


    if (i + 1 <= 7 && chess[i + 1][j] == ricolor) //下侧
    {
        rowtemp = i + 2;
        coltemp = j;
        while (rowtemp <= 7)
        {
            if (chess[rowtemp][coltemp] == ricolor) //对手的棋
                rowtemp++;
            else if (chess[rowtemp][coltemp] == -1) //空格
                break;
            else if (chess[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                return true;
        }
    }


    if (j - 1 >= 0 && chess[i][j - 1] == ricolor) //左侧
    {
        rowtemp = i;
        coltemp = j - 2;
        while (coltemp >= 0)
        {
            if (chess[rowtemp][coltemp] == ricolor) //对手的棋
                coltemp--;
            else if (chess[rowtemp][coltemp] == -1) //空格
                break;
            else if (chess[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                return true;
        }
    }


    if (j + 1 <= 7 && chess[i][j + 1] == ricolor) //右侧
    {
        rowtemp = i;
        coltemp = j + 2;
        while (coltemp <= 7)
        {
            if (chess[rowtemp][coltemp] == ricolor) //对手的棋
                coltemp++;
            else if (chess[rowtemp][coltemp] == -1) //空格
                break;
            else if (chess[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                return true;
        }
    }


    if (j - 1 >= 0 && i - 1 >= 0 && chess[i - 1][j - 1] == ricolor) //左上侧
    {
        rowtemp = i - 2;
        coltemp = j - 2;
        while (coltemp >= 0 && rowtemp >= 0)
        {
            if (chess[rowtemp][coltemp] == ricolor) //对手的棋
            {
                rowtemp--;
                coltemp--;
            }
            else if (chess[rowtemp][coltemp] == -1) //空格
                break;
            else if (chess[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                return true;
        }
    }


    if (j + 1 <= 7 && i - 1 >= 0 && chess[i - 1][j + 1] == ricolor) //右上侧
    {
        rowtemp = i - 2;
        coltemp = j + 2;
        while (coltemp <= 7 && rowtemp >= 0)
        {
            if (chess[rowtemp][coltemp] == ricolor) //对手的棋
            {
                rowtemp--;
                coltemp++;
            }
            else if (chess[rowtemp][coltemp] == -1) //空格
                break;
            else if (chess[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                return true;
        }
    }


    if (j + 1 <= 7 && i + 1 <= 7 && chess[i + 1][j + 1] == ricolor) //右下侧
    {
        rowtemp = i + 2;
        coltemp = j + 2;
        while (coltemp <= 7 && rowtemp <= 7)
        {
            if (chess[rowtemp][coltemp] == ricolor) //对手的棋
            {
                rowtemp++;
                coltemp++;
            }
            else if (chess[rowtemp][coltemp] == -1) //空格
                break;
            else if (chess[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                return true;
        }
    }


    if (j - 1 >= 0 && i + 1 <= 7 && chess[i + 1][j - 1] == ricolor) //左下侧
    {
        rowtemp = i + 2;
        coltemp = j - 2;
        while (coltemp >= 0 && rowtemp <= 7)
        {
            if (chess[rowtemp][coltemp] == ricolor) //对手的棋
            {
                rowtemp++;
                coltemp--;
            }
            else if (chess[rowtemp][coltemp] == -1) //空格
                break;
            else if (chess[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                return true;
        }
    }

    return false;
}


pair<int, int> handle(int chess[][8], const int row, const int col, const int &color) //翻转棋盘。用于博弈树
{
    chess[row][col] = color; //先落子
    int rowtemp = row, coltemp = col;
    int rowend = -1, colend = -1; //翻转终止的位置
    bool flippable = false; //判断可翻转与否
    int colortemp = 1 - color; //对方棋子的颜色


    if (row >= 2) //上可翻
    {
        rowtemp--; //上方向
        while (rowtemp >= 0)
        {
            if (chess[rowtemp][coltemp] == colortemp) //对方的棋子
                rowtemp--;
            else if (chess[rowtemp][coltemp] == color) //己方的棋子
            {
                rowend = rowtemp;
                flippable = true; //可翻转
                break;
            }

            else break; //空格
        }


        if (flippable)
        {
            rowtemp = row - 1;
            while (rowtemp != rowend) //逐一翻转
            {
                chess[rowtemp][coltemp] = color;
                rowtemp--;
            }
        }

        rowtemp = row;
        coltemp = col;
        rowend = -1; //翻转终止的位置
        flippable = false; //判断可翻转与否
    }


    if (row <= 5) //下可翻
    {
        rowtemp++; //下方向
        while (rowtemp <= 7)
        {
            if (chess[rowtemp][coltemp] == colortemp) //对方的棋子
                rowtemp++;
            else if (chess[rowtemp][coltemp] == color) //己方的棋子
            {
                rowend = rowtemp;
                flippable = true; //可翻转
                break;
            }

            else break; //空格
        }


        if (flippable)
        {
            rowtemp = row + 1;
            while (rowtemp != rowend) //逐一翻转
            {
                chess[rowtemp][coltemp] = color;
                rowtemp++;
            }
        }

        rowtemp = row;
        coltemp = col;
        rowend = -1; //翻转终止的位置
        flippable = false; //判断可翻转与否
    }


    if (col >= 2) //左可翻
    {
        coltemp--; //左方向
        while (coltemp >= 0)
        {
            if (chess[rowtemp][coltemp] == colortemp) //对方的棋子
                coltemp--;
            else if (chess[rowtemp][coltemp] == color) //己方的棋子
            {
                colend = coltemp;
                flippable = true; //可翻转
                break;
            }

            else break; //空格
        }


        if (flippable)
        {
            coltemp = col - 1;
            while (coltemp != colend) //逐一翻转
            {
                chess[rowtemp][coltemp] = color;
                coltemp--;
            }
        }

        rowtemp = row;
        coltemp = col;
        colend = -1; //翻转终止的位置
        flippable = false; //判断可翻转与否
    }


    if (col <= 5) //右可翻
    {
        coltemp++; //右方向
        while (coltemp <= 7)
        {
            if (chess[rowtemp][coltemp] == colortemp) //对方的棋子
                coltemp++;
            else if (chess[rowtemp][coltemp] == color) //己方的棋子
            {
                colend = coltemp;
                flippable = true; //可翻转
                break;
            }

            else break; //空格
        }


        if (flippable)
        {
            coltemp = col + 1;
            while (coltemp != colend) //逐一翻转
            {
                chess[rowtemp][coltemp] = color;
                coltemp++;
            }
        }

        rowtemp = row;
        coltemp = col;
        colend = -1; //翻转终止的位置
        flippable = false; //判断可翻转与否
    }


    if (row >= 2 && col >= 2) //左上可翻
    {
        coltemp--; //左方向
        rowtemp--; //上方向
        while (coltemp >= 0 && rowtemp >= 0)
        {
            if (chess[rowtemp][coltemp] == colortemp) //对方的棋子
            {
                coltemp--;
                rowtemp--;
            }
            else if (chess[rowtemp][coltemp] == color) //己方的棋子
            {
                rowend = rowtemp;
                colend = coltemp;
                flippable = true; //可翻转
                break;
            }

            else break; //空格
        }


        if (flippable)
        {
            rowtemp = row - 1;
            coltemp = col - 1;
            while (coltemp != colend) //逐一翻转
            {
                chess[rowtemp][coltemp] = color;
                coltemp--;
                rowtemp--;
            }
        }

        rowtemp = row;
        coltemp = col;
        rowend = -1;
        colend = -1; //翻转终止的位置
        flippable = false; //判断可翻转与否
    }


    if (row >= 2 && col <= 5) //右上可翻
    {
        coltemp++; //右方向
        rowtemp--; //上方向
        while (coltemp <= 7 && rowtemp >= 0)
        {
            if (chess[rowtemp][coltemp] == colortemp) //对方的棋子
            {
                coltemp++;
                rowtemp--;
            }
            else if (chess[rowtemp][coltemp] == color) //己方的棋子
            {
                rowend = rowtemp;
                colend = coltemp;
                flippable = true; //可翻转
                break;
            }

            else break; //空格
        }


        if (flippable)
        {
            rowtemp = row - 1;
            coltemp = col + 1;
            while (coltemp != colend) //逐一翻转
            {
                chess[rowtemp][coltemp] = color;
                coltemp++;
                rowtemp--;
            }
        }

        rowtemp = row;
        coltemp = col;
        rowend = -1;
        colend = -1; //翻转终止的位置
        flippable = false; //判断可翻转与否
    }


    if (row <= 5 && col <= 5) //右下可翻
    {
        coltemp++; //右方向
        rowtemp++; //下方向
        while (coltemp <= 7 && rowtemp <= 7)
        {
            if (chess[rowtemp][coltemp] == colortemp) //对方的棋子
            {
                coltemp++;
                rowtemp++;
            }
            else if (chess[rowtemp][coltemp] == color) //己方的棋子
            {
                rowend = rowtemp;
                colend = coltemp;
                flippable = true; //可翻转
                break;
            }

            else break; //空格
        }


        if (flippable)
        {
            rowtemp = row + 1;
            coltemp = col + 1;
            while (coltemp != colend) //逐一翻转
            {
                chess[rowtemp][coltemp] = color;
                coltemp++;
                rowtemp++;
            }
        }

        rowtemp = row;
        coltemp = col;
        rowend = -1;
        colend = -1; //翻转终止的位置
        flippable = false; //判断可翻转与否
    }


    if (row <= 5 && col >= 2) //左下可翻
    {
        coltemp--; //左方向
        rowtemp++; //下方向
        while (coltemp >= 0 && rowtemp <= 7)
        {
            if (chess[rowtemp][coltemp] == colortemp) //对方的棋子
            {
                coltemp--;
                rowtemp++;
            }
            else if (chess[rowtemp][coltemp] == color) //己方的棋子
            {
                rowend = rowtemp;
                colend = coltemp;
                flippable = true; //可翻转
                break;
            }

            else break; //空格
        }


        if (flippable)
        {
            rowtemp = row + 1;
            coltemp = col - 1;
            while (coltemp != colend) //逐一翻转
            {
                chess[rowtemp][coltemp] = color;
                coltemp--;
                rowtemp++;
            }
        }

        //rowtemp = row;
        //coltemp = col;
        //rowend = -1;
        //colend = -1; //翻转终止的位置
        //flippable = false; //判断可翻转与否
    }


    return make_pair(row, col); //返回成"lastmove"
}


int get_motive(int chess[][8], const int &color, const pair<int, int> &lastmove) //计算行动力
{
    int num = 0; //计数可落的子的个数

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            if (i == 0 && j == 1 || i == 1 && j == 0 || i == 1 && j == 1 || \
                i == 7 && j == 6 || i == 6 && j == 6 || i == 6 && j == 7 || \
                i == 0 && j == 6 || i == 1 && j == 6 || i == 1 && j == 7 || \
                i == 7 && j == 1 || i == 6 && j == 0 || i == 6 && j == 1)
                continue;
            
            if (judge(chess, i, j, color, lastmove)) //可落子
                    num++;
        }

    return num;
}


inline int get_pawnnum(int chess[][8], const int &color) //计算现存的子的个数
{
    int num = 0; //计数现存的子的个数
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (chess[i][j] == color) //己方子
                num++;

    return num;
}


int get_locweight(int chess[][8], const int &color) //计算己方的位置权重估值
{
    //int num = 0;
    //for (int i = 0; i < 8; i++)
    //    for (int j = 0; j < 8; j++)
    //        if (chess[i][j] == color) //己方子
    //            num += weight[i][j];

    //return num;


    if (chess[0][0] == color)
    {
        weight[1][0] = 1;
        weight[1][1] = 1;
        weight[0][1] = 1;
    }

    if (chess[7][7] == color)
    {
        weight[6][6] = 1;
        weight[7][6] = 1;
        weight[6][7] = 1;
    }

    if (chess[7][0] == color)
    {
        weight[6][0] = 1;
        weight[7][1] = 1;
        weight[6][1] = 1;
    }

    if (chess[0][7] == color)
    {
        weight[1][7] = 1;
        weight[1][6] = 1;
        weight[0][6] = 1;
    }

    int a = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (chess[i][j] == color)
                a += weight[i][j];

    weight[1][0] = -1000;
    weight[1][1] = -1000;
    weight[0][1] = -1000;
    weight[6][6] = -1000;
    weight[7][6] = -1000;
    weight[6][7] = -1000;
    weight[6][0] = -1000;
    weight[7][1] = -1000;
    weight[6][1] = -1000;
    weight[1][7] = -1000;
    weight[1][6] = -1000;
    weight[0][6] = -1000;
    return a;
}


int get_locweight_2(int chess[][8], const int &color) //计算己方的位置权重估值--2
{
    if (chess[0][0] == color)
    {
        weight2[1][0] = 1;
        weight2[1][1] = 1;
        weight2[0][1] = 1;
    }

    if (chess[7][7] == color)
    {
        weight2[6][6] = 1;
        weight2[7][6] = 1;
        weight2[6][7] = 1;
    }

    if (chess[7][0] == color)
    {
        weight2[6][0] = 1;
        weight2[7][1] = 1;
        weight2[6][1] = 1;
    }

    if (chess[0][7] == color)
    {
        weight2[1][7] = 1;
        weight2[1][6] = 1;
        weight2[0][6] = 1;
    }


    int a = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (chess[i][j] == color) a += weight[i][j];
                //a += weight2[i][j];


    weight2[1][0] = -1000;
    weight2[1][1] = -1000;
    weight2[0][1] = -1000;
    weight2[6][6] = -1000;
    weight2[7][6] = -1000;
    weight2[6][7] = -1000;
    weight2[6][0] = -1000;
    weight2[7][1] = -1000;
    weight2[6][1] = -1000;
    weight2[1][7] = -1000;
    weight2[1][6] = -1000;
    weight2[0][6] = -1000;
    return a;
}


int assess(int chess[][8], const int &color, int turns, const pair<int, int> &lastmove) //估值函数
{
    turns += 5;

    if (turns <= 20)
    {
        int motive_power = get_motive(chess, 1 - color, lastmove); //行动力
        int pawnnum = get_pawnnum(chess, color); //获得现有的己方的子的个数
        int loc_weight = get_locweight(chess, color); //获得己方的位置权重估值
        return loc_weight * 2 + (-10) * pawnnum - 10 * motive_power;
    }
        

    else if (turns <= 35)
    {
        int motive_power = get_motive(chess, color, lastmove); //行动力
        int pawnnum = get_pawnnum(chess, color); //获得现有的己方的子的个数
        int loc_weight = get_locweight(chess, color); //获得己方的位置权重估值
        return loc_weight + (-20) * pawnnum + 30 * motive_power;
    }
        
    else if (turns < 50)
    {
        int motive_power = get_motive(chess, color, lastmove); //行动力
        int pawnnum = get_pawnnum(chess, color); //获得现有的己方的子的个数
        int loc_weight = get_locweight_2(chess, color); //获得己方的位置权重估值
        return loc_weight + 50 * motive_power;
    }

    else
        return get_pawnnum(chess, color);
}


inline int Max(int *t)
{
    int num = 0, max0 = 0;
    while (t[num] != -1)
    {
        if (t[num] > t[max0])
            max0 = num;
        num++;
    }
    return max0;
}


int tree(int inichess[][8], int depth, int alpha, int beta, int color, pair<int, int> lastmove) //博弈树
{
    depth--;
    if (depth == 0)
        return assess(inichess, color, turn - 3, lastmove);


    else if (depth % 2 == 0) //偶数层 自己的棋
    {
        bool on = true;

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (judge(inichess, i, j, color, lastmove)) //可以落子
                {
                    on = false;

                    int chess[8][8];
                    for (int x = 0; x < 8; x++)
                        for (int y = 0; y < 8; y++)
                            chess[x][y] = inichess[x][y];

                    pair<int, int> last = handle(chess, i, j, color); //每次都新建一个棋盘
                    int ans = tree(chess, depth, alpha, beta, color, last);
                    if (ans > alpha) alpha = ans;
                    if (alpha >= beta) return alpha; //发生 alpha剪枝
                }


        if (on)
        {
            if (lastmove.first == -1) return assess(inichess, color, turn - 3, lastmove);
            else return tree(inichess, depth, alpha, beta, color, make_pair(-1, -1)); //己方无可下的位置，于是来到这里，进入下一层
            //lastmove = make_pair(-1, -1);
            //return tree(inichess, depth, alpha, beta, color, lastmove); //己方无可下的位置，于是来到这里，进入下一层
        }

        return alpha;
    }


    else if (depth % 2 == 1)
    {
        bool on = true;

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                //if (inichess[i][j] == -1 && !banned(i, j, lastmove)) //是空格且不是禁手位置才有可能可以落子
                    if (judge(inichess, i, j, 1 - color, lastmove)) //可以落子
                    {
                        on = false;

                        int chess[8][8];
                        for (int x = 0; x < 8; x++)
                            for (int y = 0; y < 8; y++)
                                chess[x][y] = inichess[x][y];

                        pair<int, int> last = handle(chess, i, j, 1 - color);
                        int ans = tree(chess, depth, alpha, beta, color, last);
                        if (ans < beta) beta = ans;
                        if (alpha >= beta) return beta; //发生 beta剪枝
                    }


        if (on)
        {
            if (lastmove.first == -1) return assess(inichess, color, turn - 3, lastmove);
            else return tree(inichess, depth, alpha, beta, color, make_pair(-1, -1)); //己方无可下的位置，于是来到这里，进入下一层
            //lastmove = make_pair(-1, -1);
            //return tree(inichess, depth, alpha, beta, color, lastmove); //对方无可下的位置，于是来到这里，进入下一层
        }

        return beta;
    }

    return 0;
}

/*********************************************************************************************************************/

Reversi::Reversi(){
    client_socket = ClientSocket();
    oppositeColor = ownColor = -1;
}

Reversi::~Reversi(){};


 //send id and password to server by socket
 //rtn != 0 represents socket transfer error

void Reversi::authorize(const char *id , const char *pass)
{
    client_socket.connectServer();
    std::cout << "Authorize " << id << std::endl;
    char msgBuf[BUFSIZE];
    memset(msgBuf , 0 , BUFSIZE);
    msgBuf[0] = 'A';
    memcpy(&msgBuf[1] , id , 9);
    memcpy(&msgBuf[10] , pass , 6);
    int rtn = client_socket.sendMsg(msgBuf);
    if (rtn != 0) printf("Authorized Failed!\n");
}

/* user input id and password that should match id and password in file in ReversiServer "players-X.txt"
 Where X should be the maximum number of the numbers.
*/
void Reversi::gameStart()
{
    /*char id[12] = {0}, passwd[10] = {0};*/
    char id[12] = "111111113", passwd[10] = "123456";
    printf("ID: %s\n" , id);
    /*scanf("%s" , id);*/
    printf("PASSWD: %s\n", passwd);
    /*scanf("%s", passwd);*/
    
    authorize(id, passwd);


    string filetemp;
    time_t now_time = time(0);
    struct tm * timeinfo;
    timeinfo = localtime(&now_time);
    filetemp = asctime(timeinfo);
    for (int i = 0;; i++)
    {
        if (filetemp[i] == ':')
            filetemp[i] = '_';
        else if (filetemp[i + 1] == '\0')
        {
            filetemp[i] = ' ';
            break;
        }
    }

    string filename = "Match info ";
    filename += filetemp;
    filename += ".txt";
    outfile.open(filename, ios::out);
    printf("Game Start!\n");
    
    for (int round = 0 ; round < ROUNDS ; round++){
        roundStart(round);
        oneRound();
        roundOver(round);
    }

    gameOver();
    client_socket.close();
}

void Reversi::gameOver()
{
    outfile.close();
    printf("Game Over!\n");
}

/* send a message, lazi in position (row, col)
 receive 2 message:
 first message is the answer to your step.
 second message is the step of opponent player.
*/
void Reversi::roundStart(int round)
{
    printf("Round %d Ready Start!\n" , round);
    
    // first time receive msg from server
    int rtn = client_socket.recvMsg();
    if (rtn != 0) return ;
    if(strlen(client_socket.getRecvMsg()) < 2)
        printf("Authorize Failed!\n");
    else
        printf("Round start received msg %s\n", client_socket.getRecvMsg());
    switch (client_socket.getRecvMsg()[1]) {
            // this client : black chessman
        case 'B':
            ownColor = 0;
            oppositeColor = 1;
            rtn = client_socket.sendMsg("BB");
            printf("\nMy color is %d\n", ownColor);
            printf("Send BB -> rtn: %d\n", rtn);
            if (rtn != 0) return ;
            break;
        case 'W':
            ownColor = 1;
            oppositeColor = 0;
            rtn = client_socket.sendMsg("BW");
            printf("\nMy color is %d\n", ownColor);
            printf("Send BW -> rtn: %d\n", rtn);
            if (rtn != 0) return ;
            break;
        default:
            printf("Authorized Failed!\n");
            break;
    }

	//chess board init
	initChessBoard();
}

void Reversi::oneRound()
{
    int STEP = 1;
    switch (ownColor) {
        case 0:
            while (STEP < 10000) {
                
                pair<int,int> chess = step(0);                        // take action, send message
                
                // lazi only excute after server's message confirm  in observe function
                generateOneStepMessage(chess.first,chess.second);
                //saveChessBoard(0);
                
                
                if (observe() >= 1) break;     // receive RET Code
                
                if (observe() >= 1) break;    // see white move
                STEP++;
                //saveChessBoard(0);
            }
            printf("One Round End\n");
            break;
        case 1:
            while (STEP < 10000) {
                
                if (observe() >= 1) break;    // see black move
                
                pair<int,int> chess = step(1);                        // take action, send message
                // lazi only excute after server's message confirm  in observe function
                generateOneStepMessage(chess.first,chess.second);
                //saveChessBoard(1);
                
                
                if (observe() >= 1) break;     // receive RET Code
                // saveChessBoard();
                STEP++;
            }
            printf("One Round End\n");
            break;
            
        default:
            break;
    }
}
 
void Reversi::roundOver(int round)
{
    printf("Round %d Over!\n", round);
    // reset initializer
    
    outfile << "\nRound " << round << " over!\n==============================================================================================\n\n\n";
    turn = 4;
    ownColor = oppositeColor = -1;
}

int Reversi::observe()
{
    int rtn = 0;
    int recvrtn = client_socket.recvMsg();
    if (recvrtn != 0) return 1;
    printf("receive msg %s\n" , client_socket.getRecvMsg());
    switch (client_socket.getRecvMsg()[0]) {
        case 'R':
        {
            switch (client_socket.getRecvMsg()[1]) {
                case 'Y':   // valid step
                    switch (client_socket.getRecvMsg()[2]) {
                        case 'P':   // update chessboard
                        {
                            int desRow = (client_socket.getRecvMsg()[3] - '0') * 10 + client_socket.getRecvMsg()[4] - '0';
                            int desCol = (client_socket.getRecvMsg()[5] - '0') * 10 + client_socket.getRecvMsg()[6] - '0';
                            int color = (client_socket.getRecvMsg()[7] - '0');
                            //use handleMessage to handle the position (desRow , desCol)
                            handleMessage(desRow, desCol, color);
                            
                            printf("a valid step of : (%d %d)\n" , desRow , desCol);
                            break;
                        }
                        case 'N':   // R0N: enemy wrong step
                        {
							int desRow = -1, desCol = -1;
							int color = (client_socket.getRecvMsg()[3] - '0');
							handleMessage(desRow, desCol, color);

                            //
                            printf("a true judgement of no step\n");
                            break;
                        }
                    }
                    
                    break;
                case 'W':
                    // invalid step
                    switch (client_socket.getRecvMsg()[2]) {
                        case 'P':{
                            int desRow = (client_socket.getRecvMsg()[3] - '0') * 10 + client_socket.getRecvMsg()[4] - '0';
                            int desCol = (client_socket.getRecvMsg()[5] - '0') * 10 + client_socket.getRecvMsg()[6] - '0';
                            
                            int color = (client_socket.getRecvMsg()[7] - '0');
                            printf("Invalid step , server random a true step of : (%d %d)\n" , desRow , desCol);
                            //use handleMessage to handle the position (desRow , desCol)
                            handleMessage(desRow, desCol, color);
                            break;
                        }
                        case 'N':{
                            int desRow = -1, desCol = -1;
							int color = (client_socket.getRecvMsg()[3] - '0');
							handleMessage(desRow, desCol, color);
							printf("a wrong judgement of no step\n");
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                
                default:
                    
                    printf("Error : Other error!\n");
                    rtn = -5;
                    break;
            }
            break;
        }


        case 'E':
        {
            switch (client_socket.getRecvMsg()[1]) {
                case '0':
                    // game over
                    rtn = 2;
                    break;
                case '1':
                    // round over
                    rtn = 1;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    return rtn;
}


void Reversi::generateOneStepMessage(int row, int col)
{
    char msg[BUFSIZE];
    memset(msg , 0 , sizeof(msg));
    
	
    //put row and col in the message
    msg[0] = 'S';
    msg[1] = 'P';
    msg[2] = '0' + row / 10;
    msg[3] = '0' + row % 10;
    msg[4] = '0' + col / 10;
    msg[5] = '0' + col % 10;
    msg[6] = '\0';
	if(row < 0 || col < 0){
		row = -1;
		col = -1;
	}
    if(row == -1 && col == -1){
		msg[2] = '-';
		msg[3] = '1';
		msg[4] = '-';
		msg[5] = '1';
	}
    //print
    printf("generate one step at position (%2d,%2d) : %s\n", row , col , msg);
    
    
    client_socket.sendMsg(msg);
}

/*-------------------------last three function--------------------------------
 * step : find a good position to lazi your chess.
 * saveChessBoard : save the chess board now.
 * handleMessage: handle the message from server.
 */

pair<int, int> Reversi::step(const int &color)
{
    valid_loc(color); //先找出所有可下的位置
    
    if (yesloc[0].first == -1) //无可下的位置
        return make_pair(-1, -1);

    int num = 0;
    int wei[30];
    for (int i = 0; i < 30; i++)
        wei[i] = -1;

    
    time_t start, stop;
    int tempboard[8][8];
    start = time(NULL);

    while (yesloc[num].first != -1)
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                tempboard[i][j] = chessboard[i][j];

        handle(tempboard, yesloc[num].first, yesloc[num].second, color);

        if (turn + 2 < 50)
            wei[num] = tree(tempboard, 6, -80000, 80000, color, make_pair(yesloc[num].first, yesloc[num].second));
        /*else if (turn + 2 < 50)
            wei[num] = tree(tempboard, 6, -80000, 80000, color, make_pair(yesloc[num].first, yesloc[num].second));*/
        else
            wei[num] = tree(tempboard, 12, -80000, 80000, color, make_pair(yesloc[num].first, yesloc[num].second));

        stop = time(NULL);
        if (stop - start >= 4)
            break;

        num++;
    }

    num = Max(wei);
    return yesloc[num];
}


void Reversi::saveChessBoard(const int &color)
{
    outfile << turn / 2 - 1;

    if (color == 0) //黑色
        outfile << "  Black\n";
    else if (color == 1) //白色
        outfile << "  White\n";


    string temp = "+----+----+----+----+----+----+----+----+\n";
    outfile << temp;
    for (int i = 0; i < 8; i++)
    {
        outfile << "| ";
        for (int j = 0; j < 8; j++)
        {
            if (chessboard[i][j] == -1)
                outfile << "   | ";
            else if (chessboard[i][j] == 0) //黑棋
                outfile << "● | ";
            else outfile << "○ | ";
        }
            
        outfile << endl << temp;
    }
        
    turn++;
    outfile << "\n\n";
}


void Reversi::handleMessage(int row, int col, int color)
{
    if (row != -1 && col != -1) //有棋可下
    {
        chessboard[row][col] = color; //落子
        last_mov = make_pair(row, col);
        flip(row, col, color); //翻转
        if (color == oppositeColor) //是对方的棋
            setbanned(row, col, color); //设置禁手位置
    }

    else if (color == oppositeColor) //是对方的棋
        iniloc(); //初始化禁手位置数组

    saveChessBoard(color); //存盘
}

//init your chess board here
void Reversi::initChessBoard()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            chessboard[i][j] = -1; //先全部弄成空格

    chessboard[3][3] = 0;
    chessboard[4][4] = 0;
    chessboard[3][4] = 1;
    chessboard[4][3] = 1; //布局完中间的4个棋子
}


inline bool Is_banned(int &row, int &col) //判断指定的空格是否是禁手位置
{
    for (int i = 0; i < 4; i++)
        if (row == bannedloc[i].first && col == bannedloc[i].second)
            return true; //此时是禁手位置

    return false;
}


void valid_loc(const int &color) //搜索所有可落子的位置
{
    for (int i = 0; i < 30; i++)
        yesloc[i].first = yesloc[i].second = -1; //先初始化这个数组

    int ricolor = 1 - color; //对手的颜色
    int num = 0; //计数yesloc数组
    int rowtemp = 0, coltemp = 0; //临时
    bool okloc = false;

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            if (chessboard[i][j] == -1 && !Is_banned(i, j)) //是空格且不是禁手位置
            {
                if (i - 1 >= 0 && chessboard[i - 1][j] == ricolor) //上侧
                {
                    okloc = false;
                    rowtemp = i - 2;
                    coltemp = j;
                    while (rowtemp >= 0)
                    {
                        if (chessboard[rowtemp][coltemp] == ricolor) //对手的棋
                            rowtemp--;
                        else if (chessboard[rowtemp][coltemp] == -1) //空格
                            break;
                        else if (chessboard[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                        {
                            yesloc[num].first = i;
                            yesloc[num].second = j;
                            num++;
                            okloc = true;
                            break; //跳出当前while和if,开始判断下一个位置
                        }
                    }
                    if (okloc) continue; //如果是的话，开始判断下一个位置
                }


                if (i + 1 <= 7 && chessboard[i + 1][j] == ricolor) //下侧
                {
                    okloc = false;
                    rowtemp = i + 2;
                    coltemp = j;
                    while (rowtemp <= 7)
                    {
                        if (chessboard[rowtemp][coltemp] == ricolor) //对手的棋
                            rowtemp++;
                        else if (chessboard[rowtemp][coltemp] == -1) //空格
                            break;
                        else if (chessboard[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                        {
                            yesloc[num].first = i;
                            yesloc[num].second = j;
                            num++;
                            okloc = true;
                            break; //跳出当前while和if,开始判断下一个位置
                        }
                    }
                    if (okloc) continue; //如果是的话，开始判断下一个位置
                }


                if (j - 1 >= 0 && chessboard[i][j - 1] == ricolor) //左侧
                {
                    okloc = false;
                    rowtemp = i;
                    coltemp = j - 2;
                    while (coltemp >= 0)
                    {
                        if (chessboard[rowtemp][coltemp] == ricolor) //对手的棋
                            coltemp--;
                        else if (chessboard[rowtemp][coltemp] == -1) //空格
                            break;
                        else if (chessboard[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                        {
                            yesloc[num].first = i;
                            yesloc[num].second = j;
                            num++;
                            okloc = true;
                            break; //跳出当前while和if,开始判断下一个位置
                        }
                    }
                    if (okloc) continue; //如果是的话，开始判断下一个位置
                }
                

                if (j + 1 <= 7 && chessboard[i][j + 1] == ricolor) //右侧
                {
                    okloc = false;
                    rowtemp = i;
                    coltemp = j + 2;
                    while (coltemp <= 7)
                    {
                        if (chessboard[rowtemp][coltemp] == ricolor) //对手的棋
                            coltemp++;
                        else if (chessboard[rowtemp][coltemp] == -1) //空格
                            break;
                        else if (chessboard[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                        {
                            yesloc[num].first = i;
                            yesloc[num].second = j;
                            num++;
                            okloc = true;
                            break; //跳出当前while和if,开始判断下一个位置
                        }
                    }
                    if (okloc) continue; //如果是的话，开始判断下一个位置
                }


                if (j - 1 >= 0 && i - 1 >= 0 && chessboard[i - 1][j - 1] == ricolor) //左上侧
                {
                    okloc = false;
                    rowtemp = i - 2;
                    coltemp = j - 2;
                    while (coltemp >= 0 && rowtemp >= 0)
                    {
                        if (chessboard[rowtemp][coltemp] == ricolor) //对手的棋
                        {
                            rowtemp--;
                            coltemp--;
                        }
                        else if (chessboard[rowtemp][coltemp] == -1) //空格
                            break;
                        else if (chessboard[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                        {
                            yesloc[num].first = i;
                            yesloc[num].second = j;
                            num++;
                            okloc = true;
                            break; //跳出当前while和if,开始判断下一个位置
                        }
                    }
                    if (okloc) continue; //如果是的话，开始判断下一个位置
                }


                if (j + 1 <= 7 && i - 1 >= 0 && chessboard[i - 1][j + 1] == ricolor) //右上侧
                {
                    okloc = false;
                    rowtemp = i - 2;
                    coltemp = j + 2;
                    while (coltemp <= 7 && rowtemp >= 0)
                    {
                        if (chessboard[rowtemp][coltemp] == ricolor) //对手的棋
                        {
                            rowtemp--;
                            coltemp++;
                        }
                        else if (chessboard[rowtemp][coltemp] == -1) //空格
                            break;
                        else if (chessboard[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                        {
                            yesloc[num].first = i;
                            yesloc[num].second = j;
                            num++;
                            okloc = true;
                            break; //跳出当前while和if,开始判断下一个位置
                        }
                    }
                    if (okloc) continue; //如果是的话，开始判断下一个位置
                }


                if (j + 1 <= 7 && i + 1 <= 7 && chessboard[i + 1][j + 1] == ricolor) //右下侧
                {
                    okloc = false;
                    rowtemp = i + 2;
                    coltemp = j + 2;
                    while (coltemp <= 7 && rowtemp <= 7)
                    {
                        if (chessboard[rowtemp][coltemp] == ricolor) //对手的棋
                        {
                            rowtemp++;
                            coltemp++;
                        }
                        else if (chessboard[rowtemp][coltemp] == -1) //空格
                            break;
                        else if (chessboard[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                        {
                            yesloc[num].first = i;
                            yesloc[num].second = j;
                            num++;
                            okloc = true;
                            break; //跳出当前while和if,开始判断下一个位置
                        }
                    }
                    if (okloc) continue; //如果是的话，开始判断下一个位置
                }


                if (j - 1 >= 0 && i + 1 <= 7 && chessboard[i + 1][j - 1] == ricolor) //左下侧
                {
                    okloc = false;
                    rowtemp = i + 2;
                    coltemp = j - 2;
                    while (coltemp >= 0 && rowtemp <= 7)
                    {
                        if (chessboard[rowtemp][coltemp] == ricolor) //对手的棋
                        {
                            rowtemp++;
                            coltemp--;
                        }
                        else if (chessboard[rowtemp][coltemp] == -1) //空格
                            break;
                        else if (chessboard[rowtemp][coltemp] == color) //出现了己方的棋，把对方的一堆棋夹住了
                        {
                            yesloc[num].first = i;
                            yesloc[num].second = j;
                            num++;
                            okloc = true;
                            break; //跳出当前while和if,开始判断下一个位置
                        }
                    }
                }
            }
        }
}


void Reversi::setbanned(int row, int col, int color) //设置禁手位置
{
    iniloc(); //初始化禁手位置数组
    int num = 0;

    if (row > 0 && row < 7 && col > 0 && col < 7) //4个的
    {
        if (chessboard[row][col - 1] == -1) //左
        {
            bannedloc[num].first = row;
            bannedloc[num].second = col-1;
            num++;
        }

        if (chessboard[row][col + 1] == -1) //右
        {
            bannedloc[num].first = row;
            bannedloc[num].second = col + 1;
            num++;
        }

        if (chessboard[row-1][col] == -1) //上
        {
            bannedloc[num].first = row-1;
            bannedloc[num].second = col;
            num++;
        }

        if (chessboard[row+1][col] == -1) //下
        {
            bannedloc[num].first = row+1;
            bannedloc[num].second = col;
            num++;
        }
    }



    else if (row == 0 && col > 0 && col < 7) //上边
    {
        if (chessboard[row][col - 1] == -1) //左
        {
            bannedloc[num].first = row;
            bannedloc[num].second = col - 1;
            num++;
        }

        if (chessboard[row][col + 1] == -1) //右
        {
            bannedloc[num].first = row;
            bannedloc[num].second = col + 1;
            num++;
        }

        if (chessboard[row + 1][col] == -1) //下
        {
            bannedloc[num].first = row + 1;
            bannedloc[num].second = col;
            num++;
        }
    }


    else if (row == 7 && col > 0 && col < 7) //下边
    {
        if (chessboard[row][col - 1] == -1) //左
        {
            bannedloc[num].first = row;
            bannedloc[num].second = col - 1;
            num++;
        }

        if (chessboard[row][col + 1] == -1) //右
        {
            bannedloc[num].first = row;
            bannedloc[num].second = col + 1;
            num++;
        }

        if (chessboard[row - 1][col] == -1) //上
        {
            bannedloc[num].first = row - 1;
            bannedloc[num].second = col;
            num++;
        }
    }


    else if (col == 0 && row > 0 && row < 7) //左边
    {
        if (chessboard[row - 1][col] == -1) //上
        {
            bannedloc[num].first = row - 1;
            bannedloc[num].second = col;
            num++;
        }

        if (chessboard[row + 1][col] == -1) //下
        {
            bannedloc[num].first = row + 1;
            bannedloc[num].second = col;
            num++;
        }

        if (chessboard[row][col + 1] == -1) //右
        {
            bannedloc[num].first = row;
            bannedloc[num].second = col + 1;
            num++;
        }
    }


    else if (col == 7 && row > 0 && row < 7) //右边
    {
        if (chessboard[row - 1][col] == -1) //上
        {
            bannedloc[num].first = row - 1;
            bannedloc[num].second = col;
            num++;
        }

        if (chessboard[row + 1][col] == -1) //下
        {
            bannedloc[num].first = row + 1;
            bannedloc[num].second = col;
            num++;
        }

        if (chessboard[row][col - 1] == -1) //左
        {
            bannedloc[num].first = row;
            bannedloc[num].second = col - 1;
            num++;
        }
    }



    else if (row == 0 && col == 0) //左上角
    {
        if (chessboard[row + 1][col] == -1) //下
        {
            bannedloc[num].first = row + 1;
            bannedloc[num].second = col;
            num++;
        }

        if (chessboard[row][col + 1] == -1) //右
        {
            bannedloc[num].first = row;
            bannedloc[num].second = col + 1;
            num++;
        }
    }


    else if (row == 0 && col == 7) //右上角
    {
        if (chessboard[row + 1][col] == -1) //下
        {
            bannedloc[num].first = row + 1;
            bannedloc[num].second = col;
            num++;
        }

        if (chessboard[row][col - 1] == -1) //左
        {
            bannedloc[num].first = row;
            bannedloc[num].second = col - 1;
            num++;
        }
    }


    else if (row == 7 && col == 0) //左下角
    {
        if (chessboard[row][col + 1] == -1) //右
        {
            bannedloc[num].first = row;
            bannedloc[num].second = col + 1;
            num++;
        }

        if (chessboard[row - 1][col] == -1) //上
        {
            bannedloc[num].first = row - 1;
            bannedloc[num].second = col;
            num++;
        }
    }


    else if (row == 7 && col == 7) //右下角
    {
        if (chessboard[row - 1][col] == -1) //上
        {
            bannedloc[num].first = row - 1;
            bannedloc[num].second = col;
            num++;
        }

        if (chessboard[row][col - 1] == -1) //左
        {
            bannedloc[num].first = row;
            bannedloc[num].second = col - 1;
            num++;
        }
    }
}


void Reversi::flip(int row, int col, int color) //翻转对方的棋子
{
    int rowtemp = row, coltemp = col;
    int rowend = -1, colend = -1; //翻转终止的位置
    bool flippable = false; //判断可翻转与否
    int colortemp = 1 - color; //对方棋子的颜色


    if (2 <= row && row <= 5 && col <= 1) //区块1
    {
        dir_up(row, col, rowtemp, coltemp, rowend, color, colortemp, flippable); //上
        dir_down(row, col, rowtemp, coltemp, rowend, color, colortemp, flippable); //下
        dir_right(row, col, rowtemp, coltemp, colend, color, colortemp, flippable); //右
        dir_rightup(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //右上
        dir_rightdown(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //右下
    }
    

    else if (2 <= row && row <= 5 && 6 <= col) //区块2
    {
        dir_up(row, col, rowtemp, coltemp, rowend, color, colortemp, flippable); //上
        dir_down(row, col, rowtemp, coltemp, rowend, color, colortemp, flippable); //下
        dir_left(row, col, rowtemp, coltemp, colend, color, colortemp, flippable); //左
        dir_leftup(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //左上
        dir_leftdown(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //左下
    }


    else if (1 >= row && 2 <= col && col <= 5) //区块3
    {
        dir_left(row, col, rowtemp, coltemp, colend, color, colortemp, flippable); //左
        dir_right(row, col, rowtemp, coltemp, colend, color, colortemp, flippable); //右
        dir_down(row, col, rowtemp, coltemp, rowend, color, colortemp, flippable); //下
        dir_leftdown(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //左下
        dir_rightdown(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //右下
    }


    else if (6 <= row && 2 <= col && col <= 5) //区块4
    {
        dir_left(row, col, rowtemp, coltemp, colend, color, colortemp, flippable); //左
        dir_right(row, col, rowtemp, coltemp, colend, color, colortemp, flippable); //右
        dir_up(row, col, rowtemp, coltemp, rowend, color, colortemp, flippable); //上
        dir_leftup(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //左上
        dir_rightup(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //右上
    }


    else if (2 <= row && row <= 5 && 2 <= col && col <= 5) //区块9
    {
        dir_up(row, col, rowtemp, coltemp, rowend, color, colortemp, flippable); //上
        dir_down(row, col, rowtemp, coltemp, rowend, color, colortemp, flippable); //下
        dir_left(row, col, rowtemp, coltemp, colend, color, colortemp, flippable); //左
        dir_right(row, col, rowtemp, coltemp, colend, color, colortemp, flippable); //右
        dir_leftup(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //左上
        dir_rightup(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //右上
        dir_rightdown(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //右下
        dir_leftdown(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //左下
    }


    else if (row <= 1 && col <= 1) //区块5
    {
        dir_right(row, col, rowtemp, coltemp, colend, color, colortemp, flippable); //右
        dir_down(row, col, rowtemp, coltemp, rowend, color, colortemp, flippable); //下
        dir_rightdown(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //右下
    }


    else if (row <= 1 && col >= 6) //区块6
    {
        dir_down(row, col, rowtemp, coltemp, rowend, color, colortemp, flippable); //下
        dir_left(row, col, rowtemp, coltemp, colend, color, colortemp, flippable); //左
        dir_leftdown(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //左下
    }


    else if (row >= 6 && col <= 1) //区块7
    {
        dir_right(row, col, rowtemp, coltemp, colend, color, colortemp, flippable); //右
        dir_up(row, col, rowtemp, coltemp, rowend, color, colortemp, flippable); //上
        dir_rightup(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //右上
    }


    else if (row >= 6 && col >= 6) //区块8
    {
        dir_left(row, col, rowtemp, coltemp, colend, color, colortemp, flippable); //左
        dir_up(row, col, rowtemp, coltemp, rowend, color, colortemp, flippable); //上
        dir_leftup(row, col, rowtemp, coltemp, rowend, colend, color, colortemp, flippable); //左上
    }
}


void Reversi::dir_up(const int &row, const int &col, int &rowtemp, int &coltemp, int &rowend, int &color, int &colortemp, bool &flippable)
{ //上方向
    rowtemp--; //上方向
    while (rowtemp >= 0)
    {
        if (chessboard[rowtemp][coltemp] == colortemp) //对方的棋子
            rowtemp--;
        else if (chessboard[rowtemp][coltemp] == color) //己方的棋子
        {
            rowend = rowtemp;
            flippable = true; //可翻转
            break;
        }

        else break; //空格
    }


    if (flippable)
    {
        rowtemp = row - 1;
        while (rowtemp != rowend) //逐一翻转
        {
            chessboard[rowtemp][coltemp] = color;
            rowtemp--;
        }
    }

    rowtemp = row;
    coltemp = col;
    rowend = -1; //翻转终止的位置
    flippable = false; //判断可翻转与否
}

void Reversi::dir_down(const int &row, const int &col, int &rowtemp, int &coltemp, int &rowend, int &color, int &colortemp, bool &flippable)
{ //下方向
    rowtemp++; //下方向
    while (rowtemp <= 7)
    {
        if (chessboard[rowtemp][coltemp] == colortemp) //对方的棋子
            rowtemp++;
        else if (chessboard[rowtemp][coltemp] == color) //己方的棋子
        {
            rowend = rowtemp;
            flippable = true; //可翻转
            break;
        }

        else break; //空格
    }


    if (flippable)
    {
        rowtemp = row + 1;
        while (rowtemp != rowend) //逐一翻转
        {
            chessboard[rowtemp][coltemp] = color;
            rowtemp++;
        }
    }

    rowtemp = row;
    coltemp = col;
    rowend = -1; //翻转终止的位置
    flippable = false; //判断可翻转与否
}

void Reversi::dir_right(const int &row, const int &col, int &rowtemp, int &coltemp, int &colend, int &color, int &colortemp, bool &flippable)
{ //右方向
    coltemp++; //右方向
    while (coltemp <= 7)
    {
        if (chessboard[rowtemp][coltemp] == colortemp) //对方的棋子
            coltemp++;
        else if (chessboard[rowtemp][coltemp] == color) //己方的棋子
        {
            colend = coltemp;
            flippable = true; //可翻转
            break;
        }

        else break; //空格
    }


    if (flippable)
    {
        coltemp = col + 1;
        while (coltemp != colend) //逐一翻转
        {
            chessboard[rowtemp][coltemp] = color;
            coltemp++;
        }
    }

    rowtemp = row;
    coltemp = col;
    colend = -1; //翻转终止的位置
    flippable = false; //判断可翻转与否
}

void Reversi::dir_left(const int &row, const int &col, int &rowtemp, int &coltemp, int &colend, int &color, int &colortemp, bool &flippable)
{ //左方向
    coltemp--; //左方向
    while (coltemp >= 0)
    {
        if (chessboard[rowtemp][coltemp] == colortemp) //对方的棋子
            coltemp--;
        else if (chessboard[rowtemp][coltemp] == color) //己方的棋子
        {
            colend = coltemp;
            flippable = true; //可翻转
            break;
        }

        else break; //空格
    }


    if (flippable)
    {
        coltemp = col - 1;
        while (coltemp != colend) //逐一翻转
        {
            chessboard[rowtemp][coltemp] = color;
            coltemp--;
        }
    }

    rowtemp = row;
    coltemp = col;
    colend = -1; //翻转终止的位置
    flippable = false; //判断可翻转与否
}

void Reversi::dir_leftup(const int &row, const int &col, int &rowtemp, int &coltemp, int &rowend, int &colend, int &color, int &colortemp, bool &flippable)
{ //左上方向
    coltemp--; //左方向
    rowtemp--; //上方向
    while (coltemp >= 0 && rowtemp >= 0)
    {
        if (chessboard[rowtemp][coltemp] == colortemp) //对方的棋子
        {
            coltemp--;
            rowtemp--;
        }
        else if (chessboard[rowtemp][coltemp] == color) //己方的棋子
        {
            rowend = rowtemp;
            colend = coltemp;
            flippable = true; //可翻转
            break;
        }

        else break; //空格
    }


    if (flippable)
    {
        rowtemp = row - 1;
        coltemp = col - 1;
        while (coltemp != colend) //逐一翻转
        {
            chessboard[rowtemp][coltemp] = color;
            coltemp--;
            rowtemp--;
        }
    }

    rowtemp = row;
    coltemp = col;
    rowend = -1;
    colend = -1; //翻转终止的位置
    flippable = false; //判断可翻转与否
}

void Reversi::dir_rightdown(const int &row, const int &col, int &rowtemp, int &coltemp, int &rowend, int &colend, int &color, int &colortemp, bool &flippable)
{ //右下方向
    coltemp++; //右方向
    rowtemp++; //下方向
    while (coltemp <= 7 && rowtemp <= 7)
    {
        if (chessboard[rowtemp][coltemp] == colortemp) //对方的棋子
        {
            coltemp++;
            rowtemp++;
        }
        else if (chessboard[rowtemp][coltemp] == color) //己方的棋子
        {
            rowend = rowtemp;
            colend = coltemp;
            flippable = true; //可翻转
            break;
        }

        else break; //空格
    }


    if (flippable)
    {
        rowtemp = row + 1;
        coltemp = col + 1;
        while (coltemp != colend) //逐一翻转
        {
            chessboard[rowtemp][coltemp] = color;
            coltemp++;
            rowtemp++;
        }
    }

    rowtemp = row;
    coltemp = col;
    rowend = -1;
    colend = -1; //翻转终止的位置
    flippable = false; //判断可翻转与否
}

void Reversi::dir_rightup(const int &row, const int &col, int &rowtemp, int &coltemp, int &rowend, int &colend, int &color, int &colortemp, bool &flippable)
{ //右上方向
    coltemp++; //右方向
    rowtemp--; //上方向
    while (coltemp <= 7 && rowtemp >= 0)
    {
        if (chessboard[rowtemp][coltemp] == colortemp) //对方的棋子
        {
            coltemp++;
            rowtemp--;
        }
        else if (chessboard[rowtemp][coltemp] == color) //己方的棋子
        {
            rowend = rowtemp;
            colend = coltemp;
            flippable = true; //可翻转
            break;
        }

        else break; //空格
    }


    if (flippable)
    {
        rowtemp = row - 1;
        coltemp = col + 1;
        while (coltemp != colend) //逐一翻转
        {
            chessboard[rowtemp][coltemp] = color;
            coltemp++;
            rowtemp--;
        }
    }

    rowtemp = row;
    coltemp = col;
    rowend = -1;
    colend = -1; //翻转终止的位置
    flippable = false; //判断可翻转与否
}

void Reversi::dir_leftdown(const int &row, const int &col, int &rowtemp, int &coltemp, int &rowend, int &colend, int &color, int &colortemp, bool &flippable)
{ //左下方向
    coltemp--; //左方向
    rowtemp++; //下方向
    while (coltemp >= 0 && rowtemp <= 7)
    {
        if (chessboard[rowtemp][coltemp] == colortemp) //对方的棋子
        {
            coltemp--;
            rowtemp++;
        }
        else if (chessboard[rowtemp][coltemp] == color) //己方的棋子
        {
            rowend = rowtemp;
            colend = coltemp;
            flippable = true; //可翻转
            break;
        }

        else break; //空格
    }


    if (flippable)
    {
        rowtemp = row + 1;
        coltemp = col - 1;
        while (coltemp != colend) //逐一翻转
        {
            chessboard[rowtemp][coltemp] = color;
            coltemp--;
            rowtemp++;
        }
    }

    rowtemp = row;
    coltemp = col;
    rowend = -1;
    colend = -1; //翻转终止的位置
    flippable = false; //判断可翻转与否
}