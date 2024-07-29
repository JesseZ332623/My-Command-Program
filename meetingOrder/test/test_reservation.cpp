#include "../include/reservations.h"

#include "../include/defs.h"

int main(int argc, char const *argv[])
{
    Reservations::ReservationInfo testInfo {"Mike", "123456"};
    std::ofstream writeStream;

    testInfo.show();

    testInfo.writeReservationDataToFile(writeStream);

    testInfo.show();

    return EXIT_SUCCESS;
}
