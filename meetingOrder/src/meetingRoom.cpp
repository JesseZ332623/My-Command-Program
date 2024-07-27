#include "../include/meetingRoom.h"

void Meeting_Room::writeToFile(std::ofstream & __outputData)
{
    __outputData.open(DATAFILE_PATH, this->outputMode);

    if (!__outputData.is_open() || !__outputData) 
    {
        throw std::runtime_error("Can not open: " + DATAFILE_PATH + '\n');
    }

    std::size_t roomNoLen = this->roomNo.size();        // 拿到会议室号字符串的长度
    std::size_t introLen  = this->introduce.size();     // 拿到会议室介绍字符串的长度
    std::size_t objectBytes = this->getObjectBytes();   // 拿到单个对象序列化后的字节数

    // 写入整个类数据序列化后的字节数。
    __outputData.write(reinterpret_cast<char *>(&objectBytes), sizeof(std::size_t));

    // 写入 会议号字符串长度 + 会议号字符串
    __outputData.write(reinterpret_cast<char *>(&roomNoLen), sizeof(std::size_t));
    __outputData.write(this->roomNo.c_str(), this->roomNo.size());

    // 写入会议室能容纳的人数
    __outputData.write(reinterpret_cast<char *>(&this->roomContain), sizeof(std::size_t));

    // 写入是否支持多媒体
    __outputData.write(reinterpret_cast<char *>(&this->hasMidia), 1);

    // 写入会议室介绍字符串长度 + 会议号字符串
    __outputData.write(reinterpret_cast<char *>(&introLen), sizeof(std::size_t));
    __outputData.write(this->introduce.c_str(), this->introduce.size());

    __outputData.close();      // 关闭文件流
}

void Meeting_Room::show(void) const noexcept
{
    using namespace MyLib::MyLoger;
    using namespace MyLib::MyDelay;

    printSplitLine(45, '-', std::cout);

    NOTIFY_LOG("Meeting Room No. ["); CORRECT_LOG(this->roomNo); NOTIFY_LOG("]\n");
    delay(15);

    NOTIFY_LOG("Room can accommodate: "); 
    CORRECT_LOG(std::to_string(this->roomContain) + " person.\n");
    delay(15);

    NOTIFY_LOG("Meeting room media state: "); 
    if (this->hasMidia) { CORRECT_LOG("true\n\n"); }
    else { std::cout << ERROR << "false" << ORIGINAL << "\n\n"; }
    delay(15);

    NOTIFY_LOG("Meeting room introduce: \n");
    ORIGINAL_LOG(this->introduce + "\n\n");
    delay(15);
}

void Meeting_Room::inputInfo(void)
{
    using namespace MyLib::MyLoger;
    using namespace MyLib::cinCheck;

    MessageStrings peopleAccomodate = 
    {
        "Enter the number of people accomodate in: ",
        "Data format error! Enter number please!\n",
        ""
    };

    // 输入会议号
    while (true)
    {
        NOTIFY_LOG("Enter Meet Room No. (8 character limit): ");
        std::getline(std::cin, this->roomNo);

        if (!this->roomNo.size())
        {
            ERROR_LOG("Empty room number! Please enter again!\n");
            continue;
        }
        if (checkQuit(this->roomNo)) {
            CORRECT_LOG("BACK TO MENU\n"); return;
        }
        break;
    }

    // 输入会议室所容纳的人数，并作检查
    istreamInputAndCheck(std::cin, this->roomContain, peopleAccomodate);

    // 会议室是否支持多媒体
    char hasMedia;
    NOTIFY_LOG("This meeting room has media? (Y/N): ");
    std::cin >> hasMedia;
    eatLine();
    this->hasMidia = (hasMedia) ? true : false;

    // 输入会议室介绍
    while (true)
    {
        NOTIFY_LOG("Enter meeting room introduce: ");
        std::getline(std::cin, this->introduce);

        if (!this->introduce.size())
        {
            ERROR_LOG("Empty meeting room introduce! Please enter again!\n");
            continue;
        }
        break;
    }
}

std::ostream & operator<<(std::ostream & __os, const Meeting_Room & __info)
{
    using namespace MyLib::MyLoger;

    printSplitLine(45, '-', __os);

    __os << "Meeting Room No. ["      << __info.roomNo << "]\n"
         << "Room can accommodate: " << __info.roomContain << " person.\n"
         << "Meeting room media state: " << ((__info.hasMidia) ? "true" : "false") << "\n\n"
         << "Meeting room introduce: \n" << __info.introduce << '\n';

    printSplitLine(45, '-', __os);
    
    return __os;
}