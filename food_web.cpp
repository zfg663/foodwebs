#include "food_web.h"
#include <iostream>
#include <fstream>
#include <algorithm> 
using namespace std;


// ------------------------------------------------------------------------------------------// 
//							FUNCTIONS FOR MANIPULATING FOODWEB								 //
// ------------------------------------------------------------------------------------------//  



//	INTERACTIONS
	bool addLink(Species S[], Producer P[], int resource, int consumer)
	{
		if (resource == consumer)
		{
			//cout << "Error: Species cannot consume itself!" << endl << endl;
			return false;
		}
		else if (S[consumer].isProducer)
		{
			//cout << "Error: consumer is a Producer!\n" << endl << endl;
			return false;
		}
		else if (S[resource].consumers[consumer] != 0 && S[consumer].resources[resource] != 0 || S[resource].resources[consumer] != 0 && S[consumer].consumers[resource] != 0)
		{
			//cout << "Error: there is already a link between these Species!" << endl << endl;
			return false;
		}
		// preventing absurd links

		else
		{
			double consumed = eta();
			double consuming = consumed * beta();

			S[resource].consumers[consumer] = consumed;
			S[consumer].resources[resource] = consuming;
			// initializing interaction

			if (S[resource].isProducer)
			{
				P[resource].consumers[consumer] = S[resource].consumers[consumer];
			}
			// transmitting links to Producer array

			return true;
		}
	}


	void removeLinks(Species S[], Producer P[], int index)
	{
		if (S[index].isProducer) 
		{
			for (int i=Producer::nProducer; i<Species::nTotal; i++) 
			{
				if (S[index].consumers[i]*S[i].resources[index] == 0 && S[index].consumers[i]+S[i].resources[index] != 0) { cout << "Error: mismatched link." << endl; }
				// checking for errondous links

				else {
					S[index].consumers[i] = 0;
					P[index].consumers[i] = 0;
					S[i].resources[index] = 0;
				}
				// removing all consumers
			}
		}

		else 
		{
			for (int i=0; i<Species::nTotal; i++) 
			{
				if (S[i].consumers[index]*S[index].resources[i]==0 && S[i].consumers[index]+S[index].resources[i]!=0) { cout << "Error: mismatched link." << endl; }
				else if (S[index].consumers[i]*S[i].resources[index]==0 && S[index].consumers[i]+S[i].resources[index]!=0) { cout << "Error: mismatched link." << endl; }
				// checking for errendous links

				else {
					S[index].consumers[i] = 0;
					S[i].resources[index] = 0;
					// remving all consumers

					S[i].consumers[index] = 0;
					P[i].consumers[index] = 0;
					S[index].resources[i] = 0;
					// removing resources 
				}
			}
		}
	}

	void updateTrophicLevel(Species S[])
	{

	//	UPDATING LEVELS OF ProducerS
		int counter = Producer::nProducer;
		// counting species of known level

		bool levelUnknown[nMAX] = {};
		std::fill_n(levelUnknown + counter, Species::nTotal - counter, 1);
		// array for keeping track of the number of levels that is yet to be computed


	//	UPDATING LEVEL OF OTHER Species
		while (counter < Species::nTotal)
		{
			for (int i = Producer::nProducer; i < Species::nTotal; i++)
			{
				if ( levelUnknown[i] )
				{
					double sum_eta_l = 0;
					double sum_eta = 0;
					double sum_resources = 0;
					int resources_of_unknown_level = 0;
					// resetting sums to zero

					for (int j = 0; j < Species::nTotal; j++)
					{
						if (levelUnknown[j]*S[i].resources[j] > 0)
						{
							resources_of_unknown_level++;
							break;
						}
					}
					// Checking if species is consuming species of unknown levels

					if (resources_of_unknown_level == 0)
					{
						for (int j = 0; j < Species::nTotal; j++)
						{
							if (S[i].resources[j] > 0)
							{
								sum_eta_l += S[j].consumers[i] * S[j].level;
								sum_eta += S[j].consumers[i];
								sum_resources++;
							}
						}
						// updating sums

						S[i].level = 1 + sum_eta_l / (sum_resources * sum_eta);
						// computing level

						levelUnknown[i] = false;
						counter++;
						// updating counters
					}
					// Computing level if already computed level of all resources
				}
				// if level of species is yet to be computed
			}
		}
		// running loop until all species' levels have been calculated
	}



//	ADDING AND REMOVING Species
	int freeIndex(Species S[], Producer P[], bool isProducer)
	{
		if (Species::nTotal == nMAX)
		{
			cout << "Error: no free indices/array is too small!" << endl << endl;
			return -1;
		}
		// checking if array is full

		else if (isProducer && Producer::nProducer != Species::nTotal)
		{
				int newIndex = Producer::nProducer;
				// index of new species

				S[Species::nTotal] = S[newIndex];
				// moving species with index nProducer to index n

				for (int j = 0; j < Species::nTotal; j++)
				{
					if (S[newIndex].consumers[j] != 0)
					{
						S[j].resources[Species::nTotal] = S[j].resources[newIndex];
						S[j].resources[newIndex] = 0;
					}
					// updating consumers

					else if (S[newIndex].resources[j] != 0)
					{
						S[j].consumers[Species::nTotal] = S[j].consumers[newIndex];
						S[j].consumers[newIndex] = 0;

						if (S[j].level == 1)
						{
							P[j].consumers[Species::nTotal] = P[j].consumers[newIndex];
							P[j].consumers[newIndex] = 0;
						}
						// transmiting links to producer array if resource is a isProducer producer
					}
					// updating resources
				}
				// updating links of Species n

				return newIndex;
		}
		// if new Species is a isProducer Producer, and not all Species are Producers

		else
			{
				return Species::nTotal;
			}
		// if new Species is not a isProducer Producer, or all Species are Producers
	}

	void addSpecies(Species S[], Producer P[], int addAttempt)
	{
		bool isProducer = type();
		int index = freeIndex(S, P, isProducer);
		// Determining type and index of invasive Species

		if (isProducer)
		{
			Producer s(addAttempt);
			P[index] = s;
			S[index] = s;
			// declaring Species and putting it in arrays
			
			if (Species::nTotal > nMin && addConsumer())
			{
				bool link = 0;
				link = addLink(S, P, index, randomInt(Producer::nProducer, Species::nTotal));
			}
			// adding consumer if criteria fulfilled

			//P[index].printParameters(index);
			// printing paramters of new Producer
		}
		// new Species is a Producer

		else
		{
			Species s(addAttempt);
			S[index] = s;
			P[index].isProducer = 0;
			// declaring Species and putting it in array

			bool link = 0;
			while (!link)
			{
				link = addLink(S, P, randomInt(0, index), index);
			}
			// adding resource

			link = 0;
			if (Species::nTotal > nMin && addSecondResource())
			{
				while (!link)
				{
					link = addLink(S, P, randomInt(0, index), index);
				}
			}
			// adding second resource if criteria fulfilled

			link = 0;
			if (Species::nTotal >= nMin && addConsumer())
			{
				double max_level = 0;
				for (int j = 0; j < Species::nTotal; j++)
				{
					if (S[index].resources[j] > 0)
					{
						if (S[j].level > max_level)
						{
							max_level = S[j].level + 1;
						}
					}
				}
				// finding highest level of resources

				int consumer = randomInt(0, index);
				if (S[consumer].level > max_level)
				{
					link = addLink(S, P, index, consumer);
				}
				// adding consumer if criteria fulfilled
				
			}
			// adding consumer if criteria fulfilled

			//S[index].printParameters(index);
			// printing Species parameters
		}
		// new Species is not a Producer
	}

	void removeSpecies(Species S[], Producer P[], int index)
	{
		if (index >= Species::nTotal)
		{
			cout << "Error: this Species does not exist" << endl << endl;
		}
		// if Species at index "index" doesn't exist

		else
		{
		//	UPDATING NUMBER OF Species
			Species::nTotal--;
			if (S[index].isProducer) { Producer::nProducer--; }

		//	SETTING ALL PARAMETERS TO ZERO
			P[index].growth = 0;
			P[index].decay = S[index].decay = 0;
			P[index].density = S[index].density = 0;
			P[index].dS = S[index].dS = 0;
			P[index].level = S[index].level = -1;

		//	SETTING ALL INTERACTIONS TO ZERO
			removeLinks(S, P, index);

		//	UPDATING INDICES OF OTHER Species
			
			if (index < Producer::nProducer)
			{
				S[index] = S[Producer::nProducer];
				P[index] = P[Producer::nProducer];
				cout << "Species " << Producer::nProducer << " replaced Species " << index << " in the array.\n";
				// moving last Producer to empty index

				for (int j = 0; j < nMAX; j++)
				{
					if (S[Producer::nProducer].consumers[j] != 0)
					{
						S[j].resources[index] = S[j].resources[Producer::nProducer];
						S[j].resources[Producer::nProducer] = 0;
					}
				}
				// moving links of consumers
				index = Producer::nProducer;
			}
			// Producers
			
			if (index != Species::nTotal)
			{
				S[index] = S[Species::nTotal];
				cout << "Species " << Species::nTotal << " replaced Species " << index << " in the array.\n";
				// moving last Species to empty index

				for (int j = 0; j < nMAX; j++)
				{
					if (S[Species::nTotal].consumers[j] != 0)
					{
						S[j].resources[index] = S[j].resources[Species::nTotal];
						S[j].resources[Species::nTotal] = 0;
					}
					// moving consumers

					else if (S[Species::nTotal].resources[j] != 0)
					{
						S[j].consumers[index] = S[j].consumers[Species::nTotal];
						S[j].consumers[Species::nTotal] = 0;

						if (S[j].isProducer)
						{
							P[j].consumers[index] = S[j].consumers[index];
							P[j].consumers[Species::nTotal] = 0;
						}
						// transmitting links to Producer array if resource is a isProducer Producer
					}
					// moving resources
				}
				// moving links of resources and consumers
			}
			// other Species

		//	REMOVING LAST ARRAY ELEMENTS
			Species s;
			Producer p;
			P[index] = p;
			S[Species::nTotal] = s;
			P[Species::nTotal] = p;
			// using default constructors to remove Species at last active indices without affecting counters
		}
		// if Species does exist

		/*for (int i = 0; i < Species::nTotal; i++)
		{
			S[i].print_parameters(i, S);
		}*/
	}
	
	/*
	void reinitializing(Species S[], Species S_copy[], Producer P[], Producer P_copy[])
	{
		for (int i = N - 1; i = 0; i--)
		{
			remove_Species(S, P, i);
		}
		// removing all Species

		for (int i = 0; i < N; i++)
		{
			S[i] = S_copy[i];
			P[i] = P_copy[i];
		}
		// reinitializing last addAttempt
	}
	*/

//	COMPUTING DERIVATIVES
	double availableNutrients(Producer P[])
	{
		double sum = 1;
		// initial amount of nutrients

		for (int i = 0; i < Producer::nProducer; i++)
		{
			sum -= P[i].density;
		}
		// removing amount of nutrients consumed by the Producers

		return sum;
	}

	double strengthen(Species S[], int index)
	{
		double sum = 0;
		for (int j = 0; j < Species::nTotal; j++)
		{
			sum += S[index].resources[j] * S[j].density;
		}
		// computing how much Species is strengthened when consuming its resources

		return sum;
	}

	double weaken(Species S[], int index)
	{
		double sum = 0;
		for (int j = 0; j < Species::nTotal; j++)
		{
			sum += S[index].consumers[j] * S[j].density;
		}
		// computing how much Species is hurt when consumed by its consumers

		return sum;
	}

	void derivatives(Species S[], Producer P[])
	{
		double nutrients = availableNutrients(P);

		for (int i = 0; i < Producer::nProducer; i++)
		{
			P[i].derivative(nutrients, weaken(S, i));
			S[i].dS = P[i].dS;
		}
		// computing derivatives of Producers

		for (int i = Producer::nProducer; i < Species::nTotal; i++)
		{
			S[i].derivative(strengthen(S, i), weaken(S, i));
		}
		// computing derivatives of other Species
	}


