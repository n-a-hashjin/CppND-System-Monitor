#include "log.h"

Log::Log()
{
    logfile_.open("log.txt");
    logfile_ << "***Constructor Called***" << "\n";
    logfile_.close();
}

Log::~Log()
{
    logfile_.open("log.txt", std::ios_base::app);
    for (auto logtext : logvector_)
    {
        logfile_ << logtext << "\n";
    }
    logfile_ << "System Monitor Ended, destructor is called" << "\n";
    logfile_.close();
}
// record and write to log.text from object.
void Log::writetologfile(const std::string& logtext)
{
    logvector_.emplace_back(logtext);
    counter_++;
    if (counter_ == 20)
    {
        logfile_.open("log.txt", std::ios_base::app);
        for (auto writetext : logvector_)
        {
            logfile_ << logtext << "\n";
        }
        logfile_.close();
        counter_ = 0;
    }
    return;
}

// static member, call to log in log.txt file a string
void Log::writetologfile(const std::string&& logtext)
{
    std::ofstream logfile;
    logfile.open("log.txt", std::ios_base::app);
    logfile << logtext << "\n";
    logfile.close();
}