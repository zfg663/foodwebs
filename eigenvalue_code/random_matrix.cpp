// Script to compute eigenvalues of random matrices
// 23/5-2020: Used random seed 27 to run simulations for N = 2-12

#include "/conv1/zfg663/foodwebs/evolution_code/distributions.cpp"
#include <iostream>
#include <fstream>
#include <random>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

using namespace Eigen;
using namespace std;


// Defining functions used in script
void printMatrix(MatrixXd M, int N) {
	for(int i=0; i<N; i++) {
		for(int j=0; j<N; j++) { cout << "C(" << i << "," << j << "): " << M(i, j) << " "; }
		cout << endl;
	}
}

int main() {

// setting parameters, etc.
srand(27);

double prob = 0.5;		// occupation probability
int mu = 0;			// mean of gaussian
int sig = 1;			// std of gaussian
int Nrep = 1e4;			// number of community matrices in spectrum
int N = 2;			// number of species
int d = 1;			// diagonal element

ofstream file("/conv1/zfg663/foodwebs/data/spectra/May_N12.txt");


for(int rep=0; rep<Nrep; rep++) {
	MatrixXd C(N, N);		// community matrix
	VectorXcd E(N);			// Eigen values
	
	random_device rd;
	mt19937 e2(rd());
	normal_distribution<> dist(mu, sig);

	// filling community matrix with random numbers
	for(int i=0; i<N; i++) {
		for(int j=0; j<N; j++) {
		 	double u = randomDouble(0,1);
			if (u < prob) {
				C(i,j) = dist(e2);
			}
		}
	}
	
	// filling diagonal
	for (int i=0; i<N; i++) {
		C(i,i) = -d;
	}

	//printMatrix(C,N);

	// computing eigenvalues
	EigenSolver<MatrixXd> es(C);
	E = es.eigenvalues();

	// saving
	for (int i=0; i<N; i++) {
		file << E(i).real() << " " << E(i).imag() << " ";
	}
	file << endl;
}

file.close();
return 0;
}
