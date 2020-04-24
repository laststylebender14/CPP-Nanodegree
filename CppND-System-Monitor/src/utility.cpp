
#include "utility.h"

Utility::~Utility(){
	collect.clear();
}

void Utility::read(std::string path){
}




void Utility::read(std::string path,std::string req){
	std::ifstream opener(path);
  	std::string line,key,value;	
  	if(opener.is_open()){
    	while(std::getline(opener,line)){
        	std::istringstream reader(line);
          	reader>>key>>value;
          	if(key == req){
            	collect[key] = value;
              	break;
            }
        }
    }
}

void Utility::read(std::string path,std::set<std::string> req){
	std::ifstream opener(path);
  	std::string line,key,value;	
  	if(opener.is_open()){
    	while(std::getline(opener,line)){
        	std::istringstream reader(line);
          	reader>>key>>value;
          	if(req.count(key)){
            	collect[key] = value;
            }
          	if(req.size() == collect.size()){
            	break;
            }
        }
    }
}

int Utility::to_int(std::string i){
	return stoi(i,nullptr,10);
}

std::string ProcessHandler::getByKey(std::string key){
	if(collect.count(key)){
    	return collect[key];
    }
  throw std::invalid_argument("Key is Not found  getByKey(key) !");
}

std::string JiffiesG::getByKey(std::string key){
	if(collect.count(key)){
		return collect[key];
	}
   throw std::invalid_argument("Key is Not found  Jiffies::getByKey(key) !");
}

void JiffiesG::read(std::string path){
	std::ifstream opener(path);
	std::string line,cpu,value;
	if(opener.is_open()){
		std::getline(opener,line);
		std::istringstream reader(line);
		reader>>cpu;
		if(cpu == "cpu"){
			while(reader>>value){
				values.push_back(to_int(value));
			}
		}
	}
}

JiffiesG::~JiffiesG(){
	values.clear();
}
//			 0 1 2 3
//4,5 - idle 1 2 3 4
long JiffiesG::getTotal(bool is_active,bool is_idle){
	if(is_active){
		long a = std::accumulate(values.begin(),values.begin()+3,0);
		long b = std::accumulate(values.begin()+6,values.end(),a);
		return b;
	}
	if(is_idle){
		return (long)values[3]+values[4];
	}
	return (long)std::accumulate(values.begin(),values.end(),0);
}

long JiffiesG::getActiveJiffies(std::string path){
	std::vector<std::string> ans;
	std::string line,value;
  	std::ifstream opener(path);
  	if(opener.is_open()){
    	std::getline(opener,line);
      	std::istringstream reader(line);
      	while(reader>>value){
        	ans.push_back(value);
        }
    }
  long aj = std::stol(ans[13])+std::stol(ans[14])+std::stol(ans[15])+std::stol(ans[16]);
  return aj;
}