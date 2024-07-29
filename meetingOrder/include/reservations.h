#ifndef __RESERVATIONS_H_
#define __RESERVATIONS_H_

#include "./defs.h"
#include "meetingRoom.h"

const std::size_t RESERVATION_LIMIT = 3;

class Reservations
{
    public:
        struct ReservationInfo
        {
            std::string name;           // 预约人员账号
            std::string password;

            /*
                用于仿射密码加密的结构体，
                a 和 b 作为一元函数f(x) = ax + b 的常数。
            */
            struct Affine { int a; int b; };

            /**
             * @brief 对 password 进行加密，使用仿射加密。
             * 
             * @return Affine 返回仿射结构体
             */
            Affine encryption(void);
            
            /**
             * @brief 将单个加密后的预约人员对象数据写入文件
             * 
             * @param __writeStream     写文件流
             */
            void writeReservationDataToFile(std::ofstream & __writeStream);

            /**
             * @brief 清除单个预约人员账户的信息
             */
            void clearInfo(void) { this->name.clear(); this->password.clear(); }

            void show(void) { printf("{%s, %s}\n", this->name.c_str(), this->password.c_str()); }
        };

        struct MeetRoomState 
        {
            Meeting_Room meetingRoom;
            bool hasOccupaied;

            void showState(void) {}
        };
        
        std::size_t   currentReservationCount;

        std::vector<MeetRoomState> roomRecord;

        std::ifstream readStream;
        std::ofstream writeStream;

        /**
         * @brief 加载数据文件，
         *        保存到动态数组 roomRecord 之中，作为构建函数的辅助函数。
         * 
         * @param __path                数据文件路径
         */
        void loadRoomDataFile(const std::string & __path);

    public:
        /**
         * @brief Construct a new Reservations object
        */
        Reservations(void);
};

#endif // __RESERVATIONS_H_