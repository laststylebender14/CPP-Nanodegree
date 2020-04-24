#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  Processor();

  // TODO: Declare any necessary private members
 private:
    int curr_active = 0,curr_idle = 0;
};

#endif