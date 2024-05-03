#include "../include/copyOperator.h"

/**
 * @brief 检查那些无效的命令行参数输入，
 *        在 argc 为 1（没有参数）以及大于 3（参数过多）时，返回 false
 * 
 * @param __argCount    参数数量
 * 
 * @return 命令行参数数量是否合理
*/
inline bool checkInvalidArgc(const int __argCount) { return (__argCount == 1) || (__argCount > 3); };

int main(int argc, char const *argv[])
{
    system(clearCommand);

    FileCopyOperator FileCopyOp;

    if (checkInvalidArgc(argc)) { ERROR_LOG("Not or invalid argument!\n"); FileCopyOp.showHelp(); }

    else if ((std::strcmp(argv[1], "--help") == 0) && argc == 2) { FileCopyOp.showHelp(); }

    else if ((std::strcmp(argv[1], "--version") == 0) && argc == 2) { FileCopyOp.showVersion(); }

    else if (argc == 3) 
    {   
        int64_t costTime = TIMER( FileCopyOp.copyFile(argv[1], argv[2]) );

        if (costTime >= 1500) { ERROR_LOG("Copy cost [" + std::to_string(costTime) + "] ms.\n"); }
        else
        {
            CORRECT_LOG("Copy cost [" + std::to_string(costTime) + "] ms.\n");
        }

        printSplitLine(45, '-');
    }
    else 
    {
        ERROR_LOG("Invalid argument!\n"); 
        FileCopyOp.showHelp();
    }

    DONE
    return EXIT_SUCCESS;
}
