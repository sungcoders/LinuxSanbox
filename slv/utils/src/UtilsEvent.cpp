#include "UtilsEvent.h"

UtilsEvent::UtilsEvent()
: epoll_fd(-1)
, event_fd(-1)
, ev()
{
    setupEpoll();
}

UtilsEvent::~UtilsEvent()
{
    close(epoll_fd);
}

void UtilsEvent::setupEpoll()
{
    epoll_fd = epoll_create1(0);
    event_fd = eventfd(0, EFD_NONBLOCK);

    ev.events = EPOLLIN;
    ev.data.fd = event_fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event_fd, &ev);
    fcntl(epoll_fd, F_SETFL, O_NONBLOCK);
}

void UtilsEvent::triggerEvent()
{
    uint64_t dummy = 1;
    write(event_fd, &dummy, sizeof(dummy));
}

void UtilsEvent::clearEvent()
{
    uint64_t dummy;
    ssize_t n = read(event_fd, &dummy, sizeof(dummy));
    if(n == -1)
    {
        std::cout << "clean event epoll failed!" << std::endl;
    }
}

void UtilsEvent::waitEvent(int timeoutms)
{
    int nfds = epoll_wait(epoll_fd, events, 1, timeoutms); 
    for (int i = 0; i < nfds; i++)
    {
        int fd = events[i].data.fd;
        if (fd == event_fd)
        {
            clearEvent();
        }
    }
}
