#ifndef UTILS_LOCK_H
#define UTILS_LOCK_H

#include <semaphore.h>
#include <stdexcept>

class UtilsLock
{
public:
    UtilsLock();
    ~UtilsLock();

    void lock();
    void unlock();    

    UtilsLock(const UtilsLock&) = delete;               // no copy constructor
    UtilsLock& operator=(const UtilsLock&) = delete;    // no copy assignment

    UtilsLock(UtilsLock&&) = delete;                    // no move constructor
    UtilsLock& operator=(UtilsLock&&) = delete;         // no move assignment
    /**
     * fobiden
     * Copy constructor: UtilsLock b(a); or UtilsLock c = a;
     * Copy assignment:  b = a;
     * Move constructor: UtilsLock b(std::move(a)); or UtilsLock e = std::move(a);
     * Move assignment:  b = std::move(a);
    */

private:
    sem_t sem;
};



class UtilsLockAuto
{
public:
    UtilsLockAuto(UtilsLock& sem);
    ~UtilsLockAuto();

    UtilsLockAuto(const UtilsLockAuto&) = delete;               // no move constructor
    UtilsLockAuto& operator=(const UtilsLockAuto&) = delete;    // no move assignment

    UtilsLockAuto(UtilsLockAuto&&) = delete;
    UtilsLockAuto& operator=(UtilsLockAuto&&) = delete;

private:
    UtilsLock& _sem;
};


#endif // UTILS_LOCK_H
