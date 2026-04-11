#ifndef UTILS_EVENT_H
#define UTILS_EVENT_H

#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <sys/eventfd.h>

class UtilsEvent
{
public:
    UtilsEvent();
    ~UtilsEvent();

    void triggerEvent();
    void clearEvent();
    void waitEvent(int timeoutms);

private:
    int epoll_fd;
    int event_fd;
    struct epoll_event ev;
    struct epoll_event events[1];

    void setupEpoll();
    
};

#endif // UTILS_EVENT_H
