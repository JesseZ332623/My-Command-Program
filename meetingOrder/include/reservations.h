#ifndef __RESERVATIONS_H_
#define __RESERVATIONS_H_

#include "./defs.h"
#include "./meetingRoom.h"
#include "./cryption.h"

// 一个预约人员最大可预约的会议室
const std::size_t RESERVATION_LIMIT = 3;

class Reservations
{
    private:
        struct ReservationInfo
        {
            std::string name;           // 预约人员账号
            std::string password;

            Cryption cryption;

            /**
             * @brief 输入这个类的数据去初始化这个类
             */
            void getInput(void);
            
            /**
             * @brief 将单个加密后的预约人员对象数据写入文件
             * 
             * @param __writeStream     写文件流
             */
            void writeReservationDataToFile(std::ofstream & __writeStream);

            /**
             * @brief 读取数据文件填充到结构体中
             * 
             * @param __readStream      读文件流
             */
            void readDataToFill(std::ifstream & __readStream);

            /**
             * @brief 清除单个预约人员账户的信息
             */
            void clearInfo(void) { this->name.clear(); this->password.clear(); }

            void show(void) { printf("\n{%s}\n", this->password.c_str()); }

            friend bool operator==(const ReservationInfo & __a, const ReservationInfo & __b)
            { return (__a.name == __b.name) && (__a.password == __b.password); }
        };
        
        ReservationInfo reservationInfo;    // 预约人员账号对象

        std::size_t     currentReservationCount;

        std::ifstream readStream;
        std::ofstream writeStream;

        void loadAccountInfo(void);

    public:
        /**
         * @brief 默认构造函数，如果是新用户则要求注册
        */
        Reservations(void);

        /**
         * @brief 预约人员登录
         */
        void login(void);
};

#endif // __RESERVATIONS_H_