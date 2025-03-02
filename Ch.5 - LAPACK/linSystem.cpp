#include <iostream>
#include <cstdlib>


// SOLVE SYSTEM: Ax = y

using namespace std;

#define F77NAME(x) x##_

extern "C" {

    void F77NAME(dgesv) (
        const int& n,       const int& nrhs,
        const double* A,    const int& lda,
        int* ipiv,          double* B,
        const int& ldb,     int& info);
    
    void F77NAME(dgemv) (
        const char& trans,  const int& m,
        const int& n,       const double& alpha,
        const double* A,    const int& lda,
        const double* x,    const int& ldx,
        const double& beta, double* y,
        const int& incy);

}

double* defineRand(const int& n) {
    srand(time(0));
    double* randMatrix = new double[n];

    for (size_t i = 0; i < n; ++i) {
        randMatrix[i] = (static_cast<double>(rand())) / RAND_MAX;
    }
    return randMatrix;
}

int main() {
    const int n = 10;
    const int nrhs = 1;
    int info = 0;

    double* A = defineRand(n*n);
    double* x = defineRand(n);
    double* y = new double[n];
    int*    ipiv = new int[n];

    F77NAME(dgemv)('N', n, n, 1.0, A, n, x, 1, 0.0, y, 1);

    F77NAME(dgesv)(n, nrhs, A, n, ipiv, y, n, info);

    for (int i = 0; i < n; ++i){
        cout << "x[" << i << "] = " << x[i] << ", y["
        << i << "] = " << y[i] << " !" << endl;
    }

    for (int i = 0; i < n; ++i){
        cout << "ipiv[" << i << "] = " << ipiv[i] << endl;
    }

    cout << "info " << info << endl;


    delete[] A;
    delete[] x;
    delete[] y;
    delete[] ipiv;

    return 0;
}