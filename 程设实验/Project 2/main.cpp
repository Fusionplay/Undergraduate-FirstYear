#include "funcs.h"

int main()
{
    initial();

    cout << "~$ ";
    string temp; //储存用户的输入的操作
    getline(cin, temp);
    
    if (temp == "mySQL")
    {
    	cout << "(mySQL) ==> ";
        getline(cin, temp);
    	
    	while (temp != "quit")
    	{
    		func_detect(temp);
    		
    		cout << "(mySQL) ==> ";
            getline(cin, temp);
    	}
    }

    return 0;
}