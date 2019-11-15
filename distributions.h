#pragma once
/* CONSTANTS AND DISTRIBUTIONS CONTROLLING EVOLUTION OF FOOD WEB*/


// CONSTANTS
// default Species parameters
const double initialDensity = 1e-10;
const double defaultBeta = .75;
const double defaultKappa = 1;

// default threshold
const double epsilon = 1e-14;

// lower limit on Species richness before allowing Species with two resources
const int nMin = 3;


//DISTRIBUTIONS
// random numbers in [min, max]
double randomDouble(double min, double max);
int randomInt(int min, int max);

// returning true with probability a/b
bool ratio(int a, int b);

// Cumulative density of normal distribution
double normalCDF(double mu, double sigma, double x);


// Species PARAMETER DISTRIUTIONS
double kappa();		// growth rate
double alpha();		// decay rate
double eta();		// interaction strength
double beta();		// reproduction efficiency
			
bool type();		// determines type of invasive Species: 
					// Producer if true, Species at l>1 if false


// LINK DISTRIBUTIONS
// probability of Species having two preys
bool addSecondResource();

// determines if invasive Species should also establish link to consumer
// not used in final project
bool addConsumer();

