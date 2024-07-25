#include <iostream>
#include <string> //注意这里的头文件！
#include <conio.h>
using namespace std;

int main(void)
{
    cout << "请输入密码： " << endl;
    string password;
    //int i = 0;
    char ch;
    while ((ch = _getch()) != '\r')
    {
        password += ch; // string对象重载了+=
        cout << "*";
    }
    cout << endl
         << "输入完毕！您输入的是： " << password << endl;
}