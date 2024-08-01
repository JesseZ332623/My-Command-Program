#include <iostream>
#include <fstream>
#include <string>

#include "../include/cryption.h" // 假设这是你的头文件路径
#include "../include/defs.h"

//#include <MyLib/myLogerDef.h>

int main()
{
    using namespace MyLib::MyLoger;

    // 明文
    std::string plainText = "123456";
    std::string userName  = "Jesse_EC";

    // 密文存储文件路径
    std::string fileName = "../data/encrypted_data.txt";

    Cryption cryption;

    std::ofstream outputStream(fileName, std::ios_base::trunc);
    std::ifstream inputStream(fileName, std::ios_base::in);

    // 加密明文
    std::string cipherText = cryption.encryption(plainText);
    if (cipherText.empty())
    {
        std::cerr << "Encryption failed." << std::endl;
        return 1;
    }

    outputStream << cipherText;
    outputStream.close();

    NOTIFY_LOG("Chipher Text = " + cipherText + '\n');

    std::string HexCode;
    inputStream >> HexCode;

    NOTIFY_LOG(HexCode + '\n');

    std::string PassWord = cryption.decryption(HexCode);

    NOTIFY_LOG(PassWord + '\n');

    inputStream.close();

    return EXIT_SUCCESS;
}