#include "funcs.h"

int main()
{
    initial();

    cout << "~$ ";
    string temp; //�����û�������Ĳ���
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