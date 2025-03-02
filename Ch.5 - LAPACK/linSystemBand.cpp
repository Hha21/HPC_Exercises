#include <iostream>
#include <cstdlib>

using namespace std;

#define F77NAME(x) x##_

// SOLVE SYSTEM Ax = b; for general banded matrices

extern "C" {
    void F77NAME(dgbsv)(
        const int& n,       const int& kl,
        const int& ku,      const int& nrhs,
        double* AB,   const int& ldab,
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

double* populateMatrixLaP(const int& n, const int& kL, const int& kU) {

    const int ldab = (2*kL + kU + 1);

    double* bandedA = new double(ldab * n);

    cout << "ldab = " << ldab << " ,A size: " << ldab * n << endl;
    srand(time(0));

    for (int k = 0; k < ldab * n; ++k) {
        bandedA[k] = 0.0;
    }

    //if valid entry use rand, else 0:

    for (int j = 0; j < n; ++j) {               //iterate Columns

        int i_min = max(0, j - kL); 
        int i_max = min(n - 1, j + kU);

        for (int i = i_min; i < i_max; ++i) {        //iterate Rows
            int i_band = kL + (i - j);               //compute row position in banded storage
            int index = i_band + j*ldab;    

            cout <<"i,j = " << i << "," << j << endl;
            cout << "At Band: " << i_band << endl;
            bandedA[index] = static_cast<double>(rand()) / RAND_MAX;
        }
    }

    return bandedA;
}


int main() {
    const int n = 5;
    const int kL = 2;         
    const int kU = 1;
    const int rhs = 1;                  //No. RHS Vectors;

    int info;

    const int ldab = (2*kL + kU + 1);   //No. Rows in formatted Matrix;
    const int ldb = n;                  //Size of RHS Vector;

    double* A = populateMatrixLaP(n, kL, kU);
    double* b = new double[n];

    int* piv = new int[n];

    //populate b randomly
    for (int i = 0; i < n; ++i) b[i] = static_cast<double>(rand()) / RAND_MAX;

    F77NAME(dgbsv)(n, kL, kU, rhs, A, ldab, piv, b, ldb, info);

    if (info) {
        cout << "ERROR: An error occured in DGBSV: " << info << endl;
    }

    delete[] A;
    delete[] b;
    delete[] piv;
}