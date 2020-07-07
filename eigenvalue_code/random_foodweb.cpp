#include "distributions.h"
#include "distributions.cpp"
#include "species.h"
#include "species.cpp"
#include "food_web.h"
#include "food_web.cpp"
#include "linear_stability.h"
#include "linear_stability.cpp"
//#include "time_series.h"
//#include <iostream>
//#include <fstream>
#include <cstdlib>
//#include <cmath>
//#include <Eigen/Dense>
//#include <Eigen/Eigenvalues>

//using namespace Eigen;
using namespace std;

int Species::nTotal = 0;
int Producer::nProducer = 0;


int main(void) {

	srand(27);
	int N = 3;
	int Nrep = 1;

	// arrays
	MatrixXd R(N,N);	
	MatrixXcd C(N,N);
	VectorXd K(N);
	VectorXd steadyS(N);
	VectorXcd EV(N);

	Species S[N];
	Producer P[N];

	Producer s(0);
	P[0] = s;
	S[0] = s;

	ofstream file("foodweb_eigenvalues_N03.txt");

	for (int i=1; i<N; i++) {
		addSpecies(S, P, i);
		updateTrophicLevel(S);
	}

	// initializing R and K
	R = initR(R, S, P);
	K = initK(K, S, P);
	steadyS = R.inverse() * K;

	for (int i=0; i<Producer::nProducer; i++) {
		S[i].density = steadyS(i);
		P[i].density = steadyS(i);
	}
	for (int i=Producer::nProducer; i<N; i++) {
		S[i].density = steadyS(i);
	}	

		
	// initializing Jacobian
	C = Community(C, S, P);

	// computing eigenvalues of Jacobian
	EigenSolver<MatrixXd> es(C);
	EV = es.eigenvalues();

	file.close();

	return 0;
}
