#ifndef __ADMINISTRATOR_H_
#define __ADMINISTRATOR_H_

#include <vector>

#include "./defs.h"

#include "./meetingRoom.h"
#include "./reservations.h"

/**
 * @brief 管理员类的实现
 */
class Administrator
{
    private:
        /**
         * @brief 一个描述一间会议室，
         *        以及该会议室是否被占用的类
         */
        struct MettingRoomState
        {
            Meeting_Room mettingRoom;
            bool         hasocuupied;
        };

        /**
         * @brief 描述一个管理员的信息，用户名，密码。
         */
        struct AdministratorInfo
        {
            std::string userName;
            std::string passWord;
        };

        // 用一个动态数组来管理所有的会议室
        std::vector<MettingRoomState> roomRecord;

        // 用一个动态数组来管理预约人员信息
        std::vector<Reservations> reservationRecord;

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
         *        保证目标文件流处于打开状态，。
         * 
         * @param __inputFstream        输入文件流 
         * 
         * @return std::streamsize      一个文件的实际字节数
         */
        std::streamsize getFileBytes(std::ifstream & __inputFstream, std::streamoff __currentFpos)
        {
            __inputFstream.seekg(0, std::ios_base::end);
            std::streamsize fileBytes = __inputFstream.tellg();
            __inputFstream.seekg(__currentFpos, std::ios_base::beg);

            return fileBytes;
        }

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

    public:

        /**
         * @brief 默认构造函数，未完成。
         */
        Administrator() : roomRecord(), reservationRecord(), administratorInfo() 
        { 
            this->loadRoomDataFile(DATAFILE_PATH); 
        }

        void showAllRoomState(void);
};

Administrator::MettingRoomState
Administrator::getOneRoomState(char * __first, char * __last)
{
    std::size_t roomNoLen  = *reinterpret_cast<std::size_t *>(__first + sizeof(std::size_t));
    std::string roomNo(
                __first + sizeof(std::size_t) * 2, 
                __first + sizeof(std::size_t) * 2 + roomNoLen
            );
    std::size_t accomodateCount = *reinterpret_cast<std::size_t *>(__first + sizeof(std::size_t) * 2 + roomNoLen);
    bool hasMadia = *reinterpret_cast<bool *>(__first + sizeof(std::size_t) * 3 + roomNoLen);
    std::size_t introLen = *reinterpret_cast<std::size_t *>(__first + sizeof(std::size_t) * 3 + roomNoLen + 1);
    std::string introduce(
        __first + sizeof(std::size_t) * 4 + 1 + roomNoLen, 
        __first + sizeof(std::size_t) * 4 + 1 + roomNoLen + introLen
    );
    
    return {{roomNo, accomodateCount, hasMadia, introduce}, false};
}
  
void Administrator::loadRoomDataFile(const std::string & __path)
{
    /**
     * 数据文件的详细描述，见： `../documents/dataFileDescrible.md`
     */
    this->readStream.open(__path, std::ios_base::in | std::ios_base::binary);

    if (!this->readStream.is_open())                            // 打开文件
    {
        throw std::runtime_error("Can't open file: " + __path);
    }

    std::streamsize fileBytes = getFileBytes(this->readStream, this->readStream.tellg());

    char * buffer = new char[fileBytes];                          // 构建缓冲区，将整个文件的数据读入
    char * bufferBegin   = buffer;                                // 辅助指针，指向缓冲区之首
    char * bufferCurrent = buffer;                                // 辅助指针，指向缓冲区当前的操作位置
    char * bufferEnd     = buffer + fileBytes;                    // 辅助指针，指向缓冲区之尾

    this->readStream.read(buffer, fileBytes);                     // 将整个文件的数据读入

#if true
    /**
     * 在解析完整个缓冲区之前。
     */
    while (bufferCurrent != bufferEnd)
    {
        std::size_t objectSize = *reinterpret_cast<std::size_t *>(bufferCurrent);

        this->roomRecord.push_back(this->getOneRoomState(bufferCurrent, bufferCurrent + objectSize));

        bufferCurrent += objectSize;
    }
#endif

    delete[] buffer;
    this->readStream.close();
}

void Administrator::showAllRoomState(void)
{
    using namespace MyLib::MyLoger;

    std::for_each(
        this->roomRecord.begin(), this->roomRecord.end(),
        [](const Administrator::MettingRoomState & __meettingState)
        {
            __meettingState.mettingRoom.show();

            NOTIFY_LOG('[' + getCurrentTime() + " Administrator]: This meetting room\n");

            if (__meettingState.hasocuupied)
            {
                std::cout << ERROR 
                          << "HAS BEEN OCCUPIEDED.\n" 
                          << ORIGINAL;
            }
            else 
            { CORRECT_LOG("IS EMPTY.\n"); }
        }
    );
}

#endif // __ADMINISTRATOR_H_