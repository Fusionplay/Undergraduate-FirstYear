#include "funcs.h"

//ȫ�ֱ���
vector<table_column> row; //������е�����
vector<table_column>::iterator rowit; //������

vector<tables> table; //����Ŀǰӵ�е�����table
vector<tables>::iterator tableit; //��Ӧ�ĵ�����

int num_of_tables = 0; //table������
char cur_table[40]; //���浱ǰ���ڲ�����table������


int coun(char *p) //����ĳ�����ʵĳ���
{
	int i=1;
	while (p[i] != ' ')
		i++;

	return i;
}


void poi_mov(char *&p) //���ַ�ָ���ƶ�����һ�������ʡ���
{
    int i = 1;
    while (p[i] != ' ')
        i++;
    p = &p[i + 1];
}


bool In_str(char *target, char *source) //�ж�source�Ƿ�Ϊtarget���Ӵ�
{
    int tarlen = strlen(target);
    int soulen = strlen(source);

    if (soulen > tarlen)
        return false;
    
    else if (soulen == tarlen)
    {
        if (strcmp(target, source) == 0)
            return true;
        return false;
    }

    else
    {
        char *t = target;
        while (strlen(t) >= soulen)
        {
            if (strncmp(t, source, soulen) == 0)
                return true;
            t++;
        }

        return false;
    }
}


int Max(int *arr, int len) //��int�������е����Ԫ��
{
    if (len == 1)
        return arr[0];

    int max = 0;
    for (int i = 0; i < len - 1; i++)
    {
        if (arr[i] >= arr[i + 1])
            max = arr[i];
        else
            max = arr[i + 1];
    }
    return max;
}


void initial() //����ԭ�е�table
{
    FILE *state = fopen("state.txt", "r+");
    char ch = fgetc(state);
    if (ch == EOF)
        return;
    else
        fseek(state, 0, SEEK_SET);

    char tablename[40];
    char filename[40];
    tables temp0;

    fscanf(state, "%s", tablename);
    fscanf(state, "%s", filename);

    temp0.filename = filename;
    temp0.tablename = tablename;

    table.push_back(temp0);
    num_of_tables++;

    fscanf(state, "%s", tablename);

    while (!feof(state))
    {
        fscanf(state, "%s", filename);

        temp0.filename = filename;
        temp0.tablename = tablename;

        table.push_back(temp0);
        num_of_tables++;

        fscanf(state, "%s", tablename);
    }

    if (table.size() > 0)
        tableit = table.begin();
    fclose(state);
}


void func_detect(string &s) //̽�⹦��
{
	if (strncmp(s.c_str(), "CREATE TABLE ", 13) == 0)
	{
		char *p = &s[13]; //�ӵ�13���ַ���ʼ¼���p��
		char tablename[40]; //��char�������ļ�����
		strncpy(tablename, p, coun(p));
        tablename[coun(p)] = '\0';
		poi_mov(p);

		if (strncmp(p, "FROM ", 5) == 0)
		{
			poi_mov(p);
			char filename[40];
			strcpy(filename, p);
            int len = strlen(filename);
            if (strncmp(tablename, filename, len-4) != 0)
            {
                cout << "Table name and filename inconsistent!\n";
                return;
            }

            tableit = table.begin();
            while (tableit != table.end()) //����table
            {
                char *temp = &((*tableit).tablename)[0];
                if (strcmp(temp, tablename) == 0)
                {
                    tableit = table.begin();
                    cerr << "File already in database!\n";
                    return;
                }

                else
                    ++tableit;
            }

            tableit = table.begin();


			create_table(filename); //ȷ�����ļ��Ѵ��ڣ��Ż���ļ� ��˻�Ҫ�����ļ������ڵ����
            tableprint(); //��ӡһ��
		}

		else
		{
			char *t = &p[0];
			if (*t == '(' && *(t + 1) != ')')
			{
				while (*t != ')' && *t != '\0')
					t++;
				
				if (*t != '\0')
				{
					t += 2;
					if (strncmp(t, "TO ", 3) == 0)
					{
						t += 3;
						char filename[40];
						strcpy(filename, t); //����ط��Ժ�Ҫ�����û�������������
						
						create_table(tablename, filename, p+1);
					}
				}
			}
		}
	}
		
		
	else if (strncmp(s.c_str(), "DROP TABLE ", 11) == 0)
	{
		char *p = &s[11]; 
		drop_table(p); 
	}
	

	else if (strncmp(s.c_str(), "TABLE LIST", 10) == 0)
		table_list();


    else if (strncmp(s.c_str(), "INSERT INTO ", 12) == 0)
    {
        char *p = &s[12]; //�ӵ�13���ַ���ʼ¼���p��
        char tablename[40]; //��char�������ļ�����
        strncpy(tablename, p, coun(p));
        tablename[coun(p)] = '\0';
        poi_mov(p);

        if (strncmp(p, "VALUES ", 7) == 0)
        {
            poi_mov(p);
            insert_into(tablename, p); //��һ�֣�����һ��
        }

        else if (p[0] == '(')
        {
            char *t = p;
            int wordnum1 = 0;
            while(*t != ')' && *t != '\0')
            {
                if (*t == ',' || ( *(t + 1) == ')' && *t != '('))
                    wordnum1++;
                t++;
            }

            if (*t != ')' || (*t == ')' && strncmp(t+2, "VALUES (", 8) != 0) || wordnum1 == 0)
            {
                cout << "Invalid input!\n";
                return;
            }
            

            t += 10;
            int wordnum2 = 0;
            while (*t != ')' && *t != '\0')
            {
                if (*t == ',' || ( *(t + 1) == ')' && *t != '('))
                    wordnum2++;
                t++;
            }

            if (*t != ')' || wordnum2 == 0 || (*t == ')' && *(t + 1) != '\0') || (*t == ')' && wordnum1 != wordnum2))
            {
                cout << "Invalid input!\n";
                return;
            }

            insert_into(tablename, p, wordnum1);
        }

        else
            cout << "Invalid input!\n";
    }


    else if (strncmp(s.c_str(), "DELETE ", 7) == 0)
    {
        char *p = &s[7];
        if (strncmp(p, "* FROM ", 7) == 0)
        {
            p += 7;
            char tablename[40]; //��char�������ļ�����
            strcpy(tablename, p);
            
            Delete(tablename);
        }


        else if (strncmp(p, "FROM ", 5) == 0)
        {
            p += 5;
            char tablename[40]; //��char�������ļ�����
            strncpy(tablename, p, coun(p));
            tablename[coun(p)] = '\0';
            poi_mov(p);

            if (strncmp(p, "WHERE ", 6) == 0)
            {
                p += 6;
                char colutemp[60];
                strncpy(colutemp, p, coun(p));
                colutemp[coun(p)] = '\0';
                poi_mov(p);

                if (strncmp(p, "= ", 2) == 0)
                {
                    p += 2;
                    char colucont[60];
                    strcpy(colucont, p);

                    Delete(tablename, colutemp, colucont, 0); //����
                }

                else if (strncmp(p, "< ", 2) == 0)
                {
                    p += 2;
                    char colucont[60];
                    strcpy(colucont, p);

                    Delete(tablename, colutemp, colucont, -1); //С��
                }

                else if (strncmp(p, "> ", 2) == 0)
                {
                    p += 2;
                    char colucont[60];
                    strcpy(colucont, p);

                    Delete(tablename, colutemp, colucont, 1); //����
                }

                else
                    cout << "Invalid input!\n";
            }

            else
                cout << "Invalid input!\n";
        }

        else
            cout << "Invalid input!\n";
    }


    else if (strncmp(s.c_str(), "UPDATE ", 7) == 0)
    {
        char *p = &s[7]; //�ӵ�7���ַ���ʼ¼���p��
        char tablename[40]; //��char�������ļ�����
        strncpy(tablename, p, coun(p));
        tablename[coun(p)] = '\0';
        poi_mov(p);
        bool has_where = false; //ָʾ�������ĸ����

        if (strncmp(p, "SET ", 4) == 0)
        {
            p += 4;
            char *t = p;
            int num_of_terms = 0; //�м���Ҫ�޸�:��whereǰ��
            while (*t != '\0') //����ֻ�����������������жϣ�����������
            {
                if (*t == '=')
                    num_of_terms++;
                t++;
            }

            t = p;
            int num_of_comma = 0; //���ŵ�����
            while (*t != '\0') //����ֻ�����������������жϣ�����������
            {
                if (*t == ',')
                    num_of_comma++;

                if (strlen(t) >= 6)
                    if (strncmp(t, "WHERE ", 6) == 0)
                        has_where = true;
                
                t++;
            }

            if ( (!has_where && num_of_comma != num_of_terms - 1) || (has_where && num_of_comma != num_of_terms - 2) )
            {
                cout << "Invalid input!\n";
                return;
            }

            if (!has_where)
                update(tablename, p, num_of_terms, false);
            else
                update(tablename, p, num_of_terms-1, true);
        }

        else
            cout << "Invalid input!\n";
    }


    else if (strncmp(s.c_str(), "SELECT ", 7) == 0)
    {
        char *p = &s[7]; //�ӵ�7���ַ���ʼ¼���p��
        if (strncmp(p, "* FROM ", 7) == 0 && !In_str(p, " ORDER BY ") && !In_str(p, " TO ") && !In_str(p, " WHERE ")) //չʾ����table
        {
            p += 7;
            char tablename[40]; //��char�������ļ�����
            strcpy(tablename, p);

            if (strcmp(cur_table, tablename) == 0)
                tableprint();

            else //���ǵ�ǰ��table
            {
                tableit = table.begin();
                char *temp = NULL;
                bool found = false; //�Ƿ����
                while (tableit != table.end()) //����table
                {
                    temp = &((*tableit).tablename)[0];
                    if (strcmp(temp, tablename) == 0)
                    {
                        found = true;
                        break;
                    }

                    else
                        ++tableit;
                }

                tableit = table.begin();
                if (!found)
                {
                    cout << "TABLE DOES NOT EXIST!\n";
                    return;
                }

                else
                {
                    char filenametemp[40];
                    strcpy(filenametemp, tablename);
                    strcat(filenametemp, ".txt");
                    create_table(tablename, filenametemp); //�����Ŀǰ��table
                }

                tableprint();
            }
        }


        else if (strncmp(p, "* FROM ", 7) == 0 && In_str(p, " ORDER BY ") && !In_str(p, " TO ")) //��order by
        {
            p += 7;
            char tablename[40];
            strncpy(tablename, p, coun(p));
            tablename[coun(p)] = '\0';
            poi_mov(p);
            
            if (strncmp(p, "ORDER BY ", 9) != 0)
            {
                cout << "Invalid input!\n";
                return;
            }

            p += 9;

            select(tablename, p);
        }


        else if (strncmp(p, "* FROM ", 7) == 0 && !In_str(p, " ORDER BY ") && In_str(p, " TO ")\
            && In_str(p, " WHERE ")) //��to��where:�Ѳ�ѯ���д���ļ��еĵڶ���
        {
            p += 7;
            char tablename[40];
            strncpy(tablename, p, coun(p));
            tablename[coun(p)] = '\0';
            poi_mov(p); //����where��
            poi_mov(p); //����where�����һ��Word

            char *t = p;
            poi_mov(t);
            if (*t != '=')
            {
                cerr << "Invalid input!\n";
                return;
            }

            poi_mov(t);
            poi_mov(t); //����TO��
            if (strncmp(t, "TO ", 3) != 0)
            {
                cerr << "Invalid input!\n";
                return;
            }
            poi_mov(t);
            char filename[40];
            strcpy(filename, t);

            tableit = table.begin();
            while (tableit != table.end()) //����table
            {
                char *temp = &((*tableit).filename)[0];
                if (strcmp(temp, filename) == 0)
                {
                    tableit = table.begin();
                    cout << "File already in database!\n";
                    return;
                }

                else
                    ++tableit;
            }
            tableit = table.begin();
            sel_file_wh(tablename, filename, p); //�Ѳ�ѯ���д���ļ���: ��where
        }


        else if (strncmp(p, "* FROM ", 7) == 0 && !In_str(p, " ORDER BY ") && !In_str(p, " TO ")\
            && In_str(p, " WHERE ")) //����FROM��where
        {
            p += 7;
            char tablename[40];
            strncpy(tablename, p, coun(p));
            tablename[coun(p)] = '\0';
            poi_mov(p); //����where��
            poi_mov(p); //����where�����һ��Word

            char *t = p;
            poi_mov(t);
            if (strncmp(t, "= ", 2) != 0)
            {
                cerr << "Invalid input!\n";
                return;
            }

            selectspe(tablename, p);
        }


        else if (In_str(p, " FROM ")) //��FROM
        {
            if (strncmp(p, "DISTINCT ", 9) == 0) //��DISTINCT
            {
                char *t = p;
                int num_of_comma = 0;
                while (strncmp(t, " FROM ", 6) != 0)
                {
                    if (*t == ',')
                    {
                        if (*(t + 1) == ' ' || strncmp(t + 1, "FROM", 4) == 0)
                        {
                            cout << "Invalid input!\n";
                            return;
                        }

                        num_of_comma++;
                    }

                    t++;
                }


                t += 6; //����tablename��
                char tablename[40];
                strcpy(tablename, t);
                //tablename[coun(t)] = '\0';
                select(tablename, p, num_of_comma + 1, 1);
            }


            else if (In_str(p, " WHERE "))
            {
                char *t = p;
                int num_of_comma = 0;
                while (strncmp(t, " FROM ", 6) != 0)
                {
                    if (*t == ',')
                    {
                        if (*(t + 1) == ' ' || strncmp(t + 1, "FROM", 4) == 0)
                        {
                            cout << "Invalid input!\n";
                            return;
                        }

                        num_of_comma++;
                    }

                    t++;
                }


                t += 6; //����tablename��
                char tablename[40];
                strncpy(tablename, t, coun(t));
                tablename[coun(t)] = '\0';
                select(tablename, p, num_of_comma + 1, true);
            }


            else if (In_str(p, " TO "))
            {
                char *t = p;
                int num_of_comma = 0;
                while (strncmp(t, " FROM ", 6) != 0)
                {
                    if (*t == ',')
                    {
                        if (*(t + 1) == ' ' || strncmp(t + 1, "FROM", 4) == 0)
                        {
                            cout << "Invalid input!\n";
                            return;
                        }

                        num_of_comma++;
                    }

                    t++;
                }


                t += 6; //����tablename��
                char tablename[40];
                strncpy(tablename, t, coun(t));
                tablename[coun(t)] = '\0';

                poi_mov(t);
                if (strncmp(t, "TO ", 3) != 0)
                {
                    cout << "Invalid input!\n";
                    return;
                }
                poi_mov(t);


                char filename[40]; //�ļ���
                strcpy(filename, t);

                tableit = table.begin();
                while (tableit != table.end()) //����table
                {
                    char *temp = &((*tableit).filename)[0];
                    if (strcmp(temp, filename) == 0)
                    {
                        tableit = table.begin();
                        cout << "File already in database!\n";
                        return;
                    }

                    else
                        ++tableit;
                }
                tableit = table.begin();


                select(tablename, filename, p, num_of_comma + 1);
            }


            else if (In_str(p, " ORDER BY ")) //���³��ֵĸ�ʽ
            {
                char *t = p;
                int num_of_comma = 0;
                while (strncmp(t, " FROM ", 6) != 0)
                {
                    if (*t == ',')
                    {
                        if (*(t + 1) == ' ' || strncmp(t + 1, "FROM", 4) == 0)
                        {
                            cout << "Invalid input!\n";
                            return;
                        }

                        num_of_comma++;
                    }

                    t++;
                }


                t += 6; //����tablename��
                char tablename[40];
                strncpy(tablename, t, coun(t));
                tablename[coun(t)] = '\0';

                poi_mov(t); //����order by��
                if (strncmp(t, "ORDER BY ", 9) != 0)
                {
                    cerr << "Invalid input!\n";
                    return;
                }
                

                t += 9; //������Ҫ������
                char sortby[40];
                strncpy(sortby, t, coun(t));
                sortby[coun(t)] = '\0';
                poi_mov(t); //����asc, desc��


                int sort_kind = 0; //ָʾ�����������ǽ�: 1Ϊ����2Ϊ��
                if (strncmp(t, "ASC", 3) == 0)
                    sort_kind = 1;
                else if (strncmp(t, "DESC", 4) == 0)
                    sort_kind = 2;
                else
                {
                    cerr << "Invalid input!\n";
                    return;
                }

                sel_new(num_of_comma + 1, tablename, p, sortby, sort_kind);
            }


            else //��������from
            {
                char *t = p;
                int num_of_comma = 0;
                while (strncmp(t, " FROM ", 6) != 0)
                {
                    if (*t == ',')
                    {
                        if (*(t + 1) == ' ' || strncmp(t + 1, "FROM", 4) == 0)
                        {
                            cout << "Invalid input!\n";
                            return;
                        }

                        num_of_comma++;
                    }
                        
                    t++;
                }


                t += 6; //����tablename��
                char tablename[40];
                strcpy(tablename, t);
                select(tablename, p, num_of_comma + 1);
            }
        }
    }


    else if (strncmp(s.c_str(), "MAX ", 4) == 0)
    {
        char *p = &s[4];
        char colname[40];
        strncpy(colname, p, coun(p));
        colname[coun(p)] = '\0';
        poi_mov(p);

        if (strncmp(p, "FROM ", 5) != 0)
        {
            cerr << "Invalid input!\n";
            return;
        }

        p += 5;
        char tablename[40];
        strcpy(tablename, p);
        num_proc(tablename, colname, 1); //�����ֵ
    }


    else if (strncmp(s.c_str(), "MIN ", 4) == 0)
    {
        char *p = &s[4];
        char colname[40];
        strncpy(colname, p, coun(p));
        colname[coun(p)] = '\0';
        poi_mov(p);

        if (strncmp(p, "FROM ", 5) != 0)
        {
            cerr << "Invalid input!\n";
            return;
        }

        p += 5;
        char tablename[40];
        strcpy(tablename, p);
        num_proc(tablename, colname, 2); //����Сֵ
    }


    else if (strncmp(s.c_str(), "MEAN ", 5) == 0)
    {
        char *p = &s[5];
        char colname[40];
        strncpy(colname, p, coun(p));
        colname[coun(p)] = '\0';
        poi_mov(p);

        if (strncmp(p, "FROM ", 5) != 0)
        {
            cerr << "Invalid input!\n";
            return;
        }

        p += 5;
        char tablename[40];
        strcpy(tablename, p);
        num_proc(tablename, colname, 0); //���ֵ
    }


    else if (strncmp(s.c_str(), "DO ", 3) == 0) //�����ļ�
    {
        char *p = &s[3];
        char filename[40];
        strcpy(filename, p);
        todos(filename);
    }


	else cerr << "Invalid input!\n";
}


void tableprint() //��ӡ��ǰ��table
{
    if (row.size() > 0)
    {
        rowit = row.begin();
        while (rowit != row.end())
        {
            (*rowit).colit = (*rowit).col.begin();
            rowit++;
        }
        rowit = row.begin();

        printf("%-15s", "ID");
        
        while ((*rowit).colit != (*rowit).col.end())
        {
            char *outputtemp = &(*((*rowit).colit))[0];
            printf("%-15s", outputtemp);
            ++(*rowit).colit; //ָ����һ��
        }

        ++rowit;
        cout << endl;

        int k = 1;
        if (rowit == row.end())
            cout << endl;
        while (rowit != row.end())
        {
            printf("%-15d", k);
            k++;

            while ((*rowit).colit != (*rowit).col.end())
            {
                char *outputtemp = &(*((*rowit).colit))[0];
                printf("%-15s", outputtemp);
                ++(*rowit).colit; //ָ����һ��
            }
            
            ++rowit; //ָ����һ��
            cout << endl;
        }
    }

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();
}


/*******************************************************************************************************************/


void create_table(char *tablename, char *filename) //���ļ��д�table
{//ȷ�����ļ��Ѵ��ڣ��Ż���ļ�
    row.clear(); //���������row
    strcpy(cur_table, tablename); //ȷ����ǰҪ������table��
    char temp[100];
    char rowtemp[500]; //�еĻ���
    string temp0;
    table_column head;

    FILE *fp = fopen(filename, "r"); //���ļ�
    
    fgets(rowtemp, 500, fp);

    char *t = rowtemp, *ttemp = rowtemp;
    int wordlen = 0;
    while (*t != '\n')
    {
        wordlen = 0;
        while (*t != ' ' && *t != '\n')
        {
            wordlen++;
            t++;
        }

        if (*t != '\n')
            t++;
        strncpy(temp, ttemp, wordlen);
        temp[wordlen] = '\0';
        ttemp += (wordlen + 1);
        temp0 = temp;
        head.col.push_back(temp0);
    }

    head.colit = head.col.begin();
    row.push_back(head);
    head.col.clear();
    fgets(rowtemp, 500, fp);

    while (!feof(fp))
    {
        char *t = rowtemp, *ttemp = rowtemp;
        int wordlen = 0;
        while (*t != '\n')
        {
            wordlen = 0;
            while (*t != ' ' && *t != '\n')
            {
                wordlen++;
                t++;
            }

            if (*t != '\n')
                t++;
            strncpy(temp, ttemp, wordlen);
            temp[wordlen] = '\0';
            ttemp += (wordlen + 1);
            temp0 = temp;
            head.col.push_back(temp0);
        }

        head.colit = head.col.begin();
        row.push_back(head);
        head.col.clear();
        fgets(rowtemp, 500, fp);
    }

    if (row.size() > 0)
    {
        rowit = row.begin();
        while (rowit != row.end())
        {
            (*rowit).colit = (*rowit).col.begin();
            rowit++;
        }
        rowit = row.begin();
    }

    fclose(fp);
} 


void create_table(char *tablename, char *filename, char *start) //create table:���û�������������
{
    row.clear(); //���������row
    num_of_tables++;
    strcpy(cur_table, tablename); //ȷ����ǰҪ������table��
    int wordlen = 0; //��ĳ���
	table_column head; //����������ݵ�����
	char *t = &start[0];
	
	while (*t != ')')
	{
        wordlen = 0;
		while (*t != ',' && *t != ')') //�����Ժ���Ҫ����wordlenΪ0�����
		{
			wordlen++;
			t++;
		}
        
		char *p = new char[wordlen + 1];
		strncpy(p, start, wordlen);
        p[wordlen] = '\0';
		string temp = p;
		delete p;
		
		head.col.push_back(temp);
		
		if (*t != ')')
			t++;

        start += (wordlen + 1);
	}
	head.colit = head.col.begin(); //ָ���һ��Ԫ��
	
	
	row.push_back(head); //��ʽ�����һ�У�Ҳ�����ݵı�ͷ
	rowit = row.begin(); //row������ָ���һ��
	
    tableprint(); //��ӡһ��
	
	//д��״̬�ļ��ʹ����ļ�
	FILE *fp = fopen(filename, "a"); //�����ļ�
	head.colit = head.col.begin();
	while (head.colit != head.col.end())
	{
		fprintf(fp, "%s", &(*(head.colit))[0]);
		if (++head.colit != head.col.end())
			fputc(' ', fp);
		else
			fputc('\n', fp);
	}
	fclose(fp);
	
	FILE *st = fopen("state.txt", "a"); //¼�뵽state�ļ���
	fprintf(st, "%s\t", tablename);
	fprintf(st, "%s\n", filename);
	fclose(st);
	
    tables tabletemp; //��������е�state����
    tabletemp.filename = filename;
    tabletemp.tablename = tablename;
    table.push_back(tabletemp);
    if (table.size() > 0)
        tableit = table.begin();

	cout << endl;
}


void create_table(char *filename) //���ļ��д����µ�table���û������table��������ļ���һ�£����򱨴�
{
    row.clear(); //���������row
    num_of_tables++;
    char tablename[40];
    int len = strlen(filename) - 4;
    strncpy(tablename, filename, len);
    tablename[len] = '\0';

    strcpy(cur_table, tablename); //ȷ����ǰҪ������table��

    tables tabletemp = {tablename, filename};
    table.push_back(tabletemp); //�µ�tableҪ���table�б���
    FILE *st = fopen("state.txt", "a"); //¼�뵽state�ļ���
    fprintf(st, "%s\t", tablename);
    fprintf(st, "%s\n", filename);
    fclose(st);

    char temp[100];
    char rowtemp[500]; //�еĻ���
    string temp0;
    table_column head;


    FILE *fp = fopen(filename, "r"); //���ļ�

    fgets(rowtemp, 500, fp); //ע��fgets

    char *t = rowtemp, *ttemp = rowtemp;
    int wordlen = 0;
    while (*t != '\n') //ע�� linux����"\r\n"
    {
        wordlen = 0;
        while (*t != ' ' && *t != '\n')
        {
            wordlen++;
            t++;
        }

        if (*t != '\n')
            t++;
        strncpy(temp, ttemp, wordlen);
        temp[wordlen] = '\0';
        ttemp += (wordlen + 1);
        temp0 = temp;
        head.col.push_back(temp0);
    }

    head.colit = head.col.begin();
    row.push_back(head);
    head.col.clear();
    fgets(rowtemp, 500, fp); 

    while (!feof(fp))
    {
        char *t = rowtemp, *ttemp = rowtemp;
        int wordlen = 0;
        while (*t != '\n')
        {
            wordlen = 0;
            while (*t != ' ' && *t != '\n')
            {
                wordlen++;
                t++;
            }

            if (*t != '\n')
                t++;
            strncpy(temp, ttemp, wordlen);
            temp[wordlen] = '\0';
            ttemp += (wordlen + 1);
            temp0 = temp;
            head.col.push_back(temp0);
        }

        head.colit = head.col.begin();
        row.push_back(head);
        head.col.clear();
        fgets(rowtemp, 500, fp);
    }

    if (row.size() > 0)
    {
        rowit = row.begin();
        while (rowit != row.end())
        {
            (*rowit).colit = (*rowit).col.begin();
            rowit++;
        }
        rowit = row.begin();
    }

    fclose(fp);
    if (table.size() > 0)
        tableit = table.begin();
}


void drop_table(char *name) //�����ݿ���ɾ����Ϊname��TABLE
{
    if (strcmp(name, cur_table) == 0) //�������ǵ�ǰ��table
    {
        row.clear(); //���������
        for (int i = 0; i < 40; i++)
            cur_table[i] = '\0';
    }

    char filetodel[44];
    while (tableit != table.end()) //���table���е����table
    {
        char *temp = &((*tableit).tablename)[0];
        if (strcmp(temp, name) == 0)
        {
            char *temp2 = &((*tableit).filename)[0];
            strcpy(filetodel, temp2);
            tableit = table.erase(tableit);
            if (table.size() > 0)
                tableit = table.begin();
            break;
        }

        else
            ++tableit;
    }

    num_of_tables--;

    FILE *st = fopen("state.txt", "w"); //����state�ļ�
    if (table.size() > 0)
        while (tableit != table.end()) //���table���е����table
        {
            char *tabletemp = &((*tableit).tablename)[0];
            char *filetemp = &((*tableit).filename)[0];

            fprintf(st, "%s ", tabletemp);
            fprintf(st, "%s\n", filetemp);
            tableit++;
        }
    fclose(st);

    tableit = table.begin();
    remove(filetodel); //ɾ����Ӧ���ļ�
}


void table_list() //��ӡ��ǰ���ݿ�������TABLE����Ϣ
{
    if (num_of_tables == 0)
    {
        cout << "No TABLE exists!\n";
        return;
    }

    else
    {
        char cur_temp[40];
        for (int i = 0; i < 40; i++)
            cur_temp[i] = '\0';
        if (cur_table[0] != '\0')
            strcpy(cur_temp, cur_table); //��ʱ����Ŀǰ������table��

        cout << "total: " << num_of_tables << "\n";
        while (tableit != table.end())
        {
            cout << (*tableit).tablename << ": (";
            char *temptablename = &((*tableit).tablename)[0];
            char *tempfilename = &((*tableit).filename)[0];
            create_table(temptablename, tempfilename);

            int k = (*rowit).col.size();
            cout << k << ", " << row.size() - 1 << ") [";
            for (int i = 0; i < k; i++)
            {
                if (i < k - 1)
                    cout << *(*rowit).colit << ", ";
                else
                    cout << *(*rowit).colit << "]";

                (*rowit).colit++;
            }
                
            cout << endl;
            tableit++;
        }


        tableit = table.begin();
        if (cur_temp[0] != '\0')
        {
            char filetemp[40];
            strcpy(filetemp, cur_temp);
            strcat(filetemp, ".txt");
            create_table(cur_temp, filetemp);
        }
    }

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();
}


void insert_into(char *tablename, char *p) //��һ�ֲ���: ����һ����
{
    char *t = p;
    int wordnum = 0;
    if (*t == '(')
    {
        while (*t != ')' && *t != '\0')
        {
            if (*t == ',' || ( *(t+1) == ')' && *t != '(' ))
                wordnum++;
            t++;
        }

        if (*t != ')')
        {
            cout << "Invalid input!\n";
            return;
        }

        else
        {
            if (strcmp(tablename, cur_table) == 0)
            {
                if ((*rowit).col.size() != wordnum)
                {
                    cout << "Invalid input!\n";
                    return;
                }

                t = ++p;
                table_column head;
                int wordlen = 0;

                while (*t != ')')
                {
                    wordlen = 0;
                    while (*t != ',' && *t != ')')
                    {
                        wordlen++;
                        t++;
                    }

                    char *temp = new char[wordlen + 1];
                    strncpy(temp, p, wordlen);
                    temp[wordlen] = '\0';
                    string temp2 = temp;
                    delete temp;

                    head.col.push_back(temp2);

                    if (*t != ')')
                        t++;

                    p += (wordlen + 1);
                }

                head.colit = head.col.begin(); //ָ���һ��Ԫ��
                row.push_back(head);
                cout << endl;
                tableprint(); //�����ӡһ��

                //д�뵽��Ӧ���ļ���
                char filename[40];
                strcpy(filename, tablename);
                strcat(filename, ".txt");
                FILE *fp = fopen(filename, "a"); //�����ļ�
                while (head.colit != head.col.end())
                {
                    fprintf(fp, "%s", &(*(head.colit))[0]);
                    if (++head.colit != head.col.end())
                        fputc(' ', fp);
                    else
                        fputc('\n', fp);
                }
                fclose(fp);
            }


            else //���ǵ�ǰ������table
            {//��Ҫ����û�е�ǰ��table�����
                tableit = table.begin();
                char *temp = NULL;
                bool found = false; //�Ƿ����
                while (tableit != table.end()) //����table
                {
                    temp = &((*tableit).tablename)[0];
                    if (strcmp(temp, tablename) == 0)
                    {
                        found = true;
                        break;
                    }

                    else
                        ++tableit;
                }

                tableit = table.begin();
                if (!found)
                {
                    cout << "TABLE DOES NOT EXIST!\n";
                    return;
                }

                char filenametemp[40];
                strcpy(filenametemp, tablename);
                strcat(filenametemp, ".txt");
                create_table(tablename, filenametemp); //�����Ŀǰ��table

                if ((*rowit).col.size() != wordnum)
                {
                    cout << "Invalid input!\n";
                    return;
                }


                t = ++p;
                table_column head;
                int wordlen = 0;

                while (*t != ')')
                {
                    wordlen = 0;
                    while (*t != ',' && *t != ')')
                    {
                        wordlen++;
                        t++;
                    }

                    char *temp = new char[wordlen + 1];
                    strncpy(temp, p, wordlen);
                    temp[wordlen] = '\0';
                    string temp2 = temp;
                    delete temp;

                    head.col.push_back(temp2);

                    if (*t != ')')
                        t++;

                    p += (wordlen + 1);
                }

                head.colit = head.col.begin(); //ָ���һ��Ԫ��
                row.push_back(head);
                cout << endl;
                tableprint(); //�����ӡһ��

                //д�뵽��Ӧ���ļ���
                char filename[40];
                strcpy(filename, tablename);
                strcat(filename, ".txt");
                FILE *fp = fopen(filename, "a"); //�����ļ�
                while (head.colit != head.col.end())
                {
                    fprintf(fp, "%s", &(*(head.colit))[0]);
                    if (++head.colit != head.col.end())
                        fputc(' ', fp);
                    else
                        fputc('\n', fp);
                }
                fclose(fp);
            }
        }
    }

    else
    {
        cout << "Invalid input!\n";
        return;
    }
}


void insert_into(char *tablename, char *p, int num) //�ڶ��ֲ��룺ָ������
{
    if (strcmp(tablename, cur_table) == 0) //�ǵ�ǰ������table
    {
        if ((*rowit).col.size() < num) //ֻ��С��ʱ����
        {
            cout << "Invalid input!\n";
            return;
        }
    }
    
    else if (strcmp(tablename, cur_table) != 0) //���ǵ�ǰ������table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //�Ƿ����
        while (tableit != table.end()) //����table
        {
            temp = &((*tableit).tablename)[0];
            if (strcmp(temp, tablename) == 0)
            {
                found = true;
                break;
            }

            else
                ++tableit;
        }

        tableit = table.begin();
        if (!found)
        {
            cout << "TABLE DOES NOT EXIST!\n";
            return;
        }

        char filenametemp[40];
        strcpy(filenametemp, tablename);
        strcat(filenametemp, ".txt");
        create_table(tablename, filenametemp); //�����Ŀǰ��table

        if ((*rowit).col.size() < num)
        {
            cout << "Invalid input!\n";
            return;
        }
    }

    //֮���ͨ�ò���
    char *t = ++p;
    table_column head;
    int wordlen = 0;

    char **toadd = new char*[num];
    int k = 0;

    while (*t != ')')
    {
        wordlen = 0;
        while (*t != ',' && *t != ')')
        {
            wordlen++;
            t++;
        }

        if (*t == ',')
            t++;

        toadd[k] = new char[wordlen + 1];
        strncpy(toadd[k], p, wordlen);
        toadd[k][wordlen] = '\0';
        p += (wordlen + 1);
        k++;
    }
    for (int i = 0; i < num; i++)
        cout << toadd[i] << endl;

    p += 9;
    t = p;
    wordlen = 0;

    char **toadd2 = new char*[num];
    k = 0;

    while (*t != ')')
    {
        wordlen = 0;
        while (*t != ',' && *t != ')')
        {
            wordlen++;
            t++;
        }

        if (*t == ',')
            t++;

        toadd2[k] = new char[wordlen + 1];
        strncpy(toadd2[k], p, wordlen);
        toadd2[k][wordlen] = '\0';
        p += (wordlen + 1);
        k++;
    }

    string tempstr;
    char *tempch = NULL;
    rowit = row.begin();
    bool in_row = false;
    //int indi = 0; //ָʾvector�е�λ��
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();

    while ((*rowit).colit != (*rowit).col.end())
    {
        in_row = false;
        tempch = &(*(*rowit).colit)[0];
        for (int i = 0; i < num; i++)
            if (strcmp(tempch, toadd[i]) == 0)
            {
                tempstr = toadd2[i]; //��Ӧ��ֵ
                head.col.push_back(tempstr);
                in_row = true;
                break;
            }

        if (!in_row)
        {
            tempstr = "NULL";
            head.col.push_back(tempstr);
        }

        (*rowit).colit++;
    }

    head.colit = head.col.begin();
    row.push_back(head);
    cout << endl;
    tableprint(); //�����ӡһ��

    delete[] toadd;
    delete[] toadd2;

    //д�뵽��Ӧ���ļ���
    char filename[40];
    strcpy(filename, tablename);
    strcat(filename, ".txt");
    FILE *fp = fopen(filename, "a"); //�����ļ�
    while (head.colit != head.col.end())
    {
        fprintf(fp, "%s", &(*(head.colit))[0]);
        if (++head.colit != head.col.end())
            fputc(' ', fp);
        else
            fputc('\n', fp);
    }
    fclose(fp);
}


void Delete(char *tablename) //�ڶ���Delete: �����ṹ��ɾ��
{
    if (strcmp(tablename, cur_table) != 0)
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //�Ƿ����
        while (tableit != table.end()) //����table
        {
            temp = &((*tableit).tablename)[0];
            if (strcmp(temp, tablename) == 0)
            {
                found = true;
                break;
            }

            else
                ++tableit;
        }

        tableit = table.begin();
        if (!found)
        {
            cout << "TABLE DOES NOT EXIST!\n";
            return;
        }

        char filenametemp[40];
        strcpy(filenametemp, tablename);
        strcat(filenametemp, ".txt");
        create_table(tablename, filenametemp); //�����Ŀǰ��table
    }
    
    rowit++;
    rowit = row.erase(rowit, row.end());
    rowit = row.begin();

    //д�뵽��Ӧ���ļ���
    char filename[40];
    strcpy(filename, tablename);
    strcat(filename, ".txt");
    FILE *fp = fopen(filename, "w"); //�����ļ�
    while (rowit != row.end())
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            fprintf(fp, "%s", &(*(*rowit).colit)[0]);
            if (++(*rowit).colit != (*rowit).col.end())
                fputc(' ', fp);
            else
                fputc('\n', fp);
        }
        ++rowit;
    }
    fclose(fp);

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();
}


void Delete(char *tablename, char *colname, char *colcont, int choice) //��һ��Delete: ɾ��һ��ָ������
{
    if (strcmp(tablename, cur_table) != 0)
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //�Ƿ����
        while (tableit != table.end()) //����table
        {
            temp = &((*tableit).tablename)[0];
            if (strcmp(temp, tablename) == 0)
            {
                found = true;
                break;
            }

            else
                ++tableit;
        }

        tableit = table.begin();
        if (!found)
        {
            cout << "TABLE DOES NOT EXIST!\n";
            return;
        }

        char filenametemp[40];
        strcpy(filenametemp, tablename);
        strcat(filenametemp, ".txt");
        create_table(tablename, filenametemp); //�����Ŀǰ��table
    }


    char *tempcolname = NULL;
    bool found = false;
    int k = 0;
    while ((*rowit).colit != (*rowit).col.end())
    {
        tempcolname = &(*(*rowit).colit)[0];
        if (strcmp(tempcolname, colname) == 0)
        {
            found = true;
            break;
        }
            
        (*rowit).colit++;
        k++;
    }
    (*rowit).colit = (*rowit).col.begin();

    if (!found)
    {
        cout << "No such column found!\n";
        return;
    }

    rowit++;
    if (choice == 0) //����
        while (rowit != row.end())
        {
            (*rowit).colit = (*rowit).col.begin() + k;
            char *temp = &(*(*rowit).colit)[0];
            if (strcmp(temp, colcont) == 0)
            {
                rowit = row.erase(rowit); //ɾ����һ��
                continue;
            }
            
            rowit++;
        }


    else if (choice == -1) //С��
        while (rowit != row.end())
        {
            (*rowit).colit = (*rowit).col.begin() + k;
            char *temp = &(*(*rowit).colit)[0];
            if (strcmp(temp, colcont) < 0)
            {
                rowit = row.erase(rowit); //ɾ����һ��
                continue;
            }

            if (rowit != row.end())
                rowit++;
        }

    else if (choice == 1) //����
        while (rowit != row.end())
        {
            (*rowit).colit = (*rowit).col.begin() + k;
            char *temp = &(*(*rowit).colit)[0];
            if (strcmp(temp, colcont) > 0)
            {
                rowit = row.erase(rowit); //ɾ����һ��
                continue;
            }

            if (rowit != row.end())
                rowit++;
        }
    

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();

    tableprint();
    //д�뵽��Ӧ���ļ���
    char filename[40];
    strcpy(filename, tablename);
    strcat(filename, ".txt");
    FILE *fp = fopen(filename, "w"); //�����ļ�
    while (rowit != row.end())
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            fprintf(fp, "%s", &(*(*rowit).colit)[0]);
            if (++(*rowit).colit != (*rowit).col.end())
                fputc(' ', fp);
            else
                fputc('\n', fp);
        }
        ++rowit;
    }
    fclose(fp);

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();
}


void update(char *tablename, char *p, int num, bool has_where) //����TABLE������ָ���е�ֵ
{
    if (strcmp(tablename, cur_table) != 0)
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //�Ƿ����
        while (tableit != table.end()) //����table
        {
            temp = &((*tableit).tablename)[0];
            if (strcmp(temp, tablename) == 0)
            {
                found = true;
                break;
            }

            else
                ++tableit;
        }

        tableit = table.begin();
        if (!found)
        {
            cout << "TABLE DOES NOT EXIST!\n";
            return;
        }

        char filenametemp[40];
        strcpy(filenametemp, tablename);
        strcat(filenametemp, ".txt");
        create_table(tablename, filenametemp); //�����Ŀǰ��table
    }


    char **colname = new char *[num];
    char **colvalue = new char *[num];
    int k = 0; //ָʾ�ڼ����ַ���
    char *t = p;

    if (!has_where)
    {
        while (*t != '\0')
        {
            int n = coun(t);
            colname[k] = new char[n + 1];
            strncpy(colname[k], t, n);
            colname[k][n] = '\0';
            poi_mov(t); //��ʱ�����ȺŴ�
            t += 2;

            int n2 = 0;
            while (*t != ',' && *t != '\0')
            {
                n2++;
                t++;
            }
            t -= n2;
            colvalue[k] = new char[n2 + 1];
            strncpy(colvalue[k], t, n2);
            colvalue[k][n2] = '\0';
            k++;

            t += n2;
            //if (*t == ',')
            if (strncmp(t, ", ", 2) == 0)
                t += 2;
        }


        rowit = row.begin();
        while (rowit != row.end())
        {
            (*rowit).colit = (*rowit).col.begin();
            rowit++;
        }
        rowit = row.begin();


        char *tempcolname = NULL;
        vector<table_column>::iterator a = row.begin() + 1; //�µĵ������������޸�ֵ
        int colnum = 0; //ָʾ��ǰ���е�������
        while ((*rowit).colit != (*rowit).col.end())
        {
            tempcolname = &(*(*rowit).colit)[0];
            for (int i = 0; i < k; i++)
                if (strcmp(tempcolname, colname[i]) == 0)
                {
                    while (a != row.end())
                    {
                        (*a).colit = (*a).col.begin() + colnum;
                        *(*a).colit = colvalue[i];
                        a++;
                    }

                    a = row.begin() + 1;
                    break;
                }

            (*rowit).colit++;
            colnum++;
        } //�˴��޸����
    }
    

    else //��where
    {
        while (strncmp(t, "WHERE ", 6) != 0)
        {
            int n = coun(t);
            colname[k] = new char[n + 1];
            strncpy(colname[k], t, n);
            colname[k][n] = '\0';
            poi_mov(t); //��ʱ�����ȺŴ�
            t += 2;

            int n2 = 0;
            while (*t != ',' && *t != ' ')
            {
                n2++;
                t++;
            }
            t -= n2;
            colvalue[k] = new char[n2 + 1];
            strncpy(colvalue[k], t, n2);
            colvalue[k][n2] = '\0';
            k++;

            t += n2;
            if (strncmp(t, ", ", 2) == 0)
                t += 2;
            else if (strncmp(t, " WHERE ", 7) == 0)
                t++;
        }


        t += 6; //����where�������һ��
        string mod_colu(t, 0, coun(t));
        poi_mov(t);
        t += 2;
        string mod_value(t);


        //�������ж��û�����ĸ������Ƿ����
        vector<string>::iterator b = (*rowit).col.begin(); //�µĵ���������������ֵ
        for (int i = 0; i < k; i++)
        {
            string tempser = colname[i];
            b = find((*rowit).col.begin(), (*rowit).col.end(), tempser);
            if (b == (*rowit).col.end())
            {
                cout << "Column name error!\n";
                return;
            }
        }
        string tempser = mod_colu;
        b = find((*rowit).col.begin(), (*rowit).col.end(), tempser);
        if (b == (*rowit).col.end())
        {
            cout << "Column name error!\n";
            return;
        }


        vector<table_column>::iterator a = row.begin() + 1; //�µĵ������������޸�ֵ
        int colnum = 0; //ָʾ��Ҫ�޸ĵ���һ�еı��
        int colnum2 = 0;


        rowit = row.begin();
        while (rowit != row.end())
        {
            (*rowit).colit = (*rowit).col.begin();
            rowit++;
        }
        rowit = row.begin();


        while ((*rowit).colit != (*rowit).col.end())
        {
            if (mod_colu == *(*rowit).colit)
                break;
               
            (*rowit).colit++;
            colnum++;
        }
         
        (*rowit).colit = (*rowit).col.begin();


        while (a != row.end())
        {
            (*a).colit = (*a).col.begin() + colnum;
            if (*(*a).colit == mod_value)
            {
                (*a).colit = (*a).col.begin();

                char *tempcolname = NULL;
                while ((*rowit).colit != (*rowit).col.end())
                {
                    tempcolname = &(*(*rowit).colit)[0];
                    for (int i = 0; i < k; i++)
                        if (strcmp(tempcolname, colname[i]) == 0)
                        {
                            (*a).colit = (*a).col.begin() + colnum2;
                            *(*a).colit = colvalue[i];
                            break;
                        }

                    (*rowit).colit++;
                    colnum2++;
                } 
            }

            (*rowit).colit = (*rowit).col.begin();
            colnum2 = 0;
            a++;
        } //�˴��޸����
    }


    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();

    tableprint();
    //д�뵽��Ӧ���ļ���
    char filename[40];
    strcpy(filename, tablename);
    strcat(filename, ".txt");
    FILE *fp = fopen(filename, "w"); //�����ļ�
    while (rowit != row.end())
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            fprintf(fp, "%s", &(*(*rowit).colit)[0]);
            if (++(*rowit).colit != (*rowit).col.end())
                fputc(' ', fp);
            else
                fputc('\n', fp);
        }
        ++rowit;
    }
    fclose(fp);

    delete[] colname;
    delete[] colvalue;

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();
}


void select(char *tablename, char *p, int num) //select 1: ѡ����������չʾ
{
    char **colname = new char *[num];
    char *t = p;
    int k = 0;

    while (strncmp(t, "FROM ", 5) != 0)
    {
        int n = 0;
        while (*t != ',' && *t != ' ')
        {
            n++;
            t++;
        }
        t -= n;

        colname[k] = new char[n + 1];
        strncpy(colname[k], t, n);
        colname[k][n] = '\0';
        k++;
        t += (n + 1);
    }


    if (strcmp(tablename, cur_table) != 0) //���ǵ�ǰ��table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //�Ƿ����
        while (tableit != table.end()) //����table
        {
            temp = &((*tableit).tablename)[0];
            if (strcmp(temp, tablename) == 0)
            {
                found = true;
                break;
            }

            else
                ++tableit;
        }

        tableit = table.begin();
        if (!found)
        {
            cout << "TABLE DOES NOT EXIST!\n";
            return;
        }

        char filenametemp[40];
        strcpy(filenametemp, tablename);
        strcat(filenametemp, ".txt");
        create_table(tablename, filenametemp); //�����Ŀǰ��table
    }


    if ((*rowit).col.size() < num)
    {
        cout << "Invalid input!\n";
        return;
    }

    vector<string>::iterator b = (*rowit).col.begin(); //�µĵ���������������ֵ
    for (int i = 0; i < k; i++)
    {
        string tempser = colname[i];
        b = find((*rowit).col.begin(), (*rowit).col.end(), tempser);
        if (b == (*rowit).col.end())
        {
            cout << "Column name error!\n";
            return;
        }
    }

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();


    printf("%-15s", "ID");
    int *loc = new int[num]; //����λ��
    int colloc = 0;
    for (int i = 0; i < k; i++)
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            char *outputtemp = &(*((*rowit).colit))[0];
            if (strcmp(colname[i], outputtemp) == 0)
            {
                printf("%-15s", outputtemp);
                loc[i] = colloc;
                break;
            }
            
            ++(*rowit).colit; //ָ����һ��
            colloc++;
        }

        (*rowit).colit = (*rowit).col.begin();
        colloc = 0;
    }
    cout << endl;

    rowit++;
    int rowno = 1; //�к�
    while (rowit != row.end())
    {
        printf("%-15d", rowno);
        for (int i = 0; i < num; i++)
        {
            //printf("%-15d", i + 1);
            (*rowit).colit = (*rowit).col.begin() + loc[i];
            char *outputtemp = &(*((*rowit).colit))[0];
            printf("%-15s", outputtemp);
        }

        cout << "\n";
        rowit++;
        rowno++;
    }

    cout << endl;
    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();
}


void select(char *tablename, char *p, int num, int Is_dis) //��distinct
{
    char **colname = new char *[num];
    char *t = p + 9;
    int k = 0;

    while (strncmp(t, "FROM ", 5) != 0)
    {
        int n = 0;
        while (*t != ',' && *t != ' ')
        {
            n++;
            t++;
        }
        t -= n;

        colname[k] = new char[n + 1];
        strncpy(colname[k], t, n);
        colname[k][n] = '\0';
        k++;
        t += (n + 1);
    }


    if (strcmp(tablename, cur_table) != 0) //���ǵ�ǰ��table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //�Ƿ����
        while (tableit != table.end()) //����table
        {
            temp = &((*tableit).tablename)[0];
            if (strcmp(temp, tablename) == 0)
            {
                found = true;
                break;
            }

            else
                ++tableit;
        }

        tableit = table.begin();
        if (!found)
        {
            cout << "TABLE DOES NOT EXIST!\n";
            return;
        }

        char filenametemp[40];
        strcpy(filenametemp, tablename);
        strcat(filenametemp, ".txt");
        create_table(tablename, filenametemp); //�����Ŀǰ��table
    }


    if ((*rowit).col.size() < num)
    {
        cout << "Invalid input!\n";
        return;
    }


    vector<string>::iterator b = (*rowit).col.begin(); //�µĵ���������������ֵ
    for (int i = 0; i < k; i++)
    {
        string tempser = colname[i];
        b = find((*rowit).col.begin(), (*rowit).col.end(), tempser);
        if (b == (*rowit).col.end())
        {
            cout << "Column name error!\n";
            return;
        }
    }

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();



    printf("%-15s", "ID");
    int *loc = new int[num]; //������Щ�����������е�λ��
    int colloc = 0;
    for (int i = 0; i < k; i++)
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            char *outputtemp = &(*((*rowit).colit))[0];
            if (strcmp(colname[i], outputtemp) == 0)
            {
                printf("%-15s", outputtemp);
                loc[i] = colloc;
                break;
            }

            ++(*rowit).colit; //ָ����һ��
            colloc++;
        }

        (*rowit).colit = (*rowit).col.begin();
        colloc = 0;
    }
    cout << endl; //��ӡ���һ��


    rowit++;
    int* rownums = new int[num]; //���ÿ���ж�Ӧ�ж�����
    for (int i = 0; i < num; i++)
        rownums[i] = 0; //��ʼ��
    vector<string> *vtemp = new vector<string>[num]; //�������򴢴沢����ɾ��row
    for (int i = 0; i < num; i++)
    {
        while (rowit != row.end())
        {
            (*rowit).colit = (*rowit).col.begin() + loc[i];
            vtemp[i].push_back(*(*rowit).colit);
            rowit++;
        }
        rowit = row.begin();
        sort(vtemp[i].begin(), vtemp[i].end()); //������
        vtemp[i].erase(unique(vtemp[i].begin(), vtemp[i].end()), vtemp[i].end()); //��ɾ�ظ�Ԫ��
        rownums[i] = vtemp[i].size();
    }
    
    //���
    int max_row_num = Max(rownums, num);
    for (int i = 0; i < max_row_num; i++) //��
    {
        printf("%-15d", i + 1);
        for (int j = 0; j < num; j++) //��
        {
            if (vtemp[j].size() < i + 1)
                printf("%-15s", " ");
            else
            {
                char *oute = &(vtemp[j][i][0]);
                printf("%-15s", oute);
            }
        }
        printf("\n");
    }
    
    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();

    delete[] colname;
    delete[] loc;
    delete[] rownums;
    delete[] vtemp;
}


void select(char *tablename, char *p) //ORDER BY
{
    char *t = p;

    int num_of_comma = 0;
    while (strncmp(t, " ASC", 4) != 0 && strncmp(t, " DESC", 5) != 0)
    {
        if (*t == ',')
        {
            if (*(t + 1) == ' ')
            {
                cout << "Invalid input!\n";
                return;
            }

            num_of_comma++;
        }

        t++;
    }


    t = p;
    int k = 0;
    char **colname = new char *[num_of_comma + 1];
    while (strncmp(t, " ASC", 4) != 0 && strncmp(t, " DESC", 5) != 0)
    {
        int n = 0;
        while (*t != ',' && *t != ' ')
        {
            n++;
            t++;
        }
        t -= n;

        colname[k] = new char[n + 1];
        strncpy(colname[k], t, n);
        colname[k][n] = '\0';
        k++;
        t += n;
        if (strncmp(t, " ASC", 4) != 0 && strncmp(t, " DESC", 5) != 0)
            t++;
    }


    if (strcmp(tablename, cur_table) != 0) //���ǵ�ǰ��table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //�Ƿ����
        while (tableit != table.end()) //����table
        {
            temp = &((*tableit).tablename)[0];
            if (strcmp(temp, tablename) == 0)
            {
                found = true;
                break;
            }

            else
                ++tableit;
        }

        tableit = table.begin();
        if (!found)
        {
            cout << "TABLE DOES NOT EXIST!\n";
            return;
        }

        char filenametemp[40];
        strcpy(filenametemp, tablename);
        strcat(filenametemp, ".txt");
        create_table(tablename, filenametemp); //�����Ŀǰ��table
    }


    if ((*rowit).col.size() < num_of_comma + 1)
    {
        cout << "Invalid input!\n";
        return;
    }


    vector<string>::iterator b = (*rowit).col.begin(); //�µĵ���������������ֵ
    for (int i = 0; i < k; i++)
    {
        string tempser = colname[i];
        b = find((*rowit).col.begin(), (*rowit).col.end(), tempser);
        if (b == (*rowit).col.end())
        {
            cout << "Column name error!\n";
            return;
        }
    }

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();


    printf("%-15s", "ID");
    int *loc = new int[num_of_comma+1]; //������Щ�����������е�λ��
    int colloc = 0;


    for (int i = 0; i < k; i++) //��ӵ�loc
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            char *outputtemp = &(*((*rowit).colit))[0];
            if (strcmp(colname[i], outputtemp) == 0)
            {
                //printf("%-15s", outputtemp);
                loc[i] = colloc;
                break;
            }

            ++(*rowit).colit; //ָ����һ��
            colloc++;
        }

        (*rowit).colit = (*rowit).col.begin();
        colloc = 0;
    }

    
    while ((*rowit).colit != (*rowit).col.end()) //��ӡ��һ��
    {
        char *outputtemp = &(*((*rowit).colit))[0];
        printf("%-15s", outputtemp); //ÿһ�ж�Ҫ���
        ++(*rowit).colit; //ָ����һ��
    }
    cout << endl; //��ӡ���һ��


    rowit++;
    vector< vector<string> > vtemp;
    vector<string> vstrtemp;


    while (rowit != row.end())
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            vstrtemp.push_back(*(*rowit).colit); //���ж�Ҫѹ�룬����������
            ++(*rowit).colit; //ָ����һ��
        }
            

        vtemp.push_back(vstrtemp);
        vstrtemp.clear();
        rowit++;
    }


    if (strncmp(t, " ASC", 4) == 0) //����
    {
        for (int cl = num_of_comma; cl >= 0; cl--)
            for (int i = vtemp.size() - 1; i > 0; i--)
                for (int j = 0; j < i; j++)
                {
                    if (vtemp[j][loc[cl]].compare(vtemp[j + 1][loc[cl]]) > 0)
                        swap(vtemp[j], vtemp[j + 1]);
                }
    }

    else if (strncmp(t, " DESC", 5) == 0) //����
    {
        for (int cl = num_of_comma; cl >= 0; cl--)
            for (int i = vtemp.size() - 1; i > 0; i--)
                for (int j = 0; j < i; j++)
                {
                    if (vtemp[j][loc[cl]].compare(vtemp[j + 1][loc[cl]]) < 0)
                        swap(vtemp[j], vtemp[j + 1]);
                }
    }


    //���
    for (int i = 0; i < vtemp.size(); i++) //��
    {
        printf("%-15d", i + 1);
        for (int j = 0; j < vtemp[0].size(); j++) //�� //num_of_comma + 1
        {
            char *oute = &(vtemp[i][j][0]);
            printf("%-15s", oute);
        }
        printf("\n");
    }

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();


    delete[] colname;
    delete[] loc;
}


void select(char *tablename, char *p, int num, bool Is_dis) ////������where
{
    char **colname = new char *[num];
    char *t = p;
    int k = 0;

    while (strncmp(t, "FROM ", 5) != 0)
    {
        int n = 0;
        while (*t != ',' && *t != ' ')
        {
            n++;
            t++;
        }
        t -= n;

        colname[k] = new char[n + 1];
        strncpy(colname[k], t, n);
        colname[k][n] = '\0';
        k++;
        t += (n + 1);
    }

    poi_mov(t);
    poi_mov(t);
    poi_mov(t);

    char modcol[40]; //Ҫѡ����е�����
    strncpy(modcol, t, coun(t));
    modcol[coun(t)] = '\0';
    poi_mov(t);
    t += 2;

    char modval[40]; //Ҫѡ����еľ���ֵ
    strcpy(modval, t);

    for (int i = 0; i < num; i++)
        if (strcmp(colname[i], modcol) == 0)
        {
            cout << "Repeated columns!\n";
            return;
        }


    if (strcmp(tablename, cur_table) != 0) //���ǵ�ǰ��table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //�Ƿ����
        while (tableit != table.end()) //����table
        {
            temp = &((*tableit).tablename)[0];
            if (strcmp(temp, tablename) == 0)
            {
                found = true;
                break;
            }

            else
                ++tableit;
        }

        tableit = table.begin();
        if (!found)
        {
            cout << "TABLE DOES NOT EXIST!\n";
            return;
        }

        char filenametemp[40];
        strcpy(filenametemp, tablename);
        strcat(filenametemp, ".txt");
        create_table(tablename, filenametemp); //�����Ŀǰ��table
    }


    if ((*rowit).col.size() < num + 1)
    {
        cout << "Invalid input!\n";
        return;
    }

    vector<string>::iterator b = (*rowit).col.begin(); //�µĵ���������������ֵ
    for (int i = 0; i < k; i++)
    {
        string tempser = colname[i];
        b = find((*rowit).col.begin(), (*rowit).col.end(), tempser);
        if (b == (*rowit).col.end())
        {
            cout << "Column name error!\n";
            return;
        }
    }

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();


    printf("%-15s", "ID"); 
    int *loc = new int[num + 1]; //����ָ���е�λ��
    int colloc = 0;

    while ((*rowit).colit != (*rowit).col.end())
    {
        char *outputtemp = &(*((*rowit).colit))[0];

        if (strcmp(outputtemp, modcol) == 0)
        {
            loc[num] = colloc;
            break;
        }

        ++(*rowit).colit; //ָ����һ��
        colloc++;
    }

    (*rowit).colit = (*rowit).col.begin();
    colloc = 0;


    for (int i = 0; i < k; i++)
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            char *outputtemp = &(*((*rowit).colit))[0];
            if (strcmp(colname[i], outputtemp) == 0)
            {
                printf("%-15s", outputtemp);
                loc[i] = colloc;
                break;
            }

            ++(*rowit).colit; //ָ����һ��
            colloc++;
        }

        (*rowit).colit = (*rowit).col.begin();
        colloc = 0;
    }

    printf("%-15s", modcol);
    cout << endl; //��ӡ��һ��


    rowit++;
    int rowno = 1; //�к�
    while (rowit != row.end())
    {
        //for (int i = 0; i < num; i++)
        //{
        //    //printf("%-15d", i + 1);
        //    (*rowit).colit = (*rowit).col.begin() + loc[i];
        //    char *outputtemp = &(*((*rowit).colit))[0];
        //    printf("%-15s", outputtemp);
        //}

        //cout << "\n";
        //rowit++;
        //rowno++;
        (*rowit).colit = (*rowit).col.begin() + loc[num]; //��ָ�������е�λ��
        char *outputtemp = &(*((*rowit).colit))[0];
        if (strcmp(outputtemp, modval) == 0) //��Ҫ�������
        {
            printf("%-15d", rowno);
            for (int i = 0; i < num; i++)
            {
                (*rowit).colit = (*rowit).col.begin() + loc[i]; //�����i��λ�õ���
                char *outpu = &(*((*rowit).colit))[0];
                printf("%-15s", outpu);
            }

            printf("%-15s\n", modval);
            rowno++;
            /*(*rowit).colit = (*rowit).col.begin();
            printf("%-15d", rowno);
            int outcol = 0;
            while ((*rowit).colit != (*rowit).col.end())
            {
                char *outpu = &(*((*rowit).colit))[0];
                if (strcmp(outpu, ) == 0)
                printf("%-15s", outpu);
                
                (*rowit).colit++;
            }

            rowno++;
            cout << endl;*/
        }

        rowit++;
    }

    cout << endl;
    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();
}


void select(char *tablename, char *filename, char *p, int num) //�Ѳ�ѯ���д���ļ���: û��where
{
    char **colname = new char *[num];
    char *t = p;
    int k = 0;

    while (strncmp(t, "FROM ", 5) != 0)
    {
        int n = 0;
        while (*t != ',' && *t != ' ')
        {
            n++;
            t++;
        }
        t -= n;

        colname[k] = new char[n + 1];
        strncpy(colname[k], t, n);
        colname[k][n] = '\0';
        k++;
        t += (n + 1);
    }


    if (strcmp(tablename, cur_table) != 0) //���ǵ�ǰ��table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //�Ƿ����
        while (tableit != table.end()) //����table
        {
            temp = &((*tableit).tablename)[0];
            if (strcmp(temp, tablename) == 0)
            {
                found = true;
                break;
            }

            else
                ++tableit;
        }

        tableit = table.begin();
        if (!found)
        {
            cout << "TABLE DOES NOT EXIST!\n";
            return;
        }

        char filenametemp[40];
        strcpy(filenametemp, tablename);
        strcat(filenametemp, ".txt");
        create_table(tablename, filenametemp); //�����Ŀǰ��table
    }


    if ((*rowit).col.size() < num)
    {
        cout << "Invalid input!\n";
        return;
    }

    vector<string>::iterator b = (*rowit).col.begin(); //�µĵ���������������ֵ
    for (int i = 0; i < k; i++)
    {
        string tempser = colname[i];
        b = find((*rowit).col.begin(), (*rowit).col.end(), tempser);
        if (b == (*rowit).col.end())
        {
            cout << "Column name error!\n";
            return;
        }
    }

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();


    FILE *towrite = fopen(filename, "w"); //Ҫ��������ļ�������

    printf("%-15s", "ID");
    int *loc = new int[num]; //����λ��
    int colloc = 0;
    for (int i = 0; i < k; i++)
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            char *outputtemp = &(*((*rowit).colit))[0];
            if (strcmp(colname[i], outputtemp) == 0)
            {
                printf("%-15s", outputtemp); //���������̨

                fprintf(towrite, "%s", outputtemp); //������ļ�
                //if ((*rowit).colit + 1 != (*rowit).col.end())
                if (i != k - 1)
                    fputc(' ', towrite);
                else
                    fputc('\n', towrite);

                loc[i] = colloc;
                break;
            }
            
            ++(*rowit).colit; //ָ����һ��
            colloc++;
        }

        (*rowit).colit = (*rowit).col.begin();
        colloc = 0;
    }
    cout << endl; //��һ�������


    rowit++;
    int rowno = 1; //�к�
    while (rowit != row.end())
    {
        printf("%-15d", rowno);
        for (int i = 0; i < num; i++)
        {
            //printf("%-15d", i + 1);
            (*rowit).colit = (*rowit).col.begin() + loc[i];
            char *outputtemp = &(*((*rowit).colit))[0];
            printf("%-15s", outputtemp);

            fprintf(towrite, "%s", outputtemp); //������ļ�
            //if ((*rowit).colit + 1 != (*rowit).col.end())
            if (i < num-1)
                fputc(' ', towrite);
            else
                fputc('\n', towrite);
        }

        cout << "\n";
        rowit++;
        rowno++;
    }

    fclose(towrite);

    cout << endl;
    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();
}


void sel_file_wh(char *tablename, char *filename, char *p) //�Ѳ�ѯ���д���ļ���: ��where
{
    char *t = p;
    char modcol[40]; //Ҫѡ����е�����
    strncpy(modcol, t, coun(t));
    modcol[coun(t)] = '\0';
    poi_mov(t); //�����ȺŴ�
    t += 2;

    char modval[40]; //Ҫѡ����еľ���ֵ
    strncpy(modval, t, coun(t));
    modval[coun(t)] = '\0';


    if (strcmp(tablename, cur_table) != 0) //���ǵ�ǰ��table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //�Ƿ����
        while (tableit != table.end()) //����table
        {
            temp = &((*tableit).tablename)[0];
            if (strcmp(temp, tablename) == 0)
            {
                found = true;
                break;
            }

            else
                ++tableit;
        }

        tableit = table.begin();
        if (!found)
        {
            cout << "TABLE DOES NOT EXIST!\n";
            return;
        }

        char filenametemp[40];
        strcpy(filenametemp, tablename);
        strcat(filenametemp, ".txt");
        create_table(tablename, filenametemp); //�����Ŀǰ��table
    }


    vector<string>::iterator b = (*rowit).col.begin(); //�µĵ���������������ֵ
    string tempser = modcol;
    b = find((*rowit).col.begin(), (*rowit).col.end(), tempser);
    if (b == (*rowit).col.end())
    {
        cout << "Column name error!\n";
        return;
    }

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();


    FILE *towrite = fopen(filename, "w"); //Ҫ��������ļ�������

    printf("%-15s", "ID");
    int loc = 0; //����ָ���е�λ��
    int colloc = 0;

    while ((*rowit).colit != (*rowit).col.end())
    {
        char *outputtemp = &(*((*rowit).colit))[0];
        printf("%-15s", outputtemp); //�����ж�Ҫչʾ��������ļ�
        fprintf(towrite, "%s", outputtemp);
        //if (i != k - 1)
        if ((*rowit).colit + 1 != (*rowit).col.end())
            fputc(' ', towrite);
        else
            fputc('\n', towrite);


        if (strcmp(outputtemp, modcol) == 0)
        {
            loc = colloc;
            //break;
        }

        ++(*rowit).colit; //ָ����һ��
        colloc++;
    }

    (*rowit).colit = (*rowit).col.begin();
    cout << endl; //��ӡ���һ��


    rowit++;
    int rowno = 1; //�к�
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin() + loc; //��ָ�������е�λ��
        char *outputtemp = &(*((*rowit).colit))[0];
        if (strcmp(outputtemp, modval) == 0) //��Ҫ�������
        {
            printf("%-15d", rowno);
            (*rowit).colit = (*rowit).col.begin();

            while ((*rowit).colit != (*rowit).col.end())
            {
                char *outputtemp = &(*((*rowit).colit))[0];
                printf("%-15s", outputtemp); //�����ж�Ҫչʾ
                fprintf(towrite, "%s", outputtemp);
                if ((*rowit).colit + 1 != (*rowit).col.end())
                    fputc(' ', towrite);
                else
                    fputc('\n', towrite);

                (*rowit).colit++;
            }
            cout << endl;

            rowno++;
        }

        rowit++;
    }


    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();

    fclose(towrite);
}


void selectspe(char *tablename, char *p) //��ĳһ���ѯ
{
    char *t = p;
    char modcol[40]; //Ҫѡ����е�����
    strncpy(modcol, t, coun(t));
    modcol[coun(t)] = '\0';
    poi_mov(t); //�����ȺŴ�
    t += 2;

    char modval[40]; //Ҫѡ����еľ���ֵ
    strcpy(modval, t);
    //modval[coun(t)] = '\0';


    if (strcmp(tablename, cur_table) != 0) //���ǵ�ǰ��table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //�Ƿ����
        while (tableit != table.end()) //����table
        {
            temp = &((*tableit).tablename)[0];
            if (strcmp(temp, tablename) == 0)
            {
                found = true;
                break;
            }

            else
                ++tableit;
        }

        tableit = table.begin();
        if (!found)
        {
            cout << "TABLE DOES NOT EXIST!\n";
            return;
        }

        char filenametemp[40];
        strcpy(filenametemp, tablename);
        strcat(filenametemp, ".txt");
        create_table(tablename, filenametemp); //�����Ŀǰ��table
    }

    vector<string>::iterator b = (*rowit).col.begin(); //�µĵ���������������ֵ
    string tempser = modcol;
    b = find((*rowit).col.begin(), (*rowit).col.end(), tempser);
    if (b == (*rowit).col.end())
    {
        cout << "Column name error!\n";
        return;
    }

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();


    int loc = 0;
    int colloc = 0;
    //��ӡ��һ��
    while ((*rowit).colit != (*rowit).col.end())
    {
        char *outputtemp = &(*((*rowit).colit))[0];
        printf("%-15s", outputtemp); //�����ж�Ҫչʾ

        if (strcmp(outputtemp, modcol) == 0)
            loc = colloc;

        ++(*rowit).colit; //ָ����һ��
        colloc++;
    }

    (*rowit).colit = (*rowit).col.begin();
    cout << endl; //��ӡ���һ��


    rowit++;
    int rowno = 1; //�к�
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin() + loc; //��ָ�������е�λ��
        char *outputtemp = &(*((*rowit).colit))[0];
        if (strcmp(outputtemp, modval) == 0) //��Ҫ�������
        {
            printf("%-15d", rowno);
            (*rowit).colit = (*rowit).col.begin();
            while ((*rowit).colit != (*rowit).col.end())
            {
                char *outputtemp = &(*((*rowit).colit))[0];
                printf("%-15s", outputtemp); //�����ж�Ҫչʾ

                (*rowit).colit++;
            }
            cout << endl;
            rowno++;
        }

        rowit++;
    }


    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();
}


void sel_new(int num, char *tablename, char *p, char *sortby, int sort_kind) //���³��ֵĸ�ʽ
{
    char **colname = new char *[num];
    char *t = p;
    int k = 0;

    while (strncmp(t, "FROM ", 5) != 0)
    {
        int n = 0;
        while (*t != ',' && *t != ' ')
        {
            n++;
            t++;
        }
        t -= n;

        colname[k] = new char[n + 1];
        strncpy(colname[k], t, n);
        colname[k][n] = '\0';
        k++;
        t += (n + 1);
    }


    if (strcmp(tablename, cur_table) != 0) //���ǵ�ǰ��table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //�Ƿ����
        while (tableit != table.end()) //����table
        {
            temp = &((*tableit).tablename)[0];
            if (strcmp(temp, tablename) == 0)
            {
                found = true;
                break;
            }

            else
                ++tableit;
        }

        tableit = table.begin();
        if (!found)
        {
            cout << "TABLE DOES NOT EXIST!\n";
            return;
        }

        char filenametemp[40];
        strcpy(filenametemp, tablename);
        strcat(filenametemp, ".txt");
        create_table(tablename, filenametemp); //�����Ŀǰ��table
    }


    if ((*rowit).col.size() < num)
    {
        cout << "Invalid input!\n";
        return;
    }

    vector<string>::iterator b = (*rowit).col.begin(); //�µĵ���������������ֵ
    for (int i = 0; i < k; i++)
    {
        string tempser = colname[i];
        b = find((*rowit).col.begin(), (*rowit).col.end(), tempser);
        if (b == (*rowit).col.end())
        {
            cout << "Column name error!\n";
            return;
        }
    }

    bool Incol = false;
    for (int i = 0; i < num; i++)
    {
        if (strcmp(colname[i], sortby) == 0)
        {
            Incol = true;
            break;
        }
    }
    if (!Incol)
    {
        cerr << "Column to sort not in selected columns!\n";
        return;
    }


    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();


    printf("%-15s", "ID");
    int *loc = new int[num]; //������Щ�����������е�λ��
    int sortloc = 0;
    int colloc = 0;


    for (int i = 0; i < k; i++) //��ӵ�loc
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            char *outputtemp = &(*((*rowit).colit))[0];
            if (strcmp(colname[i], outputtemp) == 0)
            {
                printf("%-15s", outputtemp);
                loc[i] = colloc;

                if (strcmp(sortby, outputtemp) == 0)
                    sortloc = colloc;

                break;
            }

            ++(*rowit).colit; //ָ����һ��
            colloc++;
        }

        (*rowit).colit = (*rowit).col.begin();
        colloc = 0;
    }
    cout << endl; //��ӡ���һ��


    rowit++;
    vector< vector<string> > vtemp;
    vector<string> vstrtemp;

    while (rowit != row.end())
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            //if ()
            vstrtemp.push_back(*(*rowit).colit); //���ж�Ҫѹ�룬����������
            ++(*rowit).colit; //ָ����һ��
        }


        vtemp.push_back(vstrtemp);
        vstrtemp.clear();
        rowit++;
    }


    if (sort_kind == 1)//����
    {
        for (int i = vtemp.size() - 1; i > 0; i--)
            for (int j = 0; j < i; j++)
            {
                if (vtemp[j][sortloc].compare(vtemp[j + 1][sortloc]) > 0)
                    swap(vtemp[j], vtemp[j + 1]);
            }
    }

    else if (sort_kind == 2) //����
    {
        for (int i = vtemp.size() - 1; i > 0; i--)
            for (int j = 0; j < i; j++)
            {
                if (vtemp[j][sortloc].compare(vtemp[j + 1][sortloc]) < 0)
                    swap(vtemp[j], vtemp[j + 1]);
            }
    }


    //���
    for (int i = 0; i < vtemp.size(); i++) //��
    {
        printf("%-15d", i + 1);
        for (int j = 0; j < num; j++) //��
        {
            char *oute = &(vtemp[i][loc[j]][0]);
            printf("%-15s", oute);
        }

        printf("\n");
    }

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();


    delete[] colname;
    delete[] loc;
}


void num_proc(char *tablename, char *colname, int choice) //��ֵ����:���ֵ����Сֵ��ƽ��ֵ 0:��ֵ; 1�����; 2����С
{
    if (strcmp(tablename, cur_table) != 0) //���ǵ�ǰ��table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //�Ƿ����
        while (tableit != table.end()) //����table
        {
            temp = &((*tableit).tablename)[0];
            if (strcmp(temp, tablename) == 0)
            {
                found = true;
                break;
            }

            else
                ++tableit;
        }

        tableit = table.begin();
        if (!found)
        {
            cout << "TABLE DOES NOT EXIST!\n";
            return;
        }

        char filenametemp[40];
        strcpy(filenametemp, tablename);
        strcat(filenametemp, ".txt");
        create_table(tablename, filenametemp); //�����Ŀǰ��table
    }

    vector<string>::iterator b = (*rowit).col.begin(); //�µĵ���������������ֵ
    string tempser = colname;
    b = find((*rowit).col.begin(), (*rowit).col.end(), tempser);
    if (b == (*rowit).col.end())
    {
        cout << "Column name error!\n";
        return;
    }

    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();


    if (choice == 1) //���ֵ
    {
        int loc = 0;
        while (tempser != row[0].col[loc])
            loc++;

        if (row.size() <= 1)
        {
            cerr << "No elements to be compared!\n";
            return;
        }

        if (row.size() == 2)
        {
            cout << "Max Element: " << atof(&row[1].col[loc][0]) << endl;
            return;
        }


        double maxnum = atof(&row[1].col[loc][0]);
        for (int i = 1; i < row.size(); i++)
        {
            char *temp1 = &row[i].col[loc][0];
            if (atof(temp1) > maxnum)
                maxnum = atof(temp1);
        }


        cout << "Max Element: " << maxnum << endl;
    }

    
    else if (choice == 2) //����Сֵ
    {
        int loc = 0;
        while (tempser != row[0].col[loc])
            loc++;

        if (row.size() <= 1)
        {
            cerr << "No elements to be compared!\n";
            return;
        }

        if (row.size() == 2)
        {
            cout << "Min Element: " << atof(&row[1].col[loc][0]) << endl;
            return;
        }


        double minnum = atof(&row[1].col[loc][0]);
        for (int i = 1; i < row.size(); i++)
        {
            char *temp1 = &row[i].col[loc][0];
            if (atof(temp1) < minnum)
                minnum = atof(temp1);
        }


        cout << "Max Element: " << minnum << endl;
    }


    else if (choice == 0) //���ֵ
    {
        int loc = 0;
        while (tempser != row[0].col[loc])
            loc++;

        if (row.size() <= 1)
        {
            cerr << "No elements to be compared!\n";
            return;
        }

        if (row.size() == 2)
        {
            cout << "Mean value: " << atof(&row[1].col[loc][0]) << endl;
            return;
        }


        double meannum = 0;
        for (int i = 1; i < row.size(); i++)
            meannum += atof(&row[i].col[loc][0]);

        cout << "Mean value: " << meannum / row.size() << endl;
    }
}


void todos(char *filename) //�����ļ��Ķ�ȡ��ִ��
{
    FILE *todolist = fopen(filename, "r"); //�������ļ�
    if (todolist == NULL)
    {
        cerr << "Failed to open file!\n";
        return;
    }

    char statement_temp[150]; //����ÿһ�����
    int serial = 1; //ִ�е���������

    fgets(statement_temp, 152, todolist);
    int len = strlen(statement_temp);

    strncpy(statement_temp, statement_temp, len - 1); 
    statement_temp[len - 1] = '\0';

    cout << "Event " << serial << endl;
    string temp = statement_temp;
    func_detect(temp);
    serial++;
    cout << "\n===========================================================================================\n\n";


    while (!feof(todolist))
    {
        fgets(statement_temp, 152, todolist);
        len = strlen(statement_temp);

        strncpy(statement_temp, statement_temp, len - 1);
        statement_temp[len - 1] = '\0';

        cout << "Event " << serial << endl;
        temp = statement_temp;
        func_detect(temp);
        serial++;
        cout << "\n===========================================================================================\n\n";
    }

    cout << "Events End!\n";
}