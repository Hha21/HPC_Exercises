#include <iostream>
#include <mpi.h>
#include <cmath>
#include <string>
#include <iomanip>

using namespace std;

inline double f(const double& x) {
    return 4.0 / (1.0 + x*x);
}

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int WORLD_INTERVALS;

    int WORLD_RANK, WORLD_SIZE;
    MPI_Comm_rank(MPI_COMM_WORLD, &WORLD_RANK);
    MPI_Comm_size(MPI_COMM_WORLD, &WORLD_SIZE);

    if (WORLD_RANK == 0) {
        string input;
        cout << "Enter No. of Intervals: ";
        cin >> input;
        WORLD_INTERVALS = stoi(input);
    }

    MPI_Bcast(&WORLD_INTERVALS, 1, MPI_INT, 0, MPI_COMM_WORLD);

    const double low = 0.0;
    const double high = 1.0;

    const double dx = (high - low) / (static_cast<double>(WORLD_INTERVALS));

    int REMAINDER = WORLD_INTERVALS % WORLD_SIZE;
    int CHUNKS = (WORLD_INTERVALS - REMAINDER) / WORLD_SIZE;
    int start = 0;
    int end = 0;

    if (WORLD_RANK < (WORLD_INTERVALS % WORLD_SIZE)) {
        CHUNKS++;
        start = CHUNKS * WORLD_RANK;
        end = CHUNKS * (WORLD_RANK + 1);
    } else {
        start = (CHUNKS + 1) * REMAINDER + CHUNKS * (WORLD_RANK - REMAINDER);
        end = (CHUNKS + 1) * REMAINDER + CHUNKS * (WORLD_RANK - REMAINDER + 1);
    }

    cout << "RANK " << WORLD_RANK << " / " << WORLD_SIZE << " START/END = " << start << " / " << end << endl;

    double LOCAL_PI = 0.0;

    for (int i = 0; i < (end - start); i += 2) {
        double d1 = (start + i) * dx;
        double d2 = (start + i + 1) * dx;
        double d3 = (start + i + 2) * dx;
        
        double f_d1 = f(d1);
        double f_d2 = f(d2);
        double f_d3 = f(d3);

        LOCAL_PI += (f_d1 + f_d2) * 0.5 * dx;
        LOCAL_PI += (f_d2 + f_d3) * 0.5 * dx;
    }

    double WORLD_PI;

    MPI_Reduce(&LOCAL_PI, &WORLD_PI, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (WORLD_RANK == 0) {
        cout << "Pi Estimate: " << setprecision(8) << WORLD_PI << endl;
    }


    MPI_Finalize();
    return 0;
}