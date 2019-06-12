#pragma once
/*
Definition of species, producer and foodweb classes
*/


// Parameters
const int N = 20;
// maximal number of species allowed in food web


class species
{
public:
	species();						// default constructor
	species(int added_i);			// constructor
	~species();						// destructor

//	counters
	static int n;					// total number of species
	bool primary;					// if producer: 1, else: 0
	int added_i;					// added as number i_added

//	parameters
	double dS, S, a;				// time-derivative, density, decay rate
	double l;						// trophic level
	double consumers[N] = { 0 };	// if resourceed upon by S[i]: consumers = eta
	double resources[N] = { 0 };	// if resourceing upon S[i]: resources[i] = beta*eta
	
//	functions
	void derivative(double strengthen, double weaken);
	// Lotka-Volterra equation for time derivative
	void print_parameters(int index, species S_array[]);
	// printing all paramters
};


class producer : public species
{
public:
	producer();						// default constructor
	producer(int added_i);			// constructor
	~producer();					// destructor

	//	counters
	static int n1;					// total number of producer species

	//	parameters
	double k;						// growth rate

	//	functions
	void derivative(double nutrients, double weaken);
	// Lotka-Voterra equation for time derivative
	void print_parameters(int index, species S_array[]);
	// printing all parameters
};


class foodweb
{
public:
	foodweb();
	~foodweb();

	// characteristics of food web
	static bool feasible;			// true if food web is feasible
	static bool stable;				// true if food web is linearly stable

	// characteristics of last food web
	// used for time optimization
	static int last_iteration;		// behavior after last invasion
	static int last_extinct;		// last species to go extinct
};



