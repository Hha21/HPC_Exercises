#include <cmath>

void populateMatrix(double* M, const int N) {
    int j;
    int i;
    for (j = 0; j < N; ++j) {
        for (i = 0; i < N; ++i) {
            double alpha = pow(exp(i/N), 2.0);
            double beta  = pow(exp(j/N), 2.0);
            M[j*N+i] = alpha / beta;
        }
    }
}

void setup(const int N) {
    double* M = new double[N*N];
    populateMatrix(M, N);
    delete[] M;
}

int main() {
    const int N = 2000;

    setup(N);

    return 0;
}
