using namespace std;
#include <iostream>
#include <cstring>
#include <cstdlib>

class String
{
    char *str;

public:
    String()
    {
        str = NULL;
    }

    String(const char *p)
    {
        str = new char[strlen(p) + 1];
        strcpy_s(str, strlen(p) + 1, p);
    }

    ~String()
    {
        delete[] str;
        str = NULL;
    }

    int length()
    {
        return strlen(str);
    }

    char &char_at(int i)
    {
        if (i < 0 || i >= strlen(str))
        {
            cerr << "³¬³ö×Ö·û´®·¶Î§£¡\n";
            exit(-1);
        }
        return str[i];
    }

    const char *get_str()
    {
        return str;
    }

    String &copy(const char *p)
    {
        delete[] str;
        str = new char[strlen(p) + 1];
        strcpy_s(str, strlen(p) + 1, p);
        return *this;
    }

    String &copy(const String &s) { return copy(s.str); }

    String &append(const char *p)
    {
        char *p1 = new char[strlen(str) + strlen(p) + 1];
        strcpy_s(p1, strlen(str) + 1, str);
        strcat_s(p1, strlen(str) + strlen(p) + 1, p);
        delete[] str;
        str = p1;
        return *this;
    }

    String &append(const String &s) { return append(s.str); }

    int compare(const char *p) { return strcmp(str, p); }
    int compare(const String &s) { return strcmp(str, s.str); }
};

int main()
{
    String s1, s2("abcdefg");
    s1.copy("xyz");
    s2.append(s1);
    for (int i = 0; i < s2.length(); i++)
    {
        if (s2.char_at(i) >= 'a' && s2.char_at(i) <= 'z')
            s2.char_at(i) = s2.char_at(i) - 'a' + 'A';
    }

    if (s2.compare("ABCDEFGXYZ") == 0)
        cout << "OK\n";
    cout << s1.get_str() << endl << s2.get_str();

    return 0;
}