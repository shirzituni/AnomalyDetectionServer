// shir zituni	316537992

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <iterator>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include "timeseries.h"

#include "HybridAnomalyDetector.h"

using namespace std;
static vector<AnomalyReport> vec;
static int sizeOfTs;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

};

class Command{
	protected: DefaultIO* dio;
	public: string description;
public:
	Command(DefaultIO* dio):dio(dio){
	}
	virtual void execute()=0;
	virtual ~Command(){}
};

// implement here your command classes
class Command1: public Command{
public:
	Command1(DefaultIO* dio):Command(dio){
		description = "1.upload a time series csv file\n";
	}
	void readFile(string path){
		string line;
		// Create an output filestream object
    	ofstream out_file(path);
		do{
			line = dio->read();
			if(line == "done")
				break;
			out_file << line + '\n';	
		}while(true);
		//out_file.close();			
	}
	void execute(){
		dio->write("Please upload your local train CSV file.\n");
		readFile("anomalyTrain.csv");
		dio->write("Upload complete.\n");

		dio->write("Please upload your local test CSV file.\n");
		readFile("anomalyTest.csv");
		dio->write("Upload complete.\n");

	}
};

class Command2: public Command{
public:	
	Command2(DefaultIO* dio):Command(dio){
		description = "2.algorithm settings\n";
	}
	void execute(){
		bool is_legal = false;
		string r;
		float num;
		while(!is_legal){
			r= dio->read();
			num=stof(r);
			this->dio->write("The current correlation threshold is 0.9\n");
			this->dio->write("Type a new threshold\n");
			if(num > 0 && num < 1 ){
				is_legal = true;
				SimpleAnomalyDetector ad;
				ad.setThreshold(num);
			}
			//illigal threshold
			else{
				this->dio->write("please choose a value between 0 and 1.\n");
			}
		}	
	}	

};

class Command3: public Command{
public:	
	Command3(DefaultIO* dio):Command(dio){
		description = "3.detect anomalies\n";
	}
	void execute(){
		TimeSeries ts_train("anomalyTrain.csv");
		TimeSeries ts_test("anomalyTest.csv");
		sizeOfTs = ts_test.getMap().begin()->second.size();
		HybridAnomalyDetector ad;
		ad.learnNormal(ts_train);
		vec= ad.detect(ts_test);
		this->dio->write("anomaly detection complete.\n");
	}
};

class Command4: public Command{
public:	
	Command4(DefaultIO* dio):Command(dio){
		description = "4.display results\n";
	}
	void execute(){
		string line;
		//print detects
		for(int i = 0; i< vec.size(); i++){
			line =to_string(vec[i].timeStep) +" 	"+ vec[i].description;
			dio->write(line+'\n');
		}
		dio->write("Done.\n");

	}

};

class Command5: public Command{
protected: vector<int> splites_value;

public:	
Command5(DefaultIO* dio):Command(dio){
		description = "5.upload anomalies and analyze results\n";
	}
bool split(const string &s, char delimiter) {   
	int temp_int = 0;
	string token;     
	istringstream tokenStream(s);    
	bool flag = false; 
	// get 2 new values (start,end)
	getline(tokenStream, token, delimiter);
	int new_start = stoi(token);
	getline(tokenStream, token, delimiter);
	int new_end = stoi(token);
	// check if the vector is empty
	if(splites_value.size() == 0){
		splites_value.push_back(new_start);
		splites_value.push_back(new_end);
		return flag;
	}
	else{
		for(int i=0; i<splites_value.size(); i++){
			// if the new (start,end) exsist in the vector
			if(splites_value[i]==new_start && splites_value[i+1]==new_end){
				flag = true;
				break;
			}
			i++;
		}
		// if its a new couple
		if(flag == false){
			splites_value.push_back(new_start);
			splites_value.push_back(new_end);				
		}	
	}
	return flag;
}
int readFile(){
	int P = 0;
	string line;
	bool flag = false;
	do{
		line = dio->read();
		if(line == "done")
			break;
		flag = split(line, ','); 
		if(!flag){	
			P++;
		}
	}while(true);
	return P;
}
void execute(){
	splites_value.clear();
	dio->write("Please upload your local anomalies file.\n");
	//union detect
	int j = 0;
	vector<long> resultsStart;
	vector<long> resultsEnd;
	long startTime = 0, endTime = 0;
	string curr_discrip;
	for(int i = 0; i < vec.size(); i++){
		j = i + 1;
		curr_discrip = vec[i].description;
		startTime = vec[i].timeStep;
		endTime = vec[i].timeStep;
		while(curr_discrip.compare(vec[j].description) == 0) {
			endTime = vec[j].timeStep;
			j++;
		}
		resultsStart.push_back(startTime);
		resultsEnd.push_back(endTime);
		i = j-1;
		j=0;
	}
	//read from file
	int P = readFile();
	//calculate Minus values 
	vector<int> minus_valeus;
	for(int i=0; i<splites_value.size(); i++){
		minus_valeus.push_back(splites_value[i+1]-splites_value[i]+1);
		i++;
	}
	int N = sizeOfTs;
	for(int i=0; i < minus_valeus.size(); i++){
		N -= minus_valeus[i];
	}
	dio->write("Upload complete.\n");
	//calculate TP, FP;
	float TP = 0, FP = 0;
	bool flag = false; // Is there Overlap?!
	
	for(int i=0; i<resultsStart.size(); i++){
		flag = false;
		for(int j = 0; j<splites_value.size(); j++){
			if((splites_value[j+1] < resultsStart[i] || splites_value[j] > resultsEnd[i]) && flag == false){
				j++;
				continue;
			}
			else	
				flag = true; //Overlap found!!
		}
		if(flag) //There is Overlap 
			TP++;
		else
			FP++;
	}
	float print1 =floor(TP/(float)P * 1000)/1000;
	float print2 =floor(FP/(float)N * 1000)/1000;
	dio->write("True Positive Rate: ");
	dio->write(print1);
	dio->write("\nFalse Positive Rate: ");
	dio->write(print2);
	dio->write("\n");
	}
};

class Command6: public Command{
public:	
Command6(DefaultIO* dio):Command(dio){
		description = "6.exit\n";
	}
	void execute(){
	}

};

class socketIO:public DefaultIO{
	int socket;
public:
	socketIO(int id): socket(id){}
	//read()
	virtual string read(){
		string clientInput="";
		char c=0;
		::read(socket,&c,sizeof(char));
		while(c!='\n'){
			clientInput+=c;
			::read(socket,&c,sizeof(char));
		}
		return clientInput;
	}
	
	//write(string text)
	virtual void write(string text){
		::write(socket,text.c_str(),text.length());
	}
	virtual void write(float f){
		std::ostringstream s;
		s << f;
		::write(socket, s.str().c_str(), s.str().length());
	}
	// read(float* f)
	virtual void read(float* f){
		string clientInput="";
		char c=0;
		::read(socket,&c,sizeof(char));
		while (c!='\n'){
			clientInput+=c;
			::read(socket,&c,sizeof(char));
		}
		stringstream s(clientInput);
		s >> *f;
	}
	virtual ~socketIO(){
	}
};

#endif /* COMMANDS_H_ */
