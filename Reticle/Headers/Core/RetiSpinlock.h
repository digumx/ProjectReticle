#ifndef RETISPINLOCK_H_INCLUDED
#define RETISPINLOCK_H_INCLUDED

#include <thread>
#include <atomic>
#ifdef PLATFORM_LINUX
#include <pthread.h>
#endif // PLATFORM_LINUX

class RetiSpinlock
{
private:
    std::atomic_flag flag;

public:

    RetiSpinlock();
    RetiSpinlock(const RetiSpinlock& other) = delete;
    RetiSpinlock& operator=(const RetiSpinlock& other) = delete;

    void acquire();
    void release();
};

#endif // RETISPINLOCK_H_INCLUDED
