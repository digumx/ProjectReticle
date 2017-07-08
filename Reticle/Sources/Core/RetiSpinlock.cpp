#include <Core/RetiSpinlock.h>


using namespace std;

RetiSpinlock::RetiSpinlock()
{
    flag.clear();
}

void RetiSpinlock::acquire()
{
    while(flag.test_and_set()) asm volatile("pause\n" : : : "memory");
}

void RetiSpinlock::release()
{
    flag.clear();
}
