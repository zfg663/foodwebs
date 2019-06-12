#include "time_series.h"
#include "food_web.h"
#include <iostream>
#include <fstream>
using namespace std;

void rk4(species S[], producer P[], double dt)
{
//	INITIALIZING TEMPORARY ARRAYS
	int i; 
	double dS_1[N], dS_2[N], dS_3[N], dS_4[N] = { 0 };
	// temprary time derivatives

	producer P_temp[N];
	species S_temp[N];
	// temporary species arrays

	for (int i = 0; i < species::n; i++)
		{
			P_temp[i] = P[i];
			S_temp[i] = S[i];
		}
	// initializing temporary species arrays


//	1ST ESTIMATE
	derivatives(S, P);
	// computing derivatives

	for (i = 0; i < producer::n1; i++)
		{
			dS_1[i] = S[i].dS * dt;
			// differential step: dS(S_0)/dt * dt

			P_temp[i].S = P[i].S + 0.5 * dS_1[i];
			S_temp[i].S = S[i].S + 0.5 * dS_1[i];
			// updating temporary densities: S_0 + (1/2) * dS_1
		}
	// producers

	for (i = producer::n1; i < species::n; i++)
		{
			dS_1[i] = S[i].dS * dt;
			// differential step: dS(S_0)/dt * dt

			S_temp[i].S = S[i].S + 0.5 * dS_1[i];
			// updating temporary densities: S_0 + (1/2) * dS_1
		}
	// other species



//	2ND ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < producer::n1; i++)
	{
		dS_2[i] = S_temp[i].dS * dt;
		// differential step: dS(S_0 + (1/2) * dS_1)/dt * dt

		P_temp[i].S = P[i].S + 0.5 * dS_2[i];
		S_temp[i].S = S[i].S + 0.5 * dS_2[i];
		// updating temporary densities: S_0 + (1/2) * dS_2
	}
	// producers

	for (i = producer::n1; i < species::n; i++)
	{
		dS_2[i] = S_temp[i].dS * dt;
		// differential step: dS(S_0 + (1/2) * dS_1)/dt * dt

		S_temp[i].S = S[i].S + 0.5 * dS_2[i];
		// updating temporary densities: S_0 + (1/2) * dS_2
	}
	// other species



//	3RD ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < producer::n1; i++)
	{
		dS_3[i] = S_temp[i].dS * dt;
		// differential step: dS(S_0 + (1/2) * dS_2)/dt * dt

		P_temp[i].S = P[i].S + dS_3[i];
		S_temp[i].S = S[i].S + dS_3[i];
		// updating temporary densities: S_0 + dS_3
	}
	// producers

	for (i = producer::n1; i < species::n; i++)
	{
		dS_3[i] = S_temp[i].dS * dt;
		// differential step: dS(S_0 + (1/2) * dS_2)/dt * dt

		S_temp[i].S = S[i].S + dS_3[i];
		// updating temporary densities: S_0 + dS_3
	}
	// other species



//	4TH ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < species::n; i++)
	{
		dS_4[i] = S_temp[i].dS * dt;
		// differential step: dS(S_0 + dS_3)/dt *dt
	}
	// all species



//	CALCULATING S(t+dt)
	for (i = 0; i < producer::n1; i++)
	{
		// cout << "S(t)_" << i << ": " << S[i].S;

		P[i].S += (dS_1[i] + 2 * dS_2[i] + 2 * dS_3[i] + dS_4[i]) / 6;
		S[i].S += (dS_1[i] + 2 * dS_2[i] + 2 * dS_3[i] + dS_4[i]) / 6;

		// cout << ", S(t+dt)_" << i << ": " << S[i].S << endl;
	}
	// computing density of producers

	for (i = producer::n1; i < species::n; i++)
	{
		// cout << "S(t)_" << i << ": " << S[i].S;

		S[i].S += (dS_1[i] + 2 * dS_2[i] + 2 * dS_3[i] + dS_4[i]) / 6;

		// cout << ", S(t+dt)_" << i << ": " << S[i].S << endl;
	}
	// computing density of other species
}

double rkf45_dt(species S[], producer P[], double dt)
{
	double tol = 5e-4;
	// tolerance on time step

	int i, j = 0;
	for (i = 0; i < species::n; i++)
	{
		if (abs(S[i].dS) >= abs(S[j].dS))
		{
			j = i;
		}
	}
	// finding species with largest derivative

	double dS_1[N], dS_2[N], dS_3[N], dS_4[N], dS_5[N], dS_6[N];
	// temporary derivative arrays

	species S_temp[N];
	producer P_temp[N];
	//temporary species arrays

	for (int i = 0; i < N; i++)
	{
		P_temp[i] = P[i];
		S_temp[i] = S[i];
	}
	// initializing temporary arrays


//	FIRST ESTIMATE
	for (i = 0; i < producer::n1; i++)
	{
		dS_1[i] = S[i].dS * dt;

		P_temp[i].S = P_temp[i].S + 0.25 * dS_1[i];
		S_temp[i].S = S_temp[i].S + 0.25 * dS_1[i];
		// updating temporary densities
	}
	// producers

	for (i = producer::n1; i < species::n; i++)
	{
		dS_1[i] = S[i].dS * dt;

		S_temp[i].S = S_temp[i].S + 0.25 * dS_1[i];
		// updating temporary densities
	}
	// other species


//	SECOND ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < producer::n1; i++)
	{
		dS_2[i] = S_temp[i].dS * dt;

		P_temp[i].S = P[i].S + (3 / 32) * dS_1[i] + (9 / 32) *dS_2[i];
		S_temp[i].S = S[i].S + (3 / 32) * dS_1[i] + (9 / 32) *dS_2[i];
		// updating temporary densities
	}
	// producers

	for (i = producer::n1; i < species::n; i++)
	{
		dS_2[i] = S_temp[i].dS * dt;

		S_temp[i].S = S[i].S + (3 / 32) * dS_1[i] + (9 / 32) *dS_2[i];
		// updating temporary densities
	}
	// other species


//	THIRD ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives
	for (i = 0; i < producer::n1; i++)
	{
		dS_3[i] = S_temp[i].dS * dt;

		P_temp[i].S = P[i].S + (1932 / 2197) * dS_1[i] - (7200 / 2197) * dS_2[i] + (7296 / 2197) * dS_3[i];
		S_temp[i].S = S[i].S + (1932 / 2197) * dS_1[i] - (7200 / 2197) * dS_2[i] + (7296 / 2197) * dS_3[i];
		// upddatinf temporary densities
	}
	// producers

	for (i = producer::n1; i < species::n; i++)
	{
		dS_3[i] = S_temp[i].dS * dt;

		S_temp[i].S = S[i].S + (1932 / 2197) * dS_1[i] - (7200 / 2197) * dS_2[i] + (7296 / 2197) * dS_3[i];
		// updating temporary densities
	}
	// other species


//	FORTH ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < producer::n1; i++)
	{
		dS_4[i] = S_temp[i].dS * dt;

		P_temp[i].S = P[i].S + (439 / 216) * dS_1[i] - 8 * dS_2[i] + (3680 / 513) * dS_3[i] - (845 / 4104) * dS_4[i];
		S_temp[i].S = S[i].S + (439 / 216) * dS_1[i] - 8 * dS_2[i] + (3680 / 513) * dS_3[i] - (845 / 4104) * dS_4[i];
		// updating temporary densities
	}
	// producers

	for (i = producer::n1; i < species::n; i++)
	{
		dS_4[i] = S_temp[i].dS * dt;

		S_temp[i].S = S[i].S + (439 / 216) * dS_1[i] - 8 * dS_2[i] + (3680 / 513) * dS_3[i] - (845 / 4104) * dS_4[i];
		// updating temporary densities
	}
	// other species


//	FIFTH ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < producer::n1; i++)
	{
		dS_5[i] = S_temp[i].dS * dt;

		P_temp[i].S = P[i].S - (8 / 27) * dS_1[i] + 2 * dS_2[i] - (3544 / 2565) * dS_3[i] + (1859 / 4104) * dS_4[i] - (11 / 40)*dS_5[i];
		S_temp[i].S = S[i].S - (8 / 27) * dS_1[i] + 2 * dS_2[i] - (3544 / 2565) * dS_3[i] + (1859 / 4104) * dS_4[i] - (11 / 40)*dS_5[i];
		// updating temporary densities
	}
	// producers

	for (i = producer::n1; i < species::n; i++)
	{
		dS_5[i] = S_temp[i].dS * dt;

		S_temp[i].S = S[i].S - (8 / 27) * dS_1[i] + 2 * dS_2[i] - (3544 / 2565) * dS_3[i] + (1859 / 4104) * dS_4[i] - (11 / 40)*dS_5[i];
		// updating temporary densities
	}
	// other species


	//	SIXTH ESTIMATE
	derivatives(S_temp, P_temp);
	// computing derivatives

	for (i = 0; i < species::n; i++)
	{
		dS_6[i] = S_temp[i].dS * dt;
	}
	// all species

	double S_0, S_1;
	S_0 = S[j].S + (16 / 135)*dS_1[j] + (6656 / 12825)*dS_3[j] + (28561 / 56430)*dS_4[j] - (9 / 50)*dS_5[j] + (2 / 55)*dS_6[j];
	S_1 = S[j].S + (25 / 216)*dS_1[j] + (1408 / 2565)*dS_3[j] + (2197 / 4101)*dS_4[j] - 0.2*dS_5[j];
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

bool check_for_extinction(species S[], producer P[], double steady_states[], ofstream& stab_file, ofstream& unstab_file, ofstream& data, int iter)
{
	int k, i = 0;
	for (k = 0; k < species::n; k++)
	{
		if (S[k].S < S[i].S)
		{
			i = k;
		}
	}
	// determining species with the smallest density

	if (S[i].S <=  1e2 * epsilon)
	{
		cout << "Species " << i << " has gone extinct" << endl;
		// printing

		data << 0 << S[i].added_i << endl;
		// save behavior of food web
		
		foodweb::last_extinct = S[i].added_i;
		// updating food web

		remove_species(S, P, i);
		// removing extinct species

		check_for_steady_solution(S, P, steady_states, stab_file, unstab_file, iter);
		// checking if food web has steady state after removal of extinct species
		
		data << iter << " " << species::n << " " << foodweb::feasible << " " << foodweb::stable << " ";
		// saving behavior of new food web
		
		return true;
	}
	// Removing smallest species if it is extinct

	else { return false; }
	// else nothing
}

bool sustainable(species S[], producer P[], double steady_states[], ofstream& data, double t, int iter, bool rep_per)
{
	int steady_count = 0;
	if (rep_per)
	{
		data << foodweb::last_iteration << endl;
		// saving behavior of food web
		return true;
	}
	// if food web was periodic/chaotic before invasion and invasive species went extinct
	// food web is again considered periodic/chaotic

//	LINEARLY STABLE
	else if (foodweb::feasible && foodweb::stable && t < 1e4)
	{
		for (int i = 0; i < species::n; i++)
		{
			if (abs(S[i].S - steady_states[i]) / steady_states[i] <= (1e8)*epsilon)
			{
				steady_count++;
			}
		}
		// counting species with steady density

		if (steady_count == species::n)
		{
			foodweb::last_iteration = 1;
			data << 1 << endl;
			// saving behavior of food web

			return true;
		}
		// if all species have reached steady states

		else { return false; }
	}
	// feasible, linearly stable food webs

	else if (foodweb::feasible && foodweb::stable && t >= 1e4 && t < 5e4)
	{
		for (int i = 0; i < species::n; i++)
		{
			if (abs(S[i].S - steady_states[i]) / steady_states[i] <= (1e12)*epsilon)
			{
				steady_count++;
			}
		}
		// counting species with steady density

		if (steady_count == species::n)
		{
			foodweb::last_iteration = 1;
			data << 1 << endl;
			// saving behavior of food web

			return true;
		}
		// if all species have reached steady states

		else { return false; }
	}
	// feasible, linearly stable food webs that take long to stabilize

	else if (foodweb::feasible && foodweb::stable && t >= 5e4)
	{
		for (int i = 0; i < species::n; i++)
		{
			if (abs(S[i].S - steady_states[i]) / steady_states[i] <= (1e12)*epsilon)
			{
				steady_count++;
			}
		}
		// counting species with steady density

		if (steady_count == species::n)
		{
			foodweb::last_iteration = 1;
			data << 1 << endl;
			// saving behavior of food web

			return true;
		}
		// if all species have reached the steady states

		if (decreasing(S, P, iter))
		{
			foodweb::last_iteration = 2;
			data << 2 << endl;
			// saving behavior of food web
			
			set_densities_steady(steady_states, S, P);
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
					timefile << " " << S[i].S;
				}
				timefile << endl;

				t += dt;
			}
			timefile.close();
			*/

			foodweb::last_iteration = 6;
			data << 6 << endl;
			set_densities_steady(steady_states, S, P);
			return true;
		}
	}
	// feasible, linearly stable food webs that take very long to stabilize


//	LINEARLY UNSTABLE
	else if (foodweb::feasible && !foodweb::stable && t >= 1e5)
	{
		if (periodic(S, P, iter))
		{
			foodweb::last_iteration = 3;
			data << 3 << endl;
			// saving behavior of food web

			return true;
		}
		// checking for periodic oscillations

		else
		{
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
				for (int i = 0; i < N; i++ )
				{
					timefile << " " << S[i].S;
				}
				timefile << endl;
				// saving time series

				t += dt;
			}
			timefile.close();

			foodweb::last_iteration = 9;
			data << 9 << endl;
			// saving behavior of food web

			return true;
		}
		// saving time series and consider chaotic
		// not known if actually sustainable, but implemented to allow evolution to continue
	}

	else { return false; }
}

bool periodic(species S[], producer P[], int iter)
{
	cout << "Checking for periodic oscillations.\n" << endl;
	//	DECLARING VARIABLES
		double time = 1e3;
		double t = 0;
		double dt = 0.01;

		int i = 0;
		int period = 0;
		int counter = 0;
		// counters

		double max_density_1[N] = { 0 };
		double max_density_2[N] = { 0 };
		// arrays for storing max values


	//	FIRST EXTREMAL DENSITIES
		while ( period < 1 && t <= time)
		{
			t += dt;
			counter = 0;
			// reinitializing counter

			dt = rkf45_dt(S, P, dt);
			if (dt < 0.01) { dt = 0.01; }
			rk4(S, P, dt);
			// time evolution

			for (i = 0; i < species::n; i++)
			{
				if (S[i].S > max_density_1[i])
				{
					max_density_1[i] = S[i].S;
				}
				// updating max if new density higher than old

				else if (t > time && S[i].S < max_density_1[i])
				{
					counter++;
				}
				// counting if new density not higher than old
			}
			// Saving and counting species that reached max value

			if (counter == species::n)
			{
				period++;
			}
			// breaking loop if all species has reached max value once

			if (t >= 1e4)
			{
				cout << endl << "ERROR: maximal values not reached within t = 1e4" << endl << endl;
				return false;
			}
			// breaking if cycle takes too long
		
		}


		//	RUNNING SYSTEM FOR A WHILE
		t = 0;
		
		while (t <= 5e3)
		{
			dt = rkf45_dt(S, P, dt);
			if (dt < 0.01) { dt = 0.01; }
			rk4(S, P, dt);
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

			dt = rkf45_dt(S, P, dt);
			if (dt < 0.01) { dt = 0.01; }
			rk4(S, P, dt);
			// time evolution

			for (i = 0; i < species::n; i++)
			{
				if (S[i].S > max_density_2[i])
				{
					max_density_2[i] = S[i].S;
				}
				// updating max if new density higher than old

				else if (t > time && S[i].S < max_density_2[i])
				{
					counter++;
				}
				// counting if new density not higher than old

			}
			// Saving and counting species that reached max value

			if (counter == species::n)
			{
				period++;
			}
			// breaking loop if all species has reached max value once

			if (t >= 1e4)
			{
				cout << endl << "ERROR: maximal values not reached within t = 1e4" << endl << endl;
				return false;
			}
			// breaking if cycle takes too long
		}

		
	//	CHECK FOR PERIODICITY
		counter = 0;
		// reinitializing counter

		for (i = 0; i < species::n; i++)
		{
			cout << "Comparing max1 and max2" << endl;
			cout << max_density_1[i] << " " << max_density_2[i] << endl << endl;

			if ( abs(max_density_1[i] - max_density_2[i]) <= 1e8*epsilon )
			{
				counter++;
			}
		}
		// counting species with max values equal within uncertainty

		if (counter == species::n)
		{
			cout << "System is periodic\n" << endl;
			return true;
		}
		// periodic

		else
		{
			cout << "System is not periodic\n" << endl;
			return false;
		}
		// not periodic

}

bool decreasing(species S[], producer P[], int iter)
{
	cout << "Checking for damped oscillations.\n" << endl;
	//	DECLARING VARIABLES
		double time = 2e3;
		double t = 0;
		double dt = 0.01;

		int i = 0;
		int period = 0;
		int counter;
		// counters

		double max_density_1[N] = { 0 };
		double max_density_2[N] = { 0 };
		// arrays for storing max values


	//	FIRST EXTREMAL DENSITIES
		while (t <= time && period < 1)
		{
			t += dt;
			counter = 0;
			// reinitializing counter

			dt = rkf45_dt(S, P, dt);
			if (dt < 0.01) { dt = 0.01; }
			rk4(S, P, dt);
			// time evolution

			for (i = 0; i < species::n; i++)
			{
				if (S[i].S > max_density_1[i])
				{
					max_density_1[i] = S[i].S;
				}
				// updating max if new density higher than old

				else if (t > time && S[i].S < max_density_1[i])
				{
					counter++;
				}
				// counting if new density not higher than old

			}
			// Saving and counting species that reached max value

			if (counter == species::n)
			{
				period++; 
			}
			// breaking loop if all species has reached max value once

			if (t >= 1e4)
			{
				cout << endl << "ERROR: maximal values not reached within t = 1e4" << endl << endl;
				return false;
			}
			// breaking if cycle takes too long

		}

		
	//	RUNNING SYSTEM FOR A WHILE
		t = 0;
		
		while (t <= 3e3)
		{
			dt = rkf45_dt(S, P, dt);
			if (dt < 0.01) { dt = 0.01; }
			rk4(S, P, dt);
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

			dt = rkf45_dt(S, P, dt);
			if (dt < 0.01) { dt = 0.01; }
			rk4(S, P, dt);
			// time evolution

			for (i = 0; i < species::n; i++)
			{
				if (S[i].S > max_density_2[i])
				{
					max_density_2[i] = S[i].S;
				}
				// updating max if new density higher than old

				else if(t > time && S[i].S < max_density_2[i])
				{
					counter++;
				}
				// counting if new density not higher than old

			}
			// Saving and counting species that reached max value

			if (counter == species::n)
			{
				period++;
			}
			// breaking loop if all species has reached max value once

			if (t >= 1e4)
			{
				cout << endl << "ERROR: maximal values not reached within t = 1e4" << endl << endl;
				return false;
			}
			// breaking if cycle takes too long

		}


	//	CHECK FOR DECREASING OSCILLATIONS
		counter = 0;
		// reinitializing counter

		for (i = 0; i < species::n; i++)
		{
			cout << "Comparing max1 and max2" << endl;
			cout << max_density_1[i] << " " << max_density_2[i] << endl << endl;

			if ( max_density_1[i] - max_density_2[i] > 1e6*epsilon)
			{
				counter++;
			}
		}
		// counting species with max1 larger than max2

		if (counter == species::n)
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

void time_series(species S[], producer P[], double steady_states[], ofstream& stab_file, ofstream& unstab_file, ofstream& data, int iter)
{
	double t = 0;
	double dt = 0.01;

	int N_species = species::n;
	// number of species when invasion starts

	bool rep_per = 0;
	// logic variable used for time optimization if invasive species go extinct
	
	data << iter << " " << species::n << " " << foodweb::feasible << " " << foodweb::stable<< " ";
	// saving behavior of food web
	
	while (!sustainable(S, P, steady_states, data, t, iter, rep_per) && species::n != 0)
	{
		t += dt;
		dt = rkf45_dt(S, P, dt);
		if (dt < 0.01) { dt = 0.01; }
		// computing time step and setting lower limit

		rk4(S, P, dt);
		// performing time step

		bool extinct = check_for_extinction(S, P, steady_states, stab_file, unstab_file, data, iter);
		// checking for extinction

		if (extinct)
		{
			t = 0;

			if (foodweb::last_iteration == 3 && foodweb::last_extinct == iter && species::n == N_species-1)
			{
				while (t < 1e3)
				{
					t += dt;
					dt = rkf45_dt(S, P, dt);
					if (dt < 0.01) { dt = 0.01; }
					// computing time step and setting lower limit

					rk4(S, P, dt);
				}
				rep_per = 1;
			}
			// if food web was periodic before invasion and invasive species is extinct
			// food web is integrated for sometime before considered as periodic if noother species go extinct
			// implemented for time optimization

			else if (foodweb::last_iteration == 9 && foodweb::last_extinct == iter && species::n == N_species-1)
			{
				while (t < 1e3)
				{
					t += dt;
					dt = rkf45_dt(S, P, dt);
					if (dt < 0.01) { dt = 0.01; }
					// computing time step and setting lower limit

					rk4(S, P, dt);
				}
				rep_per = 1;
			}
			// if food web was chaotic before invasion and invasive species is extinct
			// food web is integrated for sometime before considered as chaotic if no other species go extinct
			// implemented for time optimization

		}
		// time optimization if invasive species went extinct
	}
	// Integrating food web until sustainable behavir is detected
}

