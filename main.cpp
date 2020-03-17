//Rakshak Shrestha CSCI262 A3

#include "ActivityEngine.h"
#include "AnalysisEngine.h"
#include "AlertEngine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <random>
#include <cassert>

using namespace std;

int numOfVehicleTypes;           //global stats
int lengthOfRoad;
int speedLimit;
int numOfParking;
int progState = 0;

struct Vehicle {
	string name;
	int parkingFlag;
	string regiFormat;
	int volumeWeight;
	int speedWeight;
};

struct Stats {
	string name;
	double numMean;
	double numSD;
	double speedMean;
	double speedSD;
};

int main() {
	string x, y;
	int days;
	cout << "Please enter the vehicle stats filename: ";
	cin >> x;
	ifstream fin;
	fin.open(x);
	if (!fin.good()) {
		cout << "Invalid filename." << endl;
		return 0;
	}
		fin >> numOfVehicleTypes;
		Vehicle V[100];
		string parkFlag;
		string vw;
		string sw;
		for (int i = 0; i < numOfVehicleTypes; i++) {
			fin.ignore(numeric_limits<streamsize>::max(), '\n');
			getline(fin, V[i].name, ':');
			if (V[i].name == "Red car") {
				V[i].name = "RedCar";
			}
			getline(fin, parkFlag, ':');
			V[i].parkingFlag = atoi(parkFlag.c_str());
			getline(fin, V[i].regiFormat, ':');
			getline(fin, vw, ':');
			V[i].volumeWeight = atoi(vw.c_str());
			getline(fin, sw, ':');
			V[i].speedWeight = atoi(sw.c_str());
			if (V[i].name.compare("") == 0) {
				cout << "\nInvalid file formata." << endl;
				return 0;
			}
			if (V[i].regiFormat.compare("") == 0) {
				cout << "\nInvalid file formatb." << endl;
				return 0;
			}
			if (V[i].parkingFlag != 0 && V[i].parkingFlag != 1) {
				cout << "\nInvalid file formatc." << endl;
				return 0;
			}
			if (V[i].speedWeight < 0) {
				cout << "\nInvalid file formatd." << endl;
				return 0;
			}
			if (V[i].volumeWeight < 0) {
				cout << "\nInvalid file formate." << endl;
				return 0;
			}
		}
		fin.close();
	
	AnalysisEngine B;
	AlertEngine C;
	do {
		if (progState == 0) {
			cout << "\nPlease enter the baseline stats filename: ";
		}
		else {
			cout << "\nPlease enter the live data stats filename: ";
		}
		cin >> y;
		fin.open(y);
		if (!fin.good()) {
			cout << "Invalid filename." << endl;
			return 0;
		}
		fin >> numOfVehicleTypes;
		fin >> lengthOfRoad;
		fin >> speedLimit;
		fin >> numOfParking;
		Stats S[100];
		string numMean;
		string numSD;
		string speedMean;
		string speedSD;
		for (int i = 0; i < numOfVehicleTypes; i++) {
			fin.ignore(numeric_limits<streamsize>::max(), '\n');
			getline(fin, S[i].name, ':');
			getline(fin, numMean, ':');
			S[i].numMean = atoi(numMean.c_str());
			getline(fin, numSD, ':');
			S[i].numSD = atof(numSD.c_str());
			getline(fin, speedMean, ':');
			S[i].speedMean = atoi(speedMean.c_str());
			getline(fin, speedSD, ':');
			S[i].speedSD = atoi(speedSD.c_str());
			bool s = true;
			for (int i = 0; i < numOfVehicleTypes; i++) {
				if (S[i].numMean < 0) {
					cout << "\nInvalid file formatb." << endl;
					return 0;
				}
				if (S[i].numSD  < 0) {
					cout << "\nInvalid file formatc." << endl;
					return 0;
				}
				if (S[i].speedMean < 0) {
					cout << "\nInvalid file formatd." << endl;
					return 0;
				}
				if (S[i].speedSD < 0) {
					cout << "\nInvalid file formate." << endl;
					return 0;
				}
			}
		}
		fin.close();
		cout << "\nPlease enter the number of days to simulate :";
		cin >> days;
		for (int i = 0; i < days; i++) {
			ActivityEngine *A = new ActivityEngine();
			A->setValues(lengthOfRoad, numOfParking, i, progState);
			for (int j = 0; j < numOfVehicleTypes; j++) {
				A->addArrivalEvents(V[j].name, V[j].regiFormat, S[j].numMean, S[j].numSD, S[j].speedMean, S[j].speedSD, V[j].parkingFlag);
			};
			A->startSimulation();
			delete A;
		}
		
		B.readLogs(days, speedLimit, progState);
		if (progState == 0) {
			cout << "\nThe following baseline data has been generated.\n" << endl;
			for (int k = 0; k < numOfVehicleTypes; k++) {
				B.getBaselineData(days, V[k].name, V[k].volumeWeight, V[k].speedWeight, k);
			}
			progState++;
		}
		
		else if (progState > 0) {
			if (progState == 1) {
				C.readBaselineData();
				progState++;
			}
			C.detectAnomalies(days);

			char input;
			do {
				cin.clear();
            	cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Conduct a new simulation? (y/n)" << endl;
				cout << "<NOTE> Doing so will overwrite the current AnomalyReport.txt file." << endl;
				cin >> input;
				if (toupper(input) == 'N') {
						progState = -1;
						cout << "Program terminated." << endl;
				}
			} while (toupper(input) != 'Y' && toupper(input) != 'N');
		}
	} while (progState >= 0);

	return 0;
}
