#include "../include/defs.h"

#include <signal.h>

void hiddenPassword(std::string & password)
{
    using namespace std;
    using namespace MyLib::MyLoger;

    password.clear();

    char passwordChar;
    bool backspacePressed = false; // 标记是否按下了退格键

    while (true)
    {
        passwordChar = _getch();

        if (passwordChar == '\r') // 按下回车，退出循环
            break;

        else if (passwordChar == '\b') // 按下退格
        {
            if (!password.empty())
            {
                password.pop_back();
                //cout << "\b \b"; // 删除星号显示
                CORRECT_LOG("\b \b"); // 删除最后一个星号
                backspacePressed = true;
            }
        }
        else if (isprint(passwordChar)) // 只有可打印字符才添加到密码
        {
            password.push_back(passwordChar);
            CORRECT_LOG('*');
            backspacePressed = false;
        }

        // 忽略其他控制字符
        if (backspacePressed && passwordChar == '\r')
            break; // 如果按了退格后直接按回车，则退出
    }

    putchar('\n');
}
