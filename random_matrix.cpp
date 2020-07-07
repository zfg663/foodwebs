#include <iostream>
#include <fstream>

#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

using namespace Eigen;
using namespace std;


// Defining functions used in script
double randomDouble(double min, double max) {
	return min + (max - min)*((double)rand() / (double)RAND_MAX);
}

int randomInt(int min, int max) {
	return min + rand() % (max - min + 1);
}

void printMatrix(MatrixXd M, int N) {
	for(int i=0; i<N; i++) {
		for(int j=0; j<N; j++) { cout << "C(" << i << "," << j << "): " << M(i, j) << " "; }
		cout << endl;
	}
}


int main() {

ofstream file("eigenvalues.txt");

srand(27);

int min = 0;
int max = 1;
int N = 4;			// number of species

MatrixXd C(N, N);		// community matrix


// filling community matrix with random numbers
for(int i=0; i<N; i++) {
	for(int j=0; j<N; j++) {
		C(i,j) = randomDouble(min, max);
	}
}

// printing matrix
printMatrix(C, N);

// computing eigenvalues
EigenSolver<MatrixXd> es(C);
VectorXcd eigenvalues(N);
eigenvalues = es.eigenvalues();


// saving
for (int i=0; i<N; i++) {
	file << eigenvalues(i).real() << " " << eigenvalues(i).imag() << " ";
}
file << endl;

return 0;
}
