#include "../include/copyOperator.h"

void FileCopyOperator::showVersion(void)
{
    printSplitLine(45, '-');
    WARNING_LOG("Software version: " + SOFTWARE_VERSION + '\n'); 
    printSplitLine(45, '-');

}

void FileCopyOperator::showHelp(void)
{
    printSplitLine(65, '-');

    loger(
            std::clog, NOTIFY,
            "Usage:\n",
            "mycp --help     Get help info.\n",
            "mycp --version  Get version.\n",
            "mycp [sorce file] [target path] Copy sorce file to target path.\n"
        );

    printSplitLine(65, '-');
}

void FileCopyOperator::forwardBackwardSwitch(std::string & __path)
{
    /*
        若在 __srcFile 中没有找到 / 或 \ 字符，
        则代表它可能是一个纯文件，或者非法字符串，直接原路返回就好。
    */
    if ((__path.find('/') == __path.npos) && (__path.find('\\') == __path.npos)) { return; }

    std::replace(__path.begin(), __path.end(), '/', OS_SPECIFIC_SLASH);
    std::replace(__path.begin(), __path.end(), '\\', OS_SPECIFIC_SLASH);
}

std::string FileCopyOperator::getFileType(const std::string & __srcFile)
{
    /*获取 . 字符第一次出现在字符串 __srcFile 中的位置。*/
    std::size_t dotPos = __srcFile.find('.');

    /*在确保字符串 __srcFile 有 . 字符的情况下 */
    if (dotPos != __srcFile.npos)
    {
        /*分割 . 字符后面的所有字符，并返回*/
        return __srcFile.substr(dotPos + 1);
    }
    else { return ""; } // 在 __srcFile 找不到 . 字符就返回空字符
}

std::string FileCopyOperator::getFileName(const std::string & __srcFile)
{
    // 查找最后一个 '/' 或 '\' （由于已使用 forwardBackwardSwitch 统一过，此处可只查找 '/'
    std::size_t pathPos = __srcFile.rfind('\\');

    // 若找不到 '/'，则返回空字符串
    if (pathPos == std::string::npos) { return ""; }

    // 分割最后一个 '/' 后面的所有字符，并返回
    return __srcFile.substr(pathPos + 1);
}

void FileCopyOperator::copyFile(std::string __srcFile, std::string __tarPath)
{
    /*将传入的路径字符串的斜杠朝向全部改为操作系统规范的斜杠*/
    this->forwardBackwardSwitch(__srcFile);
    this->forwardBackwardSwitch(__tarPath);

    /*由于可能传入的非法字符，所以需要有异常机制*/
    try
    {
        /*打开要被复制的文件*/
        this->readFStream.open(__srcFile, std::ios_base::binary);

        if (!this->readFStream.is_open()) 
        { 
            throw std::runtime_error(__srcFile + " is not exist!\n");
        }

        /*
            将要被复制的文件和要被复制到的路径组成新的文件路径，
            如 ./data/The_Art_of_Patience.txt 或 .\data\The_Art_of_Patience.txt
        */
       std::string targetFile;
        if (*(__tarPath.end() - 1) == OS_SPECIFIC_SLASH)
        {
            targetFile = (__tarPath + this->getFileName(__srcFile));
        }
        else 
        {
            targetFile = (__tarPath + OS_SPECIFIC_SLASH + this->getFileName(__srcFile));
        }

        loger(
                std::cout, NOTIFY,
                "Copy [", __srcFile, "] to [", targetFile, "]\n"
        );

        /*打开 或 创建（在目标文件不存在的情况下）targetFile*/
        this->writeFstream.open(targetFile, std::ios_base::out | std::ios_base::binary);

        if (!this->writeFstream.is_open())
        {
            throw std::runtime_error("Failed to open the target file: " + targetFile + '\n');
        }

        /*当没有读到源文件末尾时*/
        while (!this->readFStream.eof())
        {
            /*从源文件挖 bufferSize 字节的数据到 byteBuffer*/
            readFStream.read(reinterpret_cast<char *>(&this->byteBuffer), BUFFER_SIZE);

            // 确定一共读了多少字节的数据，避免文件本身比缓冲区还小的情况。
            std::streamsize bytesRead = readFStream.gcount();

            /*把缓冲区内的数据全部写入目标文件*/
            writeFstream.write(reinterpret_cast<char *>(&this->byteBuffer), bytesRead);

            /*写入字节数累加*/
            this->sourceFileByteCount += bytesRead;

            if (readFStream.eof()) { break; }
        }

        loger(
                std::cout, NOTIFY,
                "Size of [", this->getFileName(__srcFile), "] = ", 
                (double)this->sourceFileByteCount / 1024, " KBytes.\n"
        );
    }
    catch (const std::exception & __except)
    {
        ERROR_LOG(__except.what());
    }
}

FileCopyOperator::~FileCopyOperator()
{
    this->readFStream.clear();
    this->writeFstream.clear();
    this->readFStream.close();
    this->writeFstream.close();
}