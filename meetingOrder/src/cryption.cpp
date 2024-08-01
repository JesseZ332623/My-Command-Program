#include "../include/cryption.h"

#include <fstream>

Cryption::Cryption()
{
    using namespace MyLib::MyLoger;

    this->randSeedPool.GenerateBlock(this->key, sizeof(this->key));
    this->randSeedPool.GenerateBlock(this->randomVector, sizeof(randomVector));
}

std::string Cryption::encryption(std::string & __plainText)
{
    using namespace MyLib::MyLoger;

    std::string tempCipherText;

    try
    {
        CBC_Mode<AES>::Encryption encryptor(
            this->key, sizeof(this->key), this->randomVector
        );

        StringSource encryptionSource(
            __plainText, true,
            new StreamTransformationFilter(
                encryptor, 
                new HexEncoder(new StringSink(tempCipherText))
            )
        );
    }
    catch (const CryptoPP::Exception & __except)
    {
        ERROR_LOG(__except.what() + '\n');
        tempCipherText.clear();
    }

    return tempCipherText;
}

std::string Cryption::decryption(std::string & __cipherText)
{
    using namespace MyLib::MyLoger;

    std::string tempPlainText;

    try
    {
        CBC_Mode<AES>::Decryption decryptor(
            this->key, sizeof(this->key), this->randomVector
        );
        
        StringSource decryptionSource(
            __cipherText, true,
            new HexDecoder(
                new StreamTransformationFilter(decryptor,
                    new StringSink(tempPlainText)
                )
            )
        );
    }
    catch (const CryptoPP::Exception & __except)
    {
        ERROR_LOG(__except.what() + '\n');
        tempPlainText.clear();
    }

    return tempPlainText;
}

void writeEncryptedDataToFile(const std::string &cipherText, const std::string &fileName)
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
    file.write(cipherText.c_str(), cipherText.size());
    file.close();
}

std::string readEncryptedDataFromFile(const std::string &fileName)
{
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file for reading." << std::endl;
        return "";
    }
    
    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);

    // 创建一个临时字符串，暂时用空字符填充
    std::string cipherText(length, '\0');

    file.read(&cipherText[0], length);
    file.close();

    return cipherText;
}