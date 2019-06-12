#include "food_web.h"
#include "time_series.h"
#include <iostream>
#include <sstream>

int species::n = 0;
int producer::n1 = 0;
// number of species

bool foodweb::feasible = 0;
bool foodweb::stable = 0;
// characteristics of food web

int foodweb::last_iteration = 0;
int foodweb::last_extinct = 0;
// behavior of last iteration


int j = 0;
double j_max = 1e5;
// first and last iteration

int main()
{	

//	FILES
	ofstream st_eig("eig_stab.txt");			// eigenvalues of linearly stable food web
	ofstream ust_eig("eig_unstab.txt");			// eigenvalues of linearly unstable food web
	// iteration - number of species - real - imaginary

	ofstream data("stab_data.txt");				// behavior of food web
	// iteration - number of species - feasibility - stability - convergence

	ofstream sfile("species_par.txt");			// species parameters
	ofstream pfile("producer_par.txt");			// producer parameters
	// iteration - l - S(0) - k - a - consumer - eta
	// iteration - l - S(0) - a - consumer/resource - eta - beta*eta
	
	ofstream timefile("timeseries_" + std::to_string(j) + ".txt");
	// time series of chaotic food webs


//	DECLARING ARRAYS
	double steady_states[N] = { 0 };
	species S[N];
	producer P[N];

	species S_copy[N];
	producer P_copy[N];


//	ADDING FIRST SPECIES
	producer s(0);
	P[0] = s;
	S[0] = s;
	// declaring species and updating arrays
	
	check_for_steady_solution(S, P, steady_states, st_eig, ust_eig, 0);
	time_series(S, P, steady_states, st_eig, ust_eig, data, 0);
	save_parameters(S, P, sfile, pfile, 0);
	// running food web



//	RUNNING EVOLUTION
	for (j =1; j < j_max; j++)
	{
		add_species(S, P, j);
		// add invasive species

		trophic_level(S);
		save_parameters(S, P, sfile, pfile, j);

		cout << endl << endl << "---------------------------" << endl;
		cout << " " << j << " TIME A SPECIES IS ADDED" << endl;
		cout << "---------------------------" << endl << endl;
		// printing

		check_for_steady_solution(S, P, steady_states, st_eig, ust_eig, j);
		time_series(S, P, steady_states, st_eig, ust_eig, data, j);

		trophic_level(S);
		// updating trophic levels in case of extinction

	}


	st_eig.close();
	ust_eig.close();
	data.close();
	sfile.close();
	pfile.close();

	
	return 0;
}