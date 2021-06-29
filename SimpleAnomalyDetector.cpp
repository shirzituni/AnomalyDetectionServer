// shir zituni	316537992

#include "SimpleAnomalyDetector.h"

#include <iterator>
#include <string.h>
#include <vector>


SimpleAnomalyDetector::SimpleAnomalyDetector() {
	threshold = 0.9;

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void SimpleAnomalyDetector::setThreshold(float threshold) {
	this->threshold = threshold;
}
	

//build point** of points of (corr_feature1 , corr_feature2)
void SimpleAnomalyDetector::buildPointsFromFeatures(float* corr_feature1, float* corr_feature2, int num_of_rows, Point** points) {
	for(int i = 0; i < num_of_rows; i++){
		points[i] = new Point(corr_feature1[i] , corr_feature2[i]);
	}
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts)
{
	int size =  ts.getColSize();
	int num_of_rows, i=0;
	map<string, vector<float>> table = ts.getMap();
	vector <float*> arrayofvectors(size);
	float maxResult = 0;
	float result = 0;
	int corr_feature1 = 0;
	int corr_feature2 = 0;

	//create vector of float* from the map
	for (auto it=table.begin(); it!=table.end(); ++it){
		arrayofvectors[i] = &(it->second)[0];
		num_of_rows = it->second.size();
		i++;
	}
	
	vector<string> pairs;
	//initialaize
	string mem;
	bool is_found = false;
	for(int i = 0; i<size ; i++) {
		for(int j = 0; j<size ; j++){
			is_found = false;
			if(i==j){
				continue;
			}
			mem = to_string(i) + to_string(j);
			//chek if the new pair is already exist
			for (auto it = begin(pairs); it != end(pairs); it++){
				//MATCH!
				if((it->at(0) == mem.at(1)) && (it->at(1) == mem.at(0))){
					is_found = true;
					break;
				}
			}
			//if its a new match
			if(is_found == false){
				//save the new pair
				pairs.push_back(mem);
				result = pearson(arrayofvectors[i], arrayofvectors[j], num_of_rows);
				if(result < 0){
					result = result * (-1);
				}
				//check the result for pearson
				if (result > maxResult){
					//update the most corellated feature
					maxResult = result;
					if(i<j){
						corr_feature1 = i;
						corr_feature2 = j;
					}
					else{
						corr_feature1 = j;
						corr_feature2 = i;
					}	
				}
			}
		}
		Point* points[num_of_rows];
		buildPointsFromFeatures(arrayofvectors[corr_feature1], arrayofvectors[corr_feature2], num_of_rows, points);
		
		Line l = linear_reg(points, num_of_rows);
		
		//then it gets here, I have the max feature for i, and i'll check if i and that feature are corellative
		learnHelper(maxResult, points ,num_of_rows, corr_feature1, corr_feature2, l);

		maxResult = 0;
		for(int i = 0; i < num_of_rows; i++){
			delete(points[i]);
		}		
	}
}

void SimpleAnomalyDetector::learnHelper(float maxResult, Point** ps, int num_of_rows, int corr_feature1, int corr_feature2, Line l) {
	if((maxResult>threshold)||(maxResult == threshold)) {	
		//build new struct
		correlatedFeatures corr;
		corr.corrlation = maxResult;
		corr.feature1 = to_string(corr_feature1);
		corr.feature2 = to_string(corr_feature2);

		//findThreshold
		corr.lin_reg = l;
		float maxDistance = 0,  temp = 0;
		// find the farthest point from line_reg
		for(int k = 0; k < num_of_rows; k++){
			temp = dev(*ps[k], l);
			if( temp > maxDistance){
				maxDistance = temp;
			}
		}
		corr.threshold = maxDistance;

		//push the new struct to the vector
		this->cf.push_back(corr);
	}		
}	

	
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){

	vector<AnomalyReport> report;
	int size =  ts.getColSize();
	int num_of_rows, i=0;
	map<string, vector<float>> table = ts.getMap();
	vector <float*> arrayofvectors(size);
	vector<string> features_names(size);

	//create vector of float* from the map
	for (auto it=table.begin(); it!=table.end(); ++it){
		arrayofvectors[i] = &(it->second)[0];
		num_of_rows = it->second.size();
		features_names[i] = &(it->first)[0];
		i++;
		//cout<< features_names[i] << endl;
	}
	// search deviations in the corr features
	for (auto it=this->cf.begin(); it!=this->cf.end(); ++it){
		for(int j = 0; j<num_of_rows; j++){
			int f1 = stoi(it->feature1);
			int f2 = stoi(it->feature2);
			float* corr_fea1 = arrayofvectors[f1];
			float* corr_fea2 = arrayofvectors[f2];
			Point check(corr_fea1[j], corr_fea2[j]);
			//deviation!!!
			if(isAnomalous(check.x, check.y, *it)){
				AnomalyReport an(features_names[f1] + "-" + features_names[f2] , j+1);
				report.push_back(an);
			} 
		}
	}
	return report;
}

bool SimpleAnomalyDetector::isAnomalous(float x, float y,correlatedFeatures c){
	return (abs(y - c.lin_reg.f(x)) > c.threshold * 1.1);
}