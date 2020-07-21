#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Processor {
 public:
  float Utilization();
  
 private:
  float PrevTotal_ = 0;
  float PrevActive_ = 0;
};

#endif