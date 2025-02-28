#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int wRank, wSize;

    MPI_Comm_size(MPI_COMM_WORLD, &wSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &wRank);

    if (wSize != 12) {
        if (wRank == 0) {
            cout << "Error: 12 Processes Needed!" << endl;
        }
        MPI_Finalize();
        return 1;
    }

    MPI_Comm MY_GRID;
    const int dims = 2;
    int sizes[dims] = {3, 4};
    int periods[dims] = {0, 1};
    int reorder = 1;

    MPI_Cart_create(MPI_COMM_WORLD, dims, sizes, periods, reorder, &MY_GRID);

    MPI_Comm ROW_COMM, COL_COMM;
    int coords[dims];
    int keep[dims];

    int gridRank;
    int gridSize;

    MPI_Comm_rank(MY_GRID, &gridRank);
    MPI_Comm_size(MY_GRID, &gridSize);

    MPI_Cart_coords(MY_GRID, gridRank, dims, coords);

    cout << "Hello! My World Rank is " << wRank << " / " << wSize << " ... My Coords are:  [" <<
    coords[0] << ", " << coords[1] << "]" << endl;

    keep[0] = 0;
    keep[1] = 1;
    MPI_Cart_sub(MY_GRID, keep, &ROW_COMM);

    cout << "AFTER SUBTRACTING! My World Rank is " << wRank << " / " << wSize << " ... My Coords are:  [" <<
    coords[0] << ", " << coords[1] << "]" << endl;

    MPI_Finalize();

    return 0;
}