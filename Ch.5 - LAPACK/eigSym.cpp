#include <iostream>
#include <cstdlib>
#include <iomanip>

#define F77NAME(x) x##_

using namespace std;

extern "C" {
    void F77NAME(dsyev)(
        const char& v,          const char& ul,
        const int& n,           double* a,
        const int& lda,         double* w,
        double* work,           const int& lwork,
        int* info);
}

double* defineRandSym(const int& n) {

    double* A = new double[n*n];

    srand(time(0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i*n + j] = A[j*n + i] = (static_cast<double>(rand()) / RAND_MAX);
        }
    }
    return A;
}

int main() {

    const unsigned int n = 10;
    auto lda = n;
    auto ldv = n;

    int info = 0;
    int lwork = -1;

    double wkopt;
    double* A = defineRandSym(n);
    double* w = new double[n];

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << setw(8) << fixed << setprecision(3) << A[i + j * n] << " ";
        }
        cout << endl;
    }

    // QUERY FOR OPTIMAL WORKSPACE SIZE

    F77NAME(dsyev)('V', 'U', n, A, lda, w, &wkopt, lwork, &info);

    lwork = static_cast<int>(wkopt);
    double* work = new double[lwork];

    F77NAME(dsyev)('V', 'U', n, A, lda, w, work, lwork, &info);

    for (int i = 0; i < n; ++i) {
        cout << "EV[" << i << "] = " << w[i] << endl;
    }

    delete[] A;
    delete[] w;
    delete[] work;
}