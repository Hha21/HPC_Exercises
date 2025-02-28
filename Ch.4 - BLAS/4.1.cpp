#include <iostream>
#include <cstdlib>
#include <random>
#include <cmath>

using namespace std;

#define F77NAME(x) x##_

extern "C" {
    double F77NAME(ddot) (const int& N,
                            const double *x, const int& incx,
                            const double *y, const int& incy);

    double F77NAME(dnrm2) (const int& N,
                            const double *x, const int& incx);
}

double* populateArray(const unsigned int N) {

    double* arr = new double[N*N];
    srand(time(0));

    for (size_t i = 0; i < N*N; ++i) {
        arr[i] = static_cast<double> (rand()) / RAND_MAX;
    }
    
    return arr;
}

int main() {
    
    const unsigned int N = 10;

    double* arr = populateArray(N);

    double d1 = F77NAME(ddot)(N*N, arr, 1, arr, 1);
    d1 = sqrt(d1);

    double d2 = F77NAME(dnrm2)(N*N, arr, 1);

    cout << "Method 1 - d is: " << d1 << endl;
    cout << "Method 2 - d is: " << d2 << endl;
    
    delete[] arr;

}