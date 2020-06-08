#include <iostream>
#include<string>
#include<cstring>
#include<stack>
#include<cmath>
#include<iomanip>
#include<fstream>
using namespace std;


#define RED "\e[0;31m"
#define NONE "\e[0m"
const int N = 500;

struct complex
{
    double a;
    double b;
    void init()
    {
        a = 0;
        b = 0;
    }
    complex operator +(complex &n)
    {
        complex c;
        c.a = a + n.a;
        c.b = b + n.b;
        return c;
    }
    complex operator -(complex &n)
    {
        complex c;
        c.a = a - n.a;
        c.b = b - n.b;
        return c;
    }
    complex operator *(complex &n)
    {
        complex c;
        c.a = n.a*a - n.b*b;
        c.b = n.b*a + n.a*b;
        return c;
    }
    complex operator /(complex &n)
    {
        complex c;
        c.a = (n.a*a + n.b*b) / (n.a*n.a + n.b*n.b);
        c.b = (b*n.a - a*n.b) / (n.a*n.a + n.b*n.b);
        return c;
    }
    complex operator <(complex &n)//重载负号
    {
        complex c;
        c.a = -n.a;
        c.b = -n.b;
        return c;
    }
    double operator & (complex &n)//重载arg
    {
        double m;
        if (n.a == 0)
        {
            if (n.b > 0) m = 1.570796;
            if (n.b < 0) m = -1.570796;
            if (n.b == 0) m = 0;
            return m;
        }
        m = atan(n.b / n.a);
        return m;
    }
    complex operator > (complex &n)//重载cjg
    {
        complex c;
        c.a = n.a;
        c.b = -n.b;
        return c;
    }
    complex operator ^ (int n)//重载幂乘
    {
        complex c;
        c.a = a;
        c.b = b;
        complex d;
        d.a = a;
        d.b = b;
        if (n >= 1)
        {
            for (int i = 0; i < n - 1; ++i)
                c = c * d;
            return c;
        }
        else if (n == 0)
        {
            c.a = 1;
            c.b = 0;
            return c;
        }
        else
        {
            n = 0 - n;
            for (int i = 0; i < n - 1; ++i)
                c = c * d;
            d.a = 1;
            d.b = 0;
            c = d / c;
            return c;
        }
    }
    double operator | (complex &n)//重载求模
    {
        double c;
        c = n.a * a + n.b * b;
        c = sqrt(c);
        return c;
    }
};

struct memory
{
    string mymemory[5];
    complex result[5];
};

struct Complex_Stack
{
    int top;
    complex buffer[N];
    void Complex_Init()
    {
        top = -1;
        for (int i = 0; i < N; ++i)
            buffer[i].init();
    }
    int Is_Empty()
    {
        return top == -1 ? 1 : 0;
    }
    bool push(complex temp)
    {
        if (top == N - 1)
        {
            cout << "栈已满!\n";
            return false;
        }
        else
        {
            top++;
            buffer[top] = temp;
            return true;
        }
    } //入栈
    bool pop(complex &temp)
    {
        if (top == -1)
        {
            cout << "栈已空!\n";
            return false;
        }
        else
        {
            temp = buffer[top];
            top--;
            return true;
        }
    } //出栈
    complex getTop() {
        if (top < N)
            return buffer[top];
        exit(-1);
    }
};

struct Operator_Stack
{
    int top;
    char buffer[N];
    void Operator_Init()
    {
        top = -1;
        for (int i = 0; i < N; ++i)
            buffer[i] = '\0';
    }
    int Is_Empty()
    {
        return top == -1 ? 1 : 0;
    }
    bool push(char temp)
    {
        if (top == N - 1)
        {
            cout << "栈已满!\n";
            return false;
        }
        else
        {
            top++;
            buffer[top] = temp;
            return true;
        }
    } //入栈
    bool pop(char &temp)
    {
        if (top == -1)
        {
            cout << "栈已空!\n";
            return false;
        }
        else
        {
            temp = buffer[top];
            top--;
            return true;
        }
    } //出栈
    char getTop() {
        if (top < N)
            return buffer[top];
        exit(-1);
    }
};


memory onememory;
int rolling = 5;
int sequence = 0;
Complex_Stack c_stack;

void Mycompare(string str);
void UP();//上翻
void DOWN();//下翻
void RECORD();//展示
bool LEGAL(string &str);//输入合法性判断
bool PARENTHESES(string str);//括弧匹配
bool FOUR_LEGAL(string str);//加减乘除合法性
bool HEAD_LEGAL(string str);//开头合法性
bool TAIL_LEGAL(string str);//结尾合法性
bool REAL_LEGAL(string str);//实数合法性
bool I_LEGAL(string &str);//i合法性
bool LEFT_PAR_LEGAL(string str);//左括弧合法性
bool RIGHT_PAR_LEGAL(string str);//右括弧合法性
bool POWER_LEGAL(string str);//乘幂合法性
bool MO_COUNT(string str, int now);//模的个数
void SHOW_COLOR(string str, int i, int p = 0);//输出错误表达式
int DIGIT_AFTER(string str, int i);//找到后面的数字
int DIGIT_BEFORE(string str, int i);//招待前面的数字
string CHANGE_OPERATOR(string &str);//改变操作符的名称
int PARENTHESES_AFTER(string str, int now);//找到对应的右括号
int MO_AFTER(string str, int now);//找到对应的|
string Change_STR(string str);//中缀转后缀
int PRIORITY(char att);//优先级判断
complex Calculation(string str);//计算
double String_To_Double(string num);//string转double
void SHOW_IMA(double a, double b);//展示虚数
void myshow();
int PARENTHESES_BEFORE(string str, int now);//找到对应的右括号的位置
bool WHETHER_TWO_REALS(string str);//判断是否有多个实数相邻
bool ZERO(string str);//判断分母是否为零


int main()
{
    cout << fixed << setprecision(6);
    string str = "";
    cout << "Welcome to Complex calculator!\n";
    //cout << "输入计算表达式：\n";
    getline(cin, str);
    while (str != "quit")
    {
        Mycompare(str);
        cout << "Welcome to Complex calculator!\n";
        getline(cin, str);
    }
    return 0;
}

void Mycompare(string str)
{
    if (str == "up")
    {
        UP();
        return;
    }
    else if (str == "down")
    {
        DOWN();
        return;
    }
    else if (str == "history")
    {
        RECORD();
        return;
    }
    else
    {
        bool flag = LEGAL(str);
        if (!flag)
        {
            return;
            cout << "illegal!" << endl;
        }
        rolling = 5;
        if (sequence >= 5)
        {
            for (int i = 1; i < 5; ++i)
            {
                onememory.mymemory[i - 1] = onememory.mymemory[i];
            }
            onememory.mymemory[4] = str + "     ";
        }
        else
        {
            onememory.mymemory[sequence] = str + "     ";
        }
        //        else cout << "legal!" << endl;
        CHANGE_OPERATOR(str);
        string temp = Change_STR(str);
        //        cout << aaa << endl;
        if (sequence >= 5)
        {
            onememory.result[4] = Calculation(temp);
        }
        else
        {
            onememory.result[sequence] = Calculation(temp);
        }
        SHOW_IMA(c_stack.getTop().a, c_stack.getTop().b);
        ++sequence;
        return;
    }
}

void UP()
{
    bool flag = false;
    if (rolling == 5 || onememory.mymemory[rolling] == "")
    {
        for (int i = 4; i >= 0; --i)
        {
            if (onememory.mymemory[i] == "") rolling--;
            else
            {
                flag = true;
                rolling = i;
                //                cout << rolling  << endl;
                if (i == 0)
                {
                    cout << "无上一个表达式！\n";
                    return;
                }
                cout << onememory.mymemory[i - 1];
                SHOW_IMA(onememory.result[i - 1].a, onememory.result[i - 1].b);
                return;
            }
        }
        if (flag == false)
        {
            cout << "无上一个表达式！\n";
            return;
        }
    }
    else
    {
        if (rolling == 0)
        {
            cout << "无上一个表达式！\n";
            return;
        }
        else
        {

            if (rolling <= 1)
            {
                cout << "无上一个表达式！\n";
                return;
            }
            rolling--;
            cout << rolling << endl;
            cout << onememory.mymemory[rolling - 1];
            SHOW_IMA(onememory.result[rolling - 1].a, onememory.result[rolling - 1].b);
            return;
        }
    }
}

void DOWN()
{
    if (rolling == 5 || onememory.mymemory[rolling] == "")
    {
        cout << "无下一个表达式！\n";
        return;
    }
    else
    {
        cout << onememory.mymemory[rolling];
        SHOW_IMA(onememory.result[rolling].a, onememory.result[rolling].b);
        rolling++;
        return;
        //            }
    }
}

void RECORD()
{
    for (int i = 0; i < 5; ++i)
        if (onememory.mymemory[i] != "")
        {
            cout << onememory.mymemory[i];
            SHOW_IMA(onememory.result[i].a, onememory.result[i].b);
        }
    return;
}

bool LEGAL(string &str)
{
    //  cout << str[0] << endl;
    int length = str.size();
    //    cout << length << endl;
    for (int i = 0; i < length; ++i)
        if (!(str[i] >= '0' && str[i] <= '9') && str[i] != ' ' && str[i] != 'i' && str[i] != '(' && str[i] != ')'
            && str[i] != '.' && str[i] != '+' && str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '|'
            && str[i] != '^' && str[i] != 'a' && str[i] != 'r' && str[i] != 'g' && str[i] != 'c' && str[i] != 'j')
        {
            printf(RED "出现未知字符！\n" NONE);
            return false;
        }

    if (!PARENTHESES(str)) return false;
    if (!HEAD_LEGAL(str)) return false;
    if (!TAIL_LEGAL(str)) return false;
    if (!FOUR_LEGAL(str)) return false;
    if (!REAL_LEGAL(str)) return false;
    if (!I_LEGAL(str)) return false;
    if (!LEFT_PAR_LEGAL(str)) return false;
    if (!RIGHT_PAR_LEGAL(str)) return false;
    if (!POWER_LEGAL(str)) return false;
    if (!ZERO(str)) return false;
    return true;
}

bool PARENTHESES(string str)//括弧匹配
{
    int parentheses = 0;
    int length = str.size();
    for (int i = 0; i < length; ++i)
    {
        if (str[i] == '(') parentheses++;
        else if (str[i] == ')') parentheses--;
        if (parentheses < 0)
        {
            printf(RED "error： 括弧不匹配！\n" NONE);
            return false;
        }
    }
    if (parentheses == 0) return true;
    else
    {
        printf(RED "error： 括弧不匹配！\n" NONE);
        return false;
    }
}

bool HEAD_LEGAL(string str)//开头合法性
{
    int length = str.size();
    if (str[0] == '+' || str[0] == '*' || str[0] == '/' || str[0] == '^')
    {
        printf(RED "%c" NONE, str.c_str()[0]);
        printf("%s\n", str.c_str() + 1);
        printf(RED "error： %c在开头，错误！\n" NONE, str.c_str()[0]);
        return false;
    }
    return true;
}

bool TAIL_LEGAL(string str)//结尾合法性
{
    int length = str.size();
    if (str[length - 1] == '|' || str[length - 1] == 'i' || str[length - 1] == ')' || (str[length - 1] >= '0' && str[length - 1] <= '9'))
        return true;
    char att = str.c_str()[length - 1];
    str[length - 1] = '\0';
    printf("%s", str.c_str());
    printf(RED "%c\n" NONE, att);
    printf(RED "error： %c在结尾，错误！\n" NONE, att);
    return false;
}

bool FOUR_LEGAL(string str)//加减乘除合法性
{
    int length = str.size();
    for (int i = 0; i < length; ++i)
    {
        if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
        {
            if (str[i + 1] == ')' || str[i + 1] == '+' || str[i + 1] == '-'
                || str[i + 1] == '*' || str[i + 1] == '/' || str[i + 1] == '^')
            {
                SHOW_COLOR(str, i);
                printf(RED "error: %c与%c匹配错误！\n" NONE, str.c_str()[i], str.c_str()[i + 1]);
                return false;
            }

        }
    }
    return true;
}

bool REAL_LEGAL(string str)//实数合法性
{
    if (!WHETHER_TWO_REALS(str)) return false;
    int length = str.size();
    for (int i = 0; i < length; ++i)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            if (i == length - 1) return true;
            if (str[i + 1] == '(' || str[i + 1] == 'c' || str[i + 1] == 'a')
            {
                if (str[i + 1] == '(')
                {
                    int start = DIGIT_BEFORE(str, i);
                    SHOW_COLOR(str, start, i - start);
                    printf(RED "error: %s", str.substr(start, i - start + 1).c_str());
                    printf("与%c匹配错误！\n" NONE, str.c_str()[i + 1]);
                    return false;
                }
                if (str[i + 1] == 'c' || str[i + 1] == 'a')
                {
                    int start = DIGIT_BEFORE(str, i);
                    SHOW_COLOR(str, start, i - start + 2);
                    printf(RED "error: %s", str.substr(start, i - start + 1).c_str());
                    //                    SHOW_COLOR(str, i, 2);
                    printf("与%c%c%c匹配错误！\n" NONE, str.c_str()[i + 1], str.c_str()[i + 2], str.c_str()[i + 3]);
                    return false;
                }
            }
            if (str[i + 1] == '|')
            {
                if (MO_COUNT(str, i))
                {
                    int start = DIGIT_BEFORE(str, i);
                    SHOW_COLOR(str, start, i - start);
                    printf(RED "error: %s", str.substr(start, i - start + 1).c_str());
                    //                    SHOW_COLOR(str, i);
                    printf("与%c匹配错误！\n" NONE, str.c_str()[i + 1]);
                    return false;
                }
            }
        }
    }
    return true;
}

bool I_LEGAL(string &str)//i合法性
{
    int length = str.size();
    for (int i = 0; i < length; ++i)
    {
        if (str[i] == 'i')
        {
            if (i == length - 1) goto L;
            if (str[i + 1] == '(' || str[i + 1] == 'i')
            {
                SHOW_COLOR(str, i);
                printf(RED "error: %c与%c匹配错误！\n" NONE, str.c_str()[i], str.c_str()[i + 1]);
                return false;
            }
            if (str[i + 1] >= '0' && str[i + 1] <= '9')
            {
                int end = DIGIT_AFTER(str, i);
                SHOW_COLOR(str, i, end - i - 1);
                printf(RED "error: %c与", str.c_str()[i]);
                printf("%s匹配错误！\n" NONE, str.substr(i + 1, end - i).c_str());
                return false;
            }
            if (str[i + 1] == '|')
            {
                if (MO_COUNT(str, i))
                {
                    SHOW_COLOR(str, i);
                    printf(RED "error: %c与%c匹配错误！\n" NONE, str.c_str()[i], str.c_str()[i + 1]);
                    return false;
                }
            }
            if (i == 0) continue;
        L:            if (str[i - 1] == ')')
        {
            if (PARENTHESES_BEFORE(str, i - 1) > 0 && str[PARENTHESES_BEFORE(str, i - 1) - 1] == 'g')
            {
                string temp = str.substr(PARENTHESES_BEFORE(str, i - 1) - 3, i - PARENTHESES_BEFORE(str, i - 1) + 3);
                CHANGE_OPERATOR(temp);
                string aaa = Change_STR(temp);
                complex mytemp = Calculation(aaa);
                if (mytemp.b == 0);
                else
                {
                    SHOW_COLOR(str, i - 1);
                    printf(RED "error: %c与%c匹配错误！\n" NONE, str.c_str()[i - 1], str.c_str()[i]);
                    return false;
                }
            }
            else
            {
                string temp = str.substr(PARENTHESES_BEFORE(str, i - 1), i - PARENTHESES_BEFORE(str, i - 1));
                CHANGE_OPERATOR(temp);
                string aaa = Change_STR(temp);
                complex mytemp = Calculation(aaa);
                if (mytemp.b == 0);
                else
                {
                    SHOW_COLOR(str, i - 1);
                    printf(RED "error: %c与%c匹配错误！\n" NONE, str.c_str()[i - 1], str.c_str()[i]);
                    return false;
                }
            }
            str.insert(i, "*");
            cout << str << endl;
        }
                      if (str[i - 1] == '|')
                      {
                          str.insert(i, "*");
                      }
        }
    }
    return true;
}

bool LEFT_PAR_LEGAL(string str)//左括弧合法性
{
    int length = str.size();
    for (int i = 0; i < length; ++i)
    {
        if (str[i] == '(')
        {
            if (str[i + 1] == ')' || str[i + 1] == '+' || str[i + 1] == '*' || str[i + 1] == '/')
            {
                SHOW_COLOR(str, i);
                printf(RED "error: %c与%c匹配错误！\n" NONE, str.c_str()[i], str.c_str()[i + 1]);
                return false;
            }
        }
    }
    return true;
}

bool RIGHT_PAR_LEGAL(string str)//右括弧合法性
{
    int length = str.size();
    for (int i = 0; i < length; ++i)
    {
        if (str[i] == ')')
        {
            if (i == length - 1) return true;
            if (str[i + 1] == '(' || str[i + 1] == '|')
            {
                SHOW_COLOR(str, i);
                printf(RED "error: %c与%c匹配错误！\n" NONE, str.c_str()[i], str.c_str()[i + 1]);
                return false;
            }
            if (str[i + 1] >= '0' && str[i + 1] <= '9')
            {
                int end = DIGIT_AFTER(str, i);
                SHOW_COLOR(str, i, end - i - 1);
                printf(RED "error: %c与", str.c_str()[i]);
                printf("%s匹配错误！\n" NONE, str.substr(i + 1, end - i).c_str());
                return false;
            }
            if (str[i + 1] == 'a' || str[i + 1] == 'c')
            {
                SHOW_COLOR(str, i, 2);
                printf(RED "error: %c与%c%c%c匹配错误！\n" NONE, str.c_str()[i], str.c_str()[i + 1], str.c_str()[i + 2], str.c_str()[i + 3]);
                return false;
            }
        }
    }
    return true;
}

bool MO_COUNT(string str, int now)//模的个数
{
    int mycount = 0;
    for (int j = 0; j < now; ++j)
        if (str[j] == '|') ++mycount;
    if (mycount % 2 == 0) return true;
    return false;
}

void SHOW_COLOR(string str, int i, int p)//输出错误表达式
{
    int length = str.size();
    for (int j = 0; j < i; ++j) printf("%c", str.c_str()[j]);
    for (int j = i; j < i + p + 2; ++j) printf(RED "%c" NONE, str.c_str()[j]);
    for (int j = i + p + 2; j < length; ++j) printf("%c", str.c_str()[j]);
    printf("\n");
}

int DIGIT_AFTER(string str, int i)//找到数字
{
    int length = str.size();
    for (int j = i + 1; j < length; ++j)
    {
        if (str[j] == '.') continue;
        if (str[j] < '0' || str[j] > '9') return j - 1;
    }
    return length - 1;
}

int DIGIT_BEFORE(string str, int i)//找到前面的数字
{
    int length = str.size();
    for (int j = i - 1; j >= 0; --j)
    {
        if (str[j] == '.') continue;
        if (str[j] < '0' || str[j] > '9') return j + 1;
    }
    return 0;
}

string CHANGE_OPERATOR(string &str)//改变操作符的名称
{
    for (int i = 0; i < str.size(); ++i)//i变1i
    {
        if (str[i] == 'i' && i == 0) str.insert(i, "1");
        if (str[i] == 'i' && (str[i - 1] >'9' || str[i - 1] < '0')) str.insert(i, "1");
    }
    string mystr = str;
    int length = str.size();
    for (int i = 0; i < str.size(); ++i)
    {
        if (str[i] == 'a')
        {
            str.replace(i, 3, "&");
            int end = PARENTHESES_AFTER(str, i + 1);
            string temp = str.substr(i + 1, end - i);
            CHANGE_OPERATOR(temp);
            str.insert(i, temp);
            i = i + temp.size();
        }
        else if (str[i] == 'c')
        {
            str.replace(i, 3, ">");
            int end = PARENTHESES_AFTER(str, i + 1);
            string temp = str.substr(i + 1, end - i);
            CHANGE_OPERATOR(temp);
            str.insert(i, temp);
            i = i + temp.size();
        }
        else if (str[i] == '-')
        {
            if ((i == 0 || str[i - 1] == '(') && str[i + 1] == '(')
            {
                str.replace(i, 1, "<");
                int end = PARENTHESES_AFTER(str, i + 1);
                string temp = str.substr(i + 1, end - i);
                CHANGE_OPERATOR(temp);
                str.insert(i, temp);
                i = i + temp.size();
            }
            else if ((i == 0 || str[i - 1] == '(') && str[i + 1] != '(')
            {
                str.insert(i, "0");
                ++i;

            }
        }
        else if (str[i] == '|')
        {
            int end = MO_AFTER(str, i);
            str.insert(end, ")");
            str.insert(i + 1, "(");
            str.erase(end + 2, 1);
            string temp = str.substr(i + 1, end - i + 1);
            CHANGE_OPERATOR(temp);
            str.insert(i, temp);
            i = i + temp.size();
        }
    }
    return mystr;
}

int PARENTHESES_AFTER(string str, int now)//找到对应的右括号的位置
{
    int length = str.size();
    int mycount = 1;
    ++now;
    while (mycount != 0)
    {
        if (str[now] == '(') ++mycount;
        else if (str[now] == ')') --mycount;
        ++now;
    }
    return now - 1;
}

int MO_AFTER(string str, int now)//找到对应的|
{
    int length = str.size();
    for (int i = now + 1; i < length; ++i)
        if (str[i] == '|') return i;
}

string Change_STR(string str)//中缀转后缀
{
    Operator_Stack o_stack;
    o_stack.Operator_Init();
    string temp = "";
    int length = str.size();
    for (int i = 0; i < length; ++i)
    {
        if (str[i] == '(')//'('则压入栈
        {
            o_stack.push(str[i]);
        }
        else if (str[i] >= '0' && str[i] <= '9')//复数则输出
        {
            int end = DIGIT_AFTER(str, i);
            if (end == length - 1)
            {
                temp += str.substr(i, end - i + 1);
                temp += "#";//分隔
                break;
            }
            else
            {
                if (str[end + 1] == 'i')//虚数
                {
                    temp += str.substr(i, end - i + 2);
                    temp += "#";
                    i = end + 1;
                }
                else if (str[end + 1] != 'i')//实数
                {
                    temp += str.substr(i, end - i + 1);
                    temp += "#";
                    i = end;
                }
            }
        }
        else if (str[i] == ')')
        {
            char att = '\0';
            while (o_stack.getTop() != '(')
            {
                if (o_stack.pop(att))
                {
                    string att_string;
                    att_string = att;
                    temp += att_string;
                    temp += "#";
                }
            }
            o_stack.pop(att);
        }
        else
        {
            if (o_stack.Is_Empty())
            {
                o_stack.push(str[i]);
            }
            else
            {
                if (PRIORITY(str[i]) > PRIORITY(o_stack.getTop()))//栈顶优先级低于当前，则将当前的压入栈中
                {
                    o_stack.push(str[i]);
                }
                else if (PRIORITY(str[i]) <= PRIORITY(o_stack.getTop()))
                {
                    char att = '\0';
                    while (PRIORITY(o_stack.getTop()) >= PRIORITY(str[i]) && o_stack.getTop() != '(')
                    {
                        if (o_stack.pop(att))
                        {
                            string att_string;
                            att_string = att;
                            temp += att_string;
                            temp += "#";
                        }
                    }
                    o_stack.push(str[i]);
                }
            }
        }
    }
    while (!o_stack.Is_Empty())
    {
        char att = '\0';
        if (o_stack.pop(att))
        {
            string att_string;
            att_string = att;
            temp += att_string;
            temp += "#";
        }
    }
    return temp;
}

int PRIORITY(char att)//优先级判断
{
    if (att == '+' || att == '-') return 1;
    if (att == '*' || att == '/') return 2;
    if (att == '<' || att == '|' || att == '>' || att == '&') return 3;
    if (att == '^') return 4;
    if (att == '(') return 5;
    if (att == ')') return 0;
}

complex Calculation(string str)//计算
{
    c_stack.Complex_Init();
    int length = str.size();
    for (int i = 0; i < length; ++i)
    {
        int start = i;
        while (str[i] != '#')
        {
            ++i;
        }
        if (str[i - 1] >= '0' && str[i - 1] <= '9')//数字
        {
            string temp = str.substr(start, i - start);
            double temp2 = String_To_Double(temp);
            complex temp3;
            temp3.init();
            temp3.a = temp2;
            c_stack.push(temp3);
            /////////////////////////////////////////////////////////////////////////////////////////////////////
        }
        else if (str[i - 1] == 'i')
        {
            string temp = str.substr(start, i - start - 1);
            double temp2 = String_To_Double(temp);
            complex temp3;
            temp3.init();
            temp3.b = temp2;
            c_stack.push(temp3);
            /////////////////////////////////////////////////////////////////////////////////////////////////////
        }
        else if (str[i - 1] == '+')
        {
            complex two;
            c_stack.pop(two);
            complex one;
            c_stack.pop(one);
            complex three = one + two;
            c_stack.push(three);
        }
        else if (str[i - 1] == '-')
        {
            complex two;
            c_stack.pop(two);
            complex one;
            c_stack.pop(one);
            complex three = one - two;
            c_stack.push(three);
        }
        else if (str[i - 1] == '*')
        {
            complex two;
            c_stack.pop(two);
            complex one;
            c_stack.pop(one);
            complex three = one * two;
            c_stack.push(three);
        }
        else if (str[i - 1] == '/')
        {
            complex two;
            c_stack.pop(two);
            complex one;
            c_stack.pop(one);
            complex three = one / two;
            c_stack.push(three);
        }
        else if (str[i - 1] == '|')
        {
            complex two;
            c_stack.pop(two);
            complex one;
            c_stack.pop(one);
            double three = one | two;
            complex four;
            four.init();
            four.a = three;
            c_stack.push(four);
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
        else if (str[i - 1] == '<')
        {
            complex two;
            c_stack.pop(two);
            complex one;
            c_stack.pop(one);
            complex three = one < two;
            c_stack.push(three);
        }
        else if (str[i - 1] == '>')
        {
            complex two;
            c_stack.pop(two);
            complex one;
            c_stack.pop(one);
            complex three = one > two;
            c_stack.push(three);
        }
        else if (str[i - 1] == '&')
        {
            complex two;
            c_stack.pop(two);
            complex one;
            c_stack.pop(one);
            double three = one & two;
            complex four;
            four.init();
            four.a = three;
            c_stack.push(four);
            ////////////////////////////////////////////////////////////////////////////////////////////////////
        }
        else if (str[i - 1] == '^')
        {
            complex two;
            c_stack.pop(two);
            complex one;
            c_stack.pop(one);
            complex three = one ^ two.a;
            c_stack.push(three);
        }
    }
    return c_stack.getTop();
}

double String_To_Double(string num)
{
    char c;
    int i = 0;
    double temp = 0, temp2 = 10;
    bool whether = false;       //标记是否有小数
    int length = num.size();
    while (i < length)
    {
        c = num[i];
        if (c == '.')//包含小数
        {
            whether = true;
            i++;
            continue;
        }
        if (!whether)
        {
            temp = temp * 10 + c - '0';
        }
        else//识别小数点之后都进入这个分支
        {
            temp = temp + (c - '0') / temp2;
            temp2 *= 10;
        }
        i++;
    }
    return temp;
}

void SHOW_IMA(double a, double b)//展示虚数
{
    if (a == 0)
    {
        if (b == 0)cout << "计算结果为： " << "0" << endl;
        else if (b != 0) cout << "计算结果为： " << b << "i" << endl;
    }
    else
    {
        if (b == 0) cout << "计算结果为： " << a << endl;
        else if (b >0) cout << "计算结果为： " << a << "+" << b << "i" << endl;
        else cout << "计算结果为： " << a << b << "i" << endl;
    }
}

void myshow()
{
    for (int i = 0; i < 5; ++i)
    {
        cout << onememory.mymemory[i] << onememory.result[i].a << "  " << onememory.result[i].b << endl;
    }
}

int PARENTHESES_BEFORE(string str, int now)//找到对应的右括号的位置
{
    int mycount = 1;
    --now;
    while (mycount != 0)
    {
        if (str[now] == '(') --mycount;
        else if (str[now] == ')') ++mycount;
        --now;
    }
    return now + 1;
}

bool WHETHER_TWO_REALS(string str)//判断是否有多个实数相邻
{
    int position[2] = { 0 };
    int mycount = 0;
    int length = str.size();
    for (int i = 0; i < length; ++i)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            if (i == length - 1) return true;
            for (int j = i + 1; j < length; ++j)
            {
                if (str[j] == '.')
                {
                    position[mycount] = j;
                    mycount++;
                    if (mycount > 1)
                    {
                        SHOW_COLOR(str, position[0], position[1] - position[0] - 1);
                        printf(RED "error:多个实数相邻！\n" NONE);
                        return false;
                    }
                    continue;
                }
                if (str[j] < '0' || str[j] > '9')
                {
                    i = j;
                    mycount = 0;
                    break;
                }
            }
        }
    }
    return true;
}

bool POWER_LEGAL(string str)//乘幂合法性
{
    int length = str.size();
    for (int i = 0; i < length; ++i)
    {
        if (str[i] == '^')
        {
            if (i == length - 1) return false;
            else if (str[i + 1] == '(')
            {
                int end = PARENTHESES_AFTER(str, i + 1);
                string temp = str.substr(i + 1, end - i);
                CHANGE_OPERATOR(temp);
                string aaa = Change_STR(temp);
                complex mytemp = Calculation(aaa);
                if (mytemp.b == 0);
                else
                {
                    SHOW_COLOR(str, i, end - i - 1);
                    printf(RED "error: 乘幂指数部分为虚数！\n" NONE);
                    return false;
                }
            }
        }
    }
    return true;
}

bool ZERO(string str)//判断分母是否为零
{
    string temp = str;
    int length = temp.size();
    for (int i = 0; i < length; ++i)
    {
        if (str[i] == '/')
        {
            if (str[i + 1] == '0')
            {
                SHOW_COLOR(temp, i);
                printf(RED "分母为零!\n" NONE);
                return false;
            }
            int end = 0;
            if (str[i + 1] == '(')
            {
                end = PARENTHESES_AFTER(temp, i + 1);
                string atemp = temp.substr(i + 1, end - i);
                CHANGE_OPERATOR(atemp);
                string aaa = Change_STR(atemp);
                complex mytemp = Calculation(aaa);
                if (mytemp.a == 0 && mytemp.b == 0)
                {
                    SHOW_COLOR(str, i, end - i - 1);
                    printf(RED "error: 分母为零!\n" NONE);
                    return false;
                }
            }
            else if (str[i + 1] == 'a' || str[i + 1] == 'c')
            {
                end = PARENTHESES_AFTER(str, i + 4);
                string atemp = temp.substr(i + 1, end - i);
                CHANGE_OPERATOR(atemp);
                string aaa = Change_STR(atemp);
                complex mytemp = Calculation(aaa);
                if (mytemp.a == 0 && mytemp.b == 0)
                {
                    SHOW_COLOR(str, i, end - i - 1);
                    printf(RED "error: 分母为零!\n" NONE);
                    return false;
                }
            }
            else if (str[i + 1] == '|')
            {
                end = MO_AFTER(temp, i + 1);
                string atemp = temp.substr(i + 1, end - i);
                CHANGE_OPERATOR(atemp);
                string aaa = Change_STR(atemp);
                complex mytemp = Calculation(aaa);
                if (mytemp.a == 0 && mytemp.b == 0)
                {
                    SHOW_COLOR(str, i, end - i - 1);
                    printf(RED "error: 分母为零!\n" NONE);
                    return false;
                }
            }
        }
    }
    return true;
}