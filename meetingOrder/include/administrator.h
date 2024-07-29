#ifndef __ADMINISTRATOR_H_
#define __ADMINISTRATOR_H_

#include "./defs.h"
#include "./meetingRoom.h"
#include "./reservations.h"

#include <vector>

/**
 * @brief 管理员类的实现
 */
class Administrator
{
    private:
        /**
         * @brief 一个描述一间会议室，以及该会议室是否被占用的类
         */
        struct MettingRoomState
        {
            typedef Meeting_Room MeetingRoom;

            Meeting_Room mettingRoom;
            bool         hasocuupied;

            //MettingRoomState(const MettingRoomState &&) = default;

            /**
             * @brief 输入这个类的信息，
             *        通常用于辅助构造一个类。
             */
            void getStateInfo(void);

            /**
             * @brief 将 MettingRoomState 类的数据序列化后写入文件。
             */
            void writeRoomStateTofile(std::ofstream & __writeStream);

            /**
             * @brief 调用 Meeting_Room 类的 operator==() 去比较两个
             *        MettingRoomState 类是否相同。 
             */
            friend bool operator==(const MettingRoomState & __a, const MettingRoomState & __b)
            {
                return __a.mettingRoom == __b.mettingRoom;
            }

            friend bool operator<(const MettingRoomState & __a, const MettingRoomState & __b)
            {
                return __a.mettingRoom < __b.mettingRoom;
            }

            friend bool operator>(const MettingRoomState & __a, const MettingRoomState & __b)
            {
                return __a.mettingRoom > __b.mettingRoom;
            }
        };

        /**
         * @brief 描述一个管理员的信息，用户名，密码。
         */
        struct AdministratorInfo
        {
            std::string userName;
            std::string passWord;

            /**
             * @brief 将管理员账号信息写入到指定的文件中
             * 
             * @param __writeStream     输出文件流
             * @param offset            偏移量
            */
            void writeAdminInfoToFile(std::ofstream & __writeStream, const int offset);

            /**
             * @brief 清理掉整密码字符串的数据，
             *        防止有心之人中断程序后在内核转储文件里面拿到明文密码。
            */
            void clearInfo(void) { this->userName.clear(); this->passWord.clear(); }

            /**
             * @brief 比对两个管理员信息的密码数据是否完全相同，这用于登录时的判断。
             */
            friend bool operator==(const AdministratorInfo & __a, const AdministratorInfo & __b)
            {
                return (__b.passWord == __a.passWord);
            }
        };

        typedef std::vector<MettingRoomState>::iterator RoomStatesIter;

        // 用一个动态数组来管理所有的会议室
        std::vector<MettingRoomState> roomRecord;

        // 用一个动态数组来管理预约人员信息
        std::vector<Reservations> reservationRecord;

        // 管理员用户账户信息结构体
        AdministratorInfo administratorInfo;

        std::ifstream readStream;
        std::ofstream writeStream;

        /**
         * @brief 加载数据文件，
         *        保存到动态数组 roomRecord 之中，作为构建函数的辅助函数。
         * 
         * @param __path                数据文件路径
         */
        void loadRoomDataFile(const std::string & __path);

        /**
         * @brief 获取一个文件的实际字节数，
         *        保证目标文件流处于打开状态。
         * 
         * @param __inputFstream        输入文件流 
         * 
         * @return `std::streamsize`    一个文件的实际字节数
         */
        std::streamsize getFileBytes(std::ifstream & __inputFstream);

        /**
         * @brief 对管理员密码进行加密操作，使用凯撒密码。
         * 
         * @param __password        管理员密码（明文）
         * 
         * @return int              明文的偏移量
         */
        int encryption(std::string & __password);

        /**
         * @brief 从管理员账户数据文件中读取密文，
         *        解密后返回。
         * 
         * @return std::string      解密后的明文
         */
        AdministratorInfo decryption(void);

        /**
         * @brief 在管理员密码文件不为空的时候，进行注册操作。
        */
        void administratorRegister(void);

        /**
         * @brief 解析缓冲区中的 [__first, __last) 范围内的数据，
         *        构建出一个 MettingRoomState 类，存储到 roomRecord 动态数组中。
         * 
         * @param __first   数据块头部
         * @param __last    数据块尾部
         * 
         * @return MettingRoomState 
        */
        MettingRoomState getOneRoomState(char * __first, char * __last);

        /**
         * @brief 公共接口 `betchAddNewMettingRoom()` 的辅助函数，
         *        传入数组拼接出一个会议号。
         */
        std::string assembleRoomNoString(const int number);

        /**
         * @brief 显示单个会议室类的相关信息。
         * 
         * @param __meettingState   会议室类对象的引用
         */
        void showOneRoomState(const MettingRoomState & __meettingState);

        /**
         * @brief 通过会议室号查询单间会议室的信息。
         * 
         * @param __roomNo 会议室号字符串
         * 
         * @return 目标会议室类的迭代器
         */
        RoomStatesIter
        searchOneRoomStateAux(const std::string & __roomNo);

        /**
         * @brief 输出修改会议室相关信息的操作。
         */
        void showModifyMenu(void);

        /**
         * @brief 对用户输入的选择进行不同的修改操作。
         * 
         * @param __choice  修改选择
         */
        void modifyOperator(const unsigned short __choice, RoomStatesIter __iter);

    public:
        /**
         * @brief 默认构造函数。
        */
        Administrator(void);
       
        /**
         * @brief 如果确认管理员账户信息存在的话，进行登录操作。
        */
        void login(void);

        /**
         * @brief 管理员登出操作。
         */
        bool logout(void);

        /**
         * @brief 重置管理员的账户和密码。
         */
        void resetAccount(void);

        /**
         * @brief 输出管理员可以操作的选项。
         */
        static void showOperatorMenu(void);

        /**
         * @brief 增加新的会议室，
         *        并写入到 `roomRecord` 动态数组 
         *        和 `../data/meet_room_info.dat` 数据文件末尾。
         */
        void addNewMeetingRoom(void);

        /**
         * @brief 批量的增加会议室数据。
         */
        void betchAddNewMettingRoom(void);

        /**
         * @brief 通过输入会议号删除某个指定的会议室，
         *        也注意修改 `roomRecord` 动态数组 
         *        和 `../data/meet_room_info.dat` 的数据。
         */
        void deleteMeetRoom(void);

        /**
         * @brief 删除所有的会议室记录。
         */
        void deleteAllMeetingRoom(void);

        /**
         * @brief 修改会议室的相关数据.
         */
        void modifyMeetRoomData(void);

        /**
         * @brief 输入会议号，查询该会议号对应的会议室信息。
         */
        void searchOneRoomState(void);

        /**
         * @brief 输出当前所有会议室的信息和占用状态。
        */
        void showAllRoomState(void);
};

#endif // __ADMINISTRATOR_H_