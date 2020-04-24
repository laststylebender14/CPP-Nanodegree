#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
   long hr = seconds/3600;
   long rem_h = seconds%3600;
   long min_t = rem_h/60;
   long ss = rem_h%60;
   string result = std::to_string(hr)+":"+std::to_string(min_t)+":"+std::to_string(ss);
return result; 
}