#pragma once
/*
Functions controlling evolution of food web
*/

#include "species.h"
#include <fstream>
using namespace std;


	void rk4(species S[], producer P[], double dt);
	// 4th order Runge-Kutta

	double rkf45_dt(species S[], producer P[], double dt);
	// addaptive timestep, 4th and 5th order Runge-Kutta-Fehlberg
	
	bool check_for_extinction(species S[], producer P[], double steady_states[], ofstream& stab_file, ofstream& unstab_file, ofstream& data, int iter);
	// checking if a species has gone extinct

	bool sustainable(species S[], producer P[], double steady_states[], ofstream& data, double t, int iter, bool rep_per);
	// checking if food web has reached the steady states, is periodic or chaotic

	bool periodic(species S[], producer P[], int iter);
	// checking if food web is periodic

	bool decreasing(species S[], producer P[], int iter);
	// checking if food web is oscillating with damped oscillations

	void time_series(species S[], producer P[], double steady_states[], ofstream& stab_file, ofstream& unstab_file, ofstream& data, int iter);
	// numerically integrating foodweb
