#include "../include/meetingRoom.h"

void __Meeting_Room::writeToFile(void)
{
    this->outPutData.open(DATAFILE_PATH, this->outputMode);

    if (!this->outPutData.is_open()) 
    {
        throw std::runtime_error("Can not open: " + DATAFILE_PATH + '\n');
    }

    std::size_t roomNoLen = this->roomNo.size();
    std::size_t introLen  = this->introduce.size();
    std::size_t objectBytes = this->getObjectBytes();

    // 写入整个类数据序列化后的字节数。
    this->outPutData.write(reinterpret_cast<char *>(&objectBytes), sizeof(std::size_t));

    // 写入 会议号字符串长度 + 会议号字符串
    this->outPutData.write(reinterpret_cast<char *>(&roomNoLen), sizeof(std::size_t));
    this->outPutData.write(this->roomNo.c_str(), this->roomNo.size());

    // 写入会议室能容纳的人数
    this->outPutData.write(reinterpret_cast<char *>(&this->roomContain), sizeof(std::size_t));

    // 写入是否支持多媒体
    this->outPutData.write(reinterpret_cast<char *>(&this->hasMidia), 1);

    // 写入会议室介绍字符串长度 + 会议号字符串
    this->outPutData.write(reinterpret_cast<char *>(&introLen), sizeof(std::size_t));
    this->outPutData.write(this->introduce.c_str(), this->introduce.size());

    this->outPutData.close();
}

void __Meeting_Room::show(void) const noexcept
{
    using namespace MyLib::MyLoger;

    printSplitLine(45, '-', std::cout);

    NOTIFY_LOG("Meeting Room No. ["); CORRECT_LOG(this->roomNo); NOTIFY_LOG("]\n");

    NOTIFY_LOG("Room can accommodate: "); 
    CORRECT_LOG(std::to_string(this->roomContain) + " person.\n");

    NOTIFY_LOG("Meeting room media state: "); 
    CORRECT_LOG(std::string((this->hasMidia) ? "true" : "false") + "\n\n");

    NOTIFY_LOG("Meeting room introduce: \n");
    ORIGINAL_LOG(this->introduce + '\n');

    printSplitLine(45, '-', std::cout);
}

void __Meeting_Room::inputInfo(void)
{
    using namespace MyLib::MyLoger;
    using namespace MyLib::cinCheck;

    MessageStrings peopleAccomodate = 
    {
        "Enter the number of people accomodate in: ",
        "Data format error! Enter number please!\n",
        ""
    };

    NOTIFY_LOG("Enter Meet Room No. (8 character limit): ");
    std::getline(std::cin, this->roomNo);

    istreamInputAndCheck(std::cin, this->roomContain, peopleAccomodate);

    char hasMedia;
    NOTIFY_LOG("This meeting room has media? (Y/N): ");
    std::cin >> hasMedia;
    eatLine();
    this->hasMidia = (hasMedia) ? true : false;

    NOTIFY_LOG("Enter meeting room introduce: ");
    std::getline(std::cin, this->introduce);

    this->writeToFile();

    CORRECT_LOG("OK! Construct meeting room success! Write to file: [" + DATAFILE_PATH + "]\n");
}

std::ostream & operator<<(std::ostream & __os, const __Meeting_Room & __info)
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