#ifndef _CRYPTION_H__
#define _CRYPTION_H__

/**
 * 使用 Crypto++ 加密库，
 * 提供对密码字符串进行 AES 加密解密操作。
*/

#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/aes.h>

#include <MyLib/myLogerDef.h>

using namespace CryptoPP;

class Cryption
{
    private:
        byte key[AES::DEFAULT_KEYLENGTH];
        byte randomVector[AES::BLOCKSIZE];
        AutoSeededRandomPool randSeedPool;
    
    public:
        Cryption();
        
        /**
         * @brief 使用 AES 算法加密字符串
         * 
         * @param __plainText   明文
         * @return std::string  加密后的密文
         */
        std::string encryption(std::string & __plainText);

        /**
         * @brief 使用 AES 算法解密字符串密文
         * 
         * @param __cipherText 密文
         * @return std::string 解密后的明文 
         */
        std::string decryption(std::string & __cipherText);
};

/**
 * @brief 将密文写入文件
 * 
 * @param cipherText        加密后的密文
 * @param fileName          要写入的文件
 */
void writeEncryptedDataToFile(const std::string &cipherText, const std::string &fileName);

/**
 * @brief 从指定数据文件中读取密文，返回密文字符串
 */
std::string readEncryptedDataFromFile(const std::string &fileName);

#endif // _ENCRYPTION_H__