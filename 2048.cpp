#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>//得分规则与原版相同，若4+4=8，就得8分。以此类推。
const int N = 4;
using namespace std;
int Mat[N][N] = { 0 }; //游戏主矩阵
int score = 0, unmovable=0; //score是玩家的得分，unmovable是每次进行整体移动前后矩阵中保持不变的元素数
int getnum(void); //生成每次移动之后的那个随机数（2或4）
int getloc(void); //生成新生成的随机数的位置
void print(void); //打印矩阵并显示得分
int Max(void); //找出当前矩阵中最大的数
void moveadd(int); //把磁贴往指定的方向移动,再做加法
                   //以下代码中i均为Mat矩阵的行，j均为Mat矩阵的列
int main()
{ 
    int num = 2, loc = getloc(), input=0; //以下几行：生成开局的那两个2
    Mat[loc / 4][loc % 4] = num;
    loc = getloc();
    Mat[loc / 4][loc % 4] = num;
    print();

    while (Max() < 2048) //游戏正式开始
    {
        input = _getch(); input = _getch(); //一定要两次！调试了很久的心得。
        if (isascii(input));
        system("cls");

        moveadd(input); //移动并做加法
        if (unmovable != 16) //如果等于16，则每行或列都与操作前相同，这次操作无效，不产生新的数字
        {
            loc = getloc(); //然后产生一个随机数（2或4）的位置
            num = getnum(); //产生2或4
            Mat[loc / 4][loc % 4] = num; //赋值
        }
        print();
    }

    cout << "\n\n\n" << "You've won! Wanna go on?" <<
 "\n Type \"0\" to quit, \"1\" to continue. \n\n"; // 继续游戏
    cin >> input;
    
    if (input == 1)
    {
        while (Max() < 32768)
        {
            input = _getch(); input = _getch(); 
            if (isascii(input));
            system("cls");

            moveadd(input); //移动并做加法
            if (unmovable != 16)
            {
                loc = getloc(); //然后产生一个随机数（2或4）的位置
                num = getnum(); //产生2或4
                Mat[loc / 4][loc % 4] = num; //赋值
            }
            print();
        }
        cout << "\n\nOK, that's enough!!";
        getchar();
        getchar();
    }

    return 0;
}


int getnum(void) //生成每次移动之后的那个随机数（2或4）
{
    srand(time(0));
    int k = (int)rand();
    if (k % 2 == 0)  k = 4;
    else  k = 2;
    return k;
}


int getloc(void) //生成新生成的随机数的位置:0到15之间的一个数
{
    srand(time(0));
    int k=(int)rand() % 16;
    while (Mat[k / 4][k % 4] != 0)
    {
        k = (int)rand() % 16;
    }

    return k;
}


void print(void) //打印矩阵并显示得分
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << Mat[i][j] << "\t";
        }
        cout << "\n\n\n";
    }
    cout << "当前的得分： " << score;
}


int Max(void) //找出当前矩阵中最大的数
{
    int k = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (Mat[i][j] > k) k = Mat[i][j];
        }
    }
    return k;
}


void moveadd(int a) //把磁贴往指定的方向移动,再做加法
{
    unmovable = 0;
     
    if (a == 77) // 方向“右”键 
    {
        for (int i = 0; i < N; i++) //先移动，再做加法
        {
            int temp1 = N - 1; 
            int temp[N] = { 0 }, origin[N] = { 0 };//origin是这一行 / 列在移动前的样子
            for (int j = 0; j < N; j++)  origin[j] = Mat[i][j];//先保存移动前的样子
            
            for (int j = N - 1; j >= 0; j--) //这儿开始为移动的第一步
            {//思路：从要移向的方向开始，遇见不为零的元素，就把其赋值给temp数组中相应的位置
                if (Mat[i][j] != 0)
                {
                    temp[temp1] = Mat[i][j];
                    temp1--;
                }
            }

            for (int k = 0; k < N; k++) //此为移动的第二步。
            {//把移动后的数组“归还”给Mat矩阵中相应的行或列
                Mat[i][k] = temp[k];
            }

            for (int j = N - 1; j > 0; j--) //此为做加法
            {
                if (Mat[i][j] != 0 && Mat[i][j] == Mat[i][j - 1])
                {
                    Mat[i][j] *= 2;
                    score += Mat[i][j];
                    for (int k = j - 1; k > 0; k--)
                    {
                        Mat[i][k] = Mat[i][k - 1];//整体地移动
                    }
                    Mat[i][0] = 0;//对应的末尾的元素清零
                }
            }

            for (int j = 0; j < N; j++) { if (origin[j] == Mat[i][j]) unmovable++; }
//若移动并做加法后，这一列/行与原先相同，则unmovable的值增加4。4次循环后若值为16，说明该矩阵不变。
        }
    }

    else if (a == 75) // 方向“左”键 
    {
        for (int i = 0; i < N; i++)
        {
            int temp1 = 0;
            int temp[N] = { 0 }, origin[N] = { 0 };
            for (int j = 0; j < N; j++)  origin[j] = Mat[i][j];
            for (int j = 0; j < N; j++)
            {
                if (Mat[i][j] != 0)
                {
                    temp[temp1] = Mat[i][j];
                    temp1++;
                }
            }

            for (int k = 0; k < N; k++)
            {
                Mat[i][k] = temp[k];
            }

            for (int j = 0; j < N; j++) //此为做加法
            {
                if (Mat[i][j] != 0 && Mat[i][j] == Mat[i][j + 1])
                {
                    Mat[i][j] *= 2;
                    score += Mat[i][j];
                    for (int k = j + 1; k < N - 1; k++)
                    {
                        Mat[i][k] = Mat[i][k + 1];
                    }
                    Mat[i][N-1] = 0;
                }
            }
            
            for (int j = 0; j < N; j++) { if (origin[j] == Mat[i][j]) unmovable++; }
        }
    }

    else if (a == 72) // 方向“上”键 
    {
        for (int j = 0; j < N; j++)
        {
            int temp1 = 0;
            int temp[N] = { 0 }, origin[N] = { 0 };
            for (int i = 0; i < N; i++)  origin[i] = Mat[i][j];
            for (int i = 0; i < N; i++)
            {
                if (Mat[i][j] != 0)
                {
                    temp[temp1] = Mat[i][j];
                    temp1++;
                }
            }

            for (int k = 0; k < N; k++)
            {
                Mat[k][j] = temp[k];
            }

            for (int i = 0; i < N; i++) //此为做加法
            {
                if (Mat[i][j] != 0 && Mat[i][j] == Mat[i+1][j])
                {
                    Mat[i][j] *= 2;
                    score += Mat[i][j];
                    for (int k = i + 1; k < N - 1; k++)
                    {
                        Mat[k][j] = Mat[k+1][j];
                    }
                    Mat[N - 1][j] = 0;
                }
            }

            for (int i = 0; i < N; i++) { if (origin[i] == Mat[i][j]) unmovable++; }
        }
    }

    else if (a == 80) // 方向“下”键 
    {
        for (int j = 0; j < N; j++)
        {
            int temp1 = N-1;
            int temp[N] = { 0 }, origin[N] = { 0 };
            for (int i = 0; i < N; i++)  origin[i] = Mat[i][j];
            for (int i = N-1; i >=0; i--)
            {
                if (Mat[i][j] != 0)
                {
                    temp[temp1] = Mat[i][j];
                    temp1--;
                }
            }

            for (int k = 0; k < N; k++)
            {
                Mat[k][j] = temp[k];
            }

            for (int i = N - 1; i >= 0; i--) //此为做加法
            {
                if (Mat[i][j] != 0 && Mat[i][j] == Mat[i-1][j])
                {
                    Mat[i][j] *= 2;
                    score += Mat[i][j];
                    for (int k = i - 1; k > 0; k--)
                    {
                        Mat[k][j] = Mat[k-1][j];
                    }
                    Mat[0][j] = 0;
                }
            }

            for (int i = 0; i < N; i++) { if (origin[i] == Mat[i][j]) unmovable++; }
        }
    }
}