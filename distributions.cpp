#include "distributions.h"
#include <cstdlib>
#include <cmath>


// UNIFORM DISTRIBUTIONS
double randomDouble(double min, double max)
	{
		return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
	}

int randomInt(int min, int max)
	{
		return min + (rand() * (int)(max - min + 1) / RAND_MAX);
	}

bool ratio(int a, int b)
	{
		int number = randomInt(1, b);
		if(number <= a) { return true; }
		else { return false; }
	}

double normalCDF(double mu, double sigma, double x)
	{
		double error = (x - mu) / (sigma * sqrt(2));
		return 0.5 * erfc(-error);
	}



// SPECIFIC DISTRIBUTIONS FOR MANIPULATION
double kappa()
	{
		return defaultKappa;
	}	

double alpha()
	{
		return randomDouble(0.05, 0.5);
	}
	
double eta()
	{
		return randomDouble(0.05, 1);
	}

double beta()
	{
		return defaultBeta;
	}

bool type()
	{
		return ratio(1,3);
	}



// LINKS
bool addSecondResource()
	{
		return ratio(1,2);
	}

bool addConsumer()
	{
		return false;
	}

