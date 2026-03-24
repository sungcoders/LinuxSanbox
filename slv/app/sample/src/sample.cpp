#include "sample.h"

sample::sample()
{
}

sample::~sample()
{
}

void sample::printText()
{
    std::cout << "This is sample source!" << std::endl;
    LOGD("This is a debug message: {}", 42);
}
