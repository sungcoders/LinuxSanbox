#include "UtilsSharemem.h"

UtilsSharemem::UtilsSharemem()
{
}

UtilsSharemem::~UtilsSharemem()
{
    if(ptr)
    {
        munmap(ptr, SIZE);
    }

    if(shm_fd != -1)
    {
        close(shm_fd);
    }
}

void UtilsSharemem::init(std::string shmName)
{
    name = shmName;
    // 1. Tạo vùng nhớ dùng chung
    shm_fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
    if(shm_fd == -1)
    {
        perror("shm_open");
        return;
    }
    // 2. Thiết lập kích thước cho vùng nhớ
    if(ftruncate(shm_fd, SIZE) == -1)
    {
        perror("ftruncate");
        return;
    }
    // 3. Ánh xạ (map) vùng nhớ vào không gian của tiến trình
    ptr = mmap(nullptr, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    if(ptr == MAP_FAILED)
    {
        ptr = nullptr;
        perror("mmap failed");
    }
}

void UtilsSharemem::writeData(std::string msg)
{
    // 4. Ghi dữ liệu vào vùng nhớ
    size_t len = std::min(msg.size(), (size_t)SIZE - 1);    //limit size
    memcpy(ptr, msg.c_str(), len);                          // copy char
    ((char*)ptr)[len] = '\0';                               // add end of char

    memcpy(ptr, msg.c_str(), msg.size() + 1);
    std::cout << "Writer: done." << std::endl;
}

void UtilsSharemem::readData()
{
    // void* ptrRead = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if(!ptr)
    {
        std::cout << "null ptr"<< std::endl;
        return;
    }
    std::cout << "Reader đọc được: " << (char*)ptr << std::endl;
}

void UtilsSharemem::Final()
{
    // 5. Giải phóng và xóa
    munmap(ptr, SIZE);
    close(shm_fd);
    // shm_unlink(name.c_str()); // Xóa file ảo trong /dev/shm
}
