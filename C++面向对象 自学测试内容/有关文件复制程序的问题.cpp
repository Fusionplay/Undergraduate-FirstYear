using namespace std;
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
void Filecopy(const char *file1, const char *file2);

int main()
{
    Filecopy("E:\\��ֽ\\kickass-desktop-wallpapers-1.png", "d:\\co2.png");

    return 0;
}

void Filecopy(const char *file1, const char *file2)
{
    /* FILE *fp1 = fopen(file1, "rb"), *fp2 = fopen(file2, "wb");
    char medium;
    
    while (feof(fp1))
    {
        fread(&medium, 1, 1, fp1);
        fwrite(&medium, 1, 1, fp2);
    }

    fclose(fp1);
    fclose(fp2); */
    
    /* FILE *fpRead;  // ָ��Ҫ���Ƶ��ļ�
    FILE *fpWrite;  // ָ���ƺ���ļ�
    int bufferLen = 1024 * 4;  // ����������
    char *buffer = (char*)malloc(bufferLen);  // ���ٻ���
    int readCount;  // ʵ�ʶ�ȡ���ֽ���
    if ((fpRead = fopen(file1, "rb")) == NULL || (fpWrite = fopen(file2, "wb")) == NULL) {
        printf("Cannot open file, press any key to exit!\n");
        
        exit(1);
    }
    // ���ϴ�fileRead��ȡ���ݣ����ڻ��������ٽ�������������д��fileWrite
    while ((readCount = fread(buffer, 1, bufferLen, fpRead)) > 0) {
        fwrite(buffer, readCount, 1, fpWrite);
    }
    free(buffer);
    fclose(fpRead);
    fclose(fpWrite); */

    ifstream in_file(file1, ios::in | ios::binary);
    ofstream out_file(file2, ios::out | ios::binary);
    char buffer;
    
    while (!in_file.eof())
    {
        in_file >> buffer;
        out_file << buffer;
    }
}