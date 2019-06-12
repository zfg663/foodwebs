#include "distributions.h"
#include <cstdlib>
#include <cmath>


//	UNIFORM DISTRIBUTIONS
	double random_double(double min, double max)
	{
		return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
	}

	int random_int(int min, int max)
	{
		return min + (rand() * (int)(max - min + 1) / RAND_MAX);
	}

	bool ratio(int a, int b)
	{
		int number = random_int(1, b);
		if(number <= a) { return true; }
		else { return false; }
	}

	double normal_cdf(double mu, double sigma, double x)
	{
		double error = (x - mu) / (sigma * sqrt(2));
		return 0.5 * erfc(-error);
	}



//	SPECIFIC DISTRIBUTIONS FOR MANIPULATION
	double k()
	{
		return default_k;
	}
	
	double alpha()
	{
		return random_double(0.05, 0.5);
	}
	
	double eta()
	{
		return random_double(0.05, 1);
	}

	double beta()
	{
		return default_beta;
	}

	bool type()
	{
		return ratio(1,3);
	}



//	LINKS
	bool add_second_resource()
	{
		return ratio(1,2);
	}

	bool add_consumer()
	{
		return false;
	}

