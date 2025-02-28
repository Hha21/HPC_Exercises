#include <iostream>
#include <cmath>

#define F77NAME(x) x##_

using namespace std;

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
    
    void F77NAME(dspmv) (
                const char&     uplo,  const int& n,
                const double& alpha,   const double* AP,    
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

double* createSymm(const unsigned int& N, const double& dx, const double& lambda) {

    const double alpha = -2.0/(dx*dx) - lambda;
    const double beta = 1/(dx*dx);

    //reduced form
    const int n = N - 2; //n=19

    double* A = new double[(n*(n+1))/2];

    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {

            int idx = j*(j+1) / 2 + i;

            if (j == i) {
                A[idx] = alpha;
            }
            else if (j - i == 1) {
                A[idx] = beta;
            }
            else {
                A[idx]= 0.0;

            }
        }
    }
    return A;
}

double* defineGrid (const unsigned int& N, const double& h, const double& low, const double& high) {

    double* grid = new double[N];

    for (size_t i = 0; i < N; ++i) {
        grid[i] = low + i * h;
    }

    return grid;
}

void forceGrid (const unsigned int& N, const double& lambda, double* grid) {
    const double pi = 3.14159265358979323846;

    for (size_t i = 0; i < N; ++i) {
        grid[i] = -(lambda + pi*pi)*sin(grid[i] * pi);
    }
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
    F77NAME(dspmv) ('U', n, alpha, A, x0, 1, beta, b0, 1);

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
        F77NAME(dspmv)('U', n, alpha, A, p0, 1, beta, Ap, 1);

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
    const double a = 0.0;
    const double b = 2.0;
    const double lambda = 1.0;

    const double pi = 3.14159265358979323846;

    const unsigned int N = 21;

    const double dx = (b - a) / static_cast<double>(N);

    double* A = createSymm(N, dx, lambda);
    double* grid = defineGrid(N, dx, a, b);

    forceGrid(N, lambda, grid);

    double* u = new double[N];
    u = conjGradient(N, A, grid);

    //VERIFY:
    double* analytic = defineGrid(N, dx, a, b);
    double* diff = new double[N];

    for (size_t i = 0; i < N; ++i) {
        analytic[i] = sin(pi*analytic[i]);
        diff[i] = analytic[i] - u[i];
        cout << "Analytic: " << analytic[i] << " | Numerical: " << u[i] << endl;
    }

    double epsilon = F77NAME(dnrm2)(N, diff, 1);

    cout << "EPSILON: " << epsilon << endl;

    
    delete[] A;
    delete[] u;
    delete[] diff;
    delete[] analytic;
    return 0;
}