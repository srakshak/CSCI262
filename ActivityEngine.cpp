#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "ActivityEngine.h"

using namespace std;

random_device rd;
default_random_engine generator(rd());
uniform_int_distribution<> dist(0, 1380);
uniform_int_distribution<> cases(1, 2000);

ActivityEngine::ActivityEngine(){
}

void ActivityEngine::setValues(int l, int p, int d, int s) {
	roadLength = l;
	parkSpaces = p;
	dayCount = d;
	progState = s;
}

void ActivityEngine::addArrivalEvents(string n, string form, double meanValue, double SD, double speedMean, double speedSD, int p) {
	normal_distribution<double> distribution(meanValue, SD);
	normal_distribution<double> distribution2(speedMean, speedSD);
	int numOfItems = distribution(generator);
	for (int i = 0; i < numOfItems; i++) {
		string l = generateLicense(form);
		int a = dist(generator);
		timeUsed[numOfArrivals] = a;
		vArrivals[numOfArrivals].name = n;
		vArrivals[numOfArrivals].license = l;
		vArrivals[numOfArrivals].arrivalTime = a;
		vArrivals[numOfArrivals].parkFlag = p;
		vArrivals[numOfArrivals].arrivalSpeed = distribution2(generator);
		if (vArrivals[numOfArrivals].arrivalSpeed <= 0) {
			vArrivals[numOfArrivals].arrivalSpeed = 1;
		}
		vArrivals[numOfArrivals].speedM = speedMean;
		vArrivals[numOfArrivals].speedS = speedSD;
		numOfArrivals++;
	}
}

void ActivityEngine::startSimulation() {
	bernoulli_distribution parking(0.05);
	discrete_distribution<int> casess{ 32, 5, 3, 10 };
	cout << endl << "The simulation has begun for day " << dayCount + 1 << endl;
	ofstream of;
	string logName;
	if (progState == 0) {
		logName = "baselinelog" + to_string(dayCount) + ".txt";
	}
	else {
		logName = "simlog" + to_string(dayCount) + ".txt";	
	}
	of.open(logName, ios::out | ios::trunc);
	while (currentTime < 1440) {
		for (int i = 0; i < numOfArrivals; i++) {
			if (vArrivals[i].state == 1) {
					vArrivals[i].distanceTravelled += vArrivals[i].arrivalSpeed / 60;
					if (vArrivals[i].distanceTravelled >= roadLength) {
						vArrivals[i].state = 0;
						double avgSpeed = (vArrivals[i].distanceTravelled * 60) / ((currentTime - vArrivals[i].arrivalTime)-(vArrivals[i].parkStop - vArrivals[i].parkStart));
						of << giveTime(currentTime) << setw(13) << vArrivals[i].name << setw(19) << "Departure(Main)" << setw(10) << vArrivals[i].license << setw(9) << fixed << setprecision(2) << avgSpeed << "\n";
					}
				}
			if (vArrivals[i].state > 0) {
				int rand = casess(generator);
				if (rand == 1){
					if (vArrivals[i].state == 1) {
						if (vArrivals[i].parkFlag == 1) {
							if (parkSpaces > 0) {
								of << giveTime(currentTime) << setw(13) << vArrivals[i].name << setw(19) << "Parked" << setw(10) << vArrivals[i].license << setw(9) << -1 << "\n";
								vArrivals[i].state++;
								vArrivals[i].parkStart = currentTime;
								parkSpaces--;
							}
						}
					}
					else if (vArrivals[i].state == 2)
					{
						bool b2 = parking(generator);
						if (b2) {
							of << giveTime(currentTime) << setw(13) << vArrivals[i].name << setw(19) << "Unparked" << setw(10) << vArrivals[i].license << setw(9) << -1 << "\n";
							vArrivals[i].state++;
							vArrivals[i].parkStop = currentTime;
							parkSpaces++;
						}
					}

				}
				else if (rand == 3 && vArrivals[i].state != 2) {
					normal_distribution<double> getNewSpeed(vArrivals[i].speedM, vArrivals[i].speedS);
					double s = getNewSpeed(generator);
					if (s > 0)
						vArrivals[i].arrivalSpeed = s;
					else
						vArrivals[i].arrivalSpeed = 1;
					of << giveTime(currentTime) << setw(13) << vArrivals[i].name << setw(19) << "SpeedChange" << setw(10) << vArrivals[i].license << setw(9) << fixed << setprecision(2) << vArrivals[i].arrivalSpeed << "\n";
				}
				else if (rand == 2 && vArrivals[i].state != 2 && vArrivals[i].distanceTravelled > 0.5) {
					vArrivals[i].state = 0;
					double avgSpeed = (vArrivals[i].distanceTravelled * 60) / ((currentTime - vArrivals[i].arrivalTime)-(vArrivals[i].parkStop - vArrivals[i].parkStart));
					of << giveTime(currentTime) << setw(13) << vArrivals[i].name << setw(19) << "Departure(Side)" << setw(10) << vArrivals[i].license << setw(9) << fixed << setprecision(2) << avgSpeed << "\n";
				}
			}
			if (currentTime == vArrivals[i].arrivalTime) {
				vArrivals[i].state = 1;
				of << giveTime(currentTime) << setw(13) << vArrivals[i].name << setw(19) << "Arrival" << setw(10) << vArrivals[i].license << setw(9) << fixed << setprecision(2) << vArrivals[i].arrivalSpeed << "\n";
			}
		}
		currentTime++;
	}
	cout << "Day " << dayCount + 1 << " logs created." << endl;
currentTime = 0;
numOfArrivals = 0;
}

string ActivityEngine::giveTime(int t) {
	int hr = currentTime / 60;
	int min = currentTime % 60;
	string time;
	if (hr < 10) {
		time += "0";
		time += to_string(hr);
	}
	else {
		time += to_string(hr);
	}
	time += ":";
	if (min < 10) {
		time += "0";
		time += to_string(min);
	}
	else {
		time += to_string(min);
	}
	return time;
}
string ActivityEngine::generateLicense(string f) {
	random_device rd;
	default_random_engine gen(rd());
	uniform_int_distribution<> letter(65, 90);
	uniform_int_distribution<> digit(0, 9);
	string Registration;
	for (int i = 0; i < f.length(); i++) {
		switch (f[i]) {
		case 'L':
			Registration += (char(letter(gen)));
			break;
		case 'D':
			Registration += (to_string(digit(gen)));
		}
	}
	return Registration;
}

void ActivityEngine::printArrivals() {
	for (int i = 0; i < numOfArrivals; i++) {
		cout << vArrivals[i].name << " " << vArrivals[i].license << " " << vArrivals[i].arrivalSpeed << " " << vArrivals[i].arrivalTime << endl;
	}
}

