#include "food_web.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>



//	COMPUTING STEADY STATES
	MatrixXd initR(MatrixXd R, species S[], producer P[])
	{
		int i, j, k;
		// i - row number, j - column number

		for (i = 0; i < producer::n1; i++)
		{
			for (j = 0; j < producer::n1; j++)
			{
				R(i, j) = 1;								
				//cout << "R(" << i << "," << j << "): " << R(i, j) << endl;
			}
			// producer x producer entries, n1 x n1

			for (k = producer::n1; k < species::n; k++)
			{
				R(i, k) = P[i].consumers[k] / P[i].k;
				//cout << "R(" << i << "," << k << "): " << R(i, k) << endl;
			}
			// producer x species entries, n1 x (N - n1)
		}
		// filling rows of producers, n1 x N

		for (k = producer::n1; k < species::n; k++)
		{
			for (j = 0; j < species::n; j++)
			{
				R(k, j) = S[k].resources[j] - S[k].consumers[j];
				//cout << "R(" << k << "," << j << "): " << R(k, j) << endl;
			}
		}
		// filling rows of species, (N - n1) x N (row (n1 + 1) to N)

		return R;
	}

	VectorXd initK(VectorXd K, species S[], producer P[])
	{
		for (int i = 0; i < producer::n1; i++)
		{
			K(i) = (P[i].k - P[i].a) / P[i].k;
		}
		// filling entries of producers

		for (int k = producer::n1; k < species::n; k++)
		{
			K(k) = S[k].a;
		}
		// filling entries of species

		return K;
	}



//	CHECKING FOR FEASIBILITY AND STABILITY
	bool feasible(VectorXd S)
	{
		int S_positive = 0;
		// counter

		for (int i = 0; i < species::n; i++)
		{
			if (S(i) > epsilon)
			{
				S_positive++;
			}
		}
		// counting

		if (S_positive == species::n) { return true; }
		// if all species has a positive steady state

		else { return false; }
	}

	bool negative_real_values(VectorXcd eigenvalues)
	{
		int S_stable = 0;
		// counter 

		for (int i = 0; i < species::n; i++)
		{
			if (eigenvalues(i).real() < 0)
			{
				S_stable++;
			}
		}
		// counting

		if (S_stable == species::n) { return true; }
		// if all species have negative real parts

		else { return false; }
	}



//	INITIALIZING JACOBIAN
	void set_densities_steady(double steady_states[], species S_temp[], producer P_temp[])
	{
		int i;
		for (i = 0; i < producer::n1; i++)
		{
			S_temp[i].S = steady_states[i];
			P_temp[i].S = S_temp[i].S;
		}
		// densities of producers

		for (i = producer::n1; i < species::n; i++)
		{
			S_temp[i].S = steady_states[i];
		}
		// densities of other species
	}

	MatrixXd Jacobian(MatrixXd J, species S[], producer P[])
	{
		int i, j, k;
		double nutrients = available_nutrients(P);

		for (i = 0; i < producer::n1; i++)
		{
			for (j = 0; j < producer::n1; j++)
			{
				if (i == j)
				{
					J(i, i) = P[i].k * (nutrients - P[i].S) - P[i].a - weaken(S, i);
					//cout << "J(" << i << "," << j << "): " << J(i, j) << endl;
				}
				// filling diagonal entries

				else
				{
					J(i, j) = -P[i].k * P[i].S;
					//cout << "J(" << i << "," << j << "): " << J(i, j) << endl;
				}
				// filling non-diagonal entries
			}
			// producer x producer, n1 x n1

			for (k = producer::n1; k < species::n; k++)
			{
				J(i, k) = -P[i].consumers[k] * P[i].S;
				//cout << "J(" << i << "," << k << "): " << J(i, k) << endl;
			}
			// producer x species, n1 x(N - n1)
		}
		// filling rows of producers, n1 x N

		for (k = producer::n1; k < species::n; k++)
		{
			for (j = 0; j < species::n; j++)
			{
				if (k == j)
				{
					J(k, k) = strengthen(S, k) - weaken(S, k) - S[k].a;
					//cout << "J(" << k << "," << j << "): " << J(k, j) << endl;
				}
				// filling diagonal entries

				else
				{
					J(k, j) = (S[k].resources[j] - S[k].consumers[j]) * S[k].S;
					//cout << "J(" << k << "," << j << "): " << J(k, j) << endl;
				}
				// filling non-diagonal entries
			}
		}
		// filling rows of species, (N - n1) x N (row (n1 + 1) to N)

		return J;
	}



//	SAVING PARAMETERS
	void save_complex(VectorXcd eigenvalues, ofstream& file, int iteration)
	{
		file << iteration << " " << species::n << " ";
		// iteration and number of species

		for (int i = 0; i < species::n; i++)
		{
			file << eigenvalues(i).real() << " " << eigenvalues(i).imag() << " ";
		}
		// eigenvalues of all species in food webs

		for (int i = species::n; i < N; i++)
		{
			file << 0 << " " << 0 << " ";
		}
		// zeroes for all empty entries in S[] and P[]

		file << endl;
	}

	void save_parameters(species S[], producer P[], ofstream& sfile, ofstream& bfile, int iteration)
	{
		for (int i = 0; i < producer::n1; i++)
		{
			bfile << iteration << " ";
			// iteration

			bfile << i << " " << P[i].added_i << " " << P[i].l << " " << P[i].S << " " << P[i].k << " " << P[i].a << " ";
			// index - iteration of invasion - level - final density - growth rate - decay rate

			for (int k = 0; k < N; k++)
			{
				bfile << k << " " << P[i].consumers[k] << " ";
			}
			// index of other species - interaction strength

			bfile << endl;
		}
		// saving parameters of producers

		for (int k = producer::n1; k < species::n; k++)
		{
			sfile << iteration << " ";
			// iteration

			sfile << k << " " << S[k].added_i << " " << S[k].l << " " << S[k].S << " " << S[k].a << " ";
			// index - iteration of invasion - level - final density - decay rate

			
			for (int j = 0; j < N; j++)
			{
				sfile << j << " " << S[k].consumers[j] << " " << S[k].resources[j] << " ";
			}
			// index of other species - interaction strength(as resource) - interaction strength(as consumer)

			sfile << endl;
		}
		// saving parameters of species
	}



//	INVESTIGATING LINEAR STABILITY
	void check_for_steady_solution(species S[], producer P[], double steady_states[], ofstream& stab_file, ofstream& unstab_file, int iter)
	{
	//	COMPUTING S* = R^(-1)*k
		MatrixXd R(species::n, species::n);
		VectorXd K(species::n);
		VectorXd Ssteady(species::n);
		// declaring vectors and matrices

		R = initR(R, S, P);				//cout << R << endl;
		K = initK(K, S, P);				//cout << K << endl;
		// initializing R and K

		Ssteady = R.inverse() * K;
		// computing steady states

		for (int i = 0; i < species::n; i++)
		{
			steady_states[i] = Ssteady(i);
		}
		// saving steady states

		for (int i = species::n; i < N; i++)
		{
			steady_states[i] = 0;
		}
		// zeroes for empty entries in S[] and P[]


	//	CHECKING IF SYSTEM IS FEASIBLE AND LINEARLY STABLE
		if (feasible(Ssteady))
		{
			foodweb::feasible = true;
			// updating food web

			check_for_linear_stability(S, P, steady_states, stab_file, unstab_file, iter);
			// checking for linear stability

			cout << "The system does have a feasible steady state!\n";
			cout << "---------------\n Steady states: \n---------------\n" << Ssteady << "\n---------------\n\n";
			// printing		
		}
		// if feasible

		else
		{
			foodweb::feasible = false;
			foodweb::stable = false;
			// updating food web

			cout << "The system does not have a feasible steady state\n" << endl;
			// printing
		}
		// if unfeasible
	}

	void check_for_linear_stability(species S[], producer P[], double steady_states[], ofstream& stab_file, ofstream& unstab_file, int iter)
	{
	//	INITIALIZING
		MatrixXd J(species::n, species::n);
		VectorXcd eigenvalues(species::n);
		species S_temp[N];
		producer P_temp[N];
		// declaring matrix and arrays

		for (int i = 0; i < N; i++)
		{
			P_temp[i] = P[i];
			S_temp[i] = S[i];
		}
		// inilializing temporary species arrays

		set_densities_steady(steady_states, S_temp, P_temp);
		// Setting temporary densities steady

		
	//	COMPUTING JACOBIAN AND EIGENVALUES
		J = Jacobian(J, S_temp, P_temp);
		// initializing Jacobian

		EigenSolver<MatrixXd> es(J);
		eigenvalues = es.eigenvalues();
		// computing eigenvalues of Jacobian

		
	//	CHECKING LINEAR STABILITY
		if (negative_real_values(eigenvalues))
		{
			foodweb::stable = true;
			// updating food web

			cout << "The steady states are linearly stable!\n" << endl;
			// printing

			save_complex(eigenvalues, stab_file, iter);
			// saving eigenvalues
		}
		// if linearly stable

		else
		{
			foodweb::stable = false;
			// updating food web

			cout << "The steady states are not linearly stable.\n" << endl;
			// printing

			save_complex(eigenvalues, unstab_file, iter);
			// saving eigenvalues
		}
		// if linearly unstable
	}
