#ifndef RETILOG_H_INCLUDED
#define RETILOG_H_INCLUDED

#include <iostream>
#include <fstream>

#include <Core/RetiSpinlock.h>

class RetiLog
{
private:

    static RetiLog single_instance;
    std::ofstream* log_stream;
    RetiSpinlock log_lock;

    RetiLog();
    ~RetiLog();

public:
    RetiLog(const RetiLog& other) = delete;
    RetiLog& operator=(const RetiLog& other) = delete;

    static void initLog();
    static void logln(std::string line);

};

#endif // RETILOG_H_INCLUDED
