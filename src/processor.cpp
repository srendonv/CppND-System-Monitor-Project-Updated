#include "processor.h"
#include "linux_parser.h"

#include <vector>

using std::vector, std::string;

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    auto active = LinuxParser::ActiveJiffies();
    auto idle = LinuxParser::IdleJiffies();

    auto deltaActive = active - prevActive;
    auto deltaIdle = idle - prevIdle;

    auto total = deltaActive + deltaIdle;

    prevActive = active;
    prevIdle = idle;

    float percentage = static_cast<float>(deltaActive)/total;
    return percentage;
}