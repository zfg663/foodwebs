#include "food_web.h"
#include <iostream>
#include <fstream>
using namespace std;


// ------------------------------------------------------------------------------------------// 
//							FUNCTIONS FOR MANIPULATING FOODWEB								 //
// ------------------------------------------------------------------------------------------//  



//	INTERACTIONS
	bool add_link(species S[], producer P[], int resource, int consumer)
	{
		if (resource == consumer)
		{
			//cout << "Error: species cannot consume itself!" << endl << endl;
			return false;
		}
		else if (S[consumer].primary)
		{
			//cout << "Error: consumer is a producer!\n" << endl << endl;
			return false;
		}
		else if (S[resource].consumers[consumer] != 0 && S[consumer].resources[resource] != 0 || S[resource].resources[consumer] != 0 && S[consumer].consumers[resource] != 0)
		{
			//cout << "Error: there is already a link between these species!" << endl << endl;
			return false;
		}
		// preventing absurd links

		else
		{
			S[resource].consumers[consumer] = eta();
			S[consumer].resources[resource] = eta() * beta();
			// initializing interaction

			if (S[resource].primary)
			{
				P[resource].consumers[consumer] = S[resource].consumers[consumer];
			}
			// transmitting links to producer array

			return true;
		}
	}

	void remove_link(species S[], producer P[], int resource, int consumer)
	{
		if (S[resource].consumers[consumer] == 0 && S[consumer].resources[resource] == 0)
		{
			// cout << "Error: these species are not linked in this direction." << endl;
		}
		else if (S[resource].consumers[consumer] == 0 && S[consumer].resources[resource] != 0 || S[resource].consumers[consumer] != 0 && S[consumer].resources[resource] == 0)
		{
			cout << "Error: mismatched link." << endl;
		}
		// checking if there is a link

		else
		{
			S[resource].consumers[consumer] = 0;
			S[consumer].resources[resource] = 0;
			// setting interaction parameters to zero

			if (S[resource].primary)
			{
				P[resource].consumers[consumer] = S[resource].consumers[consumer];
			}
		}
	}

	void remove_all_links(species S[], producer P[], int index)
	{
		for (int i = 0; i <= species::n; i++)
		{
			remove_link(S, P, index, i);
			// removing consumers

			if (!S[index].primary)
			{
				remove_link(S, P, i, index);
			}
			// removing resources 
		}
	}

	void trophic_level(species S[])
	{
		int counter = producer::n1;
		bool level_unknown[N] = { true };
		// array for keeping track of the number of levels that is yet to be computed

		for (int i = 0; i < producer::n1; i++)
		{
			level_unknown[i] = false;
		}
		// updating the indices of producers in leve_unknown

		while (counter < species::n)
		{
			for (int i = producer::n1; i < species::n; i++)
			{
				double level = 0;
				double max_level = 0;
				double sum_beta = 0;
				double sum_resources = 0;

				double resource_level = 0;
				bool omnivore = false;
				int resources_of_unknown_level = 0;

				for (int j = 0; j < species::n; j++)
				{
					if (level_unknown[j] && S[i].resources[j] > 0)
					{
						resources_of_unknown_level++;
					}
				}
				// Checking if species is consuming species of unknown levels

				if ( resources_of_unknown_level == 0)
				{
					for (int j = 0; j < species::n; j++)
					{
							if (S[i].resources[j] > 0 && resource_level == 0)
							{
								resource_level = S[j].l;
							}
							// saving level of resource for comparison

							else if (S[i].resources[j] > 0 && S[j].l != resource_level)
							{
								omnivore = true;
								break;
							}
							// checking if species consumes resources at different levels
					}
					// checking if omnivore

					for (int j = 0; j < species::n; j++)
					{
						if (S[i].resources[j] > 0)
						{
							sum_beta += S[i].resources[j];
							sum_resources += 1;
							// summing number of resources and interaction strength

							if (S[j].l > max_level)
							{
								max_level = S[j].l;
							}
							// finding highest level of resource
						}
					}
					// finding highest level of resource and sum of interaction.

					if (omnivore)
					{
						for (int j = 0; j < species::n; j++)
						{
								if (S[i].resources[j] > 0)
								{
									level += (S[j].l / (sum_resources * max_level)) * (S[i].resources[j] / sum_beta);
								}
						}
						// computing weighted average to add to max_level
						
						S[i].l = max_level + level;
					}
					// trophic level of omnivores

					else
					{
						S[i].l = max_level + 1;
					}
					// trophic level of non-omnivores

					level_unknown[i] = false;
					counter++;
					// Updating counters
				}
				// Computing level if already computed level of all resources
			}
		}
		// running loop until all species' levels have been calculated
	}



//	ADDING AND REMOVING SPECIES
	int free_index(species S[], producer P[], bool primary)
	{
		if (species::n == N)
		{
			cout << "Error: no free indices/array is too small!" << endl << endl;
		}
		// checking if array is full

		else if (primary && producer::n1 != species::n)
		{
				int new_index = producer::n1;
				// index of new species

				S[species::n] = S[new_index];
				// moving species with index n1 to index n

				for (int j = 0; j < species::n; j++)
				{
					if (S[new_index].consumers[j] != 0)
					{
						S[j].resources[species::n] = S[j].resources[new_index];
						S[j].resources[new_index] = 0;
					}
					// updating consumers

					else if (S[new_index].resources[j] != 0)
					{
						S[j].consumers[species::n] = S[j].consumers[new_index];
						S[j].consumers[new_index] = 0;

						if (S[j].l == 1)
						{
							P[j].consumers[species::n] = P[j].consumers[new_index];
							P[j].consumers[new_index] = 0;
						}
						// transmiting links to producer array if resource is a primary producer
					}
					// updating resources
				}
				// updating links of species n

				return new_index;
		}
		// if new species is a primary producer, and not all species are producers

		else
			{
				return species::n;
			}
		// if new species is not a primary producer, or all species are producers
	}

	void add_species(species S[], producer P[], int iteration)
	{
		bool primary = type();
		int index = free_index(S, P, primary);
		// Determining type and index of invasive species

		if (primary)
		{
			producer s(iteration);
			P[index] = s;
			S[index] = s;
			// declaring species and putting it in arrays
			
			if (species::n > n_min && add_consumer())
			{
				bool link = 0;
				link = add_link(S, P, index, random_int(producer::n1, species::n));
			}
			// adding consumer if criteria fulfilled

			P[index].print_parameters(index, S);
			// printing paramters of new producer
		}
		// new species is a producer

		else
		{
			species s(iteration);
			S[index] = s;
			P[index].primary = 0;
			// declaring species and putting it in array

			bool link = 0;
			while (!link)
			{
				link = add_link(S, P, random_int(0, index), index);
			}
			// adding resource

			link = 0;
			if (species::n > n_min && add_second_resource())
			{
				while (!link)
				{
					link = add_link(S, P, random_int(0, index), index);
				}
			}
			// adding second resource if criteria fulfilled

			link = 0;
			if (species::n >= n_min && add_consumer())
			{
				double max_level = 0;
				for (int j = 0; j < species::n; j++)
				{
					if (S[index].resources[j] > 0)
					{
						if (S[j].l > max_level)
						{
							max_level = S[j].l + 1;
						}
					}
				}
				// finding highest level of resources

				int consumer = random_int(0, index);
				if (S[consumer].l > max_level)
				{
					link = add_link(S, P, index, consumer);
				}
				// adding consumer if criteria fulfilled
				
			}
			// adding consumer if criteria fulfilled

			S[index].print_parameters(index, S);
			// printing species parameters
		}
		// new species is not a producer
	}

	void remove_species(species S[], producer P[], int index)
	{
		if (index >= species::n)
		{
			cout << "Error: this species does not exist" << endl << endl;
		}
		// if species at index "index" doesn't exist

		else
		{
		//	UPDATING NUMBER OF SPECIES
			species::n--;
			if (S[index].primary) { producer::n1--; }

		//	SETTING ALL PARAMETERS TO ZERO
			P[index].k = 0;
			P[index].a = S[index].a = 0;
			P[index].S = S[index].S = 0;
			P[index].dS = S[index].dS = 0;
			P[index].l = S[index].l = -1;

		//	SETTING ALL INTERACTIONS TO ZERO
			remove_all_links(S, P, index);

		//	UPDATING INDICES OF OTHER SPECIES
			
			if (index < producer::n1)
			{
				S[index] = S[producer::n1];
				P[index] = P[producer::n1];
				// moving last producer to empty index

				for (int j = 0; j < N; j++)
				{
					if (S[producer::n1].consumers[j] != 0)
					{
						S[j].resources[index] = S[j].resources[producer::n1];
						S[j].resources[producer::n1] = 0;
					}
				}
				// moving links of consumers
			}
			// producers
			
			if (index != species::n)
			{
				S[index] = S[species::n];
				// moving last species to empty index

				for (int j = 0; j < N; j++)
				{
					if (S[species::n].consumers[j] != 0)
					{
						S[j].resources[index] = S[j].resources[species::n];
						S[j].resources[species::n] = 0;
					}
					// moving consumers

					else if (S[species::n].resources[j] != 0)
					{
						S[j].consumers[index] = S[j].consumers[species::n];
						S[j].consumers[species::n] = 0;

						if (S[j].primary)
						{
							P[j].consumers[index] = S[j].consumers[index];
							P[j].consumers[species::n] = 0;
						}
						// transmitting links to producer array if resource is a primary producer
					}
					// moving resources
				}
				// moving links of resources and consumers
			}
			// other species

		//	REMOVING LAST ARRAY ELEMENTS
			species s;
			producer p;
			P[index] = p;
			S[species::n] = s;
			P[species::n] = p;
			// using default constructors to remove species at last active indices without affecting counters
		}
		// if species does exist
	}
	
	void reinitializing(species S[], species S_copy[], producer P[], producer P_copy[])
	{
		for (int i = N - 1; i = 0; i--)
		{
			remove_species(S, P, i);
		}
		// removing all species

		for (int i = 0; i < N; i++)
		{
			S[i] = S_copy[i];
			P[i] = P_copy[i];
		}
		// reinitializing last iteration
	}


//	COMPUTING DERIVATIVES
	double available_nutrients(producer P[])
	{
		double sum = 1;
		// initial amount of nutrients

		for (int i = 0; i < producer::n1; i++)
		{
			sum -= P[i].S;
		}
		// removing amount of nutrients consumed by the producers

		return sum;
	}

	double strengthen(species S[], int index)
	{
		double sum = 0;
		for (int j = 0; j < species::n; j++)
		{
			sum += S[index].resources[j] * S[j].S;
		}
		// computing how much species is strengthened when consuming its resources

		return sum;
	}

	double weaken(species S[], int index)
	{
		double sum = 0;
		for (int j = 0; j < species::n; j++)
		{
			sum += S[index].consumers[j] * S[j].S;
		}
		// computing how much species is hurt when consumed by its consumers

		return sum;
	}

	void derivatives(species S[], producer P[])
	{
		double nutrients = available_nutrients(P);

		for (int i = 0; i < producer::n1; i++)
		{
			P[i].derivative(nutrients, weaken(S, i));
			S[i].dS = P[i].dS;
		}
		// computing derivatives of producers

		for (int i = producer::n1; i < species::n; i++)
		{
			S[i].derivative(strengthen(S, i), weaken(S, i));
		}
		// computing derivatives of other species
	}


