#pragma once
/*
Functions for computing feasibility and linear stability
*/

#include "species.h"
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
using namespace Eigen;
using namespace std;



//	COMPUTING STEADY STATES
	MatrixXd initR(MatrixXd R, species S[], producer P[]);
	// initializing interaction matrix, R

	VectorXd initK(VectorXd K, species S[], producer P[]);
	// initializing and vector of constants, K



//	CHECKING FOR FEASIBILITY AND STABILITY
	bool feasible(VectorXd S);
	// checking if matrix is feasible

	bool negative_real_values(VectorXcd eigenvalues);
	// checking if all eigenvalues have negative real parts


//	INITIALIZING JACOBIAN
	void set_densities_steady(double steady_states[], species S_temp[], producer P_temp[]);
	// set dentities to steady densities

	MatrixXd Jacobian(MatrixXd J, species S[], producer P[]);
	// initializing Jacobian


//	SAVING PARAMETERS
	void save_complex(VectorXcd eigenvalues, ofstream& file, int iteration);
	// saving eigenvalues of all species in food web

	void save_parameters(species S[], producer P[], ofstream& sfile, ofstream& pfile, int iteration);
	// saving parameters of all species in food web


//	INVESTIGATING LINEAR STABILITY
	void check_for_steady_solution(species S[], producer P[], double steady_states[], ofstream& stab_file, ofstream& unstab_file, int iter);
	// checking if food web is feasible

	void check_for_linear_stability(species S[], producer P[], double steady_states[], ofstream& stab_file, ofstream& unstab_file, int iter);
	// checking if food web is linearly stable