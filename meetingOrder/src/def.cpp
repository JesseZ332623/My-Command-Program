#include "../include/defs.h"

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
                CORRECT_LOG("\b \b"); // 删除星号显示
                backspacePressed = true;
            }
        }
        else if (isprint(passwordChar)) // 只有可打印字符才添加到密码
        {
            password.push_back(passwordChar);
            CORRECT_LOG('*');
            backspacePressed = false;
        }
        
        /**
         * 忽略其他控制字符，
         * 如果按了退格后直接按回车，则退出。
         */
        if (backspacePressed && passwordChar == '\r') { break; }
    }

    putchar('\n');
}

bool checkQuit(const std::string & __argument)
{
    if (__argument.at(0) == 'q' && __argument.size() == 1)
    {
        return true;
    }
    else if (__argument.at(0) == 'Q' && __argument.size() == 1)
    {
        return true;
    }

    return false;
}

void showSoftwareInfo(void)
{
    using namespace MyLib::MyLoger;

    NOTIFY_LOG(
        "[Meeting room management system version: " + SOFTWARE_VERSION + "]\n"
    );
}

void clearFiledata(std::ofstream & __ofstream, const std::string & __path)
{
    /**
     * 以清空模式打开指定文件，会阶段文件到空。
     */
    __ofstream.open(__path, std::ios_base::trunc);

    if (!__ofstream.is_open() || !__ofstream)
    {
        throw std::runtime_error("Stream state error or file open failed!\n");
    }

    __ofstream.close();
}

std::streamsize getFileBytes(std::ifstream & __inputStream)
{
    if (!__inputStream.is_open() || !__inputStream) {
        throw std::runtime_error("getFileBytes(): File stream is not open or in error state.\n");
    }

    std::streampos originalPos = __inputStream.tellg();

    __inputStream.seekg(0, std::ios_base::end);
    std::streamsize fileBytes = __inputStream.tellg();

    if (fileBytes == -1) {
        std::runtime_error("Failed to determine file size.\n");
    }

    __inputStream.seekg(originalPos);

    return fileBytes;
}