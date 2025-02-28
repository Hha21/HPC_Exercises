#include <iostream>
#include <cstdlib>
#include <cmath>
#include <random>

using namespace std;

#define F77NAME(x) x##_

extern "C" {
    void F77NAME(dgemm) (
                const char&     trans,  const char& transb,
                const int&      m,      const int& n,
                const int&      k,      const double& alpha,
                const double*   A,      const int& lda,
                const double*   B,      const int& ldb,
                const double&   beta,   double* C,
                const int&      ldc);

    void F77NAME(dgemv) (
                const char&     trans,  const int& m,
                const int&      n,      const double& alpha,
                const double*   A,      const int& lda,
                const double*   x,      const int& incx,
                const double&   beta,   double* y,
                const int&      incy);  

    double F77NAME(ddot) (
                const int&      n,
                const double*   x,      const int& incx,
                const double*   y,      const int& incy); 
    
    double F77NAME(dnrm2) (
                const int&      n, 
                const double*   x,      const int& incx);
    
}

double* populateArr(const unsigned int M, const unsigned int N) {

    // M = no. Rows, N = no. Cols

    double* arr = new double[N*M];
    //srand(time(0));

    for (size_t j = 0; j < N; ++j) {
        for (size_t i = 0; i < M; ++i) {
            arr[i + j*M] = 2.0 * (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) - 1.0;
        }
    }

    return arr;
}

double* conjGradient(const unsigned int n, const double* A, const double* b) {

     cout << "Initialising... " << endl;

    //func. controls
    const double epsilon = 0.000001;
    unsigned long max_steps =  1000000;
    double conv;

    //func. utilities
    double topfrac;
    double botfrac;
    double* Ap = new double[n];
    double alphaK;
    double betaK;

    const double alpha = 1.0;
    const double beta = 0.0;


    double* x0 = populateArr(n, 1);
    double* b0 = new double[n*1];

    //find b0 := A * x0
    F77NAME(dgemv) ('N', n, n, alpha, A, n, x0, 1, beta, b0, 1);

    //init r0, p0
    double* r0 = new double[n*1];
    double* p0 = new double[n*1];

    for (size_t i = 0; i < n; ++i) {
        p0[i] = r0[i] = b[i] - b0[i];
    }

     cout << "Starting iterations: " << endl;

    for (size_t k = 0; k < max_steps; ++k) {

        //compute r_k^T dot r_k
        topfrac = F77NAME(ddot) (n, r0, 1, r0, 1);

        //compute Ap = A*p;
        F77NAME(dgemv)('N', n, n, alpha, A, n, p0, 1, beta, Ap, 1);

        //compute p^T dot Ap
        botfrac = F77NAME(ddot) (n, p0, 1, Ap, 1);
        
        alphaK = topfrac / botfrac;

        //update x_k
        for (size_t i = 0; i < n; ++i) {
            x0[i] += alphaK*p0[i];
            r0[i] -= alphaK*Ap[i];
        }
        conv = F77NAME(dnrm2)(n, r0, 1);
        if (conv <= epsilon) {
            cout << "Converged with k = " << k << endl;
            goto converged;
        }

        botfrac = topfrac;

        topfrac = F77NAME(ddot) (n, r0, 1, r0, 1);

        betaK = topfrac / botfrac;

        for (size_t i = 0; i < n; ++i) {
            p0[i] = r0[i] + betaK*p0[i];
        }

    }

    cerr << "Failed to converge :( ..." << endl;
    return nullptr;

    converged:
            cout << "Covergence Success!" << endl;
            return x0;
}

int main() {

    const int n = 25;
    const double alpha = 1.0;
    const double beta = 0.0;

    double* arrM = populateArr(n, n);   // create arr M x N
    double* vecX = populateArr(n, 1);
    double* arrA = new double[n*n];
    double* vecB = new double[n*1];

    double* y = new double[n*1];

    F77NAME(dgemm) ('T', 'N', n, n, n, alpha, arrM, n, arrM, n, beta, arrA, n);

    //compute b = Ax;

    F77NAME(dgemv) ('N', n, n, alpha, arrA, n, vecX, 1, beta, vecB, 1);

    //given b = Ax, should solve for y = x.
    y = conjGradient(n, arrA, vecB);

    for (int i = 0; i < n; ++i) {
        cout << "y[" << i << "] is: " << y[i] << " | x[" << i << "] is: " << vecX[i] << endl;
    }

    delete[] arrM;
    delete[] vecX;
    delete[] arrA;
    delete[] vecB;
    delete[] y;
    return 0;
}
