#include "processor.h"
#include "linux_parser.h"

#include <vector>

using std::vector, std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<string> prev = LinuxParser::CpuUtilization();
    return 0.5;
}