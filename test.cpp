#define _CRT_SECURE_NO_WARNINGS 1

//常见异常情况 
//语法错误 （编译错误）比如变量未定义、括号不匹配、
//关键字拼写错误等等编译器在编译时能发现的错误，这类错误可以及时被编译器发现，
//而且可以及时知道出错的位置及原因，方便改正

//运行时错误  比如数组下标越界、系统内存不足等等。
//这类错误不易被程序员发现，它能通过编译且能进入运行，但运行时会出错，导致程序崩溃

//在C++中,异常的抛出和处理主要使用了以下三个关键字:try、 throw 、 catch
#include<iostream>
#include<string>
using namespace std;


//int test2(int a,int b){
//	if (b == 0){
//		throw errno;
//	}
//
//	return a / b;
//}
//
//int main(){
//
//	try{
//		test2(10,2);
//		test2(10,10);
//		test2(10,0);
//	}
//	catch (int b){
//		cout << "除数不能为0" <<endl;
//	}
//
//	system("pause");
//	return 0;
//}


class Exception
{
public:
	Exception(int errid, const string& errmsg)
		:_errid(errid)
		, _errmsg(errmsg)
	{}
	virtual const char* What() = 0;

	virtual ~Exception()
	{}
protected:
	int _errid;
	string _errmsg;
};

class SqlException:public Exception
{
public:
	SqlException(int errid, const string& errmsg)
		:Exception(errid, errmsg)
	{}

	virtual const char* What(){
		cout << "Sql Error:";
		return _errmsg.c_str();
	}
};

void Sql()
{
	if (rand() % 3 == 0)
	{
		throw SqlException(1,"Sql语句错误");
	}
	if (rand() % 7 == 0){
		throw SqlException(2, "主键冗余");
	}
}

int main(){

	while (true){
		try{
			Sql();
		}
		catch (Exception& e){
			cout << e.What() << endl;
		}
	}
	system("pause");
	return 0;
}