#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  Process(int);
  int Pid();
  std::string User();
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_;
  float cpuUsage_;
};

#endif