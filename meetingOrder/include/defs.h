#ifndef __DEFS_H_
#define __DEFS_H_

#include <MyLib/myLib.h>
#include <fstream>
#include <conio.h>

static const std::string DATAFILE_PATH = "../data/meet_room_info.dat";  // 数据文件路径
static const std::string ADMINISTRATOR_INFO_PATH = "../data/administratorInfo.dat";

/**
 * @brief 在输入密码的时候隐去密码的明文，避免泄漏。
 * 
 * @param __password    要输入的密码字符串
 */
void hiddenPassword(std::string & __password);

#endif // __DEFS_H_