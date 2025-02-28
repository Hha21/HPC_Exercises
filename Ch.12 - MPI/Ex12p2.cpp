#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);
    
    int WORLD_RANK, WORLD_SIZE;

    MPI_Comm_rank(MPI_COMM_WORLD, &WORLD_RANK);
    MPI_Comm_size(MPI_COMM_WORLD, &WORLD_SIZE);

    if (WORLD_SIZE < 2) {
        cout << "Err. 1 Process Only..." << endl;
        MPI_Finalize();
        return 1;
    }

    int a;

    if (WORLD_RANK == 0) {
        if (argc >= 2) {
            a = stoi(argv[1]);
        }

        MPI_Send(&a, 1, MPI_INT, 1, WORLD_RANK, MPI_COMM_WORLD);
        cout << "Process No. " << WORLD_RANK << " sends " << a << " to Process No. " << 1 << endl; 
    }
    else {

        MPI_Recv(&a, 1, MPI_INT, WORLD_RANK - 1, WORLD_RANK - 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process No. " << WORLD_RANK << " recieved " << a << " from Process No. " << WORLD_RANK - 1 << endl;  
        if (WORLD_RANK != WORLD_SIZE - 1) {
            MPI_Send(&a, 1, MPI_INT, WORLD_RANK + 1, WORLD_RANK, MPI_COMM_WORLD);
            cout << "Process No. " << WORLD_RANK << " sends " << a << " to Process No. " << WORLD_RANK + 1 << endl; 
        } 
    }


    MPI_Finalize();
    return 0;
}