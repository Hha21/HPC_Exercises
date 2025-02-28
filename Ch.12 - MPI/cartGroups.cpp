#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {
    
    MPI_Init(&argc, &argv);

    int wRank, wSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &wRank);
    MPI_Comm_size(MPI_COMM_WORLD, &wSize);

    if (wSize != 16) {
        if (wRank == 0) {
            cout << "This example NEEDS 16 processes!" << endl;
        }
        MPI_Finalize();
        return 1;
    }

    MPI_Group WORLD_GROUP;
    MPI_Comm_group(MPI_COMM_WORLD, &WORLD_GROUP);

    const int n = 7;

    int ranks[n] = {1, 2, 3, 5, 7, 11, 13};

    MPI_Group PRIME_GROUP;
    MPI_Group_incl(WORLD_GROUP, n, ranks, &PRIME_GROUP);
    
    MPI_Comm PRIME_COMM;
    MPI_Comm_create(MPI_COMM_WORLD, PRIME_GROUP, &PRIME_COMM);

    int prime_rank = -1, prime_size = -1;

    if (MPI_COMM_NULL != PRIME_COMM) {
        MPI_Comm_rank(PRIME_COMM, &prime_rank);
        MPI_Comm_size(PRIME_COMM, &prime_size);
    }

    cout << "World: " << wRank << " / " << wSize << " ... Prime: " << prime_rank << " / " << prime_size << endl;

    MPI_Finalize();

    return 0;
}