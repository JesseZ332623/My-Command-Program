#include "../include/administrator.h"

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    Administrator testAdministrator;

    char ifQuit;

    testAdministrator.login();

    while (ifQuit != 'q')
    {
        testAdministrator.addNewMeetingRoom();

        testAdministrator.searchOneRoomState();

        testAdministrator.deleteMeetRoom();

        NOTIFY_LOG("Quit to search and add? (Enter q to quit): ");
        std::cin >> ifQuit;

        MyLib::cinCheck::eatLine();
    }

    testAdministrator.logout();
    
    return EXIT_SUCCESS;
}