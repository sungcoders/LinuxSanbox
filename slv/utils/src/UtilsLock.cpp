#include "UtilsLock.h"

UtilsLock::UtilsLock()
{
    /**
     * para1: semaphore
     * para2: 0/1 thread/process
     * para3: 1 binary
    */
    if(sem_init(&sem, 0, 1) != 0)
    {
        throw std::runtime_error("sem_init failed");
    }
}

UtilsLock::~UtilsLock()
{
    sem_destroy(&sem);
}

void UtilsLock::lock()
{
    sem_wait(&sem);
}


void UtilsLock::unlock()
{
    sem_post(&sem);
}

/* ----------------------------- auto lock ---------------------------*/

UtilsLockAuto::UtilsLockAuto(UtilsLock& sem)
: _sem(sem)
{
    _sem.lock();
}

UtilsLockAuto::~UtilsLockAuto()
{
    // unlock out of scope
    _sem.unlock();
}
