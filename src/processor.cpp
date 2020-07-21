#include <vector>
#include "processor.h"
#include "linux_parser.h"


using std::stol;
using std::vector;

float Processor::Utilization() {
  long active = LinuxParser::ActiveJiffies();
  long total = LinuxParser::Jiffies();

  float actived = active - PrevActive_;
  float totald = total - PrevTotal_;

  PrevActive_ = active;
  PrevTotal_ = total;
  
  // return CPU_Percentage
  return 1.0 * actived/totald;
}