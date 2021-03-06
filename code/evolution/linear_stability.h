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
	MatrixXd initR(MatrixXd R, Species S[], Producer P[]);
	// initializing interaction matrix, R

	VectorXd initK(VectorXd K, Species S[], Producer P[]);
	// initializing and vector of constants, K



//	CHECKING FOR FEASIBILITY AND STABILITY
	bool feasible(VectorXd S);
	// checking if matrix is feasible

	bool negativeRealValues(VectorXcd eigenvalues);
	// checking if all eigenvalues have negative real parts


//	INITIALIZING COMMUNITY MATRIX
	void setDensitiesSteady(double steadyStates[], Species S_temp[], Producer P_temp[]);
	// set dentities to steady densities

	MatrixXd Community(MatrixXd C, Species S[], Producer P[]);
	// initializing community matrix


//	SAVING PARAMETERS
	void saveEigenvalues(VectorXcd eigenvalues, ofstream& file, int addAttempt);
	// saving eigenvalues of all Species in food web

	void saveParameters(Species S[], Producer P[], ofstream& sFile, ofstream& pFile, int addAttempt);
	// saving parameters of all Species in food web


//	INVESTIGATING LINEAR STABILITY
	void checkFeasibility(Species S[], Producer P[], double steadyStates[], ofstream& stabFile, ofstream& unstabFile, int addAttempt);
	// checking if food web is feasible

	void checkLinearStability(Species S[], Producer P[], double steadyStates[], ofstream& stabFile, ofstream& unstabFile, int addAttempt);
	// checking if food web is linearly stable
