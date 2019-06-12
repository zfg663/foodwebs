#pragma once
/*
Constants and distributions for controlling the evolution of the food web. 
*/


//	CONSTANTS
	const double initial_density = 1e-10;
	const double default_beta = .75;
	const double default_k = 1;
	// default species parameters

	const int n_min = 3;
	// lower limit on food web size
	// allowing a second link in food webs larger than n_two_links

	const double epsilon = 1e-14;
	// threshold/uncertainty on data



//	DISTRIBUTIONS
	double random_double(double min, double max);
	// returns random double in [min, max]

	int random_int(int min, int max);
	// returns random integer in [min, max]

	bool ratio(int a, int b);
	// returning true with probability a/b

	double normal_cdf(double mu, double sigma, double x);
	// Cumulative density of normal distribution
	// Not used in final project



//	SPECIES PARAMETER DISTRIUTIONS
	double k();
	// growth rate
	
	double alpha();
	// decay rate
	
	double eta();
	// interaction strength

	double beta();
	// reproduction efficiency

	bool type();
	// determines type of invasive species
	// producer if true, species at l>1 if false



//	LINK DISTRIBUTIONS
	bool add_second_resource();
	// probability of species having two preys

	bool add_consumer();
	// determines if invasive species should also establish link to consumer
	// not used in final project
