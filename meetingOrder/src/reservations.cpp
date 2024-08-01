#include "../include/reservations.h"

void
Reservations::ReservationInfo::getInput(void)
{
    using namespace MyLib::MyLoger;

#if false
    NOTIFY_LOG("Enter your user name (8 chractor limit): ");
    std::getline(std::cin, this->name);

    this->name = this->name.substr(0, 8);
#endif

    NOTIFY_LOG("Hello XXX\nEnter your password: ");
    hiddenPassword(this->password);
    //std::getline(std::cin, this->password);
}

void 
Reservations::ReservationInfo::
writeReservationDataToFile(std::ofstream & __writeStream)
{
    __writeStream.open(RESERVATIONS_INFO_PATH, std::ios_base::out | std::ios_base::app);

    //__writeStream << this->name + '\n';

    std::string cipherText = cryption.encryption(this->password);

    __writeStream << cipherText;

    __writeStream.close();
}

void
Reservations::ReservationInfo::readDataToFill(std::ifstream & __readStream)
{
    using namespace MyLib::MyLoger;

    __readStream.open(RESERVATIONS_INFO_PATH, std::ios_base::in);

    //std::string accountInfo;
    std::string tempPassword;
    __readStream >> tempPassword;

    printf("%s\n", tempPassword);

    //this->name               = accountInfo.substr(0, 8);
    //std::string tempPassword = accountInfo.substr(9, accountInfo.size());

    this->password = cryption.decryption(tempPassword);

    __readStream.close();
}

void Reservations::loadAccountInfo(void)
{
    using std::ios_base;
    using namespace MyLib::MyLoger;

    this->readStream.open(RESERVATIONS_INFO_PATH);

    if (!getFileBytes(this->readStream))
    {
        this->readStream.close();

        NOTIFY_LOG(
            '[' + getCurrentTime() + "] Appointment person register: \n");
        this->reservationInfo.getInput();
        this->reservationInfo.writeReservationDataToFile(this->writeStream);
    }

    this->readStream.close();
}

Reservations::Reservations(void) : currentReservationCount(0ULL)
{
    this->loadAccountInfo();
}

void Reservations::login(void)
{
    using namespace MyLib::MyLoger;

    ReservationInfo tempInfo;
    this->reservationInfo.readDataToFill(this->readStream);

    this->reservationInfo.show();

    while (true)
    {
        NOTIFY_LOG('[' + getCurrentTime() + "] Appointment person login: \n");
        tempInfo.getInput();

        tempInfo.show();

        if (tempInfo == this->reservationInfo)
        {
            CORRECT_LOG
            (
                '[' + getCurrentTime() + "] Login success! Welcome " + 
                this->reservationInfo.name + "!\n"
            );
            break;
        }
        else
        {
            ERROR_LOG("User name of password error.\n");
            tempInfo.clearInfo();
            continue;
        }
    }

    this->reservationInfo.clearInfo();
    tempInfo.clearInfo();
}