// shir zituni	316537992

#include "HybridAnomalyDetector.h"
#include "minCircle.h"


HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::learnHelper(float maxResult, Point** ps, int num_of_rows, int corr_feature1, int corr_feature2, Line l) {
	//SimpleAnomalyDetector
	if(maxResult > threshold){
		SimpleAnomalyDetector::learnHelper(maxResult, ps, num_of_rows, corr_feature1, corr_feature2, l);
		return;
	}
	//HybridAnomalyDetector
	if((maxResult < threshold) && (maxResult > 0.5)){

		//build new struct
		correlatedFeatures corr;
		corr.corrlation = maxResult;
		corr.feature1 = to_string(corr_feature1);
		corr.feature2 = to_string(corr_feature2);

		Circle circle = findMinCircle(ps, num_of_rows);
		corr.circle = circle;
		corr.isCircle = true;

		//findThreshold
		float maxDistance = 0,  temp = 0;
		// find the farthest point from line_reg
		for(int k = 0; k < num_of_rows; k++){
			temp = distance(*ps[k], circle.center);
			if( temp > maxDistance){
				maxDistance = temp;
			}
		}
		corr.threshold = maxDistance;

		//push the new struct to the vector
		this->cf.push_back(corr);
	}	
}
//check if there is Anomalous
bool HybridAnomalyDetector::isAnomalous(float x, float y,correlatedFeatures c) {
	//SimpleAnomalyDetector
	if(!c.isCircle){
		return SimpleAnomalyDetector::isAnomalous(x, y, c);
	}
	Point p(x,y);
	//find if the point is outside the circle 
	float dis = distance(c.circle.center, p); 
	return(abs(dis) > c.threshold  * 1.1);
}
