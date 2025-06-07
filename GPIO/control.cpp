#include <iostream.h>
using namespace std;
int a;
void test(int *temp)
{
    temp = 20;
}

int main()
{
    a =10;
    test(a);
    cout<<a;
}