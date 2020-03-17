#ifndef ALERTENGINE_H_
#define ALERTENGINE_H_

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

class AlertEngine {
public:
	AlertEngine();
	void readBaselineData();
	void detectAnomalies(int days);
};
#endif
