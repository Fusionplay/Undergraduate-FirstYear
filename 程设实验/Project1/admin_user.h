#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef admin_user_H
#define admin_user_H
#include <cstring>
#define Max 101

struct goods_info //储存商品信息
{
    char ID[20];
    char name[21];
    char brand[21];
    float price;
    int amount;
    goods_info()
    {
        amount = -2;
    }
};

struct sale_list //储存售出清单
{
    char ID[20];
    char name[21];
    char brand[21];
    float price;
    int amount;
    char user_name[21];
    sale_list()
    {
        amount = -2;
    }
};

struct account//储存用户账户
{
    char ID[21];
    char password[21];
    account()
    {
        strcpy(ID, "X");
        strcpy(password, "X");
    }
};


//class user;

class admin
{
public:
    admin();
    friend class user;


    void print_submenu(); //打印管理员下的次级菜单
    bool log(); //控制管理员的登录、注销
    void view_goods(); //商品信息查询：显示所有商品的信息
    void add_goods(); //添加商品：往库存中添加商品
    void del_goods(); //删除商品：在库存中删除商品
    void mod_goods(); //修改商品的信息：修改价格、数量等信息
    void check_sale_list(); //查看售出清单
    void new_act(); //发布新的活动或清空活动, 以显示在所有用户的菜单
    void new_ad(); //发布新的广告，显示在用户的每一步操作中


private:
    bool log_info; //确认登录与否
    goods_info stock[100]; //储存库存商品的信息
    sale_list salelist[201]; //储存销售清单
};


class user
{
public:
    user();

    void print_submenu(); //打印用户下的次级菜单
    bool log(); //用户的登录、注销
    bool useregister(); //用户的注册
    void mod_addr(); //修改收获地址
    void view_goods(admin& adm); //商品信息查看
    void search_goods(admin& adm); //商品搜索
    void add_to_cart(admin& adm); //添加商品进购物车
    void view_cart(); //查看购物车
    void del_goods_in_cart(); //删除购物车中的某商品
    void clear_cart(); //清空购物车
    void view_history(); //查看购买记录
    void dead(); //去广告
    void checkout(admin& adm); //结账
    void mod_password(); //修改登录密码


private:
    account user_acc[Max]; //储存用户账户信息
    goods_info cart[101]; //储存购物车信息
    char address[5][51]; //储存用户的收获地址
    char login_user_name[21]; //储存当前登陆的用户名
    bool log_info; //确认登录与否
    char no_ad_users[20][21]; //储存已经购买了去广告的用户
    bool havead; //是否是去广告用户
};

#endif