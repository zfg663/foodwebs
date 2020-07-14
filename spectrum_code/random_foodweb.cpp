// including .cpp-files
#include "/conv1/zfg663/foodwebs/evolution_code/distributions.cpp"
#include "/conv1/zfg663/foodwebs/evolution_code/species.cpp"
#include "/conv1/zfg663/foodwebs/evolution_code/linear_stability.cpp"
#include "/conv1/zfg663/foodwebs/evolution_code/food_web.cpp"

// including templates
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
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
// random seed
srand(15);

// parameters
int Nrep = 1e4;		// number of repetitions
int N;			// number of species
cout << "Enter size of community matrix: " << endl;
cin >> N;

// initializing normal distribution
int mu = 0;
int sig = 1;
random_device rd;
mt19937 e2(rd());
normal_distribution<> dist(mu, sig);

// opening file for saving data
ofstream file("/conv1/zfg663/foodwebs/data/spectra/FWstruct_N" + to_string(N) + ".txt");


for (int rep=0; rep<Nrep; rep++) {
	// arrays	
	MatrixXd C(N,N);
	VectorXcd E(N);
	Species S[N];
	Producer P[N];


	// assembling food web
	Producer s(0);
	P[0] = s;
	S[0] = s;

	int test = 0;
	for (int i=1; i<N; i++) {
		addSpecies(S, P, i);
		updateTrophicLevel(S);
	}

	// assigning random densities
	for (int i=0; i<Producer::nProducer; i++) {
		S[i].density = dist(e2);
		P[i].density = S[i].density;
	}
	for (int i=Producer::nProducer; i<N; i++) {
		S[i].density = dist(e2);
	}		

	// computing eigenvalues of Jacobian
	C = Community(C, S, P);
	EigenSolver<MatrixXd> es(C);
	E = es.eigenvalues();
	
	// saving
	for (int i=0; i<N; i++) {
		file << E(i).real() << " " << E(i).imag() << " ";
	}
	file << endl;
	//printMatrix(C, N);
	
	// removing all species
	for (int i=N-1; i>=0; i--) {
		removeSpecies(S, P, i);
	}
}

file.close();
return 0;
}

