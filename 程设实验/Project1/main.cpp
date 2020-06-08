#include "admin_user.h"
#include "menu.h"
#include <iostream>

int main()
{
    admin adminuser;
    user normuser;
    int a = generalmenu();
    
    while (a != 4)
    {
        operate_menu(a, adminuser, normuser);
        a = generalmenu();
    }

    printf("\nTHANKS!\n");
    return 0;
}