#ifndef __MEETING_ROOM_H_
#define __MEETING_ROOM_H_

#include "./defs.h"

#include <cstring>

typedef struct __Room_Info
{
    std::string roomNo;
    std::size_t roomContain;
    bool hasMidia;
    std::string introduce;

    __Room_Info() : 
    roomNo("00000000"), roomContain(0ULL), hasMidia(false), introduce() {}

    __Room_Info(
                    std::string __no, std::size_t __peoples, 
                    bool __hasMidia, std::string __introdeuce
    ) : roomNo(__no), roomContain(__peoples), hasMidia(__hasMidia), introduce(__introdeuce)  {}

    void show(void) const noexcept;

    friend std::ostream & operator<<(std::ostream & __os, const __Room_Info & __info);

} Room_Info;

void Room_Info::show(void) const noexcept
{
    using namespace MyLib::MyLoger;

    printSplitLine(45, '-', std::cout);

    NOTIFY_LOG("Meeting Room No. "); CORRECT_LOG(this->roomNo + '\n');

    NOTIFY_LOG("Room can accommodate: "); 
    CORRECT_LOG(std::to_string(this->roomContain) + '\n');

    NOTIFY_LOG("Meeting room media state: "); 
    CORRECT_LOG(std::string((this->hasMidia) ? "true" : "false") + '\n');

    NOTIFY_LOG("Meeting room introduce: \n");
    ORIGINAL_LOG(this->introduce + '\n');

    printSplitLine(45, '-', std::cout);
} 

class Meeting_Room
{
    private:
        Room_Info meetingRoomInfo;

    public:
        Meeting_Room() : meetingRoomInfo() {}

};

#endif // __MEETING_ROOM_H_