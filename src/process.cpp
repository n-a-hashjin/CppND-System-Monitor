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

Process::Process(int pid) : pid_(pid) {
    cpuUsage_ = 1.0 * LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK) / LinuxParser::UpTime(pid_);
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
    return (cpuUsage_ >= 0.0) ? cpuUsage_ : 0.0;
}

string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
	return (cpuUsage_ > a.cpuUsage_) ? true : false;
}