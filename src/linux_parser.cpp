#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key;
  float mTotal{1}, mFree{0}, mBuffers{0}, mCache{0};
  float mAvailable;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> key){
        if("MemTotal:" == key){
          linestream >> mTotal;
        }else if("MemFree:" == key){
          linestream >> mFree;
        }else if("Buffers:" == key){
          linestream >> mBuffers;
        }else if("Cached:" == key){
          linestream >> mCache;
        }
      }
    }
  }
  mAvailable = mTotal - (mFree + mBuffers + mCache);
  return mAvailable/mTotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime(0);
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  auto totalJiffies = LinuxParser::UpTime() * sysconf(_SC_CLK_TCK);
  return totalJiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> time = LinuxParser::CpuUtilization();
  long active(0);
  for (int i =CPUStates::kUser_; i != CPUStates::Last_; i++){
    if(i == CPUStates::kIdle_ || i == CPUStates::kIOwait_) continue;
    active+= std::stol(time[i]);
  }
  return active;
} 

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> time = LinuxParser::CpuUtilization();
  long idle = std::stol(time[CPUStates::kIdle_]) + std::stol(time[CPUStates::kIOwait_]);
  return idle;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, time;
  vector<string> utilization;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> time;
    if("cpu" == time){
      while (linestream >> time){
        utilization.push_back(time);
      }
    }
  }
  return utilization;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int total(0);
  string line, key;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if("processes" == key) linestream >> total;
    }
  }
  return total;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int running(0);
  string line, key;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if("procs_running" == key) linestream >> running;
    }
  }
  return running;
}

// Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line{""};
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    getline(stream, line);
    return line;
  }
  return line;
}

// Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid ) {
  string line{""}, key;
  int memMB;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if("VmSize:" == key){
        linestream >> memMB;
        return std::to_string(memMB/1024);
      }
    }
  }
  return "";
}

// Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, uid{""};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if("Uid:" == key){
        linestream >> uid;
        return uid;
      }
    }
  }
return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, pattern;
  pattern = ":x:"+ Uid(pid);
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(getline(stream, line)){
      std::size_t found = line.find(pattern);
      if(found != std::string::npos){
        return line.substr(0, found);
      }
    }
  }
  return "None";
}

// Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) {
  vector<string> times;
  string value;
  int uptimeIndex = 22;
  long time;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    while(stream >> value){
      times.push_back(value);
    }
  }
  time = stol(times[uptimeIndex])/sysconf(_SC_CLK_TCK);
  return time;
}