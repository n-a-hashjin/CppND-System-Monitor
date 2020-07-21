#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"


using std::string;
using std::to_string;
using std::stol;
using std::vector;



string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  float MemTotal, MemFree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "MemTotal")
          MemTotal = std::stof(value);
        if (key == "MemFree")
          MemFree = std::stof(value);
      }
    }
    
  }
  return 1-MemFree/MemTotal;
}

long LinuxParser::UpTime() {
  string line;
  string UptimeStr;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  std::getline(stream, line);
  std::istringstream linestream(line);
  linestream >> UptimeStr;
  return stol(UptimeStr);
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> CpuInfo = LinuxParser::CpuUtilization();
  long total{0};
  for (int k = kUser_; k <= kSteal_; k++)
  {
    total += stol(CpuInfo[k]);
  }
  return total;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  int i{1};
  string line, token;
  long active = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> token)
    {
      if (i == kUtime_ || i == kStime_ || i == kCutime_)
      {
        active += stol(token);
      } else if (i == kCstime_)
      {
        active += stol(token);
        break;
      }
      i++;
    }
  }
  return active;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> CpuInfo = LinuxParser::CpuUtilization();
  return stol(CpuInfo[kIdle_]) + stol(CpuInfo[kIOwait_]);
}

vector<string> LinuxParser::CpuUtilization() {
  vector<string> CpuInfo;
  string line, token;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> token)
    {
      if (token == "cpu")
      {
        continue;
      }
      CpuInfo.push_back(token);
    }
  }
  return CpuInfo;
}

int LinuxParser::TotalProcesses() {
  string line;
  string key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "processes")
        {
          return stol(value);
        }
        
      }
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses() {
  string line;
  string key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "procs_running")
        {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid[[maybe_unused]]) {
  string line, token;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> token;
  }
  return token;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key, token, value{};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> token;
      if (key == "VmSize:")
      {
        try
        {
          value = to_string(stol(token)/1000);
          break;
        }
        catch(const std::exception& e)
        {
          //std::cerr << e.what() << '\n';
        }
      } 
    }
  }
  
  return value;
}

string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open())
  {
    while (std::getline(stream,line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "Uid")
        {
          return value;
        }
      }
    }
  }
  return string();
}

string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string line, token1, token2, token3;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> token1 >> token2 >> token3)
      {
        if (token2=="x" && token3==uid)
        {
          return token1;
        } 
      }
    }
  }
  return string();
}

long LinuxParser::UpTime(int pid) {
  int k = 0;
  bool killed = false; 
  long Starttime = 0;
  long ProcessUpTime = 0;
  string line, token;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> token)
    {
      if (k == kStartTime_)
      {
        Starttime = stol(token);
        break;
      }
      k++;
    }
  }
  else killed = true;
  ProcessUpTime = LinuxParser::UpTime() - (long)(Starttime/sysconf(_SC_CLK_TCK));
  return killed ? 0 : ProcessUpTime;
}