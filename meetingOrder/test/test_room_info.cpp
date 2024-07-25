#include "../include/meetingRoom.h"

#include <vector>


int main(int argc, char const *argv[])
{
    using namespace MyLib::MyDelay;
    using namespace MyLib::MyLoger;

    std::vector<Meeting_Room> testMettingRoom;

    testMettingRoom.push_back({"00000001", 128, true, "Test metting room A."});
    testMettingRoom[0].writeToFile();
    testMettingRoom.push_back({"00000002", 32, true, "Test metting room B."});
    testMettingRoom[1].writeToFile();
    testMettingRoom.push_back({"00000003", 45, true, "Test metting room C."});
    testMettingRoom[2].writeToFile();
    testMettingRoom.push_back({"00000004", 16, true, "Test metting room D."});
    testMettingRoom[3].writeToFile();
    testMettingRoom.push_back({"00000005", 128, true, "Test metting room E."});
    testMettingRoom[4].writeToFile();

    std::ifstream readStream(
                        DATAFILE_PATH, 
                        std::ios_base::in | std::ios_base::binary
        );

    std::size_t seekBytes = testMettingRoom[0].getObjectBytes() + 
                            testMettingRoom[1].getObjectBytes();

    std::size_t readBytes = testMettingRoom[3].getObjectBytes();

    readStream.seekg(seekBytes, std::ios_base::beg);

    char * buffer = new char[readBytes];
    readStream.read(buffer, readBytes);

    std::size_t objectSize      = *reinterpret_cast<std::size_t *>(buffer);
    std::size_t roomNoLen       = *reinterpret_cast<std::size_t *>(buffer + 8);
    std::size_t accomodateCount = *reinterpret_cast<std::size_t *>(buffer + 16 + roomNoLen);

    NOTIFY_LOG("Object size = " + std::to_string(objectSize) + '\n');
    NOTIFY_LOG("Room No. Length = " + std::to_string(roomNoLen) + '\n');
    NOTIFY_LOG(
                "Room No. " + 
                std::string(buffer + sizeof(std::size_t) * 2, buffer + sizeof(std::size_t) * 2 + roomNoLen) + '\n'
            );
    NOTIFY_LOG("Accomodate count = " + std::to_string(accomodateCount) + '\n');

    delete[] buffer;
    readStream.close();
    
    return EXIT_SUCCESS;
}
