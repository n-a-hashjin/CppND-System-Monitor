#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Log
{
private:
    std::ofstream logfile_;
    long counter_{0};
    std::vector<std::string> logvector_;
public:
    Log();
    ~Log();
    void writetologfile(const std::string& logtext);
    static void writetologfile(const std::string&& logtext);
};

#endif