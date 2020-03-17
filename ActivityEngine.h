#ifndef ACTIVITYENGINE_H_
#define ACTIVITYENGINE_H_

#include <random>
#include <iostream>

using namespace std;

class ActivityEngine {
public:
	ActivityEngine();
	void setValues(int l, int p, int d, int s);
	void addArrivalEvents(string n, string form, double meanValue, double SD, double speedMean, double speedSD, int p);
	string generateLicense(string f);
	void printArrivals();
	void startSimulation();
	string giveTime(int t);

private:
	struct Vehi {
		string name;
		string license;
		int arrivalTime;
		double arrivalSpeed;
		int parkFlag;
		double distanceTravelled;
		int parkStart;
		int parkStop;
		int state;
		double speedM;
		double speedS;

		Vehi() {
			name = "";
			state = 0;
			parkStart = 0;
			parkStop = 0;
				distanceTravelled = 0;
		}
	};

	Vehi vArrivals[2000];
	int numOfArrivals = 0;
	int timeUsed[2000];
	int roadLength;
	int parkSpaces;
	int dayCount;
	int currentTime = 1;
	int progState;

};
#endif
