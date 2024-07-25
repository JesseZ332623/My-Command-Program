#ifndef __MEETING_ROOM_H_
#define __MEETING_ROOM_H_

#include "./defs.h"

/**
 * @brief 会议室类设计
 */
class Meeting_Room
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
        Meeting_Room() : 
        roomNo("00000000"), roomContain(0ULL), hasMidia(false), introduce() {}
        
        /**
         * @brief 构造函数，传入参数初始化会议室。
         * 
         * @param __no              会议室编号
         * @param __peoples         会议室所容纳的人数
         * @param __hasMidia        会议室是否支持多媒体
         * @param __introdeuce      会议室介绍
         */
        Meeting_Room(
                        std::string __no, std::size_t __peoples, 
                        bool __hasMidia, std::string __introdeuce
        ) : roomNo(__no.substr(0, 8)), roomContain(__peoples), hasMidia(__hasMidia), introduce(__introdeuce)
        {}

        //Meeting_Room(const Meeting_Room &) = default;

        /**
         * @brief 将类数据序列化后写到数据文件中去。
         */
        void writeToFile(std::ofstream & __outputData);

        /**
         * @brief 从标注输入流里面读取会议室数据。
         */
        void inputInfo(void);

        /**
         * @brief Get the Meeting Room No object
         * 
         * @return std::string& 
         */
        std::string & getMeetingRoomNo(void) { return this->roomNo; }

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
        friend std::ostream & operator<<(std::ostream & __os, const Meeting_Room & __info);

        /**
         * @brief 通过比较 roomNo 来比较 __a 是否小于 __b。
         */
        friend bool operator<(const Meeting_Room & __a, const Meeting_Room & __b) 
        { return (__a.roomNo < __b.roomNo); }

        /**
         * @brief 通过比较 roomNo 来比较 __a 是否大于 __b。
         */
        friend bool operator>(const Meeting_Room & __a, const Meeting_Room & __b) 
        { return (__a.roomNo > __b.roomNo); }

        /**
         * @brief 通过比较 roomNo 来比较 __a 是否等于 __b。
         */
        friend bool operator==(const Meeting_Room & __a, const Meeting_Room & __b) 
        { return (__a.roomNo == __b.roomNo); }
        
};

#endif // __MEETING_ROOM_H_