#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::stol;
using std::vector;

// Class cconstructor
Process::Process(int pid) : pid_(pid) {
    user_ = LinuxParser::User(pid_);
    cpuUsage_ = 1.0 * LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK) / LinuxParser::UpTime(pid_);
}

// Returning this process's ID
int Process::Pid() { return pid_; }

// Returning this process's CPU utilization
float Process::CpuUtilization() {
    return (cpuUsage_ >= 0.0) ? cpuUsage_ : 0.0;
}

// Returning the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Returning this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Returning the user (name) that generated this process
string Process::User() { return user_; }

// Returning the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Overloading the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
	return (cpuUsage_ > a.cpuUsage_) ? true : false;
}