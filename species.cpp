#include "species.h"
#include "distributions.h"
#include <iostream>

//	Declaring species
	species::species()
	{
		S = 0;
		dS = 0;
		a = 0;
		l = -1;

		primary = 0;
		added_i = -1;
	}
	// default constructor

	species::species(int j) : added_i(j)
	{
		primary = 0;
		n++;

		S = initial_density;
		a = alpha();
		l = 0;
	}
	// constuctor

	species::~species()
	{

	}
	// destructor



//	Declaring producer
	producer::producer()
	{
		primary = 1;
		k = 0;
		l = -1;
		added_i = -1;
	}
	// default constructo

	producer::producer(int j)
	{
		primary = 1;
		added_i = j;
		n1++;
		n++;

		l = 1;
		S = initial_density;
		k = k();
		a = alpha();
	}
	// costructor

	producer::~producer()
	{

	}
	// destructor



//	Time derivatives
	void species::derivative(double strengthen, double weaken)
	{
		dS = S * (strengthen - weaken - a);
	}

	void producer::derivative(double nutrients, double weaken)
	{
		dS = S * (k*nutrients - a - weaken);
	}
	


//	Printing
	void species::print_parameters(int index, species S_array[])
	{
		// Printing parameters
		std::cout << "Species " << index << "\nAdded as #: " << added_i;
		std::cout << "\ntrophic level: " << l << ":\ndensity ";
		std::cout << S << "\nprimary: " << primary << "\na: " << a << std::endl;
		
		// Printing consumers
		std::cout << "Predators:\n";
		for (int i = 0; i < species::n; i++)
		{
			std::cout << i << ": " << consumers[i] << ", ";
		}
		
		// Printing resources
		std::cout << std::endl << "Preys:\n";
		for (int i = 0; i < species::n; i++)
		{
			std::cout << i << ": " << resources[i] << ", ";
		}
		std::cout << std::endl << std::endl;
	}

	void producer::print_parameters(int index, species S_array[])
	{
		// Printing parameters
		std::cout << "Species " << index << "\nAdded as #: " << added_i;
		std::cout << "\ntrophic level: " << l << " \ndensity: " << S;
		std::cout << "\nprimary: " << primary << ":\nk: " << k << ":\na: " << a << std::endl;

		// Printing consumers
		std::cout << "Predators:\n";
		for (int i = 0; i < species::n; i++)
		{
			std::cout << i << ": " << consumers[i] << ", ";
		}
		std::cout << std::endl << std::endl;
	}


//	FOODWEB
	foodweb::foodweb()
	{

	}
	// contructor

	foodweb::~foodweb()
	{

	}
	// destructor
