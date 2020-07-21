#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid() const;
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;  // See src/process.cpp
  bool operator>(Process const& a) const;  // See src/process.cpp

  // Declare any necessary private members
 private:
  int pid_;
};

#endif