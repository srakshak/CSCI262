#ifndef ANALYSISENGINE_H_
#define ANALYSISENGINE_H_

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

class AnalysisEngine {
public:
	struct Vehicle {
	string name;
	int parkingFlag;
	string regiFormat;
	int volumeWeight;
	int speedWeight;
	};
	
	AnalysisEngine();
	void readLogs(int days, int speedLimit, int progState);
	void getBaselineData(int days, string tempName, int vWeight, int sWeight, int currLoop);
};
#endif
