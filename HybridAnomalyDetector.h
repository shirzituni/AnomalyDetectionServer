// shir zituni	316537992


#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
	void learnHelper(float maxResult, Point** ps, int num_of_rows, int corr_feature1, int corr_feature2, Line l);
	bool isAnomalous(float x, float y,correlatedFeatures c);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
