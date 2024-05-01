#ifndef _COPY_DEFS_H_
#define _COPY_DEFS_H_

#include <MyLib/cinCheck.h>

#include <fstream>
#include <cstring>
#include <chrono>

/**
 *  @brief 计算一个函数的执行时间，精确到毫秒。
*/
#define TIMER(Run) [&]() { \
    auto t1 = std::chrono::system_clock::now(); \
    Run; \
    auto t2 = std::chrono::system_clock::now(); \
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count(); \
    return dt; \
}()

/**
 * @brief 不同操作系统对路径斜杠的使用都不一样，
 *        Windows 用反斜杠 Linux 用正斜杠
*/
#define WINDOWS_X86 true 
#define LINUX_UBUNTU false 

/*软件版本号*/
const std::string SOFTWARE_VERSION = "1.0.0";

/*通过系统确定使用正反斜杠*/
const constexpr char OS_SPECIFIC_SLASH = (WINDOWS_X86 ? '\\' : '/');

/*拷贝缓冲区大小，目前是 1 MB*/
const constexpr std::size_t BUFFER_SIZE = (1024 * 1024);

#endif // _COPY_DEFS_H_