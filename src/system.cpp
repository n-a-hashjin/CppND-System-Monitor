#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;

Processor& System::Cpu() { return cpu_; }

// container composed of the system's processes
vector<Process>& System::Processes() {
    processes_.clear();
    std::vector<int> pids = LinuxParser::Pids();
    for (auto& pid : pids)
    {
        if (!LinuxParser::Ram(pid).empty())
        {
            Process process(pid);
            processes_.emplace_back(process);
        }
    }
    std::sort(processes_.begin(), processes_.end(), [](const auto& _1st, const auto& _2nd){
        return _1st < _2nd;
    });
    return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }