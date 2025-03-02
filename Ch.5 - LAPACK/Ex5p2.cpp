#include <iostream>
#include <cmath>
#include <string>

using namespace std;

#define F77NAME(x) x##_

extern "C" {
    void F77NAME(dspmv)(
        const char& uplo,           const int& n,
        const double& alpha,        const double* AP,
        const double* x,            const int& incx,
        const double& beta,         double* y,
        const int& incy);
        
    void F77NAME(dspev)(
        const char& jobz,           const char& uplo,
        const int& n,               double* Ap,
        double* w,                  double* z,
        const int& ldz,             double* work,
        int& info);
}

double* createDiscPackedMatrix(const int& N, const double& beta, const double& alpha, char UL) {

    double* A = new double[N * (N+1) / 2]();
    int idx = 0;

    switch(UL) {

        case 'U':
        
            for (int j = 0; j < N; ++j) {
                for (int i = 0; i <= j; ++i) {
                    if (i == j) A[idx] = alpha;
                    else if (i == j - 1) A[idx] = beta;
                    idx++;
                }
            }   

            break;

        case 'L':
            
            for (int j = 0; j < N; ++j) {
                for (int i = 0; i < N; ++i) {
                    if (i == j) A[idx] = alpha;
                    else if (i == j + 1) A[idx] = beta;
                    idx++;
                }
            }
            
            break;

        default:
            goto Error;
    }

    //for (int k = 0; k < (N*(N+1) / 2); ++k) cout << A[k] << endl;

    return A;

    Error:
        cerr << "INVALID value: UL" << endl;
        delete[] A;
        return nullptr;
}

inline double f(double x) {
    return sin(M_PI * x);
}

double* initGrid(const int& N, const double& LOW, const double& dx, const double& NU, const double& Gamma0, const double& Gamma1) {

    double* U0 = new double[N];

    for (size_t i = 0; i < N; ++i) U0[i] = f(LOW + i * dx);
    
    U0[0] -= (1.0 / NU)*Gamma0;
    U0[N - 1] -= (1.0 / NU)*Gamma1;

    return U0;
}

void marchTime(const int& NUM_STEPS, double* U0, const double* A, const int& N) {
    cout << "INITIAL DOMAIN: " <<  endl;

    double alpha = 1.0;
    double beta = 0.0;

    double* y = new double[N];

    for (int i = 0; i < N; ++i) cout << "U(0)[" << i << "] = " << U0[i] << endl;

    for (int t = 0; t <= NUM_STEPS; ++t) {
        F77NAME(dspmv)('U', N, alpha, A, U0, 1, beta, y, 1);
        
        for (int i = 0; i < N; ++i) {
            U0[i] = y[i];
            y[i] = 0.0;
        }
    }

    cout << "FINAL DOMAIN: " <<  endl;

    for (int i = 0; i < N; ++i) cout << "U("<< NUM_STEPS << ")[" << i << "] = " << U0[i] << endl;
    delete[] y;
}   

double* computeEigenValues(double* A, const int& N) {

    // COMPUTE EIGENVALUES:

    double* EIG_VALUES = new double[N];
    double* EIG_VECTORS = new double[N*N];

    double* work = new double[3*N];

    int info;

    F77NAME(dspev)('V', 'U', N, A, EIG_VALUES, EIG_VECTORS, N, work, info);

    delete[] work;
    delete[] EIG_VECTORS;

    if (info == 0) {
        return EIG_VALUES;
    }
    else {
        cout << "ERROR: " << info << endl;
        delete[] EIG_VALUES;
        return nullptr;
    }
}

int main() {

    string sGRID_POINTS, sdt, sNUM_STEPS;

    cout << "USER: Define No. Grid Points to Solve: " << endl;
    cin >> sGRID_POINTS;
    
    cout << "USER: Define time-step size: " << endl;
    cin >> sdt;

    cout << "USER: Define number of steps in time: " << endl;
    cin >> sNUM_STEPS;

    const unsigned int GRID_POINTS = stoi(sGRID_POINTS);
    const unsigned int NUM_STEPS = stoi(sNUM_STEPS);
    const double dt = stod(sdt);

    const unsigned int N = GRID_POINTS - 2;

    const double LOW = 0.0;
    const double HIGH = 1.0;

    const double dx = (HIGH - LOW) / (static_cast<double>(GRID_POINTS));

    const double NU = dt / (dx*dx);

    if (NU >= 0.5) {
        cerr << "INVALID DISCRETISATION OF SYSTEM" << endl;
        return 1;
    }

    const double beta = NU;
    const double alpha = (1.0 - 2.0*NU);

    cout << "ALPHA, BETA: " << alpha << ", " << beta << endl;
    double* A = createDiscPackedMatrix(N, beta, alpha, 'U');

    double* U0 = initGrid(N, LOW, dx, NU, 0.0, 0.0);

    marchTime(NUM_STEPS, U0, A, N);

    double* EIG_VALUES = computeEigenValues(A, N);

    for (int i = 0; i < N; ++i) {
        cout << "Eig: " << i << " = " << EIG_VALUES[i] << endl;
    }

    delete[] A;
    delete[] U0;
    delete[] EIG_VALUES;
    return 0;
}