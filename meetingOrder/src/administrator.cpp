#include "../include/administrator.h"

#include <cstdlib>

void Administrator::MettingRoomState::
getStateInfo(void)
{
    using namespace MyLib::MyLoger;

    this->mettingRoom.inputInfo();
    NOTIFY_LOG("This meeting room has been occupied? (0 = false | 1 = true)");
    std::cin >> this->hasocuupied;

    MyLib::cinCheck::eatLine();

    CORRECT_LOG("Complete to input state info!\n");
}

void Administrator::MettingRoomState::
writeRoomStateTofile(std::ofstream & __writeStream)
{
    using std::ios_base;
    using namespace MyLib::MyLoger;

    //__writeStream.open(DATAFILE_PATH, ios_base::out | ios_base::app);

    this->mettingRoom.writeToFile(__writeStream);

    __writeStream.open(DATAFILE_PATH, ios_base::out | ios_base::app | ios_base::binary);

    //__writeStream.seekp(0, ios_base::end);
    __writeStream.write(reinterpret_cast<char *>(&this->hasocuupied), sizeof(bool));

    __writeStream.close();

    CORRECT_LOG("OK! Write to file: [" + DATAFILE_PATH + "]\n");
}

void Administrator::AdministratorInfo::
writeAdminInfoToFile(std::ofstream & __writeStream, const int offset)
{
    using std::ios_base;
    
    __writeStream.open(ADMINISTRATOR_INFO_PATH, ios_base::out | ios_base::trunc);

    std::size_t passwordLen = this->passWord.size();
    int offsetCopy = offset;

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
                "] Welcome!!! Befere becomming a new administrator" 
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
        } else { break; }
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
        } else { break; }
    }

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

#if true
    /**
     * 在解析完整个缓冲区之前。
     */
    while (bufferCurrent != bufferEnd)
    {
        std::size_t objectSize = *reinterpret_cast<std::size_t *>(bufferCurrent) + 1;

        this->roomRecord.push_back(this->getOneRoomState(bufferCurrent, bufferCurrent + objectSize));

        bufferCurrent += objectSize;
    }
#endif

    delete[] buffer;
    this->readStream.close();
}

int Administrator::encryption(std::string & __password)
{
    std::srand(std::time(nullptr));
    int offset = std::rand() % 10 + 1;

    std::transform(
                    __password.begin(), __password.end(), __password.begin(),
                    [&](unsigned char ch) { return ch + offset; }
                );

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
    NOTIFY_LOG("Please LOGIN: \n");

    AdministratorInfo tempAdmiInfo;
    AdministratorInfo decryptionInfo(this->decryption());

    while (true)
    {
        while (true)
        {
            NOTIFY_LOG("Your user name: ");
            std::getline(std::cin, tempAdmiInfo.userName);

            if (!tempAdmiInfo.userName.size())
            {
                ERROR_LOG("Empty user name! Please enter again!\n");
                continue;
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

    char confirm;
    std::string administratorName = decryption().userName;

    while (true)
    {
        system("cls");

        WARNING_LOG(
            "Administrator [" + administratorName + "].\n" + 
            "Are you make sure to logout? (Y/N) "
        );

        std::cin >> confirm;

        eatLine();

        if (confirm == 'Y') 
        { 
            CORRECT_LOG(
                '[' + getCurrentTime() + 
                "] Bye " + administratorName + " Have a good time!\n"
            ); 
            printSplitLine(45, '-');
            break;
        }
        else if (confirm == 'N') 
        { 
            NOTIFY_LOG("Back\n");
            return false; 
        }
        else 
        { 
            ERROR_LOG("Enter (Y/N)\n"); 
            delay(1500);
            continue; 
        }
    }

    return true;
}

void Administrator::addNewMeetingRoom(void)
{
    using namespace MyLib::MyLoger;

    Administrator::MettingRoomState tempRoomState;

    while (true)
    {
        tempRoomState.getStateInfo();

        if (
                this->searchOneRoomState(tempRoomState.mettingRoom.getMeetingRoomNo()) != 
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
}

std::vector<Administrator::MettingRoomState>::iterator 
Administrator::searchOneRoomState(const std::string & __roomNo)
{
    // 先构造出一个临时的会议室类
    MettingRoomState tempRoomState({{__roomNo, 0, false, ""}, false});

    return std::find(this->roomRecord.begin(), this->roomRecord.end(), tempRoomState);
}

void Administrator::getOneRoomState(void)
{
    using namespace MyLib::MyLoger;

    std::string roomNo;

    while (true)
    {
        NOTIFY_LOG("Enter meeting room number to search: ");
        std::getline(std::cin, roomNo);

        if (!roomNo.size())
        {
            ERROR_LOG("Empty room number, please enter again!\n");
            continue;
        }

        roomNo = roomNo.substr(0, 8);

        break;
    }

    auto targetIter = this->searchOneRoomState(roomNo);

    if (targetIter == this->roomRecord.end())
    {
        WARNING_LOG('[' + roomNo + "] are not in roomRecord.\n");
    }
    else { this->showOneRoomState(*targetIter); }
    
}

void Administrator::showAllRoomState(void)
{
    for (const MettingRoomState & __meettingState : this->roomRecord)
    {
        this->showOneRoomState(__meettingState);
    }
}