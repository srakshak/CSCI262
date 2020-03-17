#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "AnalysisEngine.h"

using namespace std;

int vehicleTypes = 0;

struct vehicleData {
	string name;
	int count;
	double speed[100];
};
vehicleData vehicleCount[100];

struct breachData {
	string name, registration;
	double speed;
};
breachData breachList[50];

struct aggrData {
	string name;
	double totalCount, totalSpeedValue, vMean, sMean, vSumOfSquaredDifference, sSumOfSquaredDifference, vStdDev, sStdDev;
	double speed[10000];
	double count[1000];
	int speedEntries, speedWeight, volumeWeight;
	
	aggrData() {
		totalCount = 0;
		totalSpeedValue = 0;
		speedEntries = 0;
		vSumOfSquaredDifference = 0;
		sSumOfSquaredDifference = 0;
	}
};
aggrData vehicleAggr[1000];

AnalysisEngine::AnalysisEngine(){}

void AnalysisEngine::readLogs(int days, int speedLimit, int progState) {
	string x = "";
	string a, b, c, d;
	double e;
	ifstream analyze;
	ofstream data;
	
	data.open("SimData.txt", ios::out | ios::trunc);
	for (int i = 0; i < days; i++) {
		int breachCount = 0;
		for (int z = 0; z < vehicleTypes; z++) {
			vehicleCount[z].count = 0;
		}
		if (progState == 0) {
			x = "baselinelog" + to_string(i) + ".txt";
		}
		else {
			x = "simlog" + to_string(i) + ".txt";
		}
		analyze.open(x);
		while (analyze >> a) {
			analyze >> b;
			analyze >> c;
			analyze >> d;
			analyze >> e;
			
			if (c == "Arrival")	{
				bool matchFound = false;
				for (int j = 0; j < vehicleTypes && !matchFound; j++) {
					if (b.compare(vehicleCount[j].name) == 0) {
							vehicleCount[j].speed[vehicleCount[j].count] = e;
							vehicleCount[j].count++;
							matchFound = true;
						}
				}
				if (!matchFound) {
					vehicleCount[vehicleTypes].name = b;
					vehicleCount[vehicleTypes].speed[0] = e;
					vehicleCount[vehicleTypes].count = 1;
					vehicleTypes++;
				}
			}
			else if (c == "Departure(Main)") {
				if (e > speedLimit) {
					breachList[breachCount].name = b;
					breachList[breachCount].registration = d;
					breachList[breachCount].speed = e;
					breachCount++;
				}
			}
		}
		
		data << "\n Day " << i+1 << " Totals:" << "\n\n";
		cout << "\n Day " << i+1 << " Totals:" << "\n\n";
		for (int k = 0; k < vehicleTypes; k++) {
			data << setw(9) << vehicleCount[k].name << setw(5) << vehicleCount[k].count;
			vehicleAggr[k].name = vehicleCount[k].name;
			vehicleAggr[k].count[i] = vehicleCount[k].count;
			vehicleAggr[k].totalCount += vehicleCount[k].count;
			for (int m = 0; m < vehicleCount[k].count; m++) {
				data << " " << vehicleCount[k].speed[m];
				vehicleAggr[k].speed[vehicleAggr[k].speedEntries] = vehicleCount[k].speed[m];
				vehicleAggr[k].speedEntries++;
				vehicleAggr[k].totalSpeedValue += vehicleCount[k].speed[m];
			}
			data << "\n";
			cout << setw(9) << vehicleCount[k].name << setw(5) << vehicleCount[k].count << "\n";
		}
		cout << "----------------------------------------------" << endl;
		data << "\n Day " << i+1 << " Speed Limit Breaches" << "\n\n";
		cout << "\n Day " << i+1 << " Speed Limit Breaches" << "\n\n";
		for (int l = 0; l < breachCount; l++) {
			data << setw(9) << breachList[l].name << setw(10) << breachList[l].registration << setw(9) << breachList[l].speed << "\n";
			cout << setw(9) << breachList[l].name << setw(10) << breachList[l].registration << setw(9) << breachList[l].speed << "\n";
		}
		cout << endl << "=================================================" << endl;
		data << "\n";
		cout << "\n";
		analyze.close();
	}
	data.close();
}

void AnalysisEngine::getBaselineData(int days, string tempName, int vWeight, int sWeight, int currLoop) {
	ofstream fout;
	if (currLoop == 0) {
		fout.open("BaselineData.txt", ios::out | ios::trunc);
	}
	else {
		fout.open("BaselineData.txt", ios::out | ios::app);
	}
	bool matchFound = false;
	for (int i = 0; i < vehicleTypes && !matchFound; i++) {
		if (tempName.compare(vehicleAggr[i].name) == 0) {
			matchFound = true;
			vehicleAggr[i].volumeWeight = vWeight;
			vehicleAggr[i].speedWeight = sWeight;
			vehicleAggr[i].vMean = vehicleAggr[i].totalCount / double(days);
			vehicleAggr[i].sMean = vehicleAggr[i].totalSpeedValue / vehicleAggr[i].totalCount;
			
			for (int j = 0; j < days; j++) {
				vehicleAggr[i].vSumOfSquaredDifference += pow((vehicleAggr[i].count[j] - vehicleAggr[i].vMean), 2);
			}
			vehicleAggr[i].vStdDev = sqrt(vehicleAggr[i].vSumOfSquaredDifference / days);
			
			for (int k = 0; k < vehicleAggr[i].speedEntries; k++) {
				vehicleAggr[i].sSumOfSquaredDifference += pow((vehicleAggr[i].speed[k] - vehicleAggr[i].sMean), 2);
			}
			vehicleAggr[i].sStdDev = sqrt(vehicleAggr[i].sSumOfSquaredDifference / vehicleAggr[i].speedEntries);
			cout << setw(9) << vehicleAggr[i].name << setprecision(2) << setw(6) << vehicleAggr[i].vMean << setw(6) << vehicleAggr[i].vStdDev;
			cout << setw(6) << vehicleAggr[i].sMean << setw(6) << vehicleAggr[i].sStdDev << setw(6) << vehicleAggr[i].volumeWeight << setw(6) << vehicleAggr[i].speedWeight << "\n";
			fout << setw(9) << vehicleAggr[i].name << setprecision(2) << setw(6) << vehicleAggr[i].vMean << setw(6) << vehicleAggr[i].vStdDev;
			fout << setw(6) << vehicleAggr[i].sMean << setw(6) << vehicleAggr[i].sStdDev << setw(6) << vehicleAggr[i].volumeWeight << setw(6) << vehicleAggr[i].speedWeight << "\n";
			fout.close();
		}
	}
}

