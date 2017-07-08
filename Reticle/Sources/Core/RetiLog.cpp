#include <ctime>

#include <Core/RetiLog.h>
#include <RetiRenderer.h>

using namespace std;

RetiLog RetiLog::single_instance;

RetiLog::RetiLog()
{
    log_stream = new ofstream();
}

RetiLog::~RetiLog()
{
    RetiLog::logln("Log Closed");
    log_lock.acquire();
    log_stream->close();
    if(log_stream != nullptr)
        delete log_stream;
    log_lock.release();
}

void write_time(ostream* out_stream, RetiSpinlock* out_lock)
{
    char buffer[128];
    time_t timenow;
    struct tm* ftime;
    time(&timenow);
    ftime = localtime(&timenow);
    strftime(buffer, 128, "%d-%m-%Y %H:%M:%S", ftime);
    if(out_lock != nullptr)
        out_lock->acquire();
    (*out_stream) << "[" << buffer << "]    ";
}

void RetiLog::initLog()
{
    single_instance.log_stream->open(RetiRenderer::getReticleRootDirectory() + "/ReticleLog.log");
    if(!single_instance.log_stream->is_open())
        cout << "COULD NOT OPEN LOG FILE! Logs will be written to cout. Log file path: "
                << RetiRenderer::getReticleRootDirectory() + "/ReticleLog.log" << endl;
    RetiLog::logln("Log Opened");
}

void RetiLog::logln(string line)
{
    if(!single_instance.log_stream->is_open())
    {
        write_time(&cout, nullptr);
        cout << line << endl;
        return;
    }
    write_time(single_instance.log_stream, &single_instance.log_lock);
    (*single_instance.log_stream) << line << endl;
    single_instance.log_lock.release();
}
