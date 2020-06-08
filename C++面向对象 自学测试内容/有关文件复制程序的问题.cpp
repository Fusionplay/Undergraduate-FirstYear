using namespace std;
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
void Filecopy(const char *file1, const char *file2);

int main()
{
    Filecopy("E:\\壁纸\\kickass-desktop-wallpapers-1.png", "d:\\co2.png");

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
    
    /* FILE *fpRead;  // 指向要复制的文件
    FILE *fpWrite;  // 指向复制后的文件
    int bufferLen = 1024 * 4;  // 缓冲区长度
    char *buffer = (char*)malloc(bufferLen);  // 开辟缓存
    int readCount;  // 实际读取的字节数
    if ((fpRead = fopen(file1, "rb")) == NULL || (fpWrite = fopen(file2, "wb")) == NULL) {
        printf("Cannot open file, press any key to exit!\n");
        
        exit(1);
    }
    // 不断从fileRead读取内容，放在缓冲区，再将缓冲区的内容写入fileWrite
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