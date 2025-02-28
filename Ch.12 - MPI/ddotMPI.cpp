#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {
    int my_rank, p, i, k;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    const unsigned int n = 64;
    int loc_n = n / p;

    double* x = new double[loc_n];
    double* y = new double[loc_n];
    srand(time(0) + my_rank);

    for (i = 0; i < loc_n; ++i) {
        x[i] = double(rand()) / RAND_MAX;
        y[i] = double(rand()) / RAND_MAX;
    }

    double loc_dot = 0.0;

    for (i = 0; i < loc_n; ++i) {
        loc_dot += x[i]*y[i];
    }

    double* loc_dots;

    cout << "Hello from rank " << my_rank << endl;

    if (my_rank == 0) loc_dots = new double[p];

    MPI_Gather(&loc_dot, 1, MPI_DOUBLE, loc_dots, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        double dot = 0.0;
        for (k = 0; k < p; ++k) {
            dot += loc_dots[k];
        }
        cout << "Dot Product: " << dot << endl;
    }

    MPI_Finalize();
    
    return 0;
}