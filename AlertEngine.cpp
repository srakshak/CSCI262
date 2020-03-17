#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "AlertEngine.h"

using namespace std;

int vThreshold = 0;
int sThreshold = 0;
int entryCount = 0;

struct baselineData {
	string name;
	double vMean, vSD, sMean, sSD;
	int vWeight, sWeight;
};
baselineData baselineCount[100];

AlertEngine::AlertEngine(){}

void AlertEngine::readBaselineData() {
	string a;
	double b, c, d, e;
	int f, g;
	ifstream fin;
	
	fin.open("BaselineData.txt");
	while (fin >> a) {
		fin >> b;
		fin >> c;
		fin >> d;
		fin >> e;
		fin >> f;
		fin >> g;
		
		baselineCount[entryCount].name = a;
		baselineCount[entryCount].vMean = b;
		baselineCount[entryCount].vSD = c;
		baselineCount[entryCount].sMean = d;
		baselineCount[entryCount].sSD = e;
		baselineCount[entryCount].vWeight = f;
		vThreshold += f;
		baselineCount[entryCount].sWeight = g;
		sThreshold += g;
		entryCount++;
	}
	fin.close();
	vThreshold *= 2;
	sThreshold *= 2;
}

void AlertEngine::detectAnomalies(int days) {
	string a;
	int b;
	double c;
	ifstream fin;
	ofstream fout;
	
	fout.open("AnomalyReport.txt", ios::out | ios::trunc);
	cout << setw(18) << "Volume Threshold: " << vThreshold << "\n" << setw(18) << "Speed Threshold: " << sThreshold << "\n\n";
	fout << setw(18) << "Volume Threshold: " << vThreshold << "\n" << setw(18) << "Speed Threshold: " << sThreshold << "\n\n";
	fin.open("SimData.txt");
	for (int i = 0; i < days; i++) {
		double vAnomalyCounter;
		double sAnomalyCounter = 0;
		
		fin.ignore(numeric_limits<streamsize>::max(), ':');
		fin >> a;
		
		bool matchFound = false;
		for (int j = 0; j < entryCount && !matchFound; j++) {
			if (a.compare(baselineCount[j].name) == 0) {
				matchFound = true;
				fin >> b;
				vAnomalyCounter = (max(double(b), baselineCount[j].vMean) - min(double(b), baselineCount[j].vMean)) / baselineCount[j].vSD * baselineCount[j].vWeight;
				
				for (int k = 0; k < b; k++) {
					double anomaly;
					fin >> c;
					anomaly = (max(c, baselineCount[j].sMean) - min(c, baselineCount[j].sMean)) / baselineCount[j].sSD * baselineCount[j].sWeight;
					sAnomalyCounter += anomaly;
				}
				
			cout << "Day " << i+1 << "\n";
			fout << "Day " << i+1 << "\n";
			
			cout << setw(24) << "Volume Anomaly Counter: " << setw(5) << fixed << setprecision(2) << vAnomalyCounter;
			fout << setw(24) << "Volume Anomaly Counter: " << setw(5) << fixed << setprecision(2) << vAnomalyCounter;
			if (vAnomalyCounter > double(vThreshold)) {
				cout << setw(20) << "Volume anomaly\n";
				fout << setw(20) << "Volume anomaly\n";
			}
			else {
				cout << setw(20) << "No anomaly\n";
				fout << setw(20) << "No anomaly\n";
			}
			
			cout << setw(24) << "Speed Anomaly Counter: " << setw(5) << fixed << setprecision(2) << sAnomalyCounter;
			fout << setw(24) << "Speed Anomaly Counter: " << setw(5) << fixed << setprecision(2) << sAnomalyCounter;
			if (sAnomalyCounter > double(sThreshold)) {
				cout << setw(20) << "Speed anomaly\n";
				fout << setw(20) << "Speed anomaly\n";
			}
			else {
				cout << setw(20) << "No anomaly\n";
				fout << setw(20) << "No anomaly\n";
			}
			
			}
		}
	}
	
	cout << "\nResults have been saved to AnomalyReport.txt.\n";
}
