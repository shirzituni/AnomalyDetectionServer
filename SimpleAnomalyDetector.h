// shir zituni	316537992

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include "minCircle.h"


struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	Circle circle;
	bool isCircle = false;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	protected: 
		vector<correlatedFeatures> cf;
	float threshold;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();
	void setThreshold(float threshold);
	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);

	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
	void setCorrelationThreshold(float threshold){
		this->threshold=threshold;
	}
	void buildPointsFromFeatures(float* corr_feature1, float* corr_feature2, int num_of_rows, Point** points);
	virtual void learnHelper(float maxResult,Point** ps, int num_of_rows, int corr_feature1, int corr_feature2, Line l);
	virtual bool isAnomalous(float x, float y,correlatedFeatures c);

};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
