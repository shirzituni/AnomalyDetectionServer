// shir zituni	316537992

#include "timeseries.h"
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream> 
using namespace std;

TimeSeries::TimeSeries(const char* CSVfileName){
    
	int col_num = 0;
	//open the file
	ifstream file(CSVfileName);
    string colname, line;
	string curr_num;
	vector<string> keys;
	vector<float> temp;

    getline(file, line);
    stringstream stream_names(line);
	while(stream_names.good()) {
		getline(stream_names, colname, ',');
    	this->table.insert(pair<string,vector<float>> (colname, temp));
		col_num++;
		keys.push_back(colname);
    }

	vector<vector<float>> values(col_num);

	while(getline(file, line)) {
		stringstream new_line(line);
		for(int i = 0; i < col_num; i++) {
			getline(new_line, curr_num, ',');
			values[i].push_back(stod(curr_num));
		}
	}

	for(int i = 0; i<col_num; i++){
		table[keys[i]] = values[i];
	}
	this->sizeOfMap = col_num;
}