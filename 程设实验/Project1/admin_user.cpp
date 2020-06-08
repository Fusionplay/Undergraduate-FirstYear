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
char activity[101]; //��������
char ad[181]; //������

admin::admin() //admin��Ĺ��캯��
{
    log_info = false;

    FILE *fp = fopen("�����嵥.txt", "r");
    FILE *fp2 = fopen("���.txt", "r");
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


void admin::print_submenu() //��ӡ����Ա�µĴμ��˵�
{
    cout << endl;
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    cout << "0. ע����¼  1. ��ѯ��Ʒ  2. ������Ʒ  3. ɾ����Ʒ  4. �޸���Ʒ��Ϣ  5. �����嵥  6. ����֪ͨ  7. ���\n";
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    cout << "���������";
}


bool admin::log() //���ƹ���Ա�ĵ�¼��ע��
{
    if (log_info == false)
    {
        char inputID[10], inputpass[10];
        cout << "�������ԱID: ";
        cin >> inputID;
        cout << "�������Ա����: ";
        cin >> inputpass;

        if (strcmp(inputID, adminid) == 0 && strcmp(inputpass, password) == 0)
        {
            log_info = true;
            cout << "��¼�ɹ���\n";
            return true;
        }

        else
        {
            cout << "��¼ʧ�ܣ�\n";
            return false;
        }
    }

    else
    {
        log_info = false;
        return false;
    }
}


void admin::view_goods() //��Ʒ��Ϣ��ѯ����ʾ������Ʒ����Ϣ,��������ɾ������Ʒ
{
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    printf("%s%18s%15s%15s%15s\n", "ID", "����", "Ʒ��", "�۸�", "����");
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


void admin::add_goods() //�����Ʒ��������������Ʒ.��ɾ������Ʒ�������ϼܣ�ID����
{
    goods_info newgood;
    cout << "������������Ҫ��ӵ���Ʒ�����ƣ�Ʒ�ƣ��۸��������ûس����ָ�������롣\n";
    cin >> newgood.name >> newgood.brand >> newgood.price >> newgood.amount;
    
    if (newgood.amount <= 0 || newgood.brand <= 0)
    {
        cout << "���벻�Ϸ����޷���ӣ�\n";
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
        if (stock[temp].amount == -1) //��ɾ������Ʒ�����ϼܣ�ID����
            stock[temp].amount = newgood.amount;
        else //�Ѵ�������������
            stock[temp].amount += newgood.amount;
        
        FILE *fp = fopen("���.txt", "w");
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

        FILE *fp = fopen("���.txt", "a");
        fputc('\n', fp);
        fprintf(fp, "%s\t", newgood.ID);
        fprintf(fp, "%s\t", newgood.name); 
        fprintf(fp, "%s\t", newgood.brand); 
        fprintf(fp, "%.1f\t", newgood.price); 
        fprintf(fp, "%d", newgood.amount);

        fclose(fp);
    }

    cout << "��ӳɹ���\n";
}


void admin::del_goods() //ɾ����Ʒ���ڿ����ɾ����Ʒ,ͬʱɾ���û��Ĺ��ﳵ�е���Щ��Ʒ
{
    goods_info newgood;
    cout << "������������Ҫɾ������Ʒ�����ƣ�Ʒ�ơ��ûس����ָ�������롣\n";
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
        FILE *fp = fopen("���.txt", "w");
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
        fclose(fp); //�ڿ����ɾ����Ʒ

        
        //����Ҫʵ��ɾ�������û��Ĺ��ﳵ�и�����Ʒ�ļ�¼
        account allusers[Max]; //�����û����˻���Ϣ
        FILE *user = fopen("�û�.txt", "r");
        for (int i = 0; !feof(user); i++)
        {
            fscanf(fp, "%s", allusers[i].ID);
            fscanf(fp, "%s", allusers[i].password);
        }
        fclose(user);


        goods_info user_cart[100]; //�����û��Ĺ��ﳵ��Ϣ���Ա�����д��
        for (int i = 0; strcmp(allusers[i].ID, "X") != 0; i++) //ɾ�������û��Ĺ��ﳵ�е������Ҫɾ������Ʒ�ļ�¼
        {
            char cart_name[27];
            strcpy(cart_name, allusers[i].ID);
            strcat(cart_name, ".txt");
            FILE *fp = fopen(cart_name, "r");
            for (int i = 0; !feof(fp); i++) //���빺�ﳵ��¼
            {
                fscanf(fp, "%s", user_cart[i].ID);
                fscanf(fp, "%s", user_cart[i].name);
                fscanf(fp, "%s", user_cart[i].brand);
                fscanf(fp, "%f", &user_cart[i].price);
                fscanf(fp, "%d", &user_cart[i].amount);
            }
            fclose(fp);

            bool in_this_cart = false;
            int to_del_loc = 0; //Ҫɾ���Ĺ��ﳵ��¼��λ��
            for (int i = 0; user_cart[i].amount != -2; i++)
                if (strcmp(user_cart[i].ID, stock[loc].ID) == 0)
                {
                    in_this_cart = true;
                    to_del_loc = i;
                    break;
                }


            if (in_this_cart)
            {
                user_cart[to_del_loc].amount = -1; //��ɾ��������Ʒ�Ĺ��ﳵ��¼

                FILE *fp = fopen(cart_name, "w"); //����д�빺�ﳵ��¼
                for (int i = 0; user_cart[i].amount != -2; i++)
                {
                    if (user_cart[i].amount != -1) //��ɾ���ļ�¼���ᱻд��
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


            for (int i = 0; i < 100; i++) //��������ʱ����
                user_cart[i].amount = -2;
        }


        cout << "ɾ���ɹ���\n";
    }

    else
        cout << "����Ʒ�����ڻ��ѱ�ɾ���� �޷�ɾ����\n";
}


void admin::mod_goods() //�޸���Ʒ����Ϣ���޸ļ۸���������Ϣ���������޸����¼ܵ���Ʒ����Ϣ�������ϼܡ�
{
    goods_info newgood;
    cout << "������������Ҫ�޸ĵ���Ʒ�����ƣ�Ʒ�ơ��ûس����ָ�������롣\n";
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
        cout << stock[loc].brand << "��" << stock[loc].name << "���ڵļ۸�������ֱ�Ϊ��\n"\
            << stock[loc].price << ",  " << stock[loc].amount << "\n������������Ҫ�޸ĳɵļ۸��������\n";

        float price_alter = 0.0;
        int amount_alter = 0;
        cin >> price_alter >> amount_alter;

        if (amount_alter < 0) //��������ߵ�Ƥ��������С��0������������������ֹ
        {
            cout << "����ɾ����Ʒ��\n";
            return;
        }

        stock[loc].price = price_alter;
        stock[loc].amount = amount_alter;

        FILE *fp = fopen("���.txt", "w");
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


        //����Ҫʵ���޸������û��Ĺ��ﳵ�и���Ʒ����Ϣ
        account allusers[Max]; //�����û����˻���Ϣ
        FILE *user = fopen("�û�.txt", "r");
        for (int i = 0; !feof(user); i++)
        {
            fscanf(fp, "%s", allusers[i].ID);
            fscanf(fp, "%s", allusers[i].password);
        }
        fclose(user);


        goods_info user_cart[100]; //�����û��Ĺ��ﳵ��Ϣ���Ա�����д��
        for (int i = 0; strcmp(allusers[i].ID, "X") != 0; i++) //�޸������û��Ĺ��ﳵ�е������Ҫ�޸ĵ���Ʒ�ļ�¼
        {
            char cart_name[27];
            strcpy(cart_name, allusers[i].ID);
            strcat(cart_name, ".txt");
            FILE *fp = fopen(cart_name, "r");
            for (int i = 0; !feof(fp); i++) //���빺�ﳵ��¼
            {
                fscanf(fp, "%s", user_cart[i].ID);
                fscanf(fp, "%s", user_cart[i].name);
                fscanf(fp, "%s", user_cart[i].brand);
                fscanf(fp, "%f", &user_cart[i].price);
                fscanf(fp, "%d", &user_cart[i].amount);
            }
            fclose(fp);

            bool in_this_cart = false;
            int to_mod_loc = 0; //Ҫ�޸ĵĹ��ﳵ��¼��λ��
            for (int i = 0; user_cart[i].amount != -2; i++)
                if (strcmp(user_cart[i].ID, stock[loc].ID) == 0)
                {
                    in_this_cart = true;
                    to_mod_loc = i;
                    break;
                }


            if (in_this_cart)
            {
                user_cart[to_mod_loc].price = stock[loc].price; //���޸ġ�����Ʒ�Ĺ��ﳵ��¼
                user_cart[to_mod_loc].amount = stock[loc].amount;

                FILE *fp = fopen(cart_name, "w"); //����д�빺�ﳵ��¼
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


            for (int i = 0; i < 100; i++) //��������ʱ����
                user_cart[i].amount = -2;
        }


        cout << "�޸ĳɹ���\n";
    }

    else
        cout << "û�и���Ʒ���޷��޸ģ�\n";
}


void admin::check_sale_list() //�鿴�۳��嵥
{
    goods_info templist[201]; //��ʱ���洢�������嵥
    for (int i = 0; salelist[i].amount != -2; i++)
    {
        strcpy(templist[i].ID, salelist[i].ID);
        strcpy(templist[i].name, salelist[i].name);
        strcpy(templist[i].brand, salelist[i].brand);
        templist[i].price = salelist[i].price;
        templist[i].amount = salelist[i].amount;
    }


    for (int i = 0; templist[i].amount != -2; i++) //��ID�����������
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

    for (int i = 0; templist[i].amount != -2; i++) //��ͬ�۸��ID���������
        if (templist[i].amount != -1)
            for (int j = i + 1; templist[j].amount != -2; j++)
                if (templist[j].amount != -1)
                    if (strcmp(templist[j].ID, templist[i].ID) == 0 && templist[i].price == templist[j].price)
                    {
                        templist[i].amount += templist[j].amount;
                        templist[j].amount = -1;
                    }


    cout << "\n�����嵥���£�\n";
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    //cout << "ID\t\t����\t\tƷ��\t\t�۸�\t\t����\n";
    printf("%s%18s%15s%15s%15s\n", "ID", "����", "Ʒ��", "�۸�", "����");
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


void admin::new_act() //�����µĻ����ջ, ����ʾ�������û��Ĳ˵�
{
    cout << "��ѡ�����Ĳ���: 1. ���֪ͨ      2. ����֪ͨ\n";
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
        cout << "�ѳɹ���գ�\n";
        break;
    }

    case 2:
    {
        cout << "��������Ҫ�����Ļ֪ͨ��������50���֡�\n";
        cin >> activity;

        FILE *fp = fopen("activity.txt", "w");
        fputs(activity, fp);
        fputc('\n', fp);
        fclose(fp);

        cout << "���! ���й˿Ͷ�������˵��п������֪ͨ��\n";
        break;
    }

    default: break;
    }
}


void admin::new_ad() //�����µĹ�棬��ʾ���û���ÿһ��������
{
    cout << "��ѡ�����Ĳ���: 1. ��չ��      2. ���¹��\n";
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
        cout << "�ѳɹ���գ�\n";
        break;
    }

    case 2:
    {
        cout << "��������Ҫ�����Ĺ�档������90���֡�\n";
        cin >> ad;

        FILE *fp = fopen("ad.txt", "w");
        fputs(ad, fp);
        fputc('\n', fp);
        fclose(fp);

        cout << "���! ����δ����ȥ���Ĺ˿��ڽ��в���ʱ�������������档\n";
        break;
    }

    default: break;
    }
}


/*================================================================================================*/
/*================================================================================================*/


user::user() //user��Ĺ��캯��
{
    log_info = false;
    havead = true; //Ĭ�����й���

    FILE *fp = fopen("�û�.txt", "r");
    for (int i = 0; !feof(fp); i++)
    {
        fscanf(fp, "%s", user_acc[i].ID);
        fscanf(fp, "%s", user_acc[i].password);
    }
    fclose(fp);

    for (int i = 0; i < 5; i++) //��ʼ���ջ���ַ�б�
        strcpy(address[i], "X"); 

    for (int i = 0; i < 20; i++) //��ʼ��ȥ����û����б�
        strcpy(no_ad_users[i], "X");

    FILE *fp1 = fopen("deaduser.txt", "r"); //����ȥ�����û�
    for (int i = 0; !feof(fp1); i++)
        fscanf(fp1, "%s", no_ad_users[i]);
    fclose(fp1);
}


bool user::log() //�û��ĵ�¼��ע��
{
    if (log_info == false)
    {
        char inputID[21], inputpass[21];
        cout << "�����û���ID: ";
        cin >> inputID;
        cout << "��������: ";
        cin >> inputpass;

        bool exist_user = false; //�ж��û��Ƿ������ע��������
        for (int i = 0; strcmp(user_acc[i].ID, "X") != 0; i++)
            if (strcmp(inputID, user_acc[i].ID) == 0 && strcmp(inputpass, user_acc[i].password) == 0)
            {
                exist_user = true;
                break;
            }
        
        if (exist_user)
        {
            cout << "��¼�ɹ���\n";
            log_info = true;
            strcpy(login_user_name, inputID); 

            for (int i = 0; strcmp(no_ad_users[i], "X") != 0; i++) //ִ��ȥ����ж�
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

            FILE *addr = fopen(user_addr, "r"); //���ջ���ַ�ļ�
            FILE *fp = fopen(cart_name, "r"); //�򿪹��ﳵ�ļ�
            for (int i = 0; !feof(fp); i++) //���빺�ﳵ
            {
                fscanf(fp, "%s", cart[i].ID);
                fscanf(fp, "%s", cart[i].name);
                fscanf(fp, "%s", cart[i].brand);
                fscanf(fp, "%f", &cart[i].price);
                fscanf(fp, "%d", &cart[i].amount);
            }
            

            for (int i = 0; !feof(addr); i++) //�����ջ���ַ
                fgets(address[i], 100, addr);

            fclose(fp);
            fclose(addr);
            return true;
        }

        else
        {
            cout << "��¼ʧ�ܣ��û������ڻ��û������������\n";
            return false;
        }
    }


    else
    {
        for (int i = 0; i < 101; i++) //��ԭ���ﳵ
            cart[i].amount = -2;

        for (int i = 0; i < 5; i++) //��ԭ�ջ���ַ�б�
            strcpy(address[i], "X");

        log_info = false;
        havead = true;
        return false;
    }
}


bool user::useregister() //�û���ע��
{
    char new_name[21], new_password[21];
    cout << "�������û���: ";
    cin >> new_name;
    cout << "����������: ";
    cin >> new_password;

    for (int i = 0; new_name[i] != '\0'; i++)
        if (new_name[i] < '0' || (new_name[i] > '9' && new_name[i] < 'a') || new_name[i] > 'z')
        {
            cout << "���û������Ϸ����޷�ע�ᣡ\n";
            return false;
        }

    for (int i = 0; new_password[i] != '\0'; i++)
        if (new_password[i] < '0' || (new_password[i] > '9' && new_password[i] < 'a') || new_password[i] > 'z')
        {
            cout << "�����벻�Ϸ����޷�ע�ᣡ\n";
            return false;
        }

    for (int i = 0; strcmp(user_acc[i].ID, "X") != 0; i++)
    {
        if (strcmp(new_name, user_acc[i].ID) == 0) //�����е��û��ظ�
        {
            cout << "����û����ѱ�ע�ᣡ\n";
            return false;
        }
    }


    strcpy(login_user_name, new_name);
    FILE *fp = fopen("�û�.txt", "a");
    fputc('\n', fp);
    fprintf(fp, "%s\t", new_name);
    fprintf(fp, "%s", new_password);
    fclose(fp);

    int len_users = 0;
    //Ҫ�������ע����û����˻���Ϣ��ӵ���������ʱ�̵�user_acc��
    for (int i = 0; strcmp(user_acc[i].ID, "X") != 0; i++)
        len_users++;
    strcpy(user_acc[len_users].ID, new_name);
    strcpy(user_acc[len_users].password, new_password);


    char user_cart[27]; //�������û��Ĺ��ﳵ�ļ�
    strcpy(user_cart, new_name);
    strcat(user_cart, ".txt");
    FILE *cart = fopen(user_cart, "w");
    fclose(cart);

    char user_addr[30]; //�������û����ջ���ַ�ļ�
    strcpy(user_addr, new_name);
    strcat(user_addr, "_addr.txt");
    FILE *addr = fopen(user_addr, "w"); 
    fclose(addr);

    char user_hist[30]; //�������û��Ĺ����¼�ļ�
    strcpy(user_hist, new_name);
    strcat(user_hist, "_hist.txt");
    FILE *hist = fopen(user_hist, "w");
    fclose(hist);

    log_info = true;
    cout << "ע��ɹ������ѵ�¼��\n";
    return true;
}


void user::print_submenu() //��ӡ�û��µĴμ��˵�
{
    cout << endl;
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    cout << "1. ע����¼  2. �鿴��Ʒ  3. ��Ʒ����  4. �����Ʒ�����ﳵ  5. ɾ�����ﳵ�е�ĳ����Ʒ\n\
6. ��չ��ﳵ  7. �鿴���ﳵ  8. �༭��ɾ���ջ���ַ  9. �鿴�����¼  10. ����  11. ��������ȥ���\n\
12. �޸����� \n\n";

    cout << "���̻��";
    if (strlen(activity) == 0)
        cout << "��������޻\n\n";
    else
        cout << activity << endl << endl;

    if (strlen(ad) != 0 && havead) //���
        cout << "��棺 " << ad << endl;

    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    cout << "���������";
}


void user::view_goods(admin& adm) //�û�����Ʒ��Ϣ�鿴
{
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    if (strlen(ad) != 0 && havead) //���
        cout << "��棺 " << ad << endl;

    printf("%s%18s%15s%15s%15s\n", "ID", "����", "Ʒ��", "�۸�", "����");
    for (int i = 0; adm.stock[i].amount != -2; i++)
        if (adm.stock[i].amount > 0) //�ѱ�ɾ��������Ϊ0����ƷӦ���ų�
            printf("%5s%15s%15s%15.1f%15d\n", adm.stock[i].ID, \
                adm.stock[i].name, adm.stock[i].brand, adm.stock[i].price, adm.stock[i].amount);

    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;
}


void user::search_goods(admin& adm) //��Ʒ����
{
    if (strlen(ad) != 0 && havead) //���
        cout  << "\n��棺 " << ad << endl << endl;

    cout << "1. ��������    2. ����Ʒ��\n��ѡ��:";
    int choice = -1;
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        char tosearch[20];
        cout << "��������Ҫ��������Ʒ������: ";
        cin >> tosearch;
        bool found = false; //�ж��û�����������Ƿ�����ڿ����

        for (int i = 0; adm.stock[i].amount != -2; i++)
            if (adm.stock[i].amount > 0) //�ѱ�ɾ��������Ϊ0����ƷӦ���ų�
                if (strcmp(tosearch, adm.stock[i].name) == 0)
                {
                    found = true;
                    printf("%5s%15s%15s%15.1f%15d\n", \
                    adm.stock[i].ID, adm.stock[i].name, adm.stock[i].brand, adm.stock[i].price, adm.stock[i].amount);
                }

        if (!found)
            cout << "�����Ƶ���Ʒ�����ڣ�\n";
        
        break;
    }

    case 2:
    {
        char tosearch[20];
        cout << "��������Ҫ��������Ʒ��Ʒ��: ";
        cin >> tosearch;
        bool found = false; //�ж��û������Ʒ���Ƿ�����ڿ����

        for (int i = 0; adm.stock[i].amount != -2; i++)
            if (adm.stock[i].amount > 0) //�ѱ�ɾ��������Ϊ0����ƷӦ���ų�
                if (strcmp(tosearch, adm.stock[i].brand) == 0)
                {
                    found = true;
                    printf("%5s%15s%15s%15.1f%15d\n", \
                    adm.stock[i].ID, adm.stock[i].name, adm.stock[i].brand, adm.stock[i].price, adm.stock[i].amount);
                }

        if (!found)
            cout << "��Ʒ�Ƶ���Ʒ�����ڣ�\n";

        break;
    }

    default: break;
    }
}


void user::add_to_cart(admin& adm) //�����Ʒ�����ﳵ
{
    if (strlen(ad) != 0 && havead) //���
        cout << "\n��棺 " << ad << endl << endl;

    char addID[20]; //Ҫ��ӵ���Ʒ��ID
    int addnum = 0; //Ҫ��ӵ�����

    bool found = false; //�ж��û��������Ʒ�������Ƿ�����ڿ����
    int loc = 0; //��¼�ҵ����Ǹ������Ʒ��λ��
    cout << "������ƷID���������ûس����ָ�������룺\n";
    cin >> addID >> addnum;

    if (addnum <= 0)
    {
        cout << "������������Ϸ����޷���ӣ�\n";
        return;
    }

    for (int i = 0; adm.stock[i].amount != -2; i++)
        if (adm.stock[i].amount > 0) //�ѱ�ɾ��������Ϊ0����ƷӦ���ų�
            if (strcmp(addID, adm.stock[i].ID) == 0)
            {
                found = true;
                loc = i;
                break;
            }


    if (!found)
        cout << "����Ʒ�����ڣ��޷���������ﳵ��\n";

    else
    {
        bool in_cart = false; //��Ʒ�Ƿ��Ѵ����ڹ��ﳵ
        int cart_loc = 0; //���ڹ��ﳵ�е�λ��
        for (int i = 0; cart[i].amount != -2; i++)
            if (cart[i].amount != -1) //���ڹ��ﳵ�б�ɾ������Ʒ������ʾ
                if (strcmp(addID, cart[i].ID) == 0)
                {
                    in_cart = true;
                    cart_loc = i;
                    break;
                }
            

        char cart_file_name[27];
        strcpy(cart_file_name, login_user_name);
        strcat(cart_file_name, ".txt"); //��Ҫ�򿪵Ĺ��ﳵ�ļ�������

        if (in_cart)
        {
            if (adm.stock[loc].amount < (addnum + cart[cart_loc].amount))
            {
                cout << "����������������Ҫ��ӵ���������ֻ�����" << adm.stock[loc].amount - cart[cart_loc].amount\
                    << "����Ʒ�����ﳵ��\n";

                cart[cart_loc].amount = adm.stock[loc].amount;
            }

            else
                cart[cart_loc].amount += addnum;


            FILE *fp = fopen(cart_file_name, "w"); //����д�빺�ﳵ
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

            printf("�ѳɹ����%d��%s��%s�����ﳵ�У�\n", cart[cart_loc].amount, cart[cart_loc].brand, cart[cart_loc].name);
        }
        

        else //Ҫ��ӵ���Ʒ�������еĹ��ﳵ��
        {
            for (int i = 0; cart[i].amount != -2; i++)
                cart_loc++;

            if (addnum > adm.stock[loc].amount)
            {
                cout << "����н���" << adm.stock[loc].amount << "��" << adm.stock[loc].brand << "��" \
                    << adm.stock[loc].name << ", ���ֻ�����" <<\
                    adm.stock[loc].amount << "�������ﳵ�С�\n";

                cart[cart_loc].amount = adm.stock[loc].amount;
            }


            else
                cart[cart_loc].amount = addnum;

            strcpy(cart[cart_loc].brand, adm.stock[loc].brand);
            strcpy(cart[cart_loc].name, adm.stock[loc].name);
            strcpy(cart[cart_loc].ID, adm.stock[loc].ID);
            cart[cart_loc].price = adm.stock[loc].price;

            FILE *fp = fopen(cart_file_name, "a"); //�ڹ��ﳵ�ļ�ĩβ�����
            fputc('\n', fp);
            fprintf(fp, "%s\t", cart[cart_loc].ID);
            fprintf(fp, "%s\t", cart[cart_loc].name);
            fprintf(fp, "%s\t", cart[cart_loc].brand);
            fprintf(fp, "%.1f\t", cart[cart_loc].price);
            fprintf(fp, "%d", cart[cart_loc].amount);
            fclose(fp);

            printf("�ѳɹ����%d��%s��%s�����ﳵ�У�\n", cart[cart_loc].amount, cart[cart_loc].brand, cart[cart_loc].name);
        }
    }
}


void user::view_cart() //�鿴���ﳵ
{
    if (strlen(ad) != 0 && havead) //���
        cout << "\n��棺 " << ad << endl << endl;

    int n_l = 0;
    for (int i = 0; cart[i].amount != -2; i++)
        if (cart[i].amount > 0)
            n_l++;

    if (n_l == 0)
    {
        cout << "���ﳵ�տ���Ҳ��������ȥ����ɣ�\n";
        return;
    }

    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    printf("%s%18s%15s%15s%15s\n", "ID", "����", "Ʒ��", "�۸�", "����");
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


void user::del_goods_in_cart() //ɾ�����ﳵ�е�ĳ��Ʒ
{
    if (strlen(ad) != 0 && havead) //���
        cout << "\n��棺 " << ad << endl << endl;

    int n_l = 0;
    for (int i = 0; cart[i].amount != -2; i++)
        if (cart[i].amount > 0)
            n_l++;

    if (n_l == 0)
    {
        cout << "���ﳵ�տ���Ҳ��������ȥ����ɣ�\n";
        return;
    }

    int choice = 0; //��¼�û�����Ĳ�������
    cout << "1. ����Ʒ���ƽ���ɾ��   2. ����Ʒ��Ʒ�ƽ���ɾ��   3. �����ƺ�Ʒ�ƽ���ɾ��\n�����룺";
    cin >> choice;
    bool del_suc = false; //�ж��Ƿ���ɾ��

    switch (choice)
    {
    case 1:
    {
        char delname[21]; //Ҫɾ������Ʒ������
        int delnum = 0; //Ҫɾ��������

        cout << "������Ҫɾ������Ʒ������: ";
        cin >> delname;
        int num = 0;
        for (int i = 0; cart[i].amount != -2; i++)
            if (cart[i].amount > 0) //�ѱ�ɾ��������Ϊ0����ƷӦ���ų�
            {
                if (strcmp(delname, cart[i].name) == 0)
                {
                    num++;
                    cout << "�ҵ�����ƥ���\n";
                    cout << cart[i].ID << "\t\t" << cart[i].name << "\t\t"\
                        << cart[i].brand << "\t\t" << fixed << setprecision(1) << cart[i].price\
                        << "\t\t" << cart[i].amount;
                    cout << endl;
                }
            }

        if (num == 0)
            cout << "����Ʒ���ڹ��ﳵ�У��޷�ɾ����\n";

        else
        {
            cout << "��������Ҫɾ����" << delname << "��Ʒ�Ƽ����������ûس����ָ����룺";
            char delbrand[21]; //Ҫɾ������Ʒ��Ʒ����
            cin >> delbrand >> delnum;

            bool found = false;
            for (int i = 0; cart[i].amount != -2; i++)
                if (cart[i].amount > 0) //�ѱ�ɾ��������Ϊ0����ƷӦ���ų�
                {
                    if (strcmp(delbrand, cart[i].brand) == 0 && strcmp(delname, cart[i].name) == 0)
                    {
                        found = true;
                        if (delnum < cart[i].amount) //Ҫɾ��������С�ڹ��ﳵ�е�����
                        {
                            cart[i].amount -= delnum; //ɾ��һ��������
                            break;
                        }
                        
                        else
                        {
                            if (delnum > cart[i].amount) //Ҫɾ�����������ڹ��ﳵ�е�����
                            {
                                cout << "���ﳵ��ֻ��" << cart[i].amount << "��" << cart[i].brand \
                                    << "��" << cart[i].name << "�� ��ע����������:)\n";
                            }

                            cart[i].amount = -1; //��ʽɾ�����ü�¼���
                            break;
                        }
                    }
                }

            if (found)
            {
                del_suc = true;
                cout << "ɾ���ɹ���\n";
            }
            else
                cout << "��������޷�ɾ����\n";
        }

        break;
    }


    case 2:
    {
        char delbrand[21]; //Ҫɾ������Ʒ��Ʒ��
        int delnum = 0; //Ҫɾ��������

        cout << "������Ҫɾ������Ʒ��Ʒ��: ";
        cin >> delbrand;
        int num = 0;
        for (int i = 0; cart[i].amount != -2; i++)
            if (cart[i].amount > 0) //�ѱ�ɾ��������Ϊ0����ƷӦ���ų�
            {
                if (strcmp(delbrand, cart[i].brand) == 0)
                {
                    num++;
                    cout << "�ҵ�����ƥ���\n";
                    cout << cart[i].ID << "\t\t" << cart[i].name << "\t\t"\
                        << cart[i].brand << "\t\t" << fixed << setprecision(1) << cart[i].price\
                        << "\t\t" << cart[i].amount;
                    cout << endl;
                }
            }

        if (num == 0)
            cout << "��Ʒ�Ʋ��ڹ��ﳵ�У��޷�ɾ����\n";

        else
        {
            cout << "��������Ҫɾ����" << delbrand << "����Ʒ�����������ûس����ָ����룺";
            char delname[21]; //Ҫɾ������Ʒ������
            cin >> delname >> delnum;

            bool found = false;
            for (int i = 0; cart[i].amount != -2; i++)
                if (cart[i].amount > 0) //�ѱ�ɾ��������Ϊ0����ƷӦ���ų�
                {
                    if (strcmp(delbrand, cart[i].brand) == 0 && strcmp(delname, cart[i].name) == 0)
                    {
                        found = true;
                        if (delnum < cart[i].amount) //Ҫɾ��������С�ڹ��ﳵ�е�����
                        {
                            cart[i].amount -= delnum; //ɾ��һ��������
                            break;
                        }

                        else
                        {
                            if (delnum > cart[i].amount) //Ҫɾ�����������ڹ��ﳵ�е�����
                            {
                                cout << "���ﳵ��ֻ��" << cart[i].amount << "��" << cart[i].brand \
                                    << "��" << cart[i].name << "�� ��ע����������:)\n";
                            }

                            cart[i].amount = -1; //��ʽɾ�����ü�¼���
                            break;
                        }
                    }
                }

            if (found)
            {
                del_suc = true;
                cout << "ɾ���ɹ���\n";
            }
            
            else
                cout << "��������޷�ɾ����\n";
        }

        break;
    }


    case 3:
    {
        char delname[21]; //Ҫɾ������Ʒ������
        char delbrand[21]; //Ҫɾ������Ʒ��Ʒ����
        int delnum = 0; //Ҫɾ��������

        cout << "������Ҫɾ������Ʒ��Ʒ�ƺ����ơ��ûس����ָ�����: " ;
        cin >> delbrand >> delname;
        int num = 0;
        int loc = 0;
        for (int i = 0; cart[i].amount != -2; i++)
            if (cart[i].amount > 0) //�ѱ�ɾ��������Ϊ0����ƷӦ���ų�
            {
                if (strcmp(delbrand, cart[i].brand) == 0 && strcmp(delname, cart[i].name) == 0)
                {
                    num++;
                    loc = i;
                    cout << "�ҵ�����ƥ���\n";
                    cout << cart[i].ID << "\t\t" << cart[i].name << "\t\t"\
                        << cart[i].brand << "\t\t" << fixed << setprecision(1) << cart[i].price\
                        << "\t\t" << cart[i].amount;
                    cout << endl;
                    break;
                }
            }

        if (num == 0)
            cout << "������ƥ����޷�ɾ����\n";

        else
        {
            cout << "��������Ҫɾ����������";
            cin >> delnum;

            if (delnum < cart[loc].amount) //Ҫɾ��������С�ڹ��ﳵ�е�����
                cart[loc].amount -= delnum; //ɾ��һ��������

            else
            {
                if (delnum > cart[loc].amount) //Ҫɾ�����������ڹ��ﳵ�е�����
                {
                    cout << "���ﳵ��ֻ��" << cart[loc].amount << "��" << cart[loc].brand \
                        << "��" << cart[loc].name << "�� ��ע����������:)\n";
                }

                cart[loc].amount = -1; //��ʽɾ�����ü�¼���
            }

            del_suc = true;
            cout << "ɾ���ɹ���\n";
        }

        break;
    }

    default: break;
    }
    
    
    if (del_suc) //�ɹ���ɾ���Żᵼ���ļ�������д��
    {
        char cart_file_name[27];
        strcpy(cart_file_name, login_user_name);
        strcat(cart_file_name, ".txt"); //��Ҫ�򿪵Ĺ��ﳵ�ļ�������

        FILE *fp = fopen(cart_file_name, "w"); //����д�빺�ﳵ
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


void user::clear_cart() //��չ��ﳵ
{
    if (strlen(ad) != 0 && havead) //���
        cout << "\n��棺 " << ad << endl << endl;

    int n_l = 0;
    for (int i = 0; cart[i].amount != -2; i++)
        if (cart[i].amount > 0)
            n_l++;

    if (n_l == 0)
    {
        cout << "���ﳵ�տ���Ҳ��������ȥ����ɣ�\n";
        return;
    }

    cout << "�����Ŀǰ�Ĺ��ﳵ��ȷ����\n1. ȷ��    2. ����\n";
    int del = 0;
    cin >> del;

    switch (del)
    {
    case 1:
    {
        char cart_file_name[27];
        strcpy(cart_file_name, login_user_name);
        strcat(cart_file_name, ".txt"); //��Ҫ�򿪵Ĺ��ﳵ�ļ�������

        FILE *fp = fopen(cart_file_name, "w"); //��չ��ﳵ
        fclose(fp);

        for (int i = 0; i < 101; i++)
            cart[i].amount = -2;

        cout << "��ճɹ�!\n";
    }
    
    default: break;
    }
}


void user::view_history() //�鿴�����¼
{
    if (strlen(ad) != 0 && havead) //���
        cout << "\n��棺 " << ad << endl << endl;

    char hist_file_name[30];
    strcpy(hist_file_name, login_user_name);
    strcat(hist_file_name, "_hist.txt"); //��Ҫ�򿪵Ĺ����¼�ļ�������
    FILE *hist = fopen(hist_file_name, "r+"); //�򿪹����¼�ļ�
    char ch = fgetc(hist);

    if (ch == EOF)
    {
        cout << "�����¼Ϊ��!\n";
        fclose(hist);
        return;
    }

    fseek(hist, 0, SEEK_SET);
    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl << "�����¼���£�\n";

    printf("%s%18s%15s%15s%15s\n", "ID", "����", "Ʒ��", "�۸�", "����");

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


void user::checkout(admin& adm) //����
{
    if (strlen(ad) != 0 && havead) //���
        cout << "\n��棺 " << ad << endl << endl;

    float totalprice = 0; //�ܼ�
    
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
                            printf("���ﳵ��%s��%s�����������˿���е���������ֻ�ܹ������н�ʣ��%d��\n", \
                                cart[i].brand, cart[i].name, adm.stock[j].amount);

                            cart[i].amount = adm.stock[j].amount;
                        }


    printf("%s%18s%15s%15s%15s\n", "ID", "����", "Ʒ��", "�۸�", "����");
    for (int i = 0; cart[i].amount != -2; i++)
        if (cart[i].amount != -1)
        {
            totalprice += (cart[i].price * cart[i].amount);
            printf("%5s%15s%15s%15.1f%15d\n", cart[i].ID, cart[i].name, cart[i].brand, cart[i].price, cart[i].amount);
        }

    if (totalprice == 0)
    {
        cout << "���ﳵ�տ���Ҳ��������ȥ����ɣ�\n";
        return;
    }

    for (int i = 0; i < NUM_OF_X; i++)
        cout << "=";
    cout << endl;

    cout << "һ����Ҫ����" << totalprice << "Ԫ�� ȷ�ϸ����밴1�� �����밴0�� ";
    int choice = -1;
    cin >> choice;


    if (choice)
    {
        if (strcmp(address[0], "X") == 0)
        {
            cout << "����û����ӹ��ջ���ַ����������ӣ�ע�ⲻҪ����25����Ŷ��\n�����룺";
            char temp[51]; //��ʱ����ջ��ַ
            cin >> temp;

            strcpy(address[0], temp);

            char user_addr[30];
            strcpy(user_addr, login_user_name);
            strcat(user_addr, "_addr.txt");

            FILE *addr = fopen(user_addr, "w"); //����д���ջ���ַ�ļ�
            for (int i = 0; strcmp(address[i], "X") != 0; i++)
            {
                fputs(address[i], addr);
                if (strcmp(address[i + 1], "X") != 0)
                    fputc('\n', addr);
            }
            fclose(addr);

            cout << "��ӳɹ�, ��ʹ�øõ�ַ��Ϊ�����ι�����ջ���ַ��ף��������죡\n";
        }

        else
        {
            cout << "��ѡ���ջ��ַ��\n";
            for (int i = 0; strcmp(address[i], "X") != 0; i++)
                printf("%d. %s\n", i+1, address[i]);

            int choi = 0;
            cin >> choi;

            printf("����ѡ�� %s ��Ϊ�����ι�����ջ��ַ��лл��ף��������죡\n", address[choi-1]);
        }


        for (int i = 0; cart[i].amount != -2; i++) //�޸ĳ����еĿ��
            if (cart[i].amount != -1) //�������ѱ�ɾ���Ĺ��ﳵ��Ʒ
                for (int j = 0; adm.stock[j].amount != -2; j++)
                    if (adm.stock[j].amount > 0) //�ѱ�ɾ��������Ϊ0����ƷӦ���ų�
                        if (strcmp(cart[i].ID, adm.stock[j].ID) == 0)
                        {
                            adm.stock[j].amount -= cart[i].amount;
                            break;
                        }


        FILE *fp = fopen("���.txt", "w"); //����д����
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


        int len_salelist = 0; //�����嵥�ĳ���
        for (int i = 0; adm.salelist[i].amount != -2; i++)
            len_salelist++;

        for (int i = 0; cart[i].amount != -2; i++) //�޸ĳ����е��۳��嵥
            if (cart[i].amount != -1) //�������ѱ�ɾ���Ĺ��ﳵ��Ʒ
            {
                strcpy(adm.salelist[len_salelist].ID, cart[i].ID);
                strcpy(adm.salelist[len_salelist].name, cart[i].name);
                strcpy(adm.salelist[len_salelist].brand, cart[i].brand);
                adm.salelist[len_salelist].amount = cart[i].amount;
                adm.salelist[len_salelist].price = cart[i].price;
                strcpy(adm.salelist[len_salelist].user_name, login_user_name);

                len_salelist++;
            }


        FILE *fp1 = fopen("�����嵥.txt", "w"); //����д�������嵥
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
        strcat(hist_file_name, "_hist.txt"); //��Ҫ�򿪵Ĺ����¼�ļ�������
        FILE *hist = fopen(hist_file_name, "a"); //��ӹ����¼
        
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
        strcat(cart_file_name, ".txt"); //��Ҫ�򿪵Ĺ��ﳵ�ļ�������
        FILE *fp2 = fopen(cart_file_name, "w"); //��չ��ﳵ
        fclose(fp2);
        for (int i = 0; i < 100; i++)
            cart[i].amount = -2; //��ճ����еĹ��ﳵ


        cout << "\n����ɹ���\n";
    }
}


void user::dead() //ȥ���
{
    if (strlen(ad) != 0 && havead) //���
        cout << "\n��棺 " << ad << endl << endl;

    if (!havead)
    {
        cout << "���ѹ��������ȥ��棬�����ٴι���\n";
        return;
    }

    cout << "����998�����Ϳ�����������ȥ���������������Ƿ�Ҫ��������ȥ��棿\n1. ��   2. ��\n��ѡ��";
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
        FILE *noad = fopen("deaduser.txt", "a"); //���ȥ�����û����ļ���
        fputs(login_user_name, noad);
        fputc('\n', noad);
        fclose(noad);
        
        cout << "����ɹ���\n";
        break;
    }

    default: break;
    }
}


void user::mod_addr() //�޸��ջ��ַ
{
    if (strlen(ad) != 0 && havead) //���
        cout << "\n��棺 " << ad << endl << endl;

    if (strcmp(address[0], "X") == 0)
        cout << "��Ŀǰû���ջ��ַ��\n";
    else
    {
        printf("Ŀǰ���ջ���ַΪ: \n");
        for (int i = 0; strcmp(address[i], "X") != 0; i++)
            printf("%d. %s\n", i + 1, address[i]);
        cout << "\n";
    }

    printf("������1. ɾ���ջ���ַ     2. �༭���е��ջ���ַ\n�����룺");
    int choice = 0;
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        if (strcmp(address[0], "X") == 0)
        {
            cout << "��Ŀǰû���ջ��ַ, �޷�ɾ����\n";
            break;
        }

        int subchoice = 0;
        cout << "��������Ҫɾ�����ջ���ַ�ı��: ";
        cin >> subchoice;

        if (strcmp(address[subchoice - 1], "X") == 0 || subchoice > 5)
        {
            cout << "�ñ����Ч, �޷�ɾ����\n";
            break;
        }

        
        int temp = subchoice - 1;
        for (; strcmp(address[temp + 1], "X") != 0 && temp < 4; temp++)
            strcpy(address[temp], address[temp + 1]);
        strcpy(address[temp], "X");

        char user_addr[30];
        strcpy(user_addr, login_user_name);
        strcat(user_addr, "_addr.txt");

        FILE *addr = fopen(user_addr, "w"); //����д���ջ���ַ�ļ�
        for (int i = 0; strcmp(address[i], "X") != 0; i++)
        {
            fputs(address[i], addr);
            if (strcmp(address[i + 1], "X") != 0)
                fputc('\n', addr);
        }
        fclose(addr);

        cout << "ɾ���ɹ���\n";
        break;
    }


    case 2:
    {
        if (strcmp(address[0], "X") == 0)
        {
            cout << "��Ŀǰû���ջ��ַ, �����޸����е��ջ��ַ��ֻ�ܽ�����ӣ�\n";
            cout << "��������Ҫ��ӵ��ջ���ַ��\n";
            char temp[51];
            cin >> temp;
            strcpy(address[0], temp);
            cout << "��ӳɹ�!\n";

            char user_addr[30];
            strcpy(user_addr, login_user_name);
            strcat(user_addr, "_addr.txt");

            FILE *addr = fopen(user_addr, "w"); //����д���ջ���ַ�ļ�
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
            cout << "��������Ҫ�༭���ջ���ַ�ı��: ";
            cin >> subchoice;

            if (strcmp(address[subchoice - 1], "X") == 0 || subchoice > 5)
            {
                cout << "�ñ����Ч, �޷��༭��\n";
                break;
            }

            cout << "�������µ��ջ���ַ��\n";
            char temp[51];
            cin >> temp;
            strcpy(address[subchoice - 1], temp);
            cout << "�༭�ɹ�!\n";

            char user_addr[30];
            strcpy(user_addr, login_user_name);
            strcat(user_addr, "_addr.txt");

            FILE *addr = fopen(user_addr, "w"); //����д���ջ���ַ�ļ�
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


void user::mod_password() //�޸ĵ�¼����
{
    if (strlen(ad) != 0 && havead) //���
        cout << "\n��棺 " << ad << endl << endl;

    char temp[21], temp2[21];
    cout << "������ԭ�������룺\n";
    cin >> temp;

    int loc = 0;
    for (int i = 0; strcmp(user_acc[i].ID, "X") != 0; i++)
        if (strcmp(login_user_name, user_acc[i].ID) == 0)
        {
            loc = i;
            if (strcmp(temp, user_acc[i].password) != 0)
            {
                cout << "��������޷��޸�!\n";
                return;
            }
        }
            

    cout << "�������µ�����\n";
    cin >> temp;

    for (int i = 0; temp[i] != '\0'; i++)
        if (temp[i] < '0' || (temp[i] > '9' && temp[i] < 'a') || temp[i] > 'z')
        {
            cout << "�����벻�Ϸ����޸�ʧ�ܣ�\n";
            return;
        }


    cout << "���ٴ������µ�����\n";
    cin >> temp2;
    if (strcmp(temp, temp2) != 0)
    {
        cout << "�������벻ƥ�䣬�޸�ʧ�ܣ�\n";
        return;
    }


    strcpy(user_acc[loc].password, temp);
    FILE *fp = fopen("�û�.txt", "w"); //����д���û��˻��ļ�
    for (int i = 0; strcmp(user_acc[i].ID, "X") != 0; i++)
    {
        fprintf(fp, "%s\t", user_acc[i].ID);
        fprintf(fp, "%s", user_acc[i].password);

        if (strcmp(user_acc[i + 1].ID, "X") != 0)
            fputc('\n', fp);
    }
    fclose(fp);

    cout << "�޸ĳɹ���\n";
}