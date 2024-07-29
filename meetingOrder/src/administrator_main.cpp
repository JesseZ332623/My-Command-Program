#include "../include/administrator.h"

enum AdministratorChoice
{
    ADD_NEW_MEETIMNG_ROOM = 1,
    ADD_NEW_MEETIMNG_ROOM_BETCH,
    MODIFY_METTING_ROOM_DATA,
    SEARCH_ONE_ROOM_STATE,
    DELETE_ONE_ROOM_STATE,
    DELETE_ALL_ROOM_STATE,
    SHOW_ALL_ROOM_STATE,
    SHOW_OPERATOR_MENU,
    RESET_ACCOUNT,
    LOG_OUT,
    EXIT_SYSYTEM = 0
};

int main(int argc, char const *argv[])
{
    using namespace MyLib::cinCheck;
    using namespace MyLib::MyLoger;

    system("cls");

    /**
     * 创建唯一的 Administrator 对象，
     * 调用构建函数，进行注册检查操作。
     */
    Administrator administrator;

#if true
    MessageStrings choiceStrings = 
    {
        "Enter your choice: ",
        "Invalid input! Enter number please!\n",
        "Out of range input! Enter number (0 ~ " + 
        std::to_string(LOG_OUT) + ") please!\n"
    };
    unsigned short administratorChoice;       // 用户的选择

    administrator.login();  // 登陆操作

    while (true)
    {
        administrator.showOperatorMenu();
        istreamInputAndCheck(
            std::cin, administratorChoice, choiceStrings,
            [] (const unsigned short __choice) { return (__choice >= 0 && __choice <= LOG_OUT); }
        );
        eatLine();

        switch (administratorChoice)
        {
            case ADD_NEW_MEETIMNG_ROOM:
                system("cls");
                administrator.addNewMeetingRoom();
                break;

            case ADD_NEW_MEETIMNG_ROOM_BETCH:
                administrator.betchAddNewMettingRoom();
                break;

            case MODIFY_METTING_ROOM_DATA:
                system("cls");
                administrator.modifyMeetRoomData();
                break;
            
            case SEARCH_ONE_ROOM_STATE:
                system("cls");
                administrator.searchOneRoomState();
                break;

            case DELETE_ONE_ROOM_STATE:
                system("cls");
                administrator.deleteMeetRoom();
                break;

            case DELETE_ALL_ROOM_STATE:
                system("cls");
                administrator.deleteAllMeetingRoom();
                break;

            case SHOW_ALL_ROOM_STATE:
                system("cls");
                administrator.showAllRoomState();
                break;
            
            case SHOW_OPERATOR_MENU:
                system("cls");
                break;

            case RESET_ACCOUNT:
                system("cls");
                administrator.resetAccount();
                break;
            
            case LOG_OUT:
                if (administrator.logout()) { administrator.login(); }
                break;
            
            case EXIT_SYSYTEM:
                system("cls");
                CORRECT_LOG("Have a good time! Bye!\n");
                DONE;
                exit(EXIT_SUCCESS);
        }
    }
#endif

    CORRECT_LOG("Have a good time! Bye!\n");
    DONE;
    
    return EXIT_SUCCESS;
}
