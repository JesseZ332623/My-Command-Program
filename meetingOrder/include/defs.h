#ifndef __DEFS_H_
#define __DEFS_H_

#include <MyLib/myLib.h>

#include <vector>
#include <fstream>
#include <conio.h>

static const std::string DATAFILE_PATH = "../data/meet_room_info.dat";  // 数据文件路径
static const std::string ADMINISTRATOR_INFO_PATH = "../data/administratorInfo.dat";
static const std::string RESERVATIONS_INFO_PATH = "../data/reservationsInfo.dat";
static const std::string SOFTWARE_VERSION = "1.0.0.0";

/**
 * @brief 在输入密码的时候隐去密码的明文，避免泄漏。
 * 
 * @param __password    要输入的密码字符串
 */
void hiddenPassword(std::string & __password);

/**
 * @brief 检查是否要退出当前的操作
 * 
 * @param __argument    待检查的参数
 * 
 * @return `true`       输入了 Q 或 q 且字符串长度为 1
 * @return `false`      上述情况之外
 */
bool checkQuit(const std::string & __argument);

/**
 * @brief 输出软件的详细信息，如版本号，软件名等。
 */
void showSoftwareInfo(void);

/**
 * @brief 清空指定文件的数据。
 * 
 * @param __fstream     文件流（可以是 ifstream 也可以是 ofstream）
 */
void clearFiledata(std::ofstream & __fstream, const std::string & __path);

/**
  * @brief 获取一个文件的实际字节数，
  *        保证目标文件流处于打开状态。
  * 
  * @param __inputFstream        输入文件流 
  * 
  * @return `std::streamsize`    一个文件的实际字节数
*/
std::streamsize getFileBytes(std::ifstream & __inputStream);

#endif // __DEFS_H_