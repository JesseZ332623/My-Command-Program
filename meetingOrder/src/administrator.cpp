#include "../include/administrator.h"

#include <random>

void Administrator::MettingRoomState::
getStateInfo(void)
{
    using namespace MyLib::MyLoger;

    this->mettingRoom.inputInfo();      // 输入相关数据填充 Meeting_Room 对象

    if (checkQuit(mettingRoom.getMeetingRoomNo())) { return; }

    // 确定该会议室是否被占用
    NOTIFY_LOG("This meeting room has been occupied? (0 = false | 1 = true) ");
    std::cin >> this->hasocuupied;

    MyLib::cinCheck::eatLine();

    CORRECT_LOG("Complete to input state info!\n");
}

void Administrator::MettingRoomState::
writeRoomStateTofile(std::ofstream & __writeStream)
{
    using std::ios_base;
    using namespace MyLib::MyLoger;

    this->mettingRoom.writeToFile(__writeStream);   // 先将 mettingRoom 对象的数据写入文件

    __writeStream.open(DATAFILE_PATH, ios_base::out | ios_base::app | ios_base::binary);
    
    // 然后打开文件将是否占用的数据写入（有点浪费）
    __writeStream.write(reinterpret_cast<char *>(&this->hasocuupied), sizeof(bool));

    __writeStream.close();
}

void Administrator::AdministratorInfo::
writeAdminInfoToFile(std::ofstream & __writeStream, const int offset)
{
    using std::ios_base;
    
    __writeStream.open(ADMINISTRATOR_INFO_PATH, ios_base::out | ios_base::trunc | std::ios_base::binary);

    std::size_t passwordLen = this->passWord.size();    // 拿到密码字符串的长度
    int offsetCopy = offset;                            // 拿到密码偏移量的拷贝

    /**
     * 将用户名，密码偏移量，密码字符串长度，密码密文写入到文件。
     * 文件数据描述详见：`../documents/administratorInfo.dio`
    */
    __writeStream.write(this->userName.c_str(), 8);
    __writeStream.write(reinterpret_cast<char *>(&offsetCopy), sizeof(int));
    __writeStream.write(reinterpret_cast<char *>(&passwordLen), sizeof(std::size_t));
    __writeStream.write(this->passWord.c_str(), passwordLen);

    __writeStream.close();
}

std::streamsize
Administrator::getFileBytes(std::ifstream & __inputFstream)
{
    if (!__inputFstream.is_open() || !__inputFstream) // 检查文件流的状态
    {
        throw std::runtime_error("File stream is not open or in error state.");
    }

    // 保存当前 read 指针的位置到 originalPos
    std::streampos originalPos = __inputFstream.tellg();

    // 移动指针到末尾，通过 tellg() 拿到文件的字节数。
    __inputFstream.seekg(0, std::ios_base::end);
    std::streamsize fileBytes = __inputFstream.tellg();

    // 若 fileBytes 为 -1 说明出现了其他问题
    if (fileBytes == -1) {
        throw std::runtime_error("Failed to determine file size.");
    }
    // read 指针归位
    __inputFstream.seekg(originalPos);

    return fileBytes;
}

void Administrator::showOneRoomState(const MettingRoomState & __meettingState)
{
    using namespace MyLib::MyLoger;
    using namespace MyLib::MyDelay;
            
    __meettingState.mettingRoom.show();

    NOTIFY_LOG('[' + getCurrentTime() + " Administrator]:\nThis meetting room ");

    if (__meettingState.hasocuupied)    // 检查会议室是否被占用
    {
        std::cout << ERROR 
                  << "[HAS BEEN OCCUPIEDED].\n" 
                  << ORIGINAL;
    }
    else 
    { CORRECT_LOG("[IS EMPTY].\n"); }

    delay(15);

    printSplitLine(45, '-'); std::cout.put('\n');
}

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
    bool isoccupaid = (bool)(*__last);
    
    return {{roomNo, accomodateCount, hasMadia, introduce}, isoccupaid};
}

void Administrator::administratorRegister(void)
{
    using namespace MyLib::MyLoger;

    NOTIFY_LOG(
                '[' + getCurrentTime() + 
                "] Welcome!!! \nBefere becomming a new administrator" 
                "please input something as follows:\n"
        );

    while (true)
    {
        NOTIFY_LOG("Your user name (8 character limits): ");
        std::getline(std::cin, this->administratorInfo.userName);
        this->administratorInfo.userName = this->administratorInfo.userName.substr(0, 8);

        if (!this->administratorInfo.userName.size())
        {
            ERROR_LOG("Empty user name! Please enter again!\n");
            continue;
        }

        if (checkQuit(this->administratorInfo.userName))
        {
            DONE;
            exit(EXIT_SUCCESS);
        }

        break;
    }

    while (true)
    {
        NOTIFY_LOG(
                "Alright! Administrator [" + this->administratorInfo.userName + "]\n" +
                "Enter your password: "
            );
        
        hiddenPassword(this->administratorInfo.passWord);

        if (!this->administratorInfo.passWord.size())
        {
            ERROR_LOG("Empty password! Please enter again!\n");
            continue;
        }
        if (checkQuit(this->administratorInfo.userName))
        {
            DONE;
            exit(EXIT_SUCCESS);
        }

        break;
    }

    // 对密码明文进行加密，返回明文的偏移量。
    int offset = this->encryption(this->administratorInfo.passWord);
    this->administratorInfo.writeAdminInfoToFile(this->writeStream, offset);
    this->administratorInfo.clearInfo();

    CORRECT_LOG(
                    '[' + getCurrentTime() +
                    "] Complete to administrator register,\n" 
                    "write administrator account data to file [" + ADMINISTRATOR_INFO_PATH + "].\n"
            );

    printSplitLine(45, '-');
}
  
void Administrator::loadRoomDataFile(const std::string & __path)
{
    /**
     * 数据文件的详细描述，见： `../documents/dataFileDescrible.md`
     */
    this->readStream.open(__path, std::ios_base::in | std::ios_base::binary);

    if (!this->readStream.is_open() || !readStream)                            // 打开文件
    {
        throw std::runtime_error("Can't open file: " + __path);
    }

    std::streamsize fileBytes = getFileBytes(this->readStream);

    if (!fileBytes) { this->readStream.close(); return; }

    char * buffer        = new char[fileBytes];                   // 构建缓冲区，将整个文件的数据读入
    char * bufferCurrent = buffer;                                // 辅助指针，指向缓冲区当前的操作位置
    char * bufferEnd     = buffer + fileBytes;                    // 辅助指针，指向缓冲区之尾

    this->readStream.read(buffer, fileBytes);                     // 将整个文件的数据读入

    /**
     * 在解析完整个缓冲区之前。
     */
    while (bufferCurrent != bufferEnd)
    {
        std::size_t objectSize = *reinterpret_cast<std::size_t *>(bufferCurrent) + 1;

        this->roomRecord.push_back(this->getOneRoomState(bufferCurrent, bufferCurrent + objectSize));

        bufferCurrent += objectSize;
    }

    delete[] buffer;
    this->readStream.close();
}

int Administrator::encryption(std::string & __password)
{
    std::random_device randDevice;
    std::mt19937_64 randEngine(randDevice());
    std::uniform_int_distribution<> distribute(3, 8);

    int offset = distribute(randEngine);

    std::transform(
                    __password.begin(), __password.end(), __password.begin(),
                    [&](unsigned char ch) { return ch + offset; }
                );

    std::reverse(__password.begin(), __password.end());

    return offset;
}

Administrator::AdministratorInfo
Administrator::decryption(void)
{
    using namespace MyLib::MyLoger;

    this->readStream.open(ADMINISTRATOR_INFO_PATH, std::ios_base::in | std::ios_base::binary);

    std::size_t readBytes = this->getFileBytes(this->readStream);

    char * buffer = new char[readBytes];
    this->readStream.read(buffer, readBytes);

    std::string userName(buffer, buffer + 8);

    int offset = *reinterpret_cast<int *>(buffer + 8);
    std::size_t passwordLen = *reinterpret_cast<std::size_t *>(buffer + 8 + sizeof(int));

    std::string cipherText(
            buffer + 8 + sizeof(int) + sizeof(std::size_t),
            buffer + 8 + sizeof(int) + sizeof(std::size_t) + passwordLen
        );

    std::transform(
            cipherText.begin(), cipherText.end(), cipherText.begin(),
            [&](const char ch) { return ch - offset; }
        );

    std::reverse(cipherText.begin(), cipherText.end());

    delete[] buffer;
    this->readStream.close();

    return AdministratorInfo({userName, cipherText});
}

Administrator::Administrator() : reservationRecord(), administratorInfo()
{
    this->loadRoomDataFile(DATAFILE_PATH); 
    this->readStream.open(ADMINISTRATOR_INFO_PATH, std::ios_base::in | std::ios_base::binary);

    if (!this->getFileBytes(this->readStream))
    {
        this->readStream.close();
        this->administratorRegister();
    }

    this->readStream.close();
}

void Administrator::login(void)
{
    using namespace MyLib::MyLoger;

    CORRECT_LOG("Find Administrator account information.\n");
    NOTIFY_LOG("Please LOGIN (Enter q / Q to back): \n");

    AdministratorInfo tempAdmiInfo;
    AdministratorInfo decryptionInfo(this->decryption());

    while (true)
    {
        while (true)
        {
            NOTIFY_LOG("Your user name : ");
            std::getline(std::cin, tempAdmiInfo.userName);

            if (!tempAdmiInfo.userName.size())
            {
                ERROR_LOG("Empty user name! Please enter again!\n");
                continue;
            } 

            if (checkQuit(tempAdmiInfo.userName))
            {
                DONE;
                exit(EXIT_SUCCESS);
            }
            
            if (decryptionInfo.userName != tempAdmiInfo.userName)
            {
                ERROR_LOG(
                    "User name does not match, you enterd: {" + 
                    tempAdmiInfo.userName + "} try again please!\n"
                );
                continue;
            }
            break;
        }

        while (true)
        {
            NOTIFY_LOG(
                '[' + getCurrentTime() + 
                "] Hello Administrator [" + tempAdmiInfo.userName + 
                "],\nYour password: "
            );
            
            hiddenPassword(tempAdmiInfo.passWord);

            if (!tempAdmiInfo.passWord.size())
            {
                ERROR_LOG("Empty password! Please enter again!\n");
                continue;
            }

            if (tempAdmiInfo == decryptionInfo) 
            {
                printSplitLine(45, '-');
                showSoftwareInfo();
                CORRECT_LOG(    
                                '[' + getCurrentTime() +
                                "] Welcome Administrator: [" + tempAdmiInfo.userName + "]\n"
                        );
                tempAdmiInfo.clearInfo();
                decryptionInfo.clearInfo();
                break;
            }
            else
            {
                ERROR_LOG("Password incorrect!\n""Please enter again!\n");
                continue;
            }
        }
        break;
    }
}

bool Administrator::logout(void)
{
    using namespace MyLib::MyLoger;
    using namespace MyLib::cinCheck;
    using namespace MyLib::MyDelay;

    system("cls");

    CORRECT_LOG("[LOG OUT]\n");

    char confirm;
    std::string administratorName = decryption().userName;

    MessageStrings logoutInfo = {
        std::string("Administrator [" + administratorName + "].\n" + 
            "Are you make sure to logout? (Y/N) "),
        "",
        "Enter (Y/N) to LOGOUT\n"
    };

    istreamInputAndCheck(
        std::cin, confirm, logoutInfo,
        [](const char __confirm) { return (__confirm != 'Y' || __confirm != 'N'); }
    );
    eatLine();

    if (confirm == 'N') { CORRECT_LOG("BACK\n"); return false; }
    else 
    { 
        WARNING_LOG("Administrator [" + administratorName + "] LOG OUT!\n");
        printSplitLine(65, '-'); 
    }

    return true;
}

void Administrator::showOperatorMenu(void)
{
    using namespace MyLib::MyLoger;

    CORRECT_LOG("[SHOW ADMINISTRATOR OPERATOR MENU]\n");

    printSplitLine(75, '-');

    ORIGINAL_LOG(
        "1. Add new meeting room\t\t\t2. Modify meeting room data\n"
        "3. Search one meeting room state\t4. Delete one meeting room\n"
        "5. Delete all meeting room\t\t6. Show all room state\n"
        "7. Show operator menu\n"
    );

    std::cout << WARNING << "8. LOG OUT\t\t\t\t"   << ORIGINAL;
    std::cout << ERROR   << "9. EXIT SYSTEM" << ORIGINAL << '\n';

    printSplitLine(75, '-');
}

void Administrator::addNewMeetingRoom(void)
{
    using namespace MyLib::MyLoger;

    Administrator::MettingRoomState tempRoomState;

    CORRECT_LOG("[ADD NEW MEETING ROOM DATA]\n");

    while (true)
    {
        tempRoomState.getStateInfo();

        if (checkQuit(tempRoomState.mettingRoom.getMeetingRoomNo())) { return; }

        if (
                this->searchOneRoomStateAux(tempRoomState.mettingRoom.getMeetingRoomNo()) != 
                this->roomRecord.end()
            )
        {
            WARNING_LOG("Repeat record! Repeat again!\n");
            continue;
        }
        break;
    }

    tempRoomState.writeRoomStateTofile(this->writeStream);
    this->roomRecord.push_back(std::move(tempRoomState));

    CORRECT_LOG("OK! Write to file: [" + DATAFILE_PATH + "]\n");
    CORRECT_LOG("OK! Push back tempRoomState to roomRecord\n");
}

void Administrator::deleteMeetRoom(void)
{
    using namespace MyLib::MyLoger;
    using std::ios_base;

    std::string tempRoomNo;
    std::vector<MettingRoomState>::iterator targetIter;

    CORRECT_LOG("[DELETE ONE MEETING ROOM DATA]\n");

    while (true)
    {
        NOTIFY_LOG("(q / Q to quit)\n");
        NOTIFY_LOG("Enter meeting room No. ");
        std::getline(std::cin, tempRoomNo);

        if (checkQuit(tempRoomNo)) { return; }

        targetIter = searchOneRoomStateAux(tempRoomNo);

        if (targetIter == this->roomRecord.end())
        {
            ERROR_LOG("NO. " + tempRoomNo + "are not in room record! Please enter again.\n");
            continue;
        }
        break;
    }

    this->roomRecord.erase(targetIter);

    this->writeStream.open(DATAFILE_PATH, ios_base::trunc);
    this->writeStream.close();

    for (MettingRoomState & __roomState : this->roomRecord)
    {
        __roomState.writeRoomStateTofile(this->writeStream);
    }

    CORRECT_LOG("Comple to delete meeting room which No is: " + tempRoomNo + '\n');
    CORRECT_LOG("OK! Update data from file: [" + DATAFILE_PATH + "]\n");
}

void Administrator::deleteAllMeetingRoom(void)
{
    using namespace MyLib::MyLoger;
    using namespace MyLib::cinCheck;

    CORRECT_LOG("[DELETE ALL MEETING ROOM DATA]\n");
    char doOperator;

    while (true)
    {
        WARNING_LOG(
            "This is a sensitive operator, \n"
            "Are you sure to do that? (Y/N): "
        );
        std::cin >> doOperator;

        if (doOperator == 'Y') { break; }
        else if (doOperator == 'N') { CORRECT_LOG("BACK\n"); return; }
        else { WARNING_LOG("Enter (Y/N)!\n"); continue; }
    }

    if (!this->roomRecord.size()) 
    { 
        CORRECT_LOG("OK! Clear room record complete!\n");
        return; 
    }

    this->roomRecord.clear();
    this->writeStream.open(DATAFILE_PATH, std::ios_base::trunc);
    this->writeStream.close();

    CORRECT_LOG("OK! Clear room record complete!\n");
}

void Administrator::showModifyMenu(void)
{
    using namespace MyLib::MyLoger;

    printSplitLine(75, '-');

    ORIGINAL_LOG(
        "1. Room accomodate\t\t" "2. Media State\n"
        "3. Room Introduce\t\t" "4. Occupation State\n"
    );

    std::cout << WARNING << "5. Back" << ORIGINAL << '\n';

    printSplitLine(75, '-');
}

void Administrator::modifyOperator(const unsigned short __choice, RoomStatesIter __iter)
{
    using namespace MyLib::MyLoger;
    using namespace MyLib::cinCheck;

    enum {
        MODIFY_ACCOMODATE = 1,
        MODIFY_MEDIA_STATE,
        MODIFY_ROOM_INTRODUCE,
        MODIFY_OCCUPATION_STATE,
    };

    MessageStrings prompt = {
            "Enter new accomodate amount: ", 
            "Invalid input, Enter number please!\n"
        };

    switch (__choice)
    {
        case MODIFY_ACCOMODATE:
            std::size_t tempAmount;
            
            istreamInputAndCheck(std::cin, tempAmount, prompt);

            (*__iter).mettingRoom.getRoomContain() = tempAmount;
            break;
        
        case MODIFY_MEDIA_STATE:
            prompt.promptMessage   = "Enter new media state (Y/N): ";
            prompt.errorMessage    = "Enter (Y or N)!\n";
            prompt.outRangeMessage = "Enter (Y or N)!\n";
            char hasMedia;

            istreamInputAndCheck(
                std::cin, hasMedia, prompt, [](char __hasmedia) 
                { return (__hasmedia != 'Y' || __hasmedia != 'N'); }
            );

            eatLine();

            (*__iter).mettingRoom.getMidaiState() = (hasMedia == 'Y') ? true : false;
            break;

        case MODIFY_ROOM_INTRODUCE:
            NOTIFY_LOG("Enter new introduce: \n");
            std::getline(std::cin, (*__iter).mettingRoom.getIntroduce());
            break;

        case MODIFY_OCCUPATION_STATE:
            prompt.promptMessage   = "This meeting room has been occupied? (0 = false | 1 = true) ";
            prompt.errorMessage    = "Enter (0 or 1)!\n";

            bool hasocuupied;
            istreamInputAndCheck(std::cin, hasocuupied, prompt);

            (*__iter).hasocuupied = hasocuupied;
            break;
    }
}

void Administrator:: modifyMeetRoomData(void)
{
    using namespace MyLib::MyLoger;
    using namespace MyLib::cinCheck;

    std::string tempRoomNo;
    std::vector<MettingRoomState>::iterator tempIter;

    CORRECT_LOG("[MODIFY MEETING ROOM DATA]\n");

    while (true)
    {
        NOTIFY_LOG("Enter Q / q to quit.\n");
        NOTIFY_LOG("Enter meeting room No. ");
        std::getline(std::cin, tempRoomNo);

        if (checkQuit(tempRoomNo)) { CORRECT_LOG("BACK\n"); return; }

        if ((tempIter = this->searchOneRoomStateAux(tempRoomNo)) == this->roomRecord.end())
        {
            ERROR_LOG('{' + tempRoomNo + "} is not in room record! Please enter again\n");
            continue;
        }
        break;
    }

    CORRECT_LOG("OK! The detail of No. " + tempRoomNo + " is: \n");
    this->showOneRoomState(*tempIter);

    unsigned short modifyChoice;
    MessageStrings modifyChString = 
    {
        "Enter your Choice: ",
        "Invalid input! Enter number please!\n",
        "Out of range input! Enter number (1 ~ 5) please!\n"
    };

    NOTIFY_LOG("Which argument do you want to modify: \n");
    this->showModifyMenu();

    istreamInputAndCheck(
        std::cin, modifyChoice, modifyChString, 
        [](const unsigned short choice) { return (choice >= 1 && choice <= 5); }
    );
    eatLine();

    if  (modifyChoice == 5) { CORRECT_LOG("Back.\n"); return; }

    this->modifyOperator(modifyChoice, tempIter);

    this->writeStream.open(DATAFILE_PATH, std::ios_base::trunc);
    this->writeStream.close();

    for (MettingRoomState & state : this->roomRecord)
    {
        state.writeRoomStateTofile(this->writeStream);
    }

    CORRECT_LOG("OK! Complete to midify, File [" + DATAFILE_PATH + "] has been update.\n");

    this->showOneRoomState(*tempIter);
}

Administrator::RoomStatesIter 
Administrator::searchOneRoomStateAux(const std::string & __roomNo)
{
    // 先构造出一个临时的会议室类
    MettingRoomState tempRoomState({{__roomNo, 0, false, ""}, false});

    return std::find(this->roomRecord.begin(), this->roomRecord.end(), tempRoomState);
}

void Administrator::searchOneRoomState(void)
{
    using namespace MyLib::MyLoger;

    CORRECT_LOG("[SEARCH ONE ROOM STATE]\n");

    std::string roomNo;
    RoomStatesIter targetIter;

    while (true)
    {
        NOTIFY_LOG("Enter Q / q to quit.\n");
        NOTIFY_LOG("Enter meeting room number to search: ");
        std::getline(std::cin, roomNo);

        if (!roomNo.size())
        {
            ERROR_LOG("Empty room number, please enter again!\n");
            continue;
        }

        if (checkQuit(roomNo)) { CORRECT_LOG("BACK\n"); return; }

        roomNo = roomNo.substr(0, 8);

        targetIter = this->searchOneRoomStateAux(roomNo);

        if (targetIter == this->roomRecord.end())
        {
            WARNING_LOG(
                '[' + roomNo + "] are not in roomRecord.\n"
                "Please enter again\n"
            );
            continue;
        }
        break;
    }

    this->showOneRoomState(*targetIter);
}

void Administrator::showAllRoomState(void)
{
    using namespace MyLib::MyLoger;
    using MyLib::MyDelay::delay;

    CORRECT_LOG("[SHOW ALL MEETING ROOM STATE]\n");

    if (!this->roomRecord.size()) {
        ERROR_LOG("NO ROOM RECORD HERE\n");
        delay(15);
        return;
    }
    
    std::sort(this->roomRecord.begin(), this->roomRecord.end());
    for (const MettingRoomState & __meettingState : this->roomRecord)
    {
        this->showOneRoomState(__meettingState);
        delay(15);
    }
}