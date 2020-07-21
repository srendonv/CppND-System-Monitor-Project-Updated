#include "processor.h"

#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;

// Return the aggregate CPU utilization
float Processor::Utilization() {
  auto active = LinuxParser::ActiveJiffies();
  auto idle = LinuxParser::IdleJiffies();

  auto deltaActive = active - prevActive;
  auto deltaIdle = idle - prevIdle;

  auto total = deltaActive + deltaIdle;

  prevActive = active;
  prevIdle = idle;

  float percentage = static_cast<float>(deltaActive) / total;
  return percentage;
}