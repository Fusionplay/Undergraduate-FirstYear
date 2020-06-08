using namespace std;
#include <iostream>

class A
{
    int m;
public:
    A()
    {
        cout << "in A's default constructor\n";
    }

    A(const A&) { cout << "in A's copy constructor\n"; }
    ~A() { cout << "in A's destructor\n"; }

    
};

class B
{
public:
    B()
    {
        cout << "in B's default constructor\n";
    }

    ~B()
    {
        cout << "in B's destructor\n";
    }

    B(const B&) { cout << "in B's copy constructor\n"; }

private:
    int x, y;
};

class C : public B
{
    int z; A a;

public:
    C()
    {
        cout << "in C's default constructor\n";
    }

    C(const C&) { cout << "in C's copy constructor\n"; }
    ~C() { cout << "in C's destructor\n"; }
};

void func1(C x)
{
    cout << "In func1\n";
}

void func2(C &x)
{
    cout << "In func2\n";
}


int main()
{
    cout << "Section 1\n";
    C c;
    cout << "Section 2\n";
    func1(c);
    cout << "Section 3\n";
    func2(c);
    cout << "Section 4\n";



    return 0;
} 
