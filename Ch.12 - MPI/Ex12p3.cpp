#include <iostream>
#include <mpi.h>
#include <random>
#include <cstdlib>
#include <bits/stdc++.h>

using namespace std;

#define F77NAME(x) x##_

extern "C" {
    double F77NAME(ddot) (
        const int&      N,
        const double*   x,      const int& incx,
        const double*   y,      const int& incy); 
    
    double F77NAME(dnrm2) (
        const int&      N,
        const double*   x,      const int& incx); 
}

bool isPowerOfTwo(int n) {
    return (ceil(log2(n)) == floor(log2(n)));
}

double* generateRandomVec(int N, int WORLD_RANK) {
    double* vec = new double[N];
    srand(time(0) + WORLD_RANK);

    for (size_t i = 0; i < N; ++i) {
        vec[i] = static_cast<double> (rand()) / RAND_MAX;
    }

    return vec;
}


int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int WORLD_RANK, WORLD_SIZE;
    MPI_Comm_rank(MPI_COMM_WORLD, &WORLD_RANK);
    MPI_Comm_size(MPI_COMM_WORLD, &WORLD_SIZE);

    if (!isPowerOfTwo(WORLD_SIZE) || WORLD_SIZE > 32) {
        MPI_Finalize();
        if (WORLD_RANK == 0) cerr << "Error: Use 2^n processes or <= 32 processes!" << endl;
        return 1;
    }

    unsigned int probSize = 1024;
    unsigned int locSize = probSize / WORLD_SIZE;
    
    double* x = generateRandomVec(locSize, WORLD_RANK);
    double* y = generateRandomVec(locSize, WORLD_RANK);

    double LOCAL_DOT = F77NAME(ddot)(locSize, x, 1, y, 1);
    
    // cout << "Process " << WORLD_RANK << " / " << WORLD_SIZE << " local = " << LOCAL_DOT << endl;

    double xNORM_LOC = F77NAME(dnrm2)(locSize, x, 1);
    double yNORM_LOC = F77NAME(dnrm2)(locSize, y, 1);

    double* LOCAL_DOTS;
    double* xNORM_LOCS;
    double* yNORM_LOCS;
    
    if (WORLD_RANK == 0) {
        LOCAL_DOTS = new double[WORLD_SIZE];
        xNORM_LOCS = new double[WORLD_SIZE];
        yNORM_LOCS = new double[WORLD_SIZE];
    }

    MPI_Gather(&LOCAL_DOT, 1, MPI_DOUBLE, LOCAL_DOTS, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(&xNORM_LOC, 1, MPI_DOUBLE, xNORM_LOCS, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(&yNORM_LOC, 1, MPI_DOUBLE, yNORM_LOCS, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (WORLD_RANK == 0) {

        double DOT_RESULT = 0.0;
        double xNORM_RESULT = 0.0;
        double yNORM_RESULT = 0.0;

        for (int k = 0; k < WORLD_SIZE; ++k) {
            DOT_RESULT += LOCAL_DOTS[k];
            xNORM_RESULT += xNORM_LOCS[k];
            yNORM_RESULT += yNORM_LOCS[k];
        }
        cout << "DOT RESULT: " << DOT_RESULT << endl;
        cout << "xNORM RESULT: " << xNORM_RESULT << endl;
        cout << "yNORM RESULT: " << yNORM_RESULT << endl;

        delete[] LOCAL_DOTS;
        delete[] xNORM_LOCS;
        delete[] yNORM_LOCS;
    }

    delete[] x;
    delete[] y;

    MPI_Finalize();
    return 0;
}