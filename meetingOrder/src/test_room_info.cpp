#include "../include/meetingRoom.h"

int main(int argc, char const *argv[])
{
    Room_Info testMeetingRoomInfo(
                    "1234567890", 45, true, 
                    "This is a test meeting room."
                );

    system("cls");
    testMeetingRoomInfo.show();
    
    return EXIT_SUCCESS;
}
