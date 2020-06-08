#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef admin_user_H
#define admin_user_H
#include <cstring>
#define Max 101

struct goods_info //������Ʒ��Ϣ
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

struct sale_list //�����۳��嵥
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

struct account//�����û��˻�
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


    void print_submenu(); //��ӡ����Ա�µĴμ��˵�
    bool log(); //���ƹ���Ա�ĵ�¼��ע��
    void view_goods(); //��Ʒ��Ϣ��ѯ����ʾ������Ʒ����Ϣ
    void add_goods(); //�����Ʒ��������������Ʒ
    void del_goods(); //ɾ����Ʒ���ڿ����ɾ����Ʒ
    void mod_goods(); //�޸���Ʒ����Ϣ���޸ļ۸���������Ϣ
    void check_sale_list(); //�鿴�۳��嵥
    void new_act(); //�����µĻ����ջ, ����ʾ�������û��Ĳ˵�
    void new_ad(); //�����µĹ�棬��ʾ���û���ÿһ��������


private:
    bool log_info; //ȷ�ϵ�¼���
    goods_info stock[100]; //��������Ʒ����Ϣ
    sale_list salelist[201]; //���������嵥
};


class user
{
public:
    user();

    void print_submenu(); //��ӡ�û��µĴμ��˵�
    bool log(); //�û��ĵ�¼��ע��
    bool useregister(); //�û���ע��
    void mod_addr(); //�޸��ջ��ַ
    void view_goods(admin& adm); //��Ʒ��Ϣ�鿴
    void search_goods(admin& adm); //��Ʒ����
    void add_to_cart(admin& adm); //�����Ʒ�����ﳵ
    void view_cart(); //�鿴���ﳵ
    void del_goods_in_cart(); //ɾ�����ﳵ�е�ĳ��Ʒ
    void clear_cart(); //��չ��ﳵ
    void view_history(); //�鿴�����¼
    void dead(); //ȥ���
    void checkout(admin& adm); //����
    void mod_password(); //�޸ĵ�¼����


private:
    account user_acc[Max]; //�����û��˻���Ϣ
    goods_info cart[101]; //���湺�ﳵ��Ϣ
    char address[5][51]; //�����û����ջ��ַ
    char login_user_name[21]; //���浱ǰ��½���û���
    bool log_info; //ȷ�ϵ�¼���
    char no_ad_users[20][21]; //�����Ѿ�������ȥ�����û�
    bool havead; //�Ƿ���ȥ����û�
};

#endif