#define _CRT_SECURE_NO_WARNINGS
#include "admin_user.h"
#include <cstdio>
#include <iostream>
#include <iomanip>
#define NUM_OF_X 90
#define voidnum 4
using namespace std;

char adminid[6] = "admin";
char password[7] = "mypass";
char activity[101]; //储存活动提醒
char ad[181]; //储存广告

admin::admin() //admin类的构造函数
{
    log_info = false;

    FILE *fp = fopen("已售清单.txt", "r");
    FILE *fp2 = fopen("库存.txt", "r");
    FILE *act = fopen("activity.txt", "r");
    FILE *ads = fopen("ad.txt", "r");

    for (int i = 0; !feof(fp); i++)
    {
        fscanf(fp, "%s", salelist[i].ID);
        fscanf(fp, "%s", salelist[i].name); 
        fscanf(fp, "%s", salelist[i].brand);
        fscanf(fp, "%f", &salelist[i].price);
        fscanf(fp, "%d", &salelist[i].amount);
        fscanf(fp, "%s", salelist[i].user_name);
    }

    for (int i = 0; !feof(fp2); i++)
    {
        fscanf(fp2, "%s", stock[i].ID);
        fscanf(fp2, "%s", stock[i].name);
        fscanf(fp2, "%s", stock[i].brand);
        fscanf(fp2, "%f", &stock[i].price);
        fscanf(fp2, "%d", &stock[i].amount);
    }

    fgets(activity, 130, act);
    fgets(ad, 130, ads);

    fclose(act);
    fclose(fp);
    fclose(fp2);
    fclose(ads);
}


void admin::print_submenu() //打印管理员下的次级菜单
{
    cout << endl;
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    cout << "0. 注销登录  1. 查询商品  2. 增进商品  3. 删除商品  4. 修改商品信息  5. 销售清单  6. 更改通知  7. 广告\n";
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    cout << "输入操作：";
}


bool admin::log() //控制管理员的登录、注销
{
    if (log_info == false)
    {
        char inputID[10], inputpass[10];
        cout << "输入管理员ID: ";
        cin >> inputID;
        cout << "输入管理员密码: ";
        cin >> inputpass;

        if (strcmp(inputID, adminid) == 0 && strcmp(inputpass, password) == 0)
        {
            log_info = true;
            cout << "登录成功！\n";
            return true;
        }

        else
        {
            cout << "登录失败！\n";
            return false;
        }
    }

    else
    {
        log_info = false;
        return false;
    }
}


void admin::view_goods() //商品信息查询：显示所有商品的信息,不包括已删除的商品
{
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    printf("%s%18s%15s%15s%15s\n", "ID", "名称", "品牌", "价格", "数量");
    for (int i = 0; stock[i].amount != -2; i++)
    {
        /*if (stock[i].amount != -1)
        {
            cout << stock[i].ID << "\t\t" << stock[i].name;
            if (strlen(stock[i].name) >= voidnum)
                cout << "\t";
            else
                cout << "\t\t";

            cout << stock[i].brand;
            if (strlen(stock[i].brand) >= voidnum)
                cout << "\t";
            else
                cout << "\t\t";

            cout << fixed << setprecision(1) << stock[i].price << "\t\t" << stock[i].amount;

            cout << endl;
        } */
        printf("%5s%15s%15s%15.1f%15d\n", stock[i].ID, stock[i].name, stock[i].brand, stock[i].price, stock[i].amount);
    }

    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;
}


void admin::add_goods() //添加商品：往库存中添加商品.已删除的商品可重新上架，ID不变
{
    goods_info newgood;
    cout << "请依次输入想要添加的商品的名称，品牌，价格，数量。用回车来分割你的输入。\n";
    cin >> newgood.name >> newgood.brand >> newgood.price >> newgood.amount;
    
    if (newgood.amount <= 0 || newgood.brand <= 0)
    {
        cout << "输入不合法，无法添加！\n";
        return;
    }

    bool preowning = false;
    int temp = 0, stocklen = 0;
    for (int i = 0; stock[i].amount != -2; i++)
    {
        stocklen++;
        if (strcmp(stock[i].brand, newgood.brand) == 0 && strcmp(stock[i].name, newgood.name) == 0)
        {
            preowning = true;
            temp = i;
            break;
        }
    }

    if (preowning)
    {
        if (stock[temp].amount == -1) //已删除的商品重新上架，ID不变
            stock[temp].amount = newgood.amount;
        else //已存在则增加数量
            stock[temp].amount += newgood.amount;
        
        FILE *fp = fopen("库存.txt", "w");
        for (int i = 0; stock[i].amount != -2; i++)
        {
            fprintf(fp, "%s\t", stock[i].ID);
            fprintf(fp, "%s\t", stock[i].name); 
            fprintf(fp, "%s\t", stock[i].brand);
            fprintf(fp, "%.1f\t", stock[i].price);
            fprintf(fp, "%d", stock[i].amount);

            if (stock[i + 1].amount != -2)
                fputc('\n', fp);
        }

        fclose(fp);
    }
    
    else
    {
        strcpy(newgood.ID, stock[stocklen - 1].ID);
        if (newgood.ID[4] == '9')
        {
            if (newgood.ID[3] == '9')
            {
                if (newgood.ID[2] == '9')
                {
                    newgood.ID[1]++;
                    newgood.ID[2] = '0';
                    newgood.ID[3] = '0';
                    newgood.ID[4] = '0';
                }

                else
                {
                    newgood.ID[2]++;
                    newgood.ID[3] = '0';
                    newgood.ID[4] = '0';
                }
            }

            else
            {
                newgood.ID[3]++;
                newgood.ID[4] = '0';
            }
        }

        else
            newgood.ID[4]++;

        stock[stocklen] = newgood;

        FILE *fp = fopen("库存.txt", "a");
        fputc('\n', fp);
        fprintf(fp, "%s\t", newgood.ID);
        fprintf(fp, "%s\t", newgood.name); 
        fprintf(fp, "%s\t", newgood.brand); 
        fprintf(fp, "%.1f\t", newgood.price); 
        fprintf(fp, "%d", newgood.amount);

        fclose(fp);
    }

    cout << "添加成功！\n";
}


void admin::del_goods() //删除商品：在库存中删除商品,同时删除用户的购物车中的这些商品
{
    goods_info newgood;
    cout << "请依次输入想要删除的商品的名称，品牌。用回车来分割你的输入。\n";
    cin >> newgood.name >> newgood.brand;

    bool found = false;
    int loc = 0;
    for (int i = 0; stock[i].amount != -2; i++)
    {
        if (stock[i].amount != -1)
        {
            if (strcmp(stock[i].brand, newgood.brand) == 0 && strcmp(stock[i].name, newgood.name) == 0)
            {
                found = true;
                loc = i;
                break;
            }
        }
    }

    if (found)
    {
        stock[loc].amount = -1;
        FILE *fp = fopen("库存.txt", "w");
        for (int i = 0; stock[i].amount != -2; i++)
        {
            fprintf(fp, "%s\t", stock[i].ID);
            fprintf(fp, "%s\t", stock[i].name);
            fprintf(fp, "%s\t", stock[i].brand);
            fprintf(fp, "%.1f\t", stock[i].price);
            fprintf(fp, "%d", stock[i].amount);

            if (stock[i + 1].amount != -2)
                fputc('\n', fp);
        }
        fclose(fp); //在库存中删除商品

        
        //以下要实现删除所有用户的购物车中该项商品的记录
        account allusers[Max]; //所有用户的账户信息
        FILE *user = fopen("用户.txt", "r");
        for (int i = 0; !feof(user); i++)
        {
            fscanf(fp, "%s", allusers[i].ID);
            fscanf(fp, "%s", allusers[i].password);
        }
        fclose(user);


        goods_info user_cart[100]; //储存用户的购物车信息，以便重新写入
        for (int i = 0; strcmp(allusers[i].ID, "X") != 0; i++) //删除所有用户的购物车中的这个所要删除的商品的记录
        {
            char cart_name[27];
            strcpy(cart_name, allusers[i].ID);
            strcat(cart_name, ".txt");
            FILE *fp = fopen(cart_name, "r");
            for (int i = 0; !feof(fp); i++) //载入购物车记录
            {
                fscanf(fp, "%s", user_cart[i].ID);
                fscanf(fp, "%s", user_cart[i].name);
                fscanf(fp, "%s", user_cart[i].brand);
                fscanf(fp, "%f", &user_cart[i].price);
                fscanf(fp, "%d", &user_cart[i].amount);
            }
            fclose(fp);

            bool in_this_cart = false;
            int to_del_loc = 0; //要删除的购物车记录的位置
            for (int i = 0; user_cart[i].amount != -2; i++)
                if (strcmp(user_cart[i].ID, stock[loc].ID) == 0)
                {
                    in_this_cart = true;
                    to_del_loc = i;
                    break;
                }


            if (in_this_cart)
            {
                user_cart[to_del_loc].amount = -1; //“删除”该商品的购物车记录

                FILE *fp = fopen(cart_name, "w"); //重新写入购物车记录
                for (int i = 0; user_cart[i].amount != -2; i++)
                {
                    if (user_cart[i].amount != -1) //被删掉的记录不会被写入
                    {
                        fprintf(fp, "%s\t", user_cart[i].ID);
                        fprintf(fp, "%s\t", user_cart[i].name);
                        fprintf(fp, "%s\t", user_cart[i].brand);
                        fprintf(fp, "%.1f\t", user_cart[i].price);
                        fprintf(fp, "%d", user_cart[i].amount);

                        if (user_cart[i + 1].amount != -2)
                            fputc('\n', fp);
                    }
                }
                fclose(fp);
            }


            for (int i = 0; i < 100; i++) //清空这个临时缓存
                user_cart[i].amount = -2;
        }


        cout << "删除成功！\n";
    }

    else
        cout << "该商品不存在或已被删除！ 无法删除。\n";
}


void admin::mod_goods() //修改商品的信息：修改价格、数量等信息。不可以修改已下架的商品的信息以重新上架。
{
    goods_info newgood;
    cout << "请依次输入想要修改的商品的名称，品牌。用回车来分割你的输入。\n";
    cin >> newgood.name >> newgood.brand;

    bool found = false;
    int loc = 0;
    for (int i = 0; stock[i].amount != -2; i++)
    {
        if (stock[i].amount != -1)
        {
            if (strcmp(stock[i].brand, newgood.brand) == 0 && strcmp(stock[i].name, newgood.name) == 0)
            {
                found = true;
                loc = i;
                break;
            }
        }
    }

    if (found)
    {
        cout << stock[loc].brand << "的" << stock[loc].name << "现在的价格和数量分别为：\n"\
            << stock[loc].price << ",  " << stock[loc].amount << "\n请依次输入你要修改成的价格和数量：\n";

        float price_alter = 0.0;
        int amount_alter = 0;
        cin >> price_alter >> amount_alter;

        if (amount_alter < 0) //如果操作者调皮，输入了小于0的数量，则函数立刻终止
        {
            cout << "不能删除商品！\n";
            return;
        }

        stock[loc].price = price_alter;
        stock[loc].amount = amount_alter;

        FILE *fp = fopen("库存.txt", "w");
        for (int i = 0; stock[i].amount != -2; i++)
        {
            fprintf(fp, "%s\t", stock[i].ID);
            fprintf(fp, "%s\t", stock[i].name);
            fprintf(fp, "%s\t", stock[i].brand);
            fprintf(fp, "%.1f\t", stock[i].price);
            fprintf(fp, "%d", stock[i].amount);

            if (stock[i + 1].amount != -2)
                fputc('\n', fp);
        }

        fclose(fp);


        //以下要实现修改所有用户的购物车中该商品的信息
        account allusers[Max]; //所有用户的账户信息
        FILE *user = fopen("用户.txt", "r");
        for (int i = 0; !feof(user); i++)
        {
            fscanf(fp, "%s", allusers[i].ID);
            fscanf(fp, "%s", allusers[i].password);
        }
        fclose(user);


        goods_info user_cart[100]; //储存用户的购物车信息，以便重新写入
        for (int i = 0; strcmp(allusers[i].ID, "X") != 0; i++) //修改所有用户的购物车中的这个所要修改的商品的记录
        {
            char cart_name[27];
            strcpy(cart_name, allusers[i].ID);
            strcat(cart_name, ".txt");
            FILE *fp = fopen(cart_name, "r");
            for (int i = 0; !feof(fp); i++) //载入购物车记录
            {
                fscanf(fp, "%s", user_cart[i].ID);
                fscanf(fp, "%s", user_cart[i].name);
                fscanf(fp, "%s", user_cart[i].brand);
                fscanf(fp, "%f", &user_cart[i].price);
                fscanf(fp, "%d", &user_cart[i].amount);
            }
            fclose(fp);

            bool in_this_cart = false;
            int to_mod_loc = 0; //要修改的购物车记录的位置
            for (int i = 0; user_cart[i].amount != -2; i++)
                if (strcmp(user_cart[i].ID, stock[loc].ID) == 0)
                {
                    in_this_cart = true;
                    to_mod_loc = i;
                    break;
                }


            if (in_this_cart)
            {
                user_cart[to_mod_loc].price = stock[loc].price; //“修改”该商品的购物车记录
                user_cart[to_mod_loc].amount = stock[loc].amount;

                FILE *fp = fopen(cart_name, "w"); //重新写入购物车记录
                for (int i = 0; user_cart[i].amount != -2; i++)
                {
                    fprintf(fp, "%s\t", user_cart[i].ID);
                    fprintf(fp, "%s\t", user_cart[i].name);
                    fprintf(fp, "%s\t", user_cart[i].brand);
                    fprintf(fp, "%.1f\t", user_cart[i].price);
                    fprintf(fp, "%d", user_cart[i].amount);

                    if (user_cart[i + 1].amount != -2)
                        fputc('\n', fp);
                }
                fclose(fp);
            }


            for (int i = 0; i < 100; i++) //清空这个临时缓存
                user_cart[i].amount = -2;
        }


        cout << "修改成功！\n";
    }

    else
        cout << "没有该商品，无法修改！\n";
}


void admin::check_sale_list() //查看售出清单
{
    goods_info templist[201]; //暂时储存储存销售清单
    for (int i = 0; salelist[i].amount != -2; i++)
    {
        strcpy(templist[i].ID, salelist[i].ID);
        strcpy(templist[i].name, salelist[i].name);
        strcpy(templist[i].brand, salelist[i].brand);
        templist[i].price = salelist[i].price;
        templist[i].amount = salelist[i].amount;
    }


    for (int i = 0; templist[i].amount != -2; i++) //按ID升序进行排序
    {
        int sn = i;
        char data[20];
        strcpy(data, templist[i].ID);
        for (int j = i + 1; templist[j].amount != -2; j++)
            if (strcmp(templist[j].ID, data) < 0)
            {
                sn = j;
                strcpy(data, templist[j].ID);
            }

        if (sn != i)
        {
            goods_info temp;
            strcpy(temp.ID, templist[i].ID);
            strcpy(temp.name, templist[i].name);
            strcpy(temp.brand, templist[i].brand);
            temp.price = templist[i].price;
            temp.amount = templist[i].amount;

            strcpy(templist[i].ID, templist[sn].ID);
            strcpy(templist[i].name, templist[sn].name);
            strcpy(templist[i].brand, templist[sn].brand);
            templist[i].price = templist[sn].price;
            templist[i].amount = templist[sn].amount;

            strcpy(templist[sn].ID, temp.ID);
            strcpy(templist[sn].name, temp.name);
            strcpy(templist[sn].brand, temp.brand);
            templist[sn].price = temp.price;
            templist[sn].amount = temp.amount;
        }
    }

    for (int i = 0; templist[i].amount != -2; i++) //相同价格和ID：数量求和
        if (templist[i].amount != -1)
            for (int j = i + 1; templist[j].amount != -2; j++)
                if (templist[j].amount != -1)
                    if (strcmp(templist[j].ID, templist[i].ID) == 0 && templist[i].price == templist[j].price)
                    {
                        templist[i].amount += templist[j].amount;
                        templist[j].amount = -1;
                    }


    cout << "\n销售清单如下：\n";
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    //cout << "ID\t\t名称\t\t品牌\t\t价格\t\t数量\n";
    printf("%s%18s%15s%15s%15s\n", "ID", "名称", "品牌", "价格", "数量");
    for (int i = 0; templist[i].amount != -2; i++)
        if (templist[i].amount != -1)
        {
            /*cout << templist[i].ID << "\t\t" << templist[i].name;
            if (strlen(templist[i].name) >= voidnum)
                cout << "\t";
            else
                cout << "\t\t";

            cout << templist[i].brand;
            if (strlen(templist[i].brand) >= voidnum)
                cout << "\t";
            else
                cout << "\t\t";

            cout << fixed << setprecision(1) << templist[i].price << "\t\t" << templist[i].amount;

            cout << endl;*/

            printf("%5s%15s%15s%15.1f%15d\n", templist[i].ID, templist[i].name, templist[i].brand, templist[i].price, templist[i].amount);
        }

    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;
}


void admin::new_act() //发布新的活动或清空活动, 以显示在所有用户的菜单
{
    cout << "请选择您的操作: 1. 清空通知      2. 更新通知\n";
    int choice = 0;
    cin >> choice;

    switch (choice)
    {
    case 1: 
    {
        for (int i = 0; i < 101; i++)
            activity[i] = '\0';

        FILE *fp = fopen("activity.txt", "w");
        fclose(fp);
        cout << "已成功清空！\n";
        break;
    }

    case 2:
    {
        cout << "请输入您要发布的活动通知。不超过50个字。\n";
        cin >> activity;

        FILE *fp = fopen("activity.txt", "w");
        fputs(activity, fp);
        fputc('\n', fp);
        fclose(fp);

        cout << "完成! 所有顾客都将在其菜单中看到这个通知。\n";
        break;
    }

    default: break;
    }
}


void admin::new_ad() //发布新的广告，显示在用户的每一步操作中
{
    cout << "请选择您的操作: 1. 清空广告      2. 更新广告\n";
    int choice = 0;
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        for (int i = 0; i < 181; i++)
            ad[i] = '\0';

        FILE *fp = fopen("ad.txt", "w");
        fclose(fp);
        cout << "已成功清空！\n";
        break;
    }

    case 2:
    {
        cout << "请输入您要发布的广告。不超过90个字。\n";
        cin >> ad;

        FILE *fp = fopen("ad.txt", "w");
        fputs(ad, fp);
        fputc('\n', fp);
        fclose(fp);

        cout << "完成! 所有未购买去广告的顾客在进行操作时都将看到这个广告。\n";
        break;
    }

    default: break;
    }
}


/*================================================================================================*/
/*================================================================================================*/


user::user() //user类的构造函数
{
    log_info = false;
    havead = true; //默认是有广告的

    FILE *fp = fopen("用户.txt", "r");
    for (int i = 0; !feof(fp); i++)
    {
        fscanf(fp, "%s", user_acc[i].ID);
        fscanf(fp, "%s", user_acc[i].password);
    }
    fclose(fp);

    for (int i = 0; i < 5; i++) //初始化收货地址列表
        strcpy(address[i], "X"); 

    for (int i = 0; i < 20; i++) //初始化去广告用户的列表
        strcpy(no_ad_users[i], "X");

    FILE *fp1 = fopen("deaduser.txt", "r"); //读入去广告的用户
    for (int i = 0; !feof(fp1); i++)
        fscanf(fp1, "%s", no_ad_users[i]);
    fclose(fp1);
}


bool user::log() //用户的登录、注销
{
    if (log_info == false)
    {
        char inputID[21], inputpass[21];
        cout << "输入用户的ID: ";
        cin >> inputID;
        cout << "输入密码: ";
        cin >> inputpass;

        bool exist_user = false; //判断用户是否存在于注册名单里
        for (int i = 0; strcmp(user_acc[i].ID, "X") != 0; i++)
            if (strcmp(inputID, user_acc[i].ID) == 0 && strcmp(inputpass, user_acc[i].password) == 0)
            {
                exist_user = true;
                break;
            }
        
        if (exist_user)
        {
            cout << "登录成功！\n";
            log_info = true;
            strcpy(login_user_name, inputID); 

            for (int i = 0; strcmp(no_ad_users[i], "X") != 0; i++) //执行去广告判断
                if (strcmp(no_ad_users[i], login_user_name) == 0)
                {
                    havead = false;
                    break;
                }

            char cart_name[27];
            strcpy(cart_name, login_user_name);
            strcat(cart_name, ".txt");

            char user_addr[30];
            strcpy(user_addr, login_user_name);
            strcat(user_addr, "_addr.txt");

            FILE *addr = fopen(user_addr, "r"); //打开收货地址文件
            FILE *fp = fopen(cart_name, "r"); //打开购物车文件
            for (int i = 0; !feof(fp); i++) //载入购物车
            {
                fscanf(fp, "%s", cart[i].ID);
                fscanf(fp, "%s", cart[i].name);
                fscanf(fp, "%s", cart[i].brand);
                fscanf(fp, "%f", &cart[i].price);
                fscanf(fp, "%d", &cart[i].amount);
            }
            

            for (int i = 0; !feof(addr); i++) //载入收货地址
                fgets(address[i], 100, addr);

            fclose(fp);
            fclose(addr);
            return true;
        }

        else
        {
            cout << "登录失败！用户不存在或用户名或密码错误。\n";
            return false;
        }
    }


    else
    {
        for (int i = 0; i < 101; i++) //还原购物车
            cart[i].amount = -2;

        for (int i = 0; i < 5; i++) //还原收货地址列表
            strcpy(address[i], "X");

        log_info = false;
        havead = true;
        return false;
    }
}


bool user::useregister() //用户的注册
{
    char new_name[21], new_password[21];
    cout << "请输入用户名: ";
    cin >> new_name;
    cout << "请输入密码: ";
    cin >> new_password;

    for (int i = 0; new_name[i] != '\0'; i++)
        if (new_name[i] < '0' || (new_name[i] > '9' && new_name[i] < 'a') || new_name[i] > 'z')
        {
            cout << "该用户名不合法，无法注册！\n";
            return false;
        }

    for (int i = 0; new_password[i] != '\0'; i++)
        if (new_password[i] < '0' || (new_password[i] > '9' && new_password[i] < 'a') || new_password[i] > 'z')
        {
            cout << "该密码不合法，无法注册！\n";
            return false;
        }

    for (int i = 0; strcmp(user_acc[i].ID, "X") != 0; i++)
    {
        if (strcmp(new_name, user_acc[i].ID) == 0) //与现有的用户重复
        {
            cout << "这个用户名已被注册！\n";
            return false;
        }
    }


    strcpy(login_user_name, new_name);
    FILE *fp = fopen("用户.txt", "a");
    fputc('\n', fp);
    fprintf(fp, "%s\t", new_name);
    fprintf(fp, "%s", new_password);
    fclose(fp);

    int len_users = 0;
    //要把这个新注册的用户的账户信息添加到程序运行时刻的user_acc里
    for (int i = 0; strcmp(user_acc[i].ID, "X") != 0; i++)
        len_users++;
    strcpy(user_acc[len_users].ID, new_name);
    strcpy(user_acc[len_users].password, new_password);


    char user_cart[27]; //创建新用户的购物车文件
    strcpy(user_cart, new_name);
    strcat(user_cart, ".txt");
    FILE *cart = fopen(user_cart, "w");
    fclose(cart);

    char user_addr[30]; //创建新用户的收货地址文件
    strcpy(user_addr, new_name);
    strcat(user_addr, "_addr.txt");
    FILE *addr = fopen(user_addr, "w"); 
    fclose(addr);

    char user_hist[30]; //创建新用户的购买记录文件
    strcpy(user_hist, new_name);
    strcat(user_hist, "_hist.txt");
    FILE *hist = fopen(user_hist, "w");
    fclose(hist);

    log_info = true;
    cout << "注册成功！现已登录。\n";
    return true;
}


void user::print_submenu() //打印用户下的次级菜单
{
    cout << endl;
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    cout << "1. 注销登录  2. 查看商品  3. 商品搜索  4. 添加商品至购物车  5. 删除购物车中的某个商品\n\
6. 清空购物车  7. 查看购物车  8. 编辑或删除收货地址  9. 查看购买记录  10. 结账  11. 购买永久去广告\n\
12. 修改密码 \n\n";

    cout << "店铺活动：";
    if (strlen(activity) == 0)
        cout << "店铺最近无活动\n\n";
    else
        cout << activity << endl << endl;

    if (strlen(ad) != 0 && havead) //广告
        cout << "广告： " << ad << endl;

    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    cout << "输入操作：";
}


void user::view_goods(admin& adm) //用户的商品信息查看
{
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    if (strlen(ad) != 0 && havead) //广告
        cout << "广告： " << ad << endl;

    printf("%s%18s%15s%15s%15s\n", "ID", "名称", "品牌", "价格", "数量");
    for (int i = 0; adm.stock[i].amount != -2; i++)
        if (adm.stock[i].amount > 0) //已被删除或数量为0的商品应被排除
            printf("%5s%15s%15s%15.1f%15d\n", adm.stock[i].ID, \
                adm.stock[i].name, adm.stock[i].brand, adm.stock[i].price, adm.stock[i].amount);

    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;
}


void user::search_goods(admin& adm) //商品搜索
{
    if (strlen(ad) != 0 && havead) //广告
        cout  << "\n广告： " << ad << endl << endl;

    cout << "1. 搜索名称    2. 搜索品牌\n请选择:";
    int choice = -1;
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        char tosearch[20];
        cout << "请输入想要搜索的商品的名称: ";
        cin >> tosearch;
        bool found = false; //判断用户输入的名称是否存在于库存中

        for (int i = 0; adm.stock[i].amount != -2; i++)
            if (adm.stock[i].amount > 0) //已被删除或数量为0的商品应被排除
                if (strcmp(tosearch, adm.stock[i].name) == 0)
                {
                    found = true;
                    printf("%5s%15s%15s%15.1f%15d\n", \
                    adm.stock[i].ID, adm.stock[i].name, adm.stock[i].brand, adm.stock[i].price, adm.stock[i].amount);
                }

        if (!found)
            cout << "该名称的商品不存在！\n";
        
        break;
    }

    case 2:
    {
        char tosearch[20];
        cout << "请输入想要搜索的商品的品牌: ";
        cin >> tosearch;
        bool found = false; //判断用户输入的品牌是否存在于库存中

        for (int i = 0; adm.stock[i].amount != -2; i++)
            if (adm.stock[i].amount > 0) //已被删除或数量为0的商品应被排除
                if (strcmp(tosearch, adm.stock[i].brand) == 0)
                {
                    found = true;
                    printf("%5s%15s%15s%15.1f%15d\n", \
                    adm.stock[i].ID, adm.stock[i].name, adm.stock[i].brand, adm.stock[i].price, adm.stock[i].amount);
                }

        if (!found)
            cout << "该品牌的商品不存在！\n";

        break;
    }

    default: break;
    }
}


void user::add_to_cart(admin& adm) //添加商品进购物车
{
    if (strlen(ad) != 0 && havead) //广告
        cout << "\n广告： " << ad << endl << endl;

    char addID[20]; //要添加的商品的ID
    int addnum = 0; //要添加的数量

    bool found = false; //判断用户输入的商品的名称是否存在于库存中
    int loc = 0; //记录找到的那个库存商品的位置
    cout << "输入商品ID和数量。用回车来分割你的输入：\n";
    cin >> addID >> addnum;

    if (addnum <= 0)
    {
        cout << "输入的数量不合法，无法添加！\n";
        return;
    }

    for (int i = 0; adm.stock[i].amount != -2; i++)
        if (adm.stock[i].amount > 0) //已被删除或数量为0的商品应被排除
            if (strcmp(addID, adm.stock[i].ID) == 0)
            {
                found = true;
                loc = i;
                break;
            }


    if (!found)
        cout << "该商品不存在，无法添加至购物车！\n";

    else
    {
        bool in_cart = false; //商品是否已存在于购物车
        int cart_loc = 0; //存于购物车中的位置
        for (int i = 0; cart[i].amount != -2; i++)
            if (cart[i].amount != -1) //已在购物车中被删除的商品不予显示
                if (strcmp(addID, cart[i].ID) == 0)
                {
                    in_cart = true;
                    cart_loc = i;
                    break;
                }
            

        char cart_file_name[27];
        strcpy(cart_file_name, login_user_name);
        strcat(cart_file_name, ".txt"); //将要打开的购物车文件的名字

        if (in_cart)
        {
            if (adm.stock[loc].amount < (addnum + cart[cart_loc].amount))
            {
                cout << "库存的数量不够你想要添加的数量，故只能添加" << adm.stock[loc].amount - cart[cart_loc].amount\
                    << "件商品至购物车。\n";

                cart[cart_loc].amount = adm.stock[loc].amount;
            }

            else
                cart[cart_loc].amount += addnum;


            FILE *fp = fopen(cart_file_name, "w"); //重新写入购物车
            for (int i = 0; cart[i].amount != -2; i++)
            {
                if (cart[i].amount != -1)
                {
                    fprintf(fp, "%s\t", cart[i].ID);
                    fprintf(fp, "%s\t", cart[i].name);
                    fprintf(fp, "%s\t", cart[i].brand);
                    fprintf(fp, "%.1f\t", cart[i].price);
                    fprintf(fp, "%d", cart[i].amount);

                    if (cart[i + 1].amount != -2)
                        fputc('\n', fp);
                }
            }
            fclose(fp);

            printf("已成功添加%d件%s的%s到购物车中！\n", cart[cart_loc].amount, cart[cart_loc].brand, cart[cart_loc].name);
        }
        

        else //要添加的商品不在现有的购物车中
        {
            for (int i = 0; cart[i].amount != -2; i++)
                cart_loc++;

            if (addnum > adm.stock[loc].amount)
            {
                cout << "库存中仅有" << adm.stock[loc].amount << "件" << adm.stock[loc].brand << "的" \
                    << adm.stock[loc].name << ", 因此只能添加" <<\
                    adm.stock[loc].amount << "件到购物车中。\n";

                cart[cart_loc].amount = adm.stock[loc].amount;
            }


            else
                cart[cart_loc].amount = addnum;

            strcpy(cart[cart_loc].brand, adm.stock[loc].brand);
            strcpy(cart[cart_loc].name, adm.stock[loc].name);
            strcpy(cart[cart_loc].ID, adm.stock[loc].ID);
            cart[cart_loc].price = adm.stock[loc].price;

            FILE *fp = fopen(cart_file_name, "a"); //在购物车文件末尾中添加
            fputc('\n', fp);
            fprintf(fp, "%s\t", cart[cart_loc].ID);
            fprintf(fp, "%s\t", cart[cart_loc].name);
            fprintf(fp, "%s\t", cart[cart_loc].brand);
            fprintf(fp, "%.1f\t", cart[cart_loc].price);
            fprintf(fp, "%d", cart[cart_loc].amount);
            fclose(fp);

            printf("已成功添加%d件%s的%s到购物车中！\n", cart[cart_loc].amount, cart[cart_loc].brand, cart[cart_loc].name);
        }
    }
}


void user::view_cart() //查看购物车
{
    if (strlen(ad) != 0 && havead) //广告
        cout << "\n广告： " << ad << endl << endl;

    int n_l = 0;
    for (int i = 0; cart[i].amount != -2; i++)
        if (cart[i].amount > 0)
            n_l++;

    if (n_l == 0)
    {
        cout << "购物车空空如也，还是先去购物吧！\n";
        return;
    }

    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    printf("%s%18s%15s%15s%15s\n", "ID", "名称", "品牌", "价格", "数量");
    for (int i = 0; cart[i].amount != -2; i++)
        if (cart[i].amount != -1)
        {
            /*cout << cart[i].ID << "\t\t" << cart[i].name << "\t\t"\
                << cart[i].brand << "\t\t" << fixed << setprecision(1) << cart[i].price\
                << "\t\t" << cart[i].amount;
            cout << endl; */
            printf("%5s%15s%15s%15.1f%15d\n", cart[i].ID, cart[i].name, cart[i].brand, cart[i].price, cart[i].amount);
        }

    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;
}


void user::del_goods_in_cart() //删除购物车中的某商品
{
    if (strlen(ad) != 0 && havead) //广告
        cout << "\n广告： " << ad << endl << endl;

    int n_l = 0;
    for (int i = 0; cart[i].amount != -2; i++)
        if (cart[i].amount > 0)
            n_l++;

    if (n_l == 0)
    {
        cout << "购物车空空如也，还是先去购物吧！\n";
        return;
    }

    int choice = 0; //记录用户输入的查找类型
    cout << "1. 按商品名称进行删除   2. 按商品的品牌进行删除   3. 按名称和品牌进行删除\n请输入：";
    cin >> choice;
    bool del_suc = false; //判断是否已删除

    switch (choice)
    {
    case 1:
    {
        char delname[21]; //要删除的商品的名称
        int delnum = 0; //要删除的数量

        cout << "请输入要删除的商品的名称: ";
        cin >> delname;
        int num = 0;
        for (int i = 0; cart[i].amount != -2; i++)
            if (cart[i].amount > 0) //已被删除或数量为0的商品应被排除
            {
                if (strcmp(delname, cart[i].name) == 0)
                {
                    num++;
                    cout << "找到如下匹配项：\n";
                    cout << cart[i].ID << "\t\t" << cart[i].name << "\t\t"\
                        << cart[i].brand << "\t\t" << fixed << setprecision(1) << cart[i].price\
                        << "\t\t" << cart[i].amount;
                    cout << endl;
                }
            }

        if (num == 0)
            cout << "该商品不在购物车中，无法删除！\n";

        else
        {
            cout << "请输入你要删除的" << delname << "的品牌及其数量。用回车来分割输入：";
            char delbrand[21]; //要删除的商品的品牌名
            cin >> delbrand >> delnum;

            bool found = false;
            for (int i = 0; cart[i].amount != -2; i++)
                if (cart[i].amount > 0) //已被删除或数量为0的商品应被排除
                {
                    if (strcmp(delbrand, cart[i].brand) == 0 && strcmp(delname, cart[i].name) == 0)
                    {
                        found = true;
                        if (delnum < cart[i].amount) //要删除的数量小于购物车中的数量
                        {
                            cart[i].amount -= delnum; //删除一定的数量
                            break;
                        }
                        
                        else
                        {
                            if (delnum > cart[i].amount) //要删除的数量大于购物车中的数量
                            {
                                cout << "购物车中只有" << cart[i].amount << "件" << cart[i].brand \
                                    << "的" << cart[i].name << "！ 请注意您的输入:)\n";
                            }

                            cart[i].amount = -1; //正式删除，该记录清空
                            break;
                        }
                    }
                }

            if (found)
            {
                del_suc = true;
                cout << "删除成功！\n";
            }
            else
                cout << "输入错误，无法删除！\n";
        }

        break;
    }


    case 2:
    {
        char delbrand[21]; //要删除的商品的品牌
        int delnum = 0; //要删除的数量

        cout << "请输入要删除的商品的品牌: ";
        cin >> delbrand;
        int num = 0;
        for (int i = 0; cart[i].amount != -2; i++)
            if (cart[i].amount > 0) //已被删除或数量为0的商品应被排除
            {
                if (strcmp(delbrand, cart[i].brand) == 0)
                {
                    num++;
                    cout << "找到如下匹配项：\n";
                    cout << cart[i].ID << "\t\t" << cart[i].name << "\t\t"\
                        << cart[i].brand << "\t\t" << fixed << setprecision(1) << cart[i].price\
                        << "\t\t" << cart[i].amount;
                    cout << endl;
                }
            }

        if (num == 0)
            cout << "该品牌不在购物车中，无法删除！\n";

        else
        {
            cout << "请输入你要删除的" << delbrand << "的商品及其数量。用回车来分割输入：";
            char delname[21]; //要删除的商品的名称
            cin >> delname >> delnum;

            bool found = false;
            for (int i = 0; cart[i].amount != -2; i++)
                if (cart[i].amount > 0) //已被删除或数量为0的商品应被排除
                {
                    if (strcmp(delbrand, cart[i].brand) == 0 && strcmp(delname, cart[i].name) == 0)
                    {
                        found = true;
                        if (delnum < cart[i].amount) //要删除的数量小于购物车中的数量
                        {
                            cart[i].amount -= delnum; //删除一定的数量
                            break;
                        }

                        else
                        {
                            if (delnum > cart[i].amount) //要删除的数量大于购物车中的数量
                            {
                                cout << "购物车中只有" << cart[i].amount << "件" << cart[i].brand \
                                    << "的" << cart[i].name << "！ 请注意您的输入:)\n";
                            }

                            cart[i].amount = -1; //正式删除，该记录清空
                            break;
                        }
                    }
                }

            if (found)
            {
                del_suc = true;
                cout << "删除成功！\n";
            }
            
            else
                cout << "输入错误，无法删除！\n";
        }

        break;
    }


    case 3:
    {
        char delname[21]; //要删除的商品的名称
        char delbrand[21]; //要删除的商品的品牌名
        int delnum = 0; //要删除的数量

        cout << "请输入要删除的商品的品牌和名称。用回车来分割输入: " ;
        cin >> delbrand >> delname;
        int num = 0;
        int loc = 0;
        for (int i = 0; cart[i].amount != -2; i++)
            if (cart[i].amount > 0) //已被删除或数量为0的商品应被排除
            {
                if (strcmp(delbrand, cart[i].brand) == 0 && strcmp(delname, cart[i].name) == 0)
                {
                    num++;
                    loc = i;
                    cout << "找到如下匹配项：\n";
                    cout << cart[i].ID << "\t\t" << cart[i].name << "\t\t"\
                        << cart[i].brand << "\t\t" << fixed << setprecision(1) << cart[i].price\
                        << "\t\t" << cart[i].amount;
                    cout << endl;
                    break;
                }
            }

        if (num == 0)
            cout << "不存在匹配项，无法删除！\n";

        else
        {
            cout << "请输入你要删除的数量：";
            cin >> delnum;

            if (delnum < cart[loc].amount) //要删除的数量小于购物车中的数量
                cart[loc].amount -= delnum; //删除一定的数量

            else
            {
                if (delnum > cart[loc].amount) //要删除的数量大于购物车中的数量
                {
                    cout << "购物车中只有" << cart[loc].amount << "件" << cart[loc].brand \
                        << "的" << cart[loc].name << "！ 请注意您的输入:)\n";
                }

                cart[loc].amount = -1; //正式删除，该记录清空
            }

            del_suc = true;
            cout << "删除成功！\n";
        }

        break;
    }

    default: break;
    }
    
    
    if (del_suc) //成功地删除才会导致文件的重新写入
    {
        char cart_file_name[27];
        strcpy(cart_file_name, login_user_name);
        strcat(cart_file_name, ".txt"); //将要打开的购物车文件的名字

        FILE *fp = fopen(cart_file_name, "w"); //重新写入购物车
        for (int i = 0; cart[i].amount != -2; i++)
        {
            if (cart[i].amount != -1)
            {
                fprintf(fp, "%s\t", cart[i].ID);
                fprintf(fp, "%s\t", cart[i].name);
                fprintf(fp, "%s\t", cart[i].brand);
                fprintf(fp, "%.1f\t", cart[i].price);
                fprintf(fp, "%d", cart[i].amount);

                if (cart[i + 1].amount != -2)
                    fputc('\n', fp);
            }
        }
        fclose(fp);
    }
}


void user::clear_cart() //清空购物车
{
    if (strlen(ad) != 0 && havead) //广告
        cout << "\n广告： " << ad << endl << endl;

    int n_l = 0;
    for (int i = 0; cart[i].amount != -2; i++)
        if (cart[i].amount > 0)
            n_l++;

    if (n_l == 0)
    {
        cout << "购物车空空如也，还是先去购物吧！\n";
        return;
    }

    cout << "将清空目前的购物车。确定吗？\n1. 确定    2. 返回\n";
    int del = 0;
    cin >> del;

    switch (del)
    {
    case 1:
    {
        char cart_file_name[27];
        strcpy(cart_file_name, login_user_name);
        strcat(cart_file_name, ".txt"); //将要打开的购物车文件的名字

        FILE *fp = fopen(cart_file_name, "w"); //清空购物车
        fclose(fp);

        for (int i = 0; i < 101; i++)
            cart[i].amount = -2;

        cout << "清空成功!\n";
    }
    
    default: break;
    }
}


void user::view_history() //查看购买记录
{
    if (strlen(ad) != 0 && havead) //广告
        cout << "\n广告： " << ad << endl << endl;

    char hist_file_name[30];
    strcpy(hist_file_name, login_user_name);
    strcat(hist_file_name, "_hist.txt"); //将要打开的购买记录文件的名字
    FILE *hist = fopen(hist_file_name, "r+"); //打开购买记录文件
    char ch = fgetc(hist);

    if (ch == EOF)
    {
        cout << "购买记录为空!\n";
        fclose(hist);
        return;
    }

    fseek(hist, 0, SEEK_SET);
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl << "购买记录如下：\n";

    printf("%s%18s%15s%15s%15s\n", "ID", "名称", "品牌", "价格", "数量");

    goods_info temp;
    fscanf(hist, "%s", temp.ID);
    fscanf(hist, "%s", temp.name);
    fscanf(hist, "%s", temp.brand);
    fscanf(hist, "%f", &temp.price);
    fscanf(hist, "%d", &temp.amount);
    fgetc(hist);
   
   // fgetc(hist);
    
    while (!feof(hist))
    {
        printf("%5s%15s%15s%15.1f%15d\n", temp.ID, temp.name, temp.brand, temp.price, temp.amount);
        fscanf(hist, "%s", temp.ID);
        fscanf(hist, "%s", temp.name);
        fscanf(hist, "%s", temp.brand);
        fscanf(hist, "%f", &temp.price);
        fscanf(hist, "%d", &temp.amount);

        fgetc(hist);
        //ch = fgetc(hist);
    }
    fclose(hist);

    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    printf("\n");
}


void user::checkout(admin& adm) //结账
{
    if (strlen(ad) != 0 && havead) //广告
        cout << "\n广告： " << ad << endl << endl;

    float totalprice = 0; //总价
    
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    
    for (int i = 0; cart[i].amount != -2; i++)
        if (cart[i].amount != -1)
            for (int j = 0; adm.stock[j].amount != -2; j++)
                if (adm.stock[j].amount > 0)
                    if (strcmp(cart[i].ID, adm.stock[j].ID) == 0)
                        if (cart[i].amount > adm.stock[j].amount)
                        {
                            printf("购物车中%s的%s的数量超过了库存中的容量，故只能购买库存中仅剩的%d件\n", \
                                cart[i].brand, cart[i].name, adm.stock[j].amount);

                            cart[i].amount = adm.stock[j].amount;
                        }


    printf("%s%18s%15s%15s%15s\n", "ID", "名称", "品牌", "价格", "数量");
    for (int i = 0; cart[i].amount != -2; i++)
        if (cart[i].amount != -1)
        {
            totalprice += (cart[i].price * cart[i].amount);
            printf("%5s%15s%15s%15.1f%15d\n", cart[i].ID, cart[i].name, cart[i].brand, cart[i].price, cart[i].amount);
        }

    if (totalprice == 0)
    {
        cout << "购物车空空如也，还是先去购物吧！\n";
        return;
    }

    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    cout << "一共需要付款" << totalprice << "元。 确认付款请按1， 否则请按0： ";
    int choice = -1;
    cin >> choice;


    if (choice)
    {
        if (strcmp(address[0], "X") == 0)
        {
            cout << "您还没有添加过收货地址，请现在添加，注意不要超过25个字哦！\n请输入：";
            char temp[51]; //暂时存放收获地址
            cin >> temp;

            strcpy(address[0], temp);

            char user_addr[30];
            strcpy(user_addr, login_user_name);
            strcat(user_addr, "_addr.txt");

            FILE *addr = fopen(user_addr, "w"); //重新写入收货地址文件
            for (int i = 0; strcmp(address[i], "X") != 0; i++)
            {
                fputs(address[i], addr);
                if (strcmp(address[i + 1], "X") != 0)
                    fputc('\n', addr);
            }
            fclose(addr);

            cout << "添加成功, 将使用该地址作为您本次购物的收货地址！祝您生活愉快！\n";
        }

        else
        {
            cout << "请选择收获地址：\n";
            for (int i = 0; strcmp(address[i], "X") != 0; i++)
                printf("%d. %s\n", i+1, address[i]);

            int choi = 0;
            cin >> choi;

            printf("您已选择 %s 作为您本次购物的收获地址。谢谢！祝您生活愉快！\n", address[choi-1]);
        }


        for (int i = 0; cart[i].amount != -2; i++) //修改程序中的库存
            if (cart[i].amount != -1) //不包括已被删除的购物车商品
                for (int j = 0; adm.stock[j].amount != -2; j++)
                    if (adm.stock[j].amount > 0) //已被删除或数量为0的商品应被排除
                        if (strcmp(cart[i].ID, adm.stock[j].ID) == 0)
                        {
                            adm.stock[j].amount -= cart[i].amount;
                            break;
                        }


        FILE *fp = fopen("库存.txt", "w"); //重新写入库存
        for (int i = 0; adm.stock[i].amount != -2; i++)
        {
            fprintf(fp, "%s\t", adm.stock[i].ID);
            fprintf(fp, "%s\t", adm.stock[i].name);
            fprintf(fp, "%s\t", adm.stock[i].brand);
            fprintf(fp, "%.1f\t", adm.stock[i].price);
            fprintf(fp, "%d", adm.stock[i].amount);

            if (adm.stock[i + 1].amount != -2)
                fputc('\n', fp);
        }
        fclose(fp);


        int len_salelist = 0; //已售清单的长度
        for (int i = 0; adm.salelist[i].amount != -2; i++)
            len_salelist++;

        for (int i = 0; cart[i].amount != -2; i++) //修改程序中的售出清单
            if (cart[i].amount != -1) //不包括已被删除的购物车商品
            {
                strcpy(adm.salelist[len_salelist].ID, cart[i].ID);
                strcpy(adm.salelist[len_salelist].name, cart[i].name);
                strcpy(adm.salelist[len_salelist].brand, cart[i].brand);
                adm.salelist[len_salelist].amount = cart[i].amount;
                adm.salelist[len_salelist].price = cart[i].price;
                strcpy(adm.salelist[len_salelist].user_name, login_user_name);

                len_salelist++;
            }


        FILE *fp1 = fopen("已售清单.txt", "w"); //重新写入已售清单
        for (int i = 0; adm.salelist[i].amount != -2; i++)
        {
            fprintf(fp, "%s\t", adm.salelist[i].ID);
            fprintf(fp, "%s\t", adm.salelist[i].name);
            fprintf(fp, "%s\t", adm.salelist[i].brand);
            fprintf(fp, "%.1f\t", adm.salelist[i].price);
            fprintf(fp, "%d\t", adm.salelist[i].amount);
            fprintf(fp, "%s", adm.salelist[i].user_name);

            if (adm.salelist[i + 1].amount != -2)
                fputc('\n', fp);
        }
        fclose(fp1);



        char hist_file_name[30];
        strcpy(hist_file_name, login_user_name);
        strcat(hist_file_name, "_hist.txt"); //将要打开的购买记录文件的名字
        FILE *hist = fopen(hist_file_name, "a"); //添加购买记录
        
        for (int i = 0; cart[i].amount != -2; i++) 
            if (cart[i].amount != -1)
            {
                fprintf(hist, "%s\t", cart[i].ID);
                fprintf(hist, "%s\t", cart[i].name);
                fprintf(hist, "%s\t", cart[i].brand);
                fprintf(hist, "%.1f\t", cart[i].price);
                fprintf(hist, "%d", cart[i].amount);

                if (cart[i + 1].amount != -2)
                    fputc('\n', hist);
            }

        fputc('\n', hist);
        fputc('\n', hist);
        fclose(hist);



        char cart_file_name[27];
        strcpy(cart_file_name, login_user_name);
        strcat(cart_file_name, ".txt"); //将要打开的购物车文件的名字
        FILE *fp2 = fopen(cart_file_name, "w"); //清空购物车
        fclose(fp2);
        for (int i = 0; i < 100; i++)
            cart[i].amount = -2; //清空程序中的购物车


        cout << "\n付款成功！\n";
    }
}


void user::dead() //去广告
{
    if (strlen(ad) != 0 && havead) //广告
        cout << "\n广告： " << ad << endl << endl;

    if (!havead)
    {
        cout << "您已购买过永久去广告，无需再次购买！\n";
        return;
    }

    cout << "仅需998，您就可以享受永久去广告操作。请问您是否要购买永久去广告？\n1. 是   2. 否\n请选择：";
    int choice = -1;
    cin >> choice;

    switch (choice)
    {
    case 1 :
    {
        havead = false;
        int loc = 0;
        while (strcmp(no_ad_users[loc], "X") != 0)
            loc++;

        strcpy(no_ad_users[loc], login_user_name);
        FILE *noad = fopen("deaduser.txt", "a"); //添加去广告的用户到文件中
        fputs(login_user_name, noad);
        fputc('\n', noad);
        fclose(noad);
        
        cout << "购买成功！\n";
        break;
    }

    default: break;
    }
}


void user::mod_addr() //修改收获地址
{
    if (strlen(ad) != 0 && havead) //广告
        cout << "\n广告： " << ad << endl << endl;

    if (strcmp(address[0], "X") == 0)
        cout << "您目前没有收获地址！\n";
    else
    {
        printf("目前的收货地址为: \n");
        for (int i = 0; strcmp(address[i], "X") != 0; i++)
            printf("%d. %s\n", i + 1, address[i]);
        cout << "\n";
    }

    printf("操作：1. 删除收货地址     2. 编辑已有的收货地址\n请输入：");
    int choice = 0;
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        if (strcmp(address[0], "X") == 0)
        {
            cout << "您目前没有收获地址, 无法删除！\n";
            break;
        }

        int subchoice = 0;
        cout << "请输入您要删除的收货地址的编号: ";
        cin >> subchoice;

        if (strcmp(address[subchoice - 1], "X") == 0 || subchoice > 5)
        {
            cout << "该编号无效, 无法删除！\n";
            break;
        }

        
        int temp = subchoice - 1;
        for (; strcmp(address[temp + 1], "X") != 0 && temp < 4; temp++)
            strcpy(address[temp], address[temp + 1]);
        strcpy(address[temp], "X");

        char user_addr[30];
        strcpy(user_addr, login_user_name);
        strcat(user_addr, "_addr.txt");

        FILE *addr = fopen(user_addr, "w"); //重新写入收货地址文件
        for (int i = 0; strcmp(address[i], "X") != 0; i++)
        {
            fputs(address[i], addr);
            if (strcmp(address[i + 1], "X") != 0)
                fputc('\n', addr);
        }
        fclose(addr);

        cout << "删除成功！\n";
        break;
    }


    case 2:
    {
        if (strcmp(address[0], "X") == 0)
        {
            cout << "您目前没有收获地址, 不能修改已有的收获地址，只能进行添加！\n";
            cout << "请输入您要添加的收货地址：\n";
            char temp[51];
            cin >> temp;
            strcpy(address[0], temp);
            cout << "添加成功!\n";

            char user_addr[30];
            strcpy(user_addr, login_user_name);
            strcat(user_addr, "_addr.txt");

            FILE *addr = fopen(user_addr, "w"); //重新写入收货地址文件
            for (int i = 0; strcmp(address[i], "X") != 0; i++)
            {
                fputs(address[i], addr);
                if (strcmp(address[i + 1], "X") != 0)
                    fputc('\n', addr);
            }
            fclose(addr);

            break;
        }
        
        else
        {
            int subchoice = 0;
            cout << "请输入您要编辑的收货地址的编号: ";
            cin >> subchoice;

            if (strcmp(address[subchoice - 1], "X") == 0 || subchoice > 5)
            {
                cout << "该编号无效, 无法编辑！\n";
                break;
            }

            cout << "请输入新的收货地址：\n";
            char temp[51];
            cin >> temp;
            strcpy(address[subchoice - 1], temp);
            cout << "编辑成功!\n";

            char user_addr[30];
            strcpy(user_addr, login_user_name);
            strcat(user_addr, "_addr.txt");

            FILE *addr = fopen(user_addr, "w"); //重新写入收货地址文件
            for (int i = 0; strcmp(address[i], "X") != 0; i++)
            {
                fputs(address[i], addr);
                if (strcmp(address[i + 1], "X") != 0)
                    fputc('\n', addr);
            }
            fclose(addr);

            break;
        }
    }

    default: break;
    }
}


void user::mod_password() //修改登录密码
{
    if (strlen(ad) != 0 && havead) //广告
        cout << "\n广告： " << ad << endl << endl;

    char temp[21], temp2[21];
    cout << "请输入原来的密码：\n";
    cin >> temp;

    int loc = 0;
    for (int i = 0; strcmp(user_acc[i].ID, "X") != 0; i++)
        if (strcmp(login_user_name, user_acc[i].ID) == 0)
        {
            loc = i;
            if (strcmp(temp, user_acc[i].password) != 0)
            {
                cout << "密码错误！无法修改!\n";
                return;
            }
        }
            

    cout << "请输入新的密码\n";
    cin >> temp;

    for (int i = 0; temp[i] != '\0'; i++)
        if (temp[i] < '0' || (temp[i] > '9' && temp[i] < 'a') || temp[i] > 'z')
        {
            cout << "该密码不合法，修改失败！\n";
            return;
        }


    cout << "请再次输入新的密码\n";
    cin >> temp2;
    if (strcmp(temp, temp2) != 0)
    {
        cout << "两次输入不匹配，修改失败！\n";
        return;
    }


    strcpy(user_acc[loc].password, temp);
    FILE *fp = fopen("用户.txt", "w"); //重新写入用户账户文件
    for (int i = 0; strcmp(user_acc[i].ID, "X") != 0; i++)
    {
        fprintf(fp, "%s\t", user_acc[i].ID);
        fprintf(fp, "%s", user_acc[i].password);

        if (strcmp(user_acc[i + 1].ID, "X") != 0)
            fputc('\n', fp);
    }
    fclose(fp);

    cout << "修改成功！\n";
}