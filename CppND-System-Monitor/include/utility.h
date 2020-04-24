#ifndef UTILITY_H
#define UTILITY_H

#include<iostream>
#include<string>
#include<vector>
#include<set>
#include<map>
#include<sstream>
#include<fstream>
#include<numeric>

class ProcessHandler;
class JiffiesG;

class Utility{
	protected:
  		std::map<std::string,std::string> collect;
  	public:
	  	virtual void read(std::string);
  		virtual void read(std::string path,std::string key );
  		virtual void read(std::string path,std::set<std::string> req);
  		virtual int to_int(std::string i);
		virtual std::string getByKey(std::string key)=0;

  		~Utility();
};

class ProcessHandler : public Utility{
	public:
  		std::string getByKey(std::string key);
};

class JiffiesG : public Utility{
		std::vector<int> values;
	public:
		void read(std::string);
		std::string getByKey(std::string key);
  		long getActiveJiffies(std::string path);
		long getTotal(bool is_active ,bool is_idle);
		~JiffiesG();
};

#endif