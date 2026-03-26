#ifndef SAMPLE_H
#define SAMPLE_H

#include <thread>
#include <iostream>
#include <atomic>
#include "UtilsLog.h"
#include "UtilsEvent.h"

class sample
{
public:
    sample();
    ~sample();
    
    void start();
    void thread1Input();
    void thread2Data();

private:
    std::thread thread1;
    std::thread thread2;
    UtilsEvent event;
    std::atomic<bool> m_bExit;
};


#endif // SAMPLE_H
