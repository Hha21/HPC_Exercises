#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int wRank, wSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &wRank);
    MPI_Comm_size(MPI_COMM_WORLD, &wSize);

    int color = wRank % 2;  // 0 for even, 1 for odd;

    // split communicator as odd or even;

    MPI_Comm ODDEVEN_COMM;
    MPI_Comm_split(MPI_COMM_WORLD, color, wRank, &ODDEVEN_COMM);

    int oddeven_rank, oddeven_size;

    MPI_Comm_rank(ODDEVEN_COMM, &oddeven_rank);
    MPI_Comm_size(ODDEVEN_COMM, &oddeven_size);

    cout << "My World Rank is : " << wRank << " / " << wSize <<
        " ... My odd/even rank is: " << oddeven_rank << " / " << oddeven_size << endl;

    MPI_Comm_free(&ODDEVEN_COMM);

    MPI_Finalize();

    return 0;
}