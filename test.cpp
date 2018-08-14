#define _CRT_SECURE_NO_WARNINGS 1

//�����쳣��� 
//�﷨���� ��������󣩱������δ���塢���Ų�ƥ�䡢
//�ؼ���ƴд����ȵȱ������ڱ���ʱ�ܷ��ֵĴ������������Լ�ʱ�����������֣�
//���ҿ��Լ�ʱ֪�������λ�ü�ԭ�򣬷������

//����ʱ����  ���������±�Խ�硢ϵͳ�ڴ治��ȵȡ�
//��������ױ�����Ա���֣�����ͨ���������ܽ������У�������ʱ��������³������

//��C++��,�쳣���׳��ʹ�����Ҫʹ�������������ؼ���:try�� throw �� catch
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
//		cout << "��������Ϊ0" <<endl;
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
		throw SqlException(1,"Sql������");
	}
	if (rand() % 7 == 0){
		throw SqlException(2, "��������");
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