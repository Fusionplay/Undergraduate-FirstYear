using namespace std;
#include <iostream>

class A
{
    int x, y;
public:
    A() { cout << "in A's default constructor\n"; f(); }
    A(const A&) { cout << "in A's copy constructor\n"; f(); }
    ~A() { cout << "in A's destructor\n"; }

    virtual void f() { cout << "in A's f\n"; }
    void g() { cout << "in A's g\n"; }
    void h() { f(); g(); }
};

class B : public A
{
    int z;
public:
    B() { cout << "in B's default constructor\n"; }
    B(const B&) { cout << "in B's copy constructor\n"; }
    ~B() { cout << "in B's destructor\n"; }

    void f() { cout << "in B's f\n"; }
    void g() { cout << "in B's g\n"; }
};

void func1(A x)
{
    x.f();
    x.g();
    x.h();
}

void func2(A &x)
{
    x.f();
    x.g();
    x.h();
}

int main()
{
    cout << "----Section1----\n";
    A a;
    A *p = new B;

    cout << "----Section2----\n";
    func1(a);
    cout << "----Section3----\n";
    func1(*p);
    cout << "----Section4----\n";
    func2(a);
    cout << "----Section5----\n";
    func2(*p);
    cout << "----Section6----\n";
    delete p;
    cout << "----Section7----\n";


    return 0;
}
