#include "../include/administrator.h"

int main(int argc, char const *argv[])
{
    Administrator testAdministrator;

    while (true)
    {
        testAdministrator.login();

        testAdministrator.showAllRoomState();

        testAdministrator.logout();
    }
    
    return EXIT_SUCCESS;
}
