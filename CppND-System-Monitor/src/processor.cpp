#include "processor.h"
#include "linux_parser.h"

Processor::Processor(){
    curr_active = LinuxParser::ActiveJiffies();
    curr_idle   = LinuxParser::IdleJiffies();
}
// TODO: Return the aggregate CPU utilization
float Processor::Utilization(){
    float old_idle   = curr_idle;
    float old_active = curr_active;

    curr_active = LinuxParser::ActiveJiffies();
    curr_idle   = LinuxParser::IdleJiffies();

    float prev_total = old_idle + old_active;
    float curr_total = curr_idle+curr_active;

    float totald = curr_total - prev_total;
    float idled  = curr_idle - old_idle;

    float cpu_util = (totald - idled)/totald;
    return (cpu_util>0.0?cpu_util:0.0);
}