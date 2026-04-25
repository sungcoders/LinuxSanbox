#ifndef UTILS_SHAREMEM_H
#define UTILS_SHAREMEM_H

#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

class UtilsSharemem
{
public:
    UtilsSharemem();
    ~UtilsSharemem();

    void init(std::string shmName);
    void writeData(std::string msg);
    void readData();
    void Final();

private:
    std::string name;   // Tên vùng nhớ
    int SIZE = 4096;    // Kích thước 4KB
    int shm_fd = -1;
    void* ptr = nullptr;
};

#endif // UTILS_SHAREMEM_H
