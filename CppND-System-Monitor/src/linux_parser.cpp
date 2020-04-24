#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "utility.h"
#include "format.h"

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel>>kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  Utility* u = new ProcessHandler();
  std::set<string> reqs{"MemTotal:","MemFree:"};
  u->read(kProcDirectory+kMeminfoFilename,reqs);
  float memTotal = std::stof(u->getByKey("MemTotal:"));
  float memFree  = std::stof(u->getByKey("MemFree:"));
  //std::cout<<memTotal<<" and  "<<memFree<<std::endl;
  delete u;
  u = nullptr;
  return (memTotal-memFree)/memTotal; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream opener(kProcDirectory+kUptimeFilename);
  string key,line;
  if(opener.is_open()){
    std::getline(opener,line);
    std::istringstream reader(line);
    reader>>key;
  }
  //std::cout<<"UpTime is  "<<key<<std::endl;
  return std::stoi(key,nullptr,10);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  JiffiesG * u = new JiffiesG();
  u->read(kProcDirectory+kStatFilename);
  long ans= u->getTotal(false,false);
  delete u; 
  u = nullptr;
  return ans;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function

long LinuxParser::ActiveJiffies(int pid) { 
	string path = kProcDirectory + std::to_string(pid) + kStatFilename;
  	JiffiesG * u = new JiffiesG();
  	long a_c = u->getActiveJiffies(path);
  	delete u;
    u = nullptr;
  return a_c;
}


// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  JiffiesG * u = new JiffiesG();
  u->read(kProcDirectory+kStatFilename);
  long ans= u->getTotal(true,false);
  delete u;
  u = nullptr;
  return ans; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  JiffiesG * u = new JiffiesG();
  u->read(kProcDirectory+kStatFilename);
  long ans= u->getTotal(false,true);
  delete u;
  u = nullptr;
  return ans; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  long old_active_f = ActiveJiffies();
  long old_idle_f = IdleJiffies();
  sleep(10);
  long curr_active_f = ActiveJiffies();
  long curr_idle_f = IdleJiffies();

  string proper_time = Format::ElapsedTime(((curr_active_f-old_active_f)-(curr_idle_f-old_idle_f)/(curr_active_f-old_active_f)));
  std::vector<string> vres;
  vres.push_back(proper_time);
  return vres;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  Utility* u = new ProcessHandler();
  u->read(kProcDirectory+kStatFilename,"processes");
  int total = u->to_int(u->getByKey("processes"));
  //std::cout<<"Total is : "<<total<<std::endl;
  delete u;
  u = nullptr;
  return total; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  Utility* u = new ProcessHandler();
  u->read(kProcDirectory+kStatFilename,"procs_running");
  int total = u->to_int(u->getByKey("procs_running"));
  //std::cout<<"Total is : "<<total<<std::endl;
  delete u;
  u=nullptr;
  return total;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid){
  string path = kProcDirectory+to_string(pid)+kCmdlineFilename;
  string line;
  std::ifstream opener(path);
  if(opener.is_open()){
    std::getline(opener,line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid){
  Utility* u = new ProcessHandler();
  u->read(kProcDirectory+to_string(pid)+kStatusFilename,"VmData:");
  float rkb = std::stof(u->getByKey("VmData:"));
  int temp = ((100*rkb)/1024+0.5);
  return to_string(temp/100)+"."+((temp%100 <10) ? "0"+to_string(temp%100):to_string(temp%100)); 
}
// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid){
  Utility* u = new ProcessHandler();
  u->read(kProcDirectory+to_string(pid)+kStatusFilename,"Uid:");
  string ans = u->getByKey("Uid:");
  delete u;
  u = nullptr;
  return ans;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid){
  string path = kPasswordPath,line,final_tk = "x:"+to_string(pid);
  std::ifstream opener(path);
  while(std::getline(opener,line)){
      auto pos = line.find(final_tk);
      if(pos != string::npos){
        return line.substr(0,pos-1);
      }
  }
  return "root";
}
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string path=kProcDirectory+to_string(pid)+kStatFilename,line,key;
  std::ifstream opener(path);
  if(opener.is_open()){
    std::getline(opener,line);
    std::istringstream reader(line);
    for(int i=0;i<14;i++){
      reader>>key;
    }
    return std::stol(key);
  }
  return 0;
}
