// This is new
#include "food_web.cpp"
#include "time_series.cpp"
#include "linear_stability.cpp"
//:wq#include "species.h"
#include "species.cpp"
#include <iostream>
#include <sstream>

int Species::nTotal = 0;
int Producer::nProducer = 0;
// initializing Species numbers

bool FoodWeb::feasible = 0;
bool FoodWeb::stable = 0;
// default stability and feasibility of food web

int FoodWeb::prevIteration = 0;
int FoodWeb::prevExtinct = 0;
// default behavior of last iteration

double addAttemptMAX = 4;
// max number of addition attempts


int main()
{
	// random seed
	srand(2);

//	FILES
	ofstream stabEigen("data0/eig_stab.txt");		// eigenvalues of linearly stable food web
	ofstream ustabEigen("data0/eig_unstab.txt");		// eigenvalues of linearly unstable food web
	// data is saved as: iteration - number of Species - real - imaginary

	ofstream webData("data0/stab_data.txt");				// behavior of food web
	// data is saved as:
	// iteration - number of Species - feasibility - stability - convergence
	// 1 - steady, 2 - damped oscillations to steady, 3 - periodic, 4 - chaotic, 88 - unknown

	ofstream sFile("data0/species_param.txt");			// Species parameters
	ofstream pFile("data0/producer_param.txt");			// Producer parameters
	// data is saved as:
	// iteration - l - S(0) - k - a - consumer - eta
	// iteration - l - S(0) - a - consumer/resource - eta - beta*eta
	
	// ofstream timefile("timeseries_" + std::to_string(j) + ".txt");
	// time series of chaotic food webs


//	DECLARING ARRAYS
	double steadyStates[nMAX] = {};
	Species S[nMAX];
	Producer P[nMAX];

	//Species S_copy[nMAX];
	//Producer P_copy[nMAX];


//	ADDING FIRST Species
	// declaring Species and updating arrays
	Producer s(0);
	P[0] = s;
	S[0] = s;

	// running food web
	checkFeasibility(S, P, steadyStates, stabEigen, ustabEigen, 0);
	timeSeries(S, P, steadyStates, stabEigen, ustabEigen, webData, 0);
	saveParameters(S, P, sFile, pFile, 0);


//	RUNNING EVOLUTION
	for (int addAttempt = 1; addAttempt < addAttemptMAX; addAttempt++)
	{
		// printing addition attempt
		cout << "\n\n---------------------------\n";
		cout << " " << addAttempt << ". ADDITION ATTEMPT\n";
		cout << "---------------------------\n\n";
		
		// adding invasive species
		addSpecies(S, P, addAttempt);
		updateTrophicLevel(S);
		saveParameters(S, P, sFile, pFile, addAttempt);

		// printing number of species
		cout << "Number of Species in food web " << Species::nTotal;
		cout << ", number of Producers " << Producer::nProducer << endl << endl;

		// running time series
		checkFeasibility(S, P, steadyStates, stabEigen, ustabEigen, addAttempt);
		timeSeries(S, P, steadyStates, stabEigen, ustabEigen, webData, addAttempt);
		updateTrophicLevel(S);
	}
	
	// closing all files
	stabEigen.close();
	ustabEigen.close();
	webData.close();
	sFile.close();
	pFile.close();
	
	return 0;
}
