//输入一个正整数n,求n!(即阶乘)末尾有多少个0？ 比如: n = 10; n! = 3628800,所以答案为2 
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
int main()
{
    long long  n;
    while(cin>>n)
    {
        long long i,x=0;
        for(i=5;i<=n;i*=5)
        {
            x=x+n/i;
        }
        cout<<x<<endl;
    }
    return 0;
}
