#include "../include/reservations.h"

#include <random>
#include <cstring>

Reservations::ReservationInfo::Affine 
Reservations::ReservationInfo::encryption(void)
{
    Affine constants;
    std::random_device randDeviece;
    std::mt19937_64 randEngine(randDeviece());

    std::uniform_int_distribution<> distribute(114, 514);

    constants.a = distribute(randEngine);
    constants.b = distribute(randDeviece);

    printf("a = %d, b = %d\n", constants.a, constants.b);

    int passwordNum = std::stoi(this->password);

    this->password = std::to_string(std::abs(passwordNum * constants.a - constants.b));

    return constants;
}

void Reservations::ReservationInfo::
writeReservationDataToFile(std::ofstream & __writeStream)
{
    using std::ios_base;

    Affine constants = encryption();
    std::size_t passwordLen = this->password.size();

    /**
     * 明日继续
     */
    __writeStream.open(RESERVATIONS_INFO_PATH, ios_base::binary | ios_base::app);

    __writeStream.write(this->name.c_str(), 8);
    __writeStream.write(reinterpret_cast<char *>(&constants), sizeof(Affine));
    std::memset(&constants, 0, sizeof(Affine));

    __writeStream.write(reinterpret_cast<char *>(&passwordLen), sizeof(std::size_t));
    __writeStream.write(this->password.c_str(), passwordLen);

    this->clearInfo();

    __writeStream.close();

}