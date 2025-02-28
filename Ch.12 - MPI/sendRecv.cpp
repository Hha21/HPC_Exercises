#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {

    int a = 0;
    int b = 0;

    int rank, size, retval_rank, retval_size;
    MPI_Init(&argc, &argv);

    retval_rank = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    retval_size = MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (retval_rank == MPI_ERR_COMM || retval_size == MPI_ERR_COMM) {
        cout << "Invalid Communicator!" << endl;
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        a = 2;
        MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&b, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else {
        b = 3;
        MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&b, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    cout << "Rank: " << rank << " a = " << a << " ,b = " << b << endl;

    MPI_Finalize();

    return 0;
}