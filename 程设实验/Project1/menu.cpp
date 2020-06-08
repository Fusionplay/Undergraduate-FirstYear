#include "menu.h"
#include "admin_user.h"
#include <iostream>
using namespace std;

int generalmenu() //����ոտ�ʼ����ʱ��������
{
    int what_operater = 0;
    for (int i = 0; i < 60; i++)
        cout << "=";
    cout << endl;

    for (int i = 0; i < 18; i++)
        cout << " ";
    cout << "��ӭ������ʳƷ�꣡";
    for (int i = 0; i < 18; i++)
        cout << " ";
    cout << endl;

    for (int i = 0; i < 60; i++)
        cout << "=";
    cout << endl << endl;

    cout << "1. �û���¼   2.�û�ע��   3.����Ա��¼   4.�˳�����\n";
    cout << "���������";
    cin >> what_operater;
    return what_operater;
}


void operate_menu(int operator_type, admin& ad, user& us) //�����˵�
{
    if (operator_type == 3) //�������Ա�Ĳ���
    {
        bool login = ad.log();
        if (login)
        {
            ad.print_submenu();
            char input;
            bool logout = false;
            cin >> input;

            while (logout == false)
            {
                switch (input)
                {
                case '0':
                {
                    cout << "ע����¼��\n";
                    ad.log();
                    logout = true;
                    break;
                }

                case '1':
                {
                    ad.view_goods();
                    break;
                }

                case '2':
                {
                    ad.add_goods();
                    break;
                }

                case '3':
                {
                    ad.del_goods();
                    break;
                }

                case '4':
                {
                    ad.mod_goods();
                    break;
                }

                case '5':
                {
                    ad.check_sale_list();
                    break;
                }

                case '6':
                {
                    ad.new_act();
                    break;
                }

                case '7':
                {
                    ad.new_ad();
                    break;
                }

                default: break;
                }

                if (!logout)
                {
                    ad.print_submenu();
                    cin >> input;
                }
            }
        }
    } 


    else if (operator_type == 1) //�û���¼
    {
        bool login = us.log();
        if (login)
        {
            us.print_submenu();
            int input = 0;
            bool logout = false;
            cin >> input;

            while (logout == false)
            {
                switch (input)
                {
                case 1:
                {
                    cout << "ע����¼����л���Ļݹ�!\n";
                    us.log();
                    logout = true;
                    break;
                }

                case 2:
                {
                    us.view_goods(ad);               
                    break;
                }

                case 3:
                {
                    us.search_goods(ad);
                    break;
                }

                case 4:
                {
                    us.add_to_cart(ad);
                    break;
                }

                case 5:
                {
                    us.del_goods_in_cart();
                    break;
                }

                case 6:
                {
                    us.clear_cart();
                    break;
                }

                case 7:
                {
                    us.view_cart();
                    break;
                }

                case 8:
                {
                    us.mod_addr();
                    break;
                }

                case 9:
                {
                    us.view_history();
                    break;
                }

                case 10:
                {
                    us.checkout(ad);
                    break;
                }

                case 11:
                {
                    us.dead();
                    break;
                }

                case 12:
                {
                    us.mod_password();
                    break;
                }
                
                default: break;
                }

                if (!logout)
                {
                    us.print_submenu();
                    cin >> input;
                }
            }
        }
    }

    
    else if (operator_type == 2) //�û�ע��
    {
        bool regis_suc = us.useregister();
        if (regis_suc)
        {
            us.print_submenu();
            int input = 0;
            bool logout = false;
            cin >> input;

            while (logout == false)
            {
                switch (input)
                {
                case 1:
                {
                    cout << "ע����¼����л���Ļݹ�!\n";
                    us.log();
                    logout = true;
                    break;
                }

                case 2:
                {
                    us.view_goods(ad);
                    break;
                }

                case 3:
                {
                    us.search_goods(ad);
                    break;
                }

                case 4:
                {
                    us.add_to_cart(ad);
                    break;
                }

                case 5:
                {
                    us.del_goods_in_cart();
                    break;
                }

                case 6:
                {
                    us.clear_cart();
                    break;
                }

                case 7:
                {
                    us.view_cart();
                    break;
                }

                case 8:
                {
                    us.mod_addr();
                    break;
                }

                case 9:
                {
                    us.view_history();
                    break;
                }

                case 10:
                {
                    us.checkout(ad);
                    break;
                }

                case 11:
                {
                    us.dead();
                    break;
                }

                case 12:
                {
                    us.mod_password();
                    break;
                }

                default: break;
                }

                if (!logout)
                {
                    us.print_submenu();
                    cin >> input;
                }
            } 
        }
    }
}