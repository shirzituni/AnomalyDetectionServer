#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <map>
#include <vector>
#include <fstream>
#include <sstream> 
#include <string>
#include <iostream>

using namespace std;

class TimeSeries{
private: 
	map<string, vector<float>> table;
	int sizeOfMap = 0;
	
public:
	TimeSeries(const char* CSVfileName);
	int getColSize() const{ return this->sizeOfMap; }
	map<string, vector<float>> getMap() const{ return this->table; }
};



#endif /* TIMESERIES_H_ */
