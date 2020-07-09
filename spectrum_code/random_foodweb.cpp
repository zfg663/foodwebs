// including .cpp-files
#include "/conv1/zfg663/foodwebs/evolution_code/distributions.cpp"
#include "/conv1/zfg663/foodwebs/evolution_code/species.cpp"
#include "/conv1/zfg663/foodwebs/evolution_code/linear_stability.cpp"
#include "/conv1/zfg663/foodwebs/evolution_code/food_web.cpp"

// including templates
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

using namespace Eigen;
using namespace std;

// defining print-function
void printMatrix(MatrixXd M, int N) {
	for(int i=0; i<N; i++) {
		for(int j=0; j<N; j++) { cout << "C(" << i << "," << j << "): " << M(i, j) << " "; }
		cout << endl;
	}
}

// initializing global variables from foodwebs
int Species::nTotal = 0;
int Producer::nProducer = 0;
bool FoodWeb::feasible = 0;
bool FoodWeb::stable = 0;

int main() {

	srand(27);		// initializing random seed

	// parameters
	int Nrep = 2;		// number of repetitions
	int N;			// number of species
	cout << "Enter size of community matrix: " << endl;
	cin >> N;

	// opening file for saving data
	ofstream file("/conv1/zfg663/data/spectra/FWstruct_N" + to_string(N) + ".txt");

	// arrays	
	MatrixXd C(N,N);
	MatrixXd R(N,N);
	VectorXd K(N);
	VectorXd steadyS(N);
	VectorXcd EV(N);

	Species S[N];
	Producer P[N];
	double steadyStates[N];

	Producer s(0);
	P[0] = s;
	S[0] = s;

	for (int i=1; i<N; i++) {
		addSpecies(S, P, i);
		updateTrophicLevel(S);
		checkFeasibility(S, P, steadyStates, file, file, 0);
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

	printMatrix(C, N);

	// computing eigenvalues of Jacobian
	EigenSolver<MatrixXd> es(C);
	EV = es.eigenvalues();

	file.close();

	return 0;
}
