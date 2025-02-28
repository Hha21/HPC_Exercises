#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {
    
    MPI_Init(&argc, &argv);

    int rank, size, retval_rank, retval_size;

    retval_rank = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    retval_size = MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (retval_rank == MPI_ERR_COMM || retval_size == MPI_ERR_COMM) {
        cout << "Invalid Communicator" << endl;

        return 1;
    }

    cout << "Process: " << rank << " , of " << size << " total" << endl;

    MPI_Finalize();
    return 0;
}