#include "sample.h"

sample::sample()
: m_bExit(false)
{
}

sample::~sample()
{
}

void sample::start()
{
    thread1 = std::thread(&sample::thread1Input, this);
    thread2 = std::thread(&sample::thread2Data, this);
    LOGD("Started thread1 and thread2");
    thread1.join();
    thread2.join();
}

void sample::thread1Input()
{
    std::string s;
    while (!m_bExit.load())
    {
        std::getline(std::cin, s);
        if(s == "")
        {
            continue;
        }
        else if(s == "trigger")
        {
            LOGI("trigger event");
            event.triggerEvent();
        }
        else if(s == "clear")
        {
            LOGI("reset event");
            event.clearEvent();
        }
        else if(s == "exit")
        {
            LOGI("Thoat");
            m_bExit.store(true);
            event.triggerEvent();
            return;
        }
        else
        {
            LOGI("input = {}", s);
            LOGI("errno = {}", errno);
        }
    }
    LOGI("data = {}", errno);
}

void sample::thread2Data()
{
    while (!m_bExit.load())
    {
        LOGW("before waiting");
        event.waitEvent();
        LOGW("after waiting");
    }
}