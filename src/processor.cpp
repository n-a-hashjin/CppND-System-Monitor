#include "processor.h"
#include "linux_parser.h"

using std::stol;

float Processor::Utilization() {
  vector<string> cpu = LinuxParser::CpuUtilization();
  string name, user, nice, system, idle, iowait, irq, softirq, steal;
  string line;
  float Idle, NonIdle, Total;
  while (!cpu.empty())
  {
    line = cpu.back();
    std::istringstream linestream(line);
    while(linestream >> name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal)
    {
      if (name == "cpu")
      {
        
        Idle = stof(idle) + stof(iowait);
        NonIdle = stof(user) + stof(nice) + stof(system) + stof(irq) + stof(softirq) + stof(steal);
        Total = Idle + NonIdle;
      }
    }
    cpu.pop_back();
  }

  float totald = Total - PrevTotal_;
  float idled = Idle - PrevIdle_;
  PrevIdle_ = Idle;
  PrevTotal_ = Total;

  // return CPU_Percentage
  return 1.0*(totald - idled)/totald;
}