#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    // argv[0] ---> "./a.out"
    // argv[1] ---> "num"
    // argv[2] ---> nullptr

    for (int i = 0; i < argc + 1; ++i) {
        if (argv[i] == nullptr) {
            cout << "FOUND NULLPTR at " << i << endl;
            break;
        }
    }

    int wRank, wSize;

    MPI_Comm_rank(MPI_COMM_WORLD, &wRank);
    MPI_Comm_size(MPI_COMM_WORLD, &wSize);

    if (argc > 2) {
        cout << "Don't Push it buddy, only 2 arguments!" << endl;
        MPI_Finalize();
        return 1; 
    }

    if (wRank == 0) {
        if (argc >= 1) {
            cout << "Num Arguments Passed: " << argc << endl;
            cout << "-----THESE ARE THE COMMAND LINE ARGUMENTS------" << endl;
            for (int i = 0; i < argc; ++i) {
                cout << "Input No. " << i + 1 << "   " << argv[i] << endl;
            }
        }
    }

    int n = stoi(argv[1]);

    cout << "Yo! It's process: " << wRank << " / " << wSize  << " ... Confirm N = " << n << endl;

    MPI_Finalize();

    return 0;
}