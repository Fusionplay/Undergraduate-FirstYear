#include "funcs.h"

//全局变量
vector<table_column> row; //储存各行的数据
vector<table_column>::iterator rowit; //迭代器

vector<tables> table; //储存目前拥有的所有table
vector<tables>::iterator tableit; //相应的迭代器

int num_of_tables = 0; //table的数量
char cur_table[40]; //储存当前正在操作的table的名称


int coun(char *p) //计算某个单词的长度
{
	int i=1;
	while (p[i] != ' ')
		i++;

	return i;
}


void poi_mov(char *&p) //把字符指针移动到下一个“单词”处
{
    int i = 1;
    while (p[i] != ' ')
        i++;
    p = &p[i + 1];
}


bool In_str(char *target, char *source) //判断source是否为target的子串
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


int Max(int *arr, int len) //求int型数组中的最大元素
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


void initial() //加载原有的table
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


void func_detect(string &s) //探测功能
{
	if (strncmp(s.c_str(), "CREATE TABLE ", 13) == 0)
	{
		char *p = &s[13]; //从第13个字符开始录入进p里
		char tablename[40]; //用char来储存文件名等
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
            while (tableit != table.end()) //查找table
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


			create_table(filename); //确保该文件已存在，才会读文件 因此还要考虑文件不存在的情况
            tableprint(); //打印一下
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
						strcpy(filename, t); //这个地方以后要考虑用户输入有误的情况
						
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
        char *p = &s[12]; //从第13个字符开始录入进p里
        char tablename[40]; //用char来储存文件名等
        strncpy(tablename, p, coun(p));
        tablename[coun(p)] = '\0';
        poi_mov(p);

        if (strncmp(p, "VALUES ", 7) == 0)
        {
            poi_mov(p);
            insert_into(tablename, p); //第一种：插入一行
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
            char tablename[40]; //用char来储存文件名等
            strcpy(tablename, p);
            
            Delete(tablename);
        }


        else if (strncmp(p, "FROM ", 5) == 0)
        {
            p += 5;
            char tablename[40]; //用char来储存文件名等
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

                    Delete(tablename, colutemp, colucont, 0); //等于
                }

                else if (strncmp(p, "< ", 2) == 0)
                {
                    p += 2;
                    char colucont[60];
                    strcpy(colucont, p);

                    Delete(tablename, colutemp, colucont, -1); //小于
                }

                else if (strncmp(p, "> ", 2) == 0)
                {
                    p += 2;
                    char colucont[60];
                    strcpy(colucont, p);

                    Delete(tablename, colutemp, colucont, 1); //大于
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
        char *p = &s[7]; //从第7个字符开始录入进p里
        char tablename[40]; //用char来储存文件名等
        strncpy(tablename, p, coun(p));
        tablename[coun(p)] = '\0';
        poi_mov(p);
        bool has_where = false; //指示到底是哪个语句

        if (strncmp(p, "SET ", 4) == 0)
        {
            p += 4;
            char *t = p;
            int num_of_terms = 0; //有几项要修改:仅where前的
            while (*t != '\0') //这里只做最基本的输入错误判断，其它不考虑
            {
                if (*t == '=')
                    num_of_terms++;
                t++;
            }

            t = p;
            int num_of_comma = 0; //逗号的数量
            while (*t != '\0') //这里只做最基本的输入错误判断，其它不考虑
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
        char *p = &s[7]; //从第7个字符开始录入进p里
        if (strncmp(p, "* FROM ", 7) == 0 && !In_str(p, " ORDER BY ") && !In_str(p, " TO ") && !In_str(p, " WHERE ")) //展示整个table
        {
            p += 7;
            char tablename[40]; //用char来储存文件名等
            strcpy(tablename, p);

            if (strcmp(cur_table, tablename) == 0)
                tableprint();

            else //不是当前的table
            {
                tableit = table.begin();
                char *temp = NULL;
                bool found = false; //是否存在
                while (tableit != table.end()) //查找table
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
                    create_table(tablename, filenametemp); //变成了目前的table
                }

                tableprint();
            }
        }


        else if (strncmp(p, "* FROM ", 7) == 0 && In_str(p, " ORDER BY ") && !In_str(p, " TO ")) //有order by
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
            && In_str(p, " WHERE ")) //有to和where:把查询结果写入文件中的第二种
        {
            p += 7;
            char tablename[40];
            strncpy(tablename, p, coun(p));
            tablename[coun(p)] = '\0';
            poi_mov(p); //来到where处
            poi_mov(p); //来到where后面的一个Word

            char *t = p;
            poi_mov(t);
            if (*t != '=')
            {
                cerr << "Invalid input!\n";
                return;
            }

            poi_mov(t);
            poi_mov(t); //来到TO处
            if (strncmp(t, "TO ", 3) != 0)
            {
                cerr << "Invalid input!\n";
                return;
            }
            poi_mov(t);
            char filename[40];
            strcpy(filename, t);

            tableit = table.begin();
            while (tableit != table.end()) //查找table
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
            sel_file_wh(tablename, filename, p); //把查询结果写入文件中: 有where
        }


        else if (strncmp(p, "* FROM ", 7) == 0 && !In_str(p, " ORDER BY ") && !In_str(p, " TO ")\
            && In_str(p, " WHERE ")) //仅有FROM和where
        {
            p += 7;
            char tablename[40];
            strncpy(tablename, p, coun(p));
            tablename[coun(p)] = '\0';
            poi_mov(p); //来到where处
            poi_mov(p); //来到where后面的一个Word

            char *t = p;
            poi_mov(t);
            if (strncmp(t, "= ", 2) != 0)
            {
                cerr << "Invalid input!\n";
                return;
            }

            selectspe(tablename, p);
        }


        else if (In_str(p, " FROM ")) //有FROM
        {
            if (strncmp(p, "DISTINCT ", 9) == 0) //有DISTINCT
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


                t += 6; //来到tablename处
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


                t += 6; //来到tablename处
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


                t += 6; //来到tablename处
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


                char filename[40]; //文件名
                strcpy(filename, t);

                tableit = table.begin();
                while (tableit != table.end()) //查找table
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


            else if (In_str(p, " ORDER BY ")) //最新出现的格式
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


                t += 6; //来到tablename处
                char tablename[40];
                strncpy(tablename, t, coun(t));
                tablename[coun(t)] = '\0';

                poi_mov(t); //来到order by处
                if (strncmp(t, "ORDER BY ", 9) != 0)
                {
                    cerr << "Invalid input!\n";
                    return;
                }
                

                t += 9; //来到所要排序的项处
                char sortby[40];
                strncpy(sortby, t, coun(t));
                sortby[coun(t)] = '\0';
                poi_mov(t); //来到asc, desc处


                int sort_kind = 0; //指示排序是升还是降: 1为升，2为降
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


            else //单纯地有from
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


                t += 6; //来到tablename处
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
        num_proc(tablename, colname, 1); //求最大值
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
        num_proc(tablename, colname, 2); //求最小值
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
        num_proc(tablename, colname, 0); //求均值
    }


    else if (strncmp(s.c_str(), "DO ", 3) == 0) //事务文件
    {
        char *p = &s[3];
        char filename[40];
        strcpy(filename, p);
        todos(filename);
    }


	else cerr << "Invalid input!\n";
}


void tableprint() //打印当前的table
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
            ++(*rowit).colit; //指向下一列
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
                ++(*rowit).colit; //指向下一列
            }
            
            ++rowit; //指向下一行
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


void create_table(char *tablename, char *filename) //从文件中打开table
{//确保该文件已存在，才会读文件
    row.clear(); //先清空整个row
    strcpy(cur_table, tablename); //确定当前要操作的table名
    char temp[100];
    char rowtemp[500]; //行的缓存
    string temp0;
    table_column head;

    FILE *fp = fopen(filename, "r"); //读文件
    
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


void create_table(char *tablename, char *filename, char *start) //create table:按用户的输入来创建
{
    row.clear(); //先清空整个row
    num_of_tables++;
    strcpy(cur_table, tablename); //确定当前要操作的table名
    int wordlen = 0; //项的长度
	table_column head; //储存各项数据的名称
	char *t = &start[0];
	
	while (*t != ')')
	{
        wordlen = 0;
		while (*t != ',' && *t != ')') //这里以后需要考虑wordlen为0的情况
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
	head.colit = head.col.begin(); //指向第一个元素
	
	
	row.push_back(head); //正式放入第一行，也即数据的标头
	rowit = row.begin(); //row迭代器指向第一行
	
    tableprint(); //打印一下
	
	//写入状态文件和储存文件
	FILE *fp = fopen(filename, "a"); //储存文件
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
	
	FILE *st = fopen("state.txt", "a"); //录入到state文件中
	fprintf(st, "%s\t", tablename);
	fprintf(st, "%s\n", filename);
	fclose(st);
	
    tables tabletemp; //存入程序中的state表里
    tabletemp.filename = filename;
    tabletemp.tablename = tablename;
    table.push_back(tabletemp);
    if (table.size() > 0)
        tableit = table.begin();

	cout << endl;
}


void create_table(char *filename) //从文件中创建新的table。用户输入的table名必须和文件名一致，否则报错
{
    row.clear(); //先清空整个row
    num_of_tables++;
    char tablename[40];
    int len = strlen(filename) - 4;
    strncpy(tablename, filename, len);
    tablename[len] = '\0';

    strcpy(cur_table, tablename); //确定当前要操作的table名

    tables tabletemp = {tablename, filename};
    table.push_back(tabletemp); //新的table要存进table列表里
    FILE *st = fopen("state.txt", "a"); //录入到state文件中
    fprintf(st, "%s\t", tablename);
    fprintf(st, "%s\n", filename);
    fclose(st);

    char temp[100];
    char rowtemp[500]; //行的缓存
    string temp0;
    table_column head;


    FILE *fp = fopen(filename, "r"); //读文件

    fgets(rowtemp, 500, fp); //注意fgets

    char *t = rowtemp, *ttemp = rowtemp;
    int wordlen = 0;
    while (*t != '\n') //注意 linux下是"\r\n"
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


void drop_table(char *name) //从数据库中删除名为name的TABLE
{
    if (strcmp(name, cur_table) == 0) //操作的是当前的table
    {
        row.clear(); //清空所有行
        for (int i = 0; i < 40; i++)
            cur_table[i] = '\0';
    }

    char filetodel[44];
    while (tableit != table.end()) //清空table表中的这个table
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

    FILE *st = fopen("state.txt", "w"); //更新state文件
    if (table.size() > 0)
        while (tableit != table.end()) //清空table表中的这个table
        {
            char *tabletemp = &((*tableit).tablename)[0];
            char *filetemp = &((*tableit).filename)[0];

            fprintf(st, "%s ", tabletemp);
            fprintf(st, "%s\n", filetemp);
            tableit++;
        }
    fclose(st);

    tableit = table.begin();
    remove(filetodel); //删除对应的文件
}


void table_list() //打印当前数据库中所有TABLE的信息
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
            strcpy(cur_temp, cur_table); //暂时保存目前操作的table名

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


void insert_into(char *tablename, char *p) //第一种插入: 插入一整行
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

                head.colit = head.col.begin(); //指向第一个元素
                row.push_back(head);
                cout << endl;
                tableprint(); //这里打印一下

                //写入到相应的文件里
                char filename[40];
                strcpy(filename, tablename);
                strcat(filename, ".txt");
                FILE *fp = fopen(filename, "a"); //储存文件
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


            else //不是当前操作的table
            {//需要考虑没有当前的table的情况
                tableit = table.begin();
                char *temp = NULL;
                bool found = false; //是否存在
                while (tableit != table.end()) //查找table
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
                create_table(tablename, filenametemp); //变成了目前的table

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

                head.colit = head.col.begin(); //指向第一个元素
                row.push_back(head);
                cout << endl;
                tableprint(); //这里打印一下

                //写入到相应的文件里
                char filename[40];
                strcpy(filename, tablename);
                strcat(filename, ".txt");
                FILE *fp = fopen(filename, "a"); //储存文件
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


void insert_into(char *tablename, char *p, int num) //第二种插入：指定的行
{
    if (strcmp(tablename, cur_table) == 0) //是当前操作的table
    {
        if ((*rowit).col.size() < num) //只有小于时不行
        {
            cout << "Invalid input!\n";
            return;
        }
    }
    
    else if (strcmp(tablename, cur_table) != 0) //不是当前操作的table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //是否存在
        while (tableit != table.end()) //查找table
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
        create_table(tablename, filenametemp); //变成了目前的table

        if ((*rowit).col.size() < num)
        {
            cout << "Invalid input!\n";
            return;
        }
    }

    //之后的通用操作
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
    //int indi = 0; //指示vector中的位置
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
                tempstr = toadd2[i]; //对应的值
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
    tableprint(); //这里打印一下

    delete[] toadd;
    delete[] toadd2;

    //写入到相应的文件里
    char filename[40];
    strcpy(filename, tablename);
    strcat(filename, ".txt");
    FILE *fp = fopen(filename, "a"); //储存文件
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


void Delete(char *tablename) //第二个Delete: 保留结构地删除
{
    if (strcmp(tablename, cur_table) != 0)
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //是否存在
        while (tableit != table.end()) //查找table
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
        create_table(tablename, filenametemp); //变成了目前的table
    }
    
    rowit++;
    rowit = row.erase(rowit, row.end());
    rowit = row.begin();

    //写入到相应的文件里
    char filename[40];
    strcpy(filename, tablename);
    strcat(filename, ".txt");
    FILE *fp = fopen(filename, "w"); //储存文件
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


void Delete(char *tablename, char *colname, char *colcont, int choice) //第一个Delete: 删除一个指定的行
{
    if (strcmp(tablename, cur_table) != 0)
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //是否存在
        while (tableit != table.end()) //查找table
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
        create_table(tablename, filenametemp); //变成了目前的table
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
    if (choice == 0) //等于
        while (rowit != row.end())
        {
            (*rowit).colit = (*rowit).col.begin() + k;
            char *temp = &(*(*rowit).colit)[0];
            if (strcmp(temp, colcont) == 0)
            {
                rowit = row.erase(rowit); //删除那一行
                continue;
            }
            
            rowit++;
        }


    else if (choice == -1) //小于
        while (rowit != row.end())
        {
            (*rowit).colit = (*rowit).col.begin() + k;
            char *temp = &(*(*rowit).colit)[0];
            if (strcmp(temp, colcont) < 0)
            {
                rowit = row.erase(rowit); //删除那一行
                continue;
            }

            if (rowit != row.end())
                rowit++;
        }

    else if (choice == 1) //大于
        while (rowit != row.end())
        {
            (*rowit).colit = (*rowit).col.begin() + k;
            char *temp = &(*(*rowit).colit)[0];
            if (strcmp(temp, colcont) > 0)
            {
                rowit = row.erase(rowit); //删除那一行
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
    //写入到相应的文件里
    char filename[40];
    strcpy(filename, tablename);
    strcat(filename, ".txt");
    FILE *fp = fopen(filename, "w"); //储存文件
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


void update(char *tablename, char *p, int num, bool has_where) //更新TABLE中若干指定列的值
{
    if (strcmp(tablename, cur_table) != 0)
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //是否存在
        while (tableit != table.end()) //查找table
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
        create_table(tablename, filenametemp); //变成了目前的table
    }


    char **colname = new char *[num];
    char **colvalue = new char *[num];
    int k = 0; //指示第几个字符串
    char *t = p;

    if (!has_where)
    {
        while (*t != '\0')
        {
            int n = coun(t);
            colname[k] = new char[n + 1];
            strncpy(colname[k], t, n);
            colname[k][n] = '\0';
            poi_mov(t); //此时来到等号处
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
        vector<table_column>::iterator a = row.begin() + 1; //新的迭代器，用于修改值
        int colnum = 0; //指示当前运行到的列数
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
        } //此处修改完毕
    }
    

    else //有where
    {
        while (strncmp(t, "WHERE ", 6) != 0)
        {
            int n = coun(t);
            colname[k] = new char[n + 1];
            strncpy(colname[k], t, n);
            colname[k][n] = '\0';
            poi_mov(t); //此时来到等号处
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


        t += 6; //来到where后面的那一项
        string mod_colu(t, 0, coun(t));
        poi_mov(t);
        t += 2;
        string mod_value(t);


        //以下来判断用户输入的各个列是否存在
        vector<string>::iterator b = (*rowit).col.begin(); //新的迭代器，用于搜索值
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


        vector<table_column>::iterator a = row.begin() + 1; //新的迭代器，用于修改值
        int colnum = 0; //指示所要修改的那一列的标号
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
        } //此处修改完毕
    }


    rowit = row.begin();
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin();
        rowit++;
    }
    rowit = row.begin();

    tableprint();
    //写入到相应的文件里
    char filename[40];
    strcpy(filename, tablename);
    strcat(filename, ".txt");
    FILE *fp = fopen(filename, "w"); //储存文件
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


void select(char *tablename, char *p, int num) //select 1: 选择若干列来展示
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


    if (strcmp(tablename, cur_table) != 0) //不是当前的table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //是否存在
        while (tableit != table.end()) //查找table
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
        create_table(tablename, filenametemp); //变成了目前的table
    }


    if ((*rowit).col.size() < num)
    {
        cout << "Invalid input!\n";
        return;
    }

    vector<string>::iterator b = (*rowit).col.begin(); //新的迭代器，用于搜索值
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
    int *loc = new int[num]; //储存位置
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
            
            ++(*rowit).colit; //指向下一列
            colloc++;
        }

        (*rowit).colit = (*rowit).col.begin();
        colloc = 0;
    }
    cout << endl;

    rowit++;
    int rowno = 1; //行号
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


void select(char *tablename, char *p, int num, int Is_dis) //有distinct
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


    if (strcmp(tablename, cur_table) != 0) //不是当前的table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //是否存在
        while (tableit != table.end()) //查找table
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
        create_table(tablename, filenametemp); //变成了目前的table
    }


    if ((*rowit).col.size() < num)
    {
        cout << "Invalid input!\n";
        return;
    }


    vector<string>::iterator b = (*rowit).col.begin(); //新的迭代器，用于搜索值
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
    int *loc = new int[num]; //储存这些列在所有列中的位置
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

            ++(*rowit).colit; //指向下一列
            colloc++;
        }

        (*rowit).colit = (*rowit).col.begin();
        colloc = 0;
    }
    cout << endl; //打印完第一行


    rowit++;
    int* rownums = new int[num]; //存放每个列对应有多少行
    for (int i = 0; i < num; i++)
        rownums[i] = 0; //初始化
    vector<string> *vtemp = new vector<string>[num]; //用来纵向储存并排序、删减row
    for (int i = 0; i < num; i++)
    {
        while (rowit != row.end())
        {
            (*rowit).colit = (*rowit).col.begin() + loc[i];
            vtemp[i].push_back(*(*rowit).colit);
            rowit++;
        }
        rowit = row.begin();
        sort(vtemp[i].begin(), vtemp[i].end()); //县排序
        vtemp[i].erase(unique(vtemp[i].begin(), vtemp[i].end()), vtemp[i].end()); //再删重复元素
        rownums[i] = vtemp[i].size();
    }
    
    //输出
    int max_row_num = Max(rownums, num);
    for (int i = 0; i < max_row_num; i++) //行
    {
        printf("%-15d", i + 1);
        for (int j = 0; j < num; j++) //列
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


    if (strcmp(tablename, cur_table) != 0) //不是当前的table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //是否存在
        while (tableit != table.end()) //查找table
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
        create_table(tablename, filenametemp); //变成了目前的table
    }


    if ((*rowit).col.size() < num_of_comma + 1)
    {
        cout << "Invalid input!\n";
        return;
    }


    vector<string>::iterator b = (*rowit).col.begin(); //新的迭代器，用于搜索值
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
    int *loc = new int[num_of_comma+1]; //储存这些列在所有列中的位置
    int colloc = 0;


    for (int i = 0; i < k; i++) //添加到loc
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

            ++(*rowit).colit; //指向下一列
            colloc++;
        }

        (*rowit).colit = (*rowit).col.begin();
        colloc = 0;
    }

    
    while ((*rowit).colit != (*rowit).col.end()) //打印第一行
    {
        char *outputtemp = &(*((*rowit).colit))[0];
        printf("%-15s", outputtemp); //每一行都要输出
        ++(*rowit).colit; //指向下一列
    }
    cout << endl; //打印完第一行


    rowit++;
    vector< vector<string> > vtemp;
    vector<string> vstrtemp;


    while (rowit != row.end())
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            vstrtemp.push_back(*(*rowit).colit); //所有都要压入，后面再排序
            ++(*rowit).colit; //指向下一列
        }
            

        vtemp.push_back(vstrtemp);
        vstrtemp.clear();
        rowit++;
    }


    if (strncmp(t, " ASC", 4) == 0) //升序
    {
        for (int cl = num_of_comma; cl >= 0; cl--)
            for (int i = vtemp.size() - 1; i > 0; i--)
                for (int j = 0; j < i; j++)
                {
                    if (vtemp[j][loc[cl]].compare(vtemp[j + 1][loc[cl]]) > 0)
                        swap(vtemp[j], vtemp[j + 1]);
                }
    }

    else if (strncmp(t, " DESC", 5) == 0) //降序
    {
        for (int cl = num_of_comma; cl >= 0; cl--)
            for (int i = vtemp.size() - 1; i > 0; i--)
                for (int j = 0; j < i; j++)
                {
                    if (vtemp[j][loc[cl]].compare(vtemp[j + 1][loc[cl]]) < 0)
                        swap(vtemp[j], vtemp[j + 1]);
                }
    }


    //输出
    for (int i = 0; i < vtemp.size(); i++) //行
    {
        printf("%-15d", i + 1);
        for (int j = 0; j < vtemp[0].size(); j++) //列 //num_of_comma + 1
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


void select(char *tablename, char *p, int num, bool Is_dis) ////单纯有where
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

    char modcol[40]; //要选择的列的名称
    strncpy(modcol, t, coun(t));
    modcol[coun(t)] = '\0';
    poi_mov(t);
    t += 2;

    char modval[40]; //要选择的列的具体值
    strcpy(modval, t);

    for (int i = 0; i < num; i++)
        if (strcmp(colname[i], modcol) == 0)
        {
            cout << "Repeated columns!\n";
            return;
        }


    if (strcmp(tablename, cur_table) != 0) //不是当前的table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //是否存在
        while (tableit != table.end()) //查找table
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
        create_table(tablename, filenametemp); //变成了目前的table
    }


    if ((*rowit).col.size() < num + 1)
    {
        cout << "Invalid input!\n";
        return;
    }

    vector<string>::iterator b = (*rowit).col.begin(); //新的迭代器，用于搜索值
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
    int *loc = new int[num + 1]; //储存指定列的位置
    int colloc = 0;

    while ((*rowit).colit != (*rowit).col.end())
    {
        char *outputtemp = &(*((*rowit).colit))[0];

        if (strcmp(outputtemp, modcol) == 0)
        {
            loc[num] = colloc;
            break;
        }

        ++(*rowit).colit; //指向下一列
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

            ++(*rowit).colit; //指向下一列
            colloc++;
        }

        (*rowit).colit = (*rowit).col.begin();
        colloc = 0;
    }

    printf("%-15s", modcol);
    cout << endl; //打印第一行


    rowit++;
    int rowno = 1; //行号
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
        (*rowit).colit = (*rowit).col.begin() + loc[num]; //所指定的那列的位置
        char *outputtemp = &(*((*rowit).colit))[0];
        if (strcmp(outputtemp, modval) == 0) //是要输出的行
        {
            printf("%-15d", rowno);
            for (int i = 0; i < num; i++)
            {
                (*rowit).colit = (*rowit).col.begin() + loc[i]; //输出第i个位置的列
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


void select(char *tablename, char *filename, char *p, int num) //把查询结果写入文件中: 没有where
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


    if (strcmp(tablename, cur_table) != 0) //不是当前的table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //是否存在
        while (tableit != table.end()) //查找table
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
        create_table(tablename, filenametemp); //变成了目前的table
    }


    if ((*rowit).col.size() < num)
    {
        cout << "Invalid input!\n";
        return;
    }

    vector<string>::iterator b = (*rowit).col.begin(); //新的迭代器，用于搜索值
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


    FILE *towrite = fopen(filename, "w"); //要储存进的文件的名字

    printf("%-15s", "ID");
    int *loc = new int[num]; //储存位置
    int colloc = 0;
    for (int i = 0; i < k; i++)
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            char *outputtemp = &(*((*rowit).colit))[0];
            if (strcmp(colname[i], outputtemp) == 0)
            {
                printf("%-15s", outputtemp); //输出到控制台

                fprintf(towrite, "%s", outputtemp); //输出到文件
                //if ((*rowit).colit + 1 != (*rowit).col.end())
                if (i != k - 1)
                    fputc(' ', towrite);
                else
                    fputc('\n', towrite);

                loc[i] = colloc;
                break;
            }
            
            ++(*rowit).colit; //指向下一列
            colloc++;
        }

        (*rowit).colit = (*rowit).col.begin();
        colloc = 0;
    }
    cout << endl; //第一行输出完


    rowit++;
    int rowno = 1; //行号
    while (rowit != row.end())
    {
        printf("%-15d", rowno);
        for (int i = 0; i < num; i++)
        {
            //printf("%-15d", i + 1);
            (*rowit).colit = (*rowit).col.begin() + loc[i];
            char *outputtemp = &(*((*rowit).colit))[0];
            printf("%-15s", outputtemp);

            fprintf(towrite, "%s", outputtemp); //输出到文件
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


void sel_file_wh(char *tablename, char *filename, char *p) //把查询结果写入文件中: 有where
{
    char *t = p;
    char modcol[40]; //要选择的列的名称
    strncpy(modcol, t, coun(t));
    modcol[coun(t)] = '\0';
    poi_mov(t); //来到等号处
    t += 2;

    char modval[40]; //要选择的列的具体值
    strncpy(modval, t, coun(t));
    modval[coun(t)] = '\0';


    if (strcmp(tablename, cur_table) != 0) //不是当前的table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //是否存在
        while (tableit != table.end()) //查找table
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
        create_table(tablename, filenametemp); //变成了目前的table
    }


    vector<string>::iterator b = (*rowit).col.begin(); //新的迭代器，用于搜索值
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


    FILE *towrite = fopen(filename, "w"); //要储存进的文件的名字

    printf("%-15s", "ID");
    int loc = 0; //储存指定列的位置
    int colloc = 0;

    while ((*rowit).colit != (*rowit).col.end())
    {
        char *outputtemp = &(*((*rowit).colit))[0];
        printf("%-15s", outputtemp); //所有列都要展示和输出到文件
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

        ++(*rowit).colit; //指向下一列
        colloc++;
    }

    (*rowit).colit = (*rowit).col.begin();
    cout << endl; //打印完第一行


    rowit++;
    int rowno = 1; //行号
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin() + loc; //所指定的那列的位置
        char *outputtemp = &(*((*rowit).colit))[0];
        if (strcmp(outputtemp, modval) == 0) //是要输出的行
        {
            printf("%-15d", rowno);
            (*rowit).colit = (*rowit).col.begin();

            while ((*rowit).colit != (*rowit).col.end())
            {
                char *outputtemp = &(*((*rowit).colit))[0];
                printf("%-15s", outputtemp); //所有列都要展示
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


void selectspe(char *tablename, char *p) //按某一项查询
{
    char *t = p;
    char modcol[40]; //要选择的列的名称
    strncpy(modcol, t, coun(t));
    modcol[coun(t)] = '\0';
    poi_mov(t); //来到等号处
    t += 2;

    char modval[40]; //要选择的列的具体值
    strcpy(modval, t);
    //modval[coun(t)] = '\0';


    if (strcmp(tablename, cur_table) != 0) //不是当前的table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //是否存在
        while (tableit != table.end()) //查找table
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
        create_table(tablename, filenametemp); //变成了目前的table
    }

    vector<string>::iterator b = (*rowit).col.begin(); //新的迭代器，用于搜索值
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
    //打印第一行
    while ((*rowit).colit != (*rowit).col.end())
    {
        char *outputtemp = &(*((*rowit).colit))[0];
        printf("%-15s", outputtemp); //所有列都要展示

        if (strcmp(outputtemp, modcol) == 0)
            loc = colloc;

        ++(*rowit).colit; //指向下一列
        colloc++;
    }

    (*rowit).colit = (*rowit).col.begin();
    cout << endl; //打印完第一行


    rowit++;
    int rowno = 1; //行号
    while (rowit != row.end())
    {
        (*rowit).colit = (*rowit).col.begin() + loc; //所指定的那列的位置
        char *outputtemp = &(*((*rowit).colit))[0];
        if (strcmp(outputtemp, modval) == 0) //是要输出的行
        {
            printf("%-15d", rowno);
            (*rowit).colit = (*rowit).col.begin();
            while ((*rowit).colit != (*rowit).col.end())
            {
                char *outputtemp = &(*((*rowit).colit))[0];
                printf("%-15s", outputtemp); //所有列都要展示

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


void sel_new(int num, char *tablename, char *p, char *sortby, int sort_kind) //最新出现的格式
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


    if (strcmp(tablename, cur_table) != 0) //不是当前的table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //是否存在
        while (tableit != table.end()) //查找table
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
        create_table(tablename, filenametemp); //变成了目前的table
    }


    if ((*rowit).col.size() < num)
    {
        cout << "Invalid input!\n";
        return;
    }

    vector<string>::iterator b = (*rowit).col.begin(); //新的迭代器，用于搜索值
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
    int *loc = new int[num]; //储存这些列在所有列中的位置
    int sortloc = 0;
    int colloc = 0;


    for (int i = 0; i < k; i++) //添加到loc
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

            ++(*rowit).colit; //指向下一列
            colloc++;
        }

        (*rowit).colit = (*rowit).col.begin();
        colloc = 0;
    }
    cout << endl; //打印完第一行


    rowit++;
    vector< vector<string> > vtemp;
    vector<string> vstrtemp;

    while (rowit != row.end())
    {
        while ((*rowit).colit != (*rowit).col.end())
        {
            //if ()
            vstrtemp.push_back(*(*rowit).colit); //所有都要压入，后面再排序
            ++(*rowit).colit; //指向下一列
        }


        vtemp.push_back(vstrtemp);
        vstrtemp.clear();
        rowit++;
    }


    if (sort_kind == 1)//升序
    {
        for (int i = vtemp.size() - 1; i > 0; i--)
            for (int j = 0; j < i; j++)
            {
                if (vtemp[j][sortloc].compare(vtemp[j + 1][sortloc]) > 0)
                    swap(vtemp[j], vtemp[j + 1]);
            }
    }

    else if (sort_kind == 2) //降序
    {
        for (int i = vtemp.size() - 1; i > 0; i--)
            for (int j = 0; j < i; j++)
            {
                if (vtemp[j][sortloc].compare(vtemp[j + 1][sortloc]) < 0)
                    swap(vtemp[j], vtemp[j + 1]);
            }
    }


    //输出
    for (int i = 0; i < vtemp.size(); i++) //行
    {
        printf("%-15d", i + 1);
        for (int j = 0; j < num; j++) //列
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


void num_proc(char *tablename, char *colname, int choice) //数值处理:最大值，最小值，平均值 0:均值; 1：最大; 2：最小
{
    if (strcmp(tablename, cur_table) != 0) //不是当前的table
    {
        tableit = table.begin();
        char *temp = NULL;
        bool found = false; //是否存在
        while (tableit != table.end()) //查找table
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
        create_table(tablename, filenametemp); //变成了目前的table
    }

    vector<string>::iterator b = (*rowit).col.begin(); //新的迭代器，用于搜索值
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


    if (choice == 1) //最大值
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

    
    else if (choice == 2) //求最小值
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


    else if (choice == 0) //求均值
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


void todos(char *filename) //事务文件的读取与执行
{
    FILE *todolist = fopen(filename, "r"); //打开事务文件
    if (todolist == NULL)
    {
        cerr << "Failed to open file!\n";
        return;
    }

    char statement_temp[150]; //储存每一行语句
    int serial = 1; //执行的任务的序号

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