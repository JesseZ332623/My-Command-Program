#ifndef  COPY_OPERATOR
#define COPY_OPERATOR

#include "./copyDefs.h"

/**
 * 从命令行中（即 argv[1] ~ argv[2] 中）获取文件路径，
 * 将路径 argv[1] 所指向的文件的内容拷贝到 argv[2] 的文件夹中去
 * 
 * 示例：
 * 
 * mycp ./a.txt ./data/ # 将 文件 a.txt 的内容拷贝到 ./data/ 目录下
 * mycp --help 输出帮助
 * mycp --version 输出版本号
*/

using namespace MyLib::MyLoger;

class FileCopyOperator
{
    public:
        using byte = unsigned char;

    private:
        /**
         * @brief 字节缓冲区，以 1024 * 1024 字节作为一段数据，避免单字节拷贝
        */
        byte byteBuffer[BUFFER_SIZE];

        std::ifstream readFStream;      // readFStream 将源文件数据写入缓冲区
        std::ofstream writeFstream;     // writeFstream 将缓冲区内数据写入目标路径文件

        std::size_t sourceFileByteCount{0ULL};

        /**
         * @brief 获取文件的格式（. 字符后面的所有字符）
         * 
         * @param __srcFile 文件名（但一般是路径）
         * 
         * @return 文件后缀名（如 .cpp .exe .txt 等）
        */
        std::string getFileType(const std::string & __srcFile);

        /**
         * @brief __srcFile 去除掉一个文件路径的所有路径名（如把 ./data/test.txt 变成 test.txt）【明天完成】
         * 
         * @param __srcFile 文件名（但一般是路径）
         * 
         * @return 不带路径的文件名
        */
        std::string getFileName(const std::string & __srcFile);

        /**
         * @brief 对文件路径的斜杠进行正反切换，以适应不同的操作系统。
         * 
         * @param __path    文件路径
         * 
         * @return non-return
        */
        void forwardBackwardSwitch(std::string & __path);
    
    public:
        /**
         * @brief 输出版本号。
        */
        void showVersion(void);
        /**
         * @brief 输出帮助信息
        */
        void showHelp(void);

        void copyFile(std::string __srcFile, std::string __tarFile);

        ~FileCopyOperator();
};

#endif // COPY_OPERATOR