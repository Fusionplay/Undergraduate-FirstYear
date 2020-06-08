#include <iostream>
using namespace std;

class M
{
public:
	M()
	{
	cout << "member cons\n";
	}

	~M()
	{
	cout << "member destru\n";
	}

	M(const M& m) {cout << "member copy\n";}
};


class Base
{
public:
	Base()
	{
	cout << "base cons\n";
	}

	~Base()
	{
	cout << "base destruction\n";
	}

	Base(const Base& b) {cout << "base copy\n";}
};


class Deri: Base
{
public:
	Deri(M mm1, M mm2): m1(mm1), m2(mm2)
	{
	cout << "derived cons\n";
	}

	~Deri()
	{
	cout << "derived destr\n";
	}

	Deri(const Deri& d) {cout << "derived copy\n";}

private:
	M m1, m2;
};




int main()
{
	M m1;
	cout << "\n";
	M m2;
	cout << "\n";
	Deri d1(m1, m2);
	cout << "\n";
	Deri d2(d1);
	cout << "\n";

	/*
	程序中各个函数的调用顺序：
	m1.cons //main函数中先创建m1, m2两个对象，故调用各自的构造函数
	m2.cons 

	//执行Derived d1(m1, m2)这条语句
	mm1.copy //首先通过拷贝构造函数来初始化mm1,mm2这两个实参
	mm2.copy
	Base.cons //1.先执行基类的构造函数
	d1.m1.copy //2.再初始化成员对象。由于使用另一个已初始化的对象来初始化本对象，故调用拷贝构造函数
	d1.m2.copy
	d1.cons //3.最后初始化本对象
	mm2.destr //d1的构造函数结束，两个对象实参析构
	mm1.destr //

	//执行Derived d2(d1)这条语句
	Base.cons //1.默认调用基类的构造函数而不是拷贝构造函数
	m1.cons //2. 默认调用成员对象的构造函数而不是拷贝构造函数
	m2.cons //
	d2.copy //最后调用本对象的默认拷贝构造函数

	//main函数中的各个对象开始消亡
	//d2的消亡
	d2.destr
	d2.m2.destr
	d2.m1.destr
	Base.destr

	//d1的消亡
	d1.destr
	d1.m2.destr
	d1.m1.destr
	Base.destr

	//m2和m1的消亡
	m2.destr
	m1.destr
	
	
	*/

	return 0;
}