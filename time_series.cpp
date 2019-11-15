#include "time_series.h"
#include "food_web.h"
#include <iostream>
#include <fstream>
#include <complex>
using namespace std;

void RK4(Species S[], Producer P[], double dt)
{
//	INITIALIZING TEMPORARY ARRAYS
	int i; 
	double dS_1[nMAX], dS_2[nMAX], dS_3[nMAX], dS_4[nMAX] = { 0 };
	// temprary time derivatives

	Producer P_temp[nMAX];
	Species S_temp[nMAX];
	// temporary Species arrays

	for (int i = 0; i < Species::nTotal; i++)
		{
			P_temp[i] = P[i];
			S_temp[i] = S[i];
		}
	// initializing temporary Species arrays


//	1ST ESTIMATE
	derivatives(S, P);
	// computing derivatives

	for (i = 0; i < Producer::nProducer; i++)
		{
			dS_1[i] = S[i].dS * dt;
			// differential step: dS(S_0)/dt * dt

			P_temp[i].density = P[i].density + 0.5 * dS_1[i];
			S_temp[i].density = S[i].density + 0.5 * dS_1[i];
			// updating temporary densities: S_0 + (1/2) * dS_1
		}
	// Producers

	for (i = Producer::nProducer; i < Species::nTotal; i++)
		{
			dS_1[i] = S[i].dS * dt;
			// differential step: dS(S_0)/dt * dt

			S_temp[i].density = S[i].density + 0.5 * dS_1[i];
			// updating temporary densities: S_0 + (1/2) * dS_1
		}
	// other Species



//	2ND ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < Producer::nProducer; i++)
	{
		dS_2[i] = S_temp[i].dS * dt;
		// differential step: dS(S_0 + (1/2) * dS_1)/dt * dt

		P_temp[i].density = P[i].density + 0.5 * dS_2[i];
		S_temp[i].density = S[i].density + 0.5 * dS_2[i];
		// updating temporary densities: S_0 + (1/2) * dS_2
	}
	// Producers

	for (i = Producer::nProducer; i < Species::nTotal; i++)
	{
		dS_2[i] = S_temp[i].dS * dt;
		// differential step: dS(S_0 + (1/2) * dS_1)/dt * dt

		S_temp[i].density = S[i].density + 0.5 * dS_2[i];
		// updating temporary densities: S_0 + (1/2) * dS_2
	}
	// other Species



//	3RD ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < Producer::nProducer; i++)
	{
		dS_3[i] = S_temp[i].dS * dt;
		// differential step: dS(S_0 + (1/2) * dS_2)/dt * dt

		P_temp[i].density = P[i].density + dS_3[i];
		S_temp[i].density = S[i].density + dS_3[i];
		// updating temporary densities: S_0 + dS_3
	}
	// Producers

	for (i = Producer::nProducer; i < Species::nTotal; i++)
	{
		dS_3[i] = S_temp[i].dS * dt;
		// differential step: dS(S_0 + (1/2) * dS_2)/dt * dt

		S_temp[i].density = S[i].density + dS_3[i];
		// updating temporary densities: S_0 + dS_3
	}
	// other Species



//	4TH ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < Species::nTotal; i++)
	{
		dS_4[i] = S_temp[i].dS * dt;
		// differential step: dS(S_0 + dS_3)/dt *dt
	}
	// all Species



//	CALCULATING S(t+dt)
	for (i = 0; i < Producer::nProducer; i++)
	{
		// cout << "S(t)_" << i << ": " << S[i].density;

		P[i].density += (dS_1[i] + 2 * dS_2[i] + 2 * dS_3[i] + dS_4[i]) / 6;
		S[i].density += (dS_1[i] + 2 * dS_2[i] + 2 * dS_3[i] + dS_4[i]) / 6;

		// cout << ", S(t+dt)_" << i << ": " << S[i].density << endl;
	}
	// computing density of Producers

	for (i = Producer::nProducer; i < Species::nTotal; i++)
	{
		// cout << "S(t)_" << i << ": " << S[i].density;

		S[i].density += (dS_1[i] + 2 * dS_2[i] + 2 * dS_3[i] + dS_4[i]) / 6;

		// cout << ", S(t+dt)_" << i << ": " << S[i].density << endl;
	}
	// computing density of other Species
}

double RKF45(Species S[], Producer P[], double dt)
{
	double tol = 5e-4;
	// tolerance on time step

	int i, j = 0;
	for (i = 0; i < Species::nTotal; i++)
	{
		if (abs(S[i].dS) >= abs(S[j].dS))
		{
			j = i;
		}
	}
	// finding Species with largest derivative

	double dS_1[nMAX], dS_2[nMAX], dS_3[nMAX], dS_4[nMAX], dS_5[nMAX], dS_6[nMAX];
	// temporary derivative arrays

	Species S_temp[nMAX];
	Producer P_temp[nMAX];
	//temporary Species arrays

	for (int i = 0; i < nMAX; i++)
	{
		P_temp[i] = P[i];
		S_temp[i] = S[i];
	}
	// initializing temporary arrays


//	FIRST ESTIMATE
	for (i = 0; i < Producer::nProducer; i++)
	{
		dS_1[i] = S[i].dS * dt;

		P_temp[i].density = P_temp[i].density + 0.25 * dS_1[i];
		S_temp[i].density = S_temp[i].density + 0.25 * dS_1[i];
		// updating temporary densities
	}
	// Producers

	for (i = Producer::nProducer; i < Species::nTotal; i++)
	{
		dS_1[i] = S[i].dS * dt;

		S_temp[i].density = S_temp[i].density + 0.25 * dS_1[i];
		// updating temporary densities
	}
	// other Species


//	SECOND ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < Producer::nProducer; i++)
	{
		dS_2[i] = S_temp[i].dS * dt;

		P_temp[i].density = P[i].density + (3 / 32) * dS_1[i] + (9 / 32) *dS_2[i];
		S_temp[i].density = S[i].density + (3 / 32) * dS_1[i] + (9 / 32) *dS_2[i];
		// updating temporary densities
	}
	// Producers

	for (i = Producer::nProducer; i < Species::nTotal; i++)
	{
		dS_2[i] = S_temp[i].dS * dt;

		S_temp[i].density = S[i].density + (3 / 32) * dS_1[i] + (9 / 32) *dS_2[i];
		// updating temporary densities
	}
	// other Species


//	THIRD ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives
	for (i = 0; i < Producer::nProducer; i++)
	{
		dS_3[i] = S_temp[i].dS * dt;

		P_temp[i].density = P[i].density + (1932 / 2197) * dS_1[i] - (7200 / 2197) * dS_2[i] + (7296 / 2197) * dS_3[i];
		S_temp[i].density = S[i].density + (1932 / 2197) * dS_1[i] - (7200 / 2197) * dS_2[i] + (7296 / 2197) * dS_3[i];
		// upddatinf temporary densities
	}
	// Producers

	for (i = Producer::nProducer; i < Species::nTotal; i++)
	{
		dS_3[i] = S_temp[i].dS * dt;

		S_temp[i].density = S[i].density + (1932 / 2197) * dS_1[i] - (7200 / 2197) * dS_2[i] + (7296 / 2197) * dS_3[i];
		// updating temporary densities
	}
	// other Species


//	FORTH ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < Producer::nProducer; i++)
	{
		dS_4[i] = S_temp[i].dS * dt;

		P_temp[i].density = P[i].density + (439 / 216) * dS_1[i] - 8 * dS_2[i] + (3680 / 513) * dS_3[i] - (845 / 4104) * dS_4[i];
		S_temp[i].density = S[i].density + (439 / 216) * dS_1[i] - 8 * dS_2[i] + (3680 / 513) * dS_3[i] - (845 / 4104) * dS_4[i];
		// updating temporary densities
	}
	// Producers

	for (i = Producer::nProducer; i < Species::nTotal; i++)
	{
		dS_4[i] = S_temp[i].dS * dt;

		S_temp[i].density = S[i].density + (439 / 216) * dS_1[i] - 8 * dS_2[i] + (3680 / 513) * dS_3[i] - (845 / 4104) * dS_4[i];
		// updating temporary densities
	}
	// other Species


//	FIFTH ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < Producer::nProducer; i++)
	{
		dS_5[i] = S_temp[i].dS * dt;

		P_temp[i].density = P[i].density - (8 / 27) * dS_1[i] + 2 * dS_2[i] - (3544 / 2565) * dS_3[i] + (1859 / 4104) * dS_4[i] - (11 / 40)*dS_5[i];
		S_temp[i].density = S[i].density - (8 / 27) * dS_1[i] + 2 * dS_2[i] - (3544 / 2565) * dS_3[i] + (1859 / 4104) * dS_4[i] - (11 / 40)*dS_5[i];
		// updating temporary densities
	}
	// Producers

	for (i = Producer::nProducer; i < Species::nTotal; i++)
	{
		dS_5[i] = S_temp[i].dS * dt;

		S_temp[i].density = S[i].density - (8 / 27) * dS_1[i] + 2 * dS_2[i] - (3544 / 2565) * dS_3[i] + (1859 / 4104) * dS_4[i] - (11 / 40)*dS_5[i];
		// updating temporary densities
	}
	// other Species


	//	SIXTH ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < Species::nTotal; i++)
	{
		dS_6[i] = S_temp[i].dS * dt;
	}
	// all Species

	double S_0, S_1;
	S_0 = S[j].density + (16 / 135)*dS_1[j] + (6656 / 12825)*dS_3[j] + (28561 / 56430)*dS_4[j] - (9 / 50)*dS_5[j] + (2 / 55)*dS_6[j];
	S_1 = S[j].density + (25 / 216)*dS_1[j] + (1408 / 2565)*dS_3[j] + (2197 / 4101)*dS_4[j] - 0.2*dS_5[j];
	// 5th and 4th order Runge-Kutta-Fehlberg

	double tau = abs(S_0 - S_1);
	if (tau <= epsilon) { return dt; }
	// returning last timestep if tau is zero

	else
	{
		double s = sqrt(sqrt(tol / (2 * tau)));
		return 0.01 * s;
		// computing timestep
	}
}

bool checkForExtinction(Species S[], Producer P[], double steadyStates[], ofstream& stabEigen, ofstream& unstabEigen, ofstream& webData, int addAttempt)
{
	int k, i = 0;
	for (k = 0; k < Species::nTotal; k++)
	{
		if (S[k].density < S[i].density)
		{
			i = k;
		}
	}
	// determining Species with the smallest density

	if (S[i].density <=  1e2 * epsilon)
	{
		cout << "Species " << i << " has gone extinct" << endl;
		// printing

		webData << 0 << S[i].addAttempt << endl;
		// save behavior of food web
		
		FoodWeb::prevExtinct = S[i].addAttempt;
		// updating food web

		removeSpecies(S, P, i);
		// removing extinct Species

		checkFeasibility(S, P, steadyStates, stabEigen, unstabEigen, addAttempt);
		// checking if food web has steady state after removal of extinct Species
		
		webData << addAttempt << " " << Species::nTotal << " " << FoodWeb::feasible << " " << FoodWeb::stable << " ";
		// saving behavior of new food web
		
		return true;
	}
	// Removing smallest Species if it is extinct

	else { return false; }
	// else nothing
}

bool steady(Species S[], Producer P[], double steadyStates[], ofstream& webData, double t, int addAttempt, bool rep_per, int tries[])
{
	int steadyCount = 0;
	if (rep_per)
	{
		webData << FoodWeb::prevIteration << endl;
		// saving behavior of food web
		return true;
	}
	// if food web was periodic/chaotic before invasion and invasive Species went extinct
	// food web is again considered periodic/chaotic

//	LINEARLY STABLE
	else if (FoodWeb::feasible && FoodWeb::stable && t < 1e4)
	{
		for (int i = 0; i < Species::nTotal; i++)
		{
			if (abs(S[i].density - steadyStates[i]) / steadyStates[i] <= (1e8)*epsilon)
			{
				steadyCount++;
			}
		}
		// counting Species with steady density

		if (steadyCount == Species::nTotal)
		{
			FoodWeb::prevIteration = 1;
			webData << 1 << endl;
			// saving behavior of food web

			return true;
		}
		// if all Species have reached steady states

		else { return false; }
	}
	// feasible, linearly stable food webs

	else if (FoodWeb::feasible && FoodWeb::stable && t >= 1e4 && t < 5e4)
	{
		for (int i = 0; i < Species::nTotal; i++)
		{
			if (abs(S[i].density - steadyStates[i]) / steadyStates[i] <= (1e12)*epsilon)
			{
				steadyCount++;
			}
		}
		// counting Species with steady density

		if (steadyCount == Species::nTotal)
		{
			FoodWeb::prevIteration = 1;
			webData << 1 << endl;
			// saving behavior of food web

			return true;
		}
		// if all Species have reached steady states

		else { return false; }
	}
	// feasible, linearly stable food webs that take long to stabilize

	else if (FoodWeb::feasible && FoodWeb::stable && t >= 5e4 + 5e4 * tries[0])
	{
		for (int i = 0; i < Species::nTotal; i++)
		{
			if (abs(S[i].density - steadyStates[i]) / steadyStates[i] <= (1e12)*epsilon)
			{
				steadyCount++;
			}
		}
		// counting Species with steady density

		if (steadyCount == Species::nTotal)
		{
			FoodWeb::prevIteration = 1;
			webData << 1 << endl;
			// saving behavior of food web

			return true;
		}
		// if all Species have reached the steady states

		if (decreasing(S, P, tries))
		{
			FoodWeb::prevIteration = 2;
			webData << 2 << endl;
			// saving behavior of food web
			
			setDensitiesSteady(steadyStates, S, P);
			// setting densities to steady states

			return true;
		}

		else
		{
			/*
			double dt = 0.01;
			t = 0;
			ofstream timefile("timeseries_" + std::to_string(iter) + ".txt");
			while (t <= 3e4)
			{
				dt = rkf45_dt(S, P, dt);
				if (dt < 0.01) { dt = 0.01; }
				rk4(S, P, dt);
				// time evolution

				timefile << t;
				for (int i = 0; i < N; i++)
				{
					timefile << " " << S[i].density;
				}
				timefile << endl;

				t += dt;
			}
			timefile.close();
			*/

			//FoodWeb::last_iteration = 6;
			//data << 6 << endl;
			//set_densities_steady(steady_states, S, P);
			tries[0]++;
			return false;
		}
	}
	// feasible, linearly stable food webs that take very long to stabilize

	/*
//	LINEARLY UNSTABLE
	else if (FoodWeb::feasible && !FoodWeb::stable && t >= 5e4 + 5e4 * tries[0])
	{
		cout << "tries: " << tries[0] << endl;

		if (periodic(S, P, tries))
		{
			FoodWeb::last_iteration = 3;
			data << 3 << endl;
			// saving behavior of food web

			return true;
		}
		// checking for periodic oscillations

		else if (chaotic(S, P, iter))
		{
			FoodWeb::last_iteration = 4;
			data << 4 << endl;

			/*double dt = 0.01;
			t = 0;
			ofstream timefile("chaos_" + std::to_string(iter) + ".txt");

			while (t <= 3e4)
			{
				dt = rkf45_dt(S, P, dt);
				if (dt < 0.01) { dt = 0.01; }
				rk4(S, P, dt);
				// time evolution

				timefile << t;
				for (int i = 0; i < N; i++)
				{
					timefile << " " << S[i].density;
				}
				timefile << endl;
				// saving time series

				t += dt;
			}
			timefile.close();
			// checking if really chaotic

			return true;
		}
		// checking for chaos

		
		else
		{
			tries[0]++;
			return false;
		}
	}
	
	else if (FoodWeb::feasible && !FoodWeb::stable && tries[0] >= 5)
	{
		double dt = 0.01;
		t = 0;

		ofstream timefile("undetermined_behavior-" + std::to_string(iter) + ".txt");

		while (t <= 1e4)
		{
			dt = rkf45_dt(S, P, dt);
			if (dt < 0.01) { dt = 0.01; }
			rk4(S, P, dt);
			// time evolution

			timefile << t;
			for (int i = 0; i < N; i++)
			{
				timefile << " " << S[i].density;
			}
			timefile << endl;
			// saving time series

			t += dt;
		}
		timefile.close();

		FoodWeb::last_iteration = 88;
		data << 88 << endl;

		return true;
	}
	*/

	else { return false; }
}

bool decreasing(Species S[], Producer P[], int tries[])
{
	cout << "Checking for damped oscillations.\n" << endl;
	//	DECLARING VARIABLES
	double time = 1e4 * (1 + tries[0]);
	double t = 0;
	double dt = 0.01;

	int i = 0;
	int period = 0;
	int counter;
	// counters

	double max_density_1[nMAX] = { 0 };
	double max_density_2[nMAX] = { 0 };
	// arrays for storing max values


//	FIRST EXTREMAL DENSITIES
	while (t <= time && period < 1)
	{
		t += dt;
		counter = 0;
		// reinitializing counter

		dt = RKF45(S, P, dt);
		if (dt < 0.01) { dt = 0.01; }
		RK4(S, P, dt);
		// time evolution

		for (i = 0; i < Species::nTotal; i++)
		{
			if (S[i].density > max_density_1[i])
			{
				max_density_1[i] = S[i].density;
			}
			// updating max if new density higher than old

			else if (t > time && S[i].density < max_density_1[i])
			{
				counter++;
			}
			// counting if new density not higher than old

		}
		// Saving and counting Species that reached max value

		if (counter == Species::nTotal)
		{
			period++;
		}
		// breaking loop if all Species has reached max value once

		if (t >= 10*time)
		{
			cout << endl << "ERROR: maximal values not reached within time limit" << endl << endl;
			return false;
		}
		// breaking if cycle takes too long

	}


	//	RUNNING SYSTEM FOR A WHILE
	t = 0;

	while (t <= 0.5 * time)
	{
		dt = RKF45(S, P, dt);
		if (dt < 0.01) { dt = 0.01; }
		RK4(S, P, dt);
		// time evolution

		t += dt;
	}


	//	SECOND EXTREMAL DENSITIES
	t = 0;
	period = 0;
	// reinitializing counter

	while (t <= time && period < 1)
	{
		t += dt;
		counter = 0;
		// reinitializing counter

		dt = RKF45(S, P, dt);
		if (dt < 0.01) { dt = 0.01; }
		RK4(S, P, dt);
		// time evolution

		for (i = 0; i < Species::nTotal; i++)
		{
			if (S[i].density > max_density_2[i])
			{
				max_density_2[i] = S[i].density;
			}
			// updating max if new density higher than old

			else if (t > time && S[i].density < max_density_2[i])
			{
				counter++;
			}
			// counting if new density not higher than old

		}
		// Saving and counting Species that reached max value

		if (counter == Species::nTotal)
		{
			period++;
		}
		// breaking loop if all Species has reached max value once

		if (t >= 10 * time)
		{
			cout << endl << "ERROR: maximal values not reached within time limit" << endl << endl;
			return false;
		}
		// breaking if cycle takes too long

	}


	//	CHECK FOR DECREASING OSCILLATIONS
	counter = 0;
	// reinitializing counter

	for (i = 0; i < Species::nTotal; i++)
	{
		cout << "Comparing max1 and max2" << endl;
		cout << max_density_1[i] - max_density_2[i] << endl << endl;

		if (max_density_1[i] - max_density_2[i] > 10*epsilon)
		{
			counter++;
		}
	}
	// counting Species with max1 larger than max2

	if (counter == Species::nTotal)
	{
		cout << "System is oscillating decreasingly\n" << endl;
		return true;
	}
	// decreasing

	else
	{
		cout << "System is not decreasing\n" << endl;
		return false;
	}
	// not decreasing
}

void timeSeries(Species S[], Producer P[], double steadyStates[], ofstream& stab_file, ofstream& unstab_file, ofstream& data, int iter)
{
	double t = 0;
	double dt = 0.01;
	int tries[1];
	tries[0] = 0;

	int N_Species = Species::nTotal;
	// number of Species when invasion starts

	bool rep_per = 0;
	// logic variable used for time optimization if invasive Species go extinct
	
	data << iter << " " << Species::nTotal << " " << FoodWeb::feasible << " " << FoodWeb::stable<< " ";
	// saving behavior of food web
	
	while (!steady(S, P, steadyStates, data, t, iter, rep_per, tries) && Species::nTotal != 0)
	{
		t += dt;
		dt = RKF45(S, P, dt);
		if (dt < 0.01) { dt = 0.01; }
		// computing time step and setting lower limit

		RK4(S, P, dt);
		// performing time step

		bool extinct = checkForExtinction(S, P, steadyStates, stab_file, unstab_file, data, iter);
		// checking for extinction

		if (extinct)
		{
			t = 0;
			if (FoodWeb::prevIteration == 2 && FoodWeb::prevExtinct == iter && Species::nTotal == N_Species - 1)
			{
				while (t < 1e3)
				{
					t += dt;
					dt = RKF45(S, P, dt);
					if (dt < 0.01) { dt = 0.01; }
					// computing time step and setting lower limit

					RK4(S, P, dt);
				}
				rep_per = 1;
			}
			// if food web damped oscillatory before invasion and invasive Species is extinct
			// food web is integrated for sometime before considered as damped oscillatiry if no other Species go extinct
			// implemented for time optimization
			
			
			else if (FoodWeb::prevIteration == 3 && FoodWeb::prevExtinct == iter && Species::nTotal == N_Species - 1)
			{
				while (t < 1e3)
				{
					t += dt;
					dt = RKF45(S, P, dt);
					if (dt < 0.01) { dt = 0.01; }
					// computing time step and setting lower limit

					RK4(S, P, dt);
				}
				rep_per = 1;
			}
			// if food web was periodic before invasion and invasive Species is extinct
			// food web is integrated for sometime before considered as periodic if no other Species go extinct
			// implemented for time optimization


			if (FoodWeb::prevIteration == 3 && FoodWeb::prevExtinct == iter && Species::nTotal == N_Species-1)
			{
				while (t < 1e3)
				{
					t += dt;
					dt = RKF45(S, P, dt);
					if (dt < 0.01) { dt = 0.01; }
					// computing time step and setting lower limit

					RK4(S, P, dt);
				}
				rep_per = 1;
			}
			// if food web was periodic before invasion and invasive Species is extinct
			// food web is integrated for sometime before considered as periodic if no other Species go extinct
			// implemented for time optimization

			else if (FoodWeb::prevIteration == 4 && FoodWeb::prevExtinct == iter && Species::nTotal == N_Species-1)
			{
				while (t < 1e3)
				{
					t += dt;
					dt = RKF45(S, P, dt);
					if (dt < 0.01) { dt = 0.01; }
					// computing time step and setting lower limit

					RK4(S, P, dt);
				}
				rep_per = 1;
			}
			// if food web was chaotic before invasion and invasive Species is extinct
			// food web is integrated for sometime before considered as chaotic if no other Species go extinct
			// implemented for time optimization

		}
		// time optimization if invasive Species went extinct
	}
	// Integrating food web until sustainable behavir is detected
}

