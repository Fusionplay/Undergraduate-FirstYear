#pragma once
#ifndef menu_H
#define menu_H

class admin;
class user;
int generalmenu(); //程序刚刚开始运行时的主界面
void operate_menu(int operator_type, admin& ad, user& us); //操作菜单
#endif