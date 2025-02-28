#include <iostream>
#include <cstdlib>
#include <random>
#include <cmath>

using namespace std;

#define F77NAME(x) x##_

extern "C" {
    void F77NAME(dgemm) (
                const char& trans,  const char& transb,
                const int& m,       const int& n,
                const int& k,       const double& alpha,
                const double* A,    const int& lda,
                const double* B,    const int& ldb,
                const double& beta, double* C,
                const int& ldc);
}

double* populateArray (const unsigned int N, bool populate) {
    double* arr = new double[N*N];
    auto M = N;

    //N = no. of columns ; M = no. of rows; COLUMN MAJOR

    if (populate) {
        srand(time(0));

        for (size_t i = 0; i < N; ++i) {                    //iterate columns
            for (size_t j = 0; j < M; ++j) {                //iterate rows
                arr[i*M + j] = static_cast<double> (rand()) / RAND_MAX;
            }
        }

    }

    return arr;
}

void naiveMultiplyNxN (const unsigned int N, const double* A, const double* B, double* C) {

    for (size_t j = 0; j < N; ++j) {            //iterate columns
        for (size_t i = 0; i < N; ++i) {        //iterate rows
            double sum = 0.0;
            for (size_t k = 0; k < N; ++k) {
                sum += A[i + k*N] * B[k + j*N];
            }
            C[i + j*N] = sum;
        }
    }
}

void copyArray (const unsigned int N, double* copy, const double* orig) {
    for (size_t j = 0; j < N; ++j) {
        for (size_t i = 0; i < N; ++i) {
            copy[i + j*N] = orig[i + j*N];
        }
    }
}

int main () {
    const unsigned int N = 1;
    const unsigned int base = 2;
    const double alpha = 1.0;
    const double beta = 0.0;
    double diff = 0.0;
    int arrSize = static_cast<int> (pow(base, N));

    double* arrA = populateArray(arrSize, true);
    double* arrB = populateArray(arrSize, true);
    double* arrC1 = populateArray(arrSize, false);
    double* copy = populateArray(arrSize, false);
    
    F77NAME(dgemm) ('N', 'N', arrSize, arrSize, arrSize, alpha,
                        arrA, arrSize, arrB, arrSize,
                        beta, arrC1, arrSize);

    cout << arrC1[0] << endl;

    copyArray(arrSize, copy, arrC1);

    naiveMultiplyNxN(arrSize, arrA, arrB, arrC1);

    cout << arrC1[0] << endl;

    for (size_t j = 0; j < N; ++j) {
        for (size_t i = 0; i < N; ++i) {
            diff += copy[i + j*arrSize] - arrC1[i + j*arrSize];
        }
    }

    cout << "Diff is: " << diff << endl;

    delete[] arrA;
    delete[] arrB;
    delete[] arrC1;
    delete[] copy;  

    return 0;
}