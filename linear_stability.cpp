#include "food_web.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>


//	COMPUTING STEADY STATES
	MatrixXd initR(MatrixXd R, Species S[], Producer P[])
	{
		int i, j, k;
		// i - row number, j - column number

		for (i = 0; i < Producer::nProducer; i++)
		{
			for (j = 0; j < Producer::nProducer; j++)
			{
				R(i, j) = 1;								
				//cout << "R(" << i << "," << j << "): " << R(i, j) << endl;
			}
			// Producer x Producer entries, nProducer x nProducer

			for (k = Producer::nProducer; k < Species::nTotal; k++)
			{
				R(i, k) = P[i].consumers[k] / P[i].growth;
				//cout << "R(" << i << "," << k << "): " << R(i, k) << endl;
			}
			// Producer x Species entries, nProducer x (N - nProducer)
		}
		// filling rows of Producers, nProducer x N

		for (k = Producer::nProducer; k < Species::nTotal; k++)
		{
			for (j = 0; j < Species::nTotal; j++)
			{
				R(k, j) = S[k].resources[j] - S[k].consumers[j];
				//cout << "R(" << k << "," << j << "): " << R(k, j) << endl;
			}
		}
		// filling rows of Species, (N - nProducer) x N (row (nProducer + 1) to N)

		return R;
	}

	VectorXd initK(VectorXd K, Species S[], Producer P[])
	{
		for (int i = 0; i < Producer::nProducer; i++)
		{
			K(i) = (P[i].growth - P[i].decay) / P[i].growth;
		}
		// filling entries of Producers

		for (int k = Producer::nProducer; k < Species::nTotal; k++)
		{
			K(k) = S[k].decay;
		}
		// filling entries of Species

		return K;
	}



//	CHECKING FOR FEASIBILITY AND STABILITY
	bool feasible(VectorXd S)
	{
		int S_positive = 0;
		// counter

		for (int i = 0; i < Species::nTotal; i++)
		{
			if (S(i) > epsilon)
			{
				S_positive++;
			}
		}
		// counting

		if (S_positive == Species::nTotal) { return true; }
		// if all Species has a positive steady state

		else { return false; }
	}

	bool negativeRealValues(VectorXcd eigenvalues)
	{
		int S_stable = 0;
		// counter 

		for (int i = 0; i < Species::nTotal; i++)
		{
			if (eigenvalues(i).real() < 0)
			{
				S_stable++;
			}
		}
		// counting

		if (S_stable == Species::nTotal) { return true; }
		// if all Species have negative real parts

		else { return false; }
	}



//	INITIALIZING COMMUNITY MATRIX
	void setDensitiesSteady(double steadyStates[], Species S_temp[], Producer P_temp[])
	{
		int i;
		for (i = 0; i < Producer::nProducer; i++)
		{
			S_temp[i].density = steadyStates[i];
			P_temp[i].density = S_temp[i].density;
		}
		// densities of Producers

		for (i = Producer::nProducer; i < Species::nTotal; i++)
		{
			S_temp[i].density = steadyStates[i];
		}
		// densities of other Species
	}

	MatrixXd Community(MatrixXd C, Species S[], Producer P[])
	{
		int i, j, k;
		double nutrients = availableNutrients(P);

		for (i = 0; i < Producer::nProducer; i++)
		{
			for (j = 0; j < Producer::nProducer; j++)
			{
				if (i == j)
				{
					C(i,i) = P[i].growth * (nutrients - P[i].density) - P[i].decay - weaken(S, i);
					//cout << "C(" << i << "," << j << "): " << C(i,j) << endl;
				}
				// filling diagonal entries

				else
				{
					C(i,j) = -P[i].growth * P[i].density;
					//cout << "C(" << i << "," << j << "): " << C(i,j) << endl;
				}
				// filling non-diagonal entries
			}
			// Producer x Producer, nProducer x nProducer

			for (k = Producer::nProducer; k < Species::nTotal; k++)
			{
				C(i,k) = -P[i].consumers[k] * P[i].density;
				//cout << "C(" << i << "," << k << "): " << C(i, k) << endl;
			}
			// Producer x Species, nProducer x(N - nProducer)
		}
		// filling rows of Producers, nProducer x N

		for (k = Producer::nProducer; k < Species::nTotal; k++)
		{
			for (j = 0; j < Species::nTotal; j++)
			{
				if (k == j)
				{
					C(k,k) = strengthen(S, k) - weaken(S, k) - S[k].decay;
					//cout << "C(" << k << "," << j << "): " << C(k,j) << endl;
				}
				// filling diagonal entries

				else
				{
					C(k,j) = (S[k].resources[j] - S[k].consumers[j]) * S[k].density;
					//cout << "C(" << k << "," << j << "): " << C(k,j) << endl;
				}
				// filling non-diagonal entries
			}
		}
		// filling rows of Species, (N - nProducer) x N (row (nProducer + 1) to N)

		return C;
	}



//	SAVING PARAMETERS
	void saveEigenvalues(VectorXcd eigenvalues, ofstream& file, int addAttempt)
	{
		file << addAttempt << " " << Species::nTotal << " ";
		// addAttempt and number of Species

		for (int i = 0; i < Species::nTotal; i++)
		{
			file << eigenvalues(i).real() << " " << eigenvalues(i).imag() << " ";
		}
		// eigenvalues of all Species in food webs

		for (int i = Species::nTotal; i < nMAX; i++)
		{
			file << 0 << " " << 0 << " ";
		}
		// zeroes for all empty entries in S[] and P[]

		file << endl;
	}

	void saveParameters(Species S[], Producer P[], ofstream& sFile, ofstream& pFile, int addAttempt)
	{
		// saving parameters of Producers
		for (int i = 0; i < Producer::nProducer; i++)
		{
			pFile << addAttempt << " ";
			// addAttempt

			pFile << i << " " << P[i].addAttempt << " " << P[i].level << " " << P[i].density << " " << P[i].growth << " " << P[i].decay << " ";
			// index - addAttempt of invasion - level - final density - growth rate - decay rate

			for (int k = 0; k < nMAX; k++)
			{
				pFile << k << " " << P[i].consumers[k] << " ";
			}
			// index of other Species - interaction strength

			pFile << endl;
		}

		// saving parameters of Species
		for (int k = Producer::nProducer; k < Species::nTotal; k++)
		{
			sFile << addAttempt << " ";
			// addAttempt

			sFile << k << " " << S[k].addAttempt << " " << S[k].level << " " << S[k].density << " " << S[k].decay << " ";
			// index - addAttempt of invasion - level - final density - decay rate

			
			for (int j = 0; j < nMAX; j++)
			{
				sFile << j << " " << S[k].consumers[j] << " " << S[k].resources[j] << " ";
			}
			// index of other Species - interaction strength(as resource) - interaction strength(as consumer)

			sFile << endl;
		}
	}



//	INVESTIGATING LINEAR STABILITY
	void checkFeasibility(Species S[], Producer P[], double steadyStates[], ofstream& stabEigen, ofstream& unstabEigen, int addAttempt)
	{
	//	COMPUTING S* = R^(-1)*k
		// declaring vectors and matrices
		MatrixXd R(Species::nTotal, Species::nTotal);
		VectorXd K(Species::nTotal);
		VectorXd Ssteady(Species::nTotal);

		// initializing R and K
		R = initR(R, S, P);				//cout << R << endl;
		K = initK(K, S, P);				//cout << K << endl;

		// computing steady states
		Ssteady = R.inverse() * K;
		std::fill_n(steadyStates, nMAX, 0);
		for (int i=0; i<Species::nTotal; i++) {
			steadyStates[i] = Ssteady(i);
		}

		//cout << "checking STEADY states \n";
		//for (int i = 0; i < Species::nTotal; i++)
		//{
		//	std::cout << steadyStates[i] << endl;
		//	std::cout << Ssteady(i) << endl;
			//cout << "print";
		//}
		

		
		for (int i = 0; i < Species::nTotal; i++)
		{
			steadyStates[i] = Ssteady(i);
		}
		// saving steady states

		for (int i = Species::nTotal; i < nMAX; i++)
		{
			steadyStates[i] = 0;
		}
		// zeroes for empty entries in S[] and P[]
		

	//	CHECKING IF SYSTEM IS FEASIBLE AND LINEARLY STABLE
		if (feasible(Ssteady))
		{
			// updating feasibility of food web
			FoodWeb::feasible = true;

			// checking for linear stability
			checkLinearStability(S, P, steadyStates, stabEigen, unstabEigen, addAttempt);

			// printing steady states
			cout << "The system does have a feasible steady state!\n";
			cout << "---------------\n Steady states: \n---------------\n";
			cout << Ssteady << "\n---------------\n\n";		
		}
		else
		{
			// updating feasibility and stability of food web
			FoodWeb::feasible = false;
			FoodWeb::stable = false;

			// printing feasibility
			cout << "The system does not have a feasible steady state\n\n";
		}
	}

	void checkLinearStability(Species S[], Producer P[], double steadyStates[], ofstream& stabEigen, ofstream& unstabEigen, int addAttempt)
	{
	//	INITIALIZING
		// declaring matrix and arrays
		MatrixXd C(Species::nTotal, Species::nTotal);
		VectorXcd eigenvalues(Species::nTotal);
		Species S_temp[nMAX];
		Producer P_temp[nMAX];

		// initializing temporary species arrays and setting density to steady state
		for (int i=0; i<nMAX; i++) {
			P_temp[i] = P[i];
			S_temp[i] = S[i];
		}
		setDensitiesSteady(steadyStates, S_temp, P_temp);

		
	//	COMPUTING JACOBIAN AND EIGENVALUES
		// initializing Jacobian
		C = Community(C, S_temp, P_temp);

		// computing eigenvalues of Jacobian
		EigenSolver<MatrixXd> es(C);
		eigenvalues = es.eigenvalues();

		
	//	CHECKING LINEAR STABILITY
		if (negativeRealValues(eigenvalues))
		{
			// updating stability of food web
			FoodWeb::stable = true;

			cout << "The steady states are linearly stable!\n\n";
			saveEigenvalues(eigenvalues, stabEigen, addAttempt);
		}
		// if linearly stable

		else
		{
			// updating stability of food web
			FoodWeb::stable = false;

			cout << "The steady states are not linearly stable.\n\n";
			saveEigenvalues(eigenvalues, unstabEigen, addAttempt);
		}
	}
