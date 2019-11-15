#pragma once
/*
Functions controlling evolution of food web
*/

#include "Species.h"
#include <fstream>
using namespace std;


	void RK4(Species S[], Producer P[], double dt);
	// 4th order Runge-Kutta

	double RKF45(Species S[], Producer P[], double dt);
	// addaptive timestep, 4th and 5th order Runge-Kutta-Fehlberg
	
	bool checkForExtinction(Species S[], Producer P[], double steadyStates[], ofstream& stabEigen, ofstream& unstabEigen, ofstream& webData, int addAttempt);
	// checking if a Species has gone extinct

	bool steady(Species S[], Producer P[], double steadyStates[], ofstream& webData, double t, int iter, bool rep_per, int tries[]);
	// checking if food web has reached the steady states, is periodic or chaotic

	bool decreasing(Species S[], Producer P[], int tries[]);
	// checking if food web is oscillating with damped oscillations

	void timeSeries(Species S[], Producer P[], double steadyStates[], ofstream& stabEigen, ofstream& unstabEigen, ofstream& webData, int addAttempt);
	// numerically integrating FoodWeb