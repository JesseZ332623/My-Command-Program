#ifndef __MEETING_ROOM_H_
#define __MEETING_ROOM_H_

#include "./defs.h"

#include <cstring>

/**
 * @brief 会议室类设计
 */
typedef class __Meeting_Room
{
    private:
        std::string roomNo;             // 会议室编号（8 位任意字符的字符串）
        std::size_t roomContain;        // 会议室所容纳的人数
        bool hasMidia;                  // 会议室是否支持多媒体
        std::string introduce;          // 会议室介绍

        std::ofstream outPutData;       // 输出文件流，在输入完数据后写入数据文件
        const std::ios_base::openmode outputMode = 
        std::ios_base::out | std::ios_base::binary | std::ios_base::app;  

        /**
         * @brief 计算这个类数据的字节数。
         */
        const std::size_t loadObjectSize(void) 
        { 
            return (4 * sizeof(std::size_t)) + 1 + roomNo.size() + introduce.size();  
        }

    public:
        /**
         * @brief 默认构造函数，初始化一个空会议室。
         */
        __Meeting_Room() : 
        roomNo("00000000"), roomContain(0ULL), hasMidia(false), introduce() {}
        
        /**
         * @brief 构造函数，传入参数初始化会议室。
         * 
         * @param __no              会议室编号
         * @param __peoples         会议室所容纳的人数
         * @param __hasMidia        会议室是否支持多媒体
         * @param __introdeuce      会议室介绍
         */
        __Meeting_Room(
                        std::string __no, std::size_t __peoples, 
                        bool __hasMidia, std::string __introdeuce
        ) : roomNo(__no.substr(0, 8)), roomContain(__peoples), hasMidia(__hasMidia), introduce(__introdeuce)
        {}

        /**
         * @brief 将类数据序列化后写到数据文件中去。
         */
        void writeToFile(void);

        /**
         * @brief 从标注输入流里面读取会议室数据。
         */
        void inputInfo(void);

        /**
         * @brief 往标准输出流输出会议室信息，文字渲染颜色。
         */
        void show(void) const noexcept;

        /**
         * @brief 计算这个类数据的字节数。
         */
        const std::size_t getObjectBytes(void) noexcept { return this->loadObjectSize(); }
        
        /**
         * @brief 重载拼接运算符（<<），
         *        往标准输出，文件，或管道发送会议室类数据。
         * 
         * @param __os                  标准输出流
         * @param __info                __Meeting_Room 类             
         */
        friend std::ostream & operator<<(std::ostream & __os, const __Meeting_Room & __info);

        /**
         * @brief 通过比较 roomNo 来比较 __a 是否小于 __b。
         */
        friend bool operator<(const __Meeting_Room & __a, const __Meeting_Room & __b) 
        { return (__a.roomNo < __b.roomNo); }

        /**
         * @brief 通过比较 roomNo 来比较 __a 是否大于 __b。
         */
        friend bool operator>(const __Meeting_Room & __a, const __Meeting_Room & __b) 
        { return (__a.roomNo > __b.roomNo); }

        /**
         * @brief 通过比较 roomNo 来比较 __a 是否等于 __b。
         */
        friend bool operator==(const __Meeting_Room & __a, const __Meeting_Room & __b) 
        { return (__a.roomNo == __b.roomNo); }
        
} Meeting_Room;


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

#endif // __MEETING_ROOM_H_