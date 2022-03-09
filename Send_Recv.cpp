#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    
    //Inicjalizacja środowiska MPI
    MPI_Init(NULL, NULL);

    //Znalezienie *rank* i rozmiaru *communicatora*
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //Zakładamy minimum 2 procesy dla tego zadania
    if (world_size < 2) {
        fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int number;
    if (world_rank == 0) {
        // Jeśli *rank* = 0, ustawiamy liczbę jako -1 i wysyłamy do procesu 1.
        number = -1;
        MPI_Send(
            /* data         = */ &number,
            /* count        = */ 1,
            /* datatype     = */ MPI_INT,
            /* destination  = */ 1,
            /* tag          = */ 0,
            /* communicator = */ MPI_COMM_WORLD);
    }
    else if (world_rank == 1) {
        MPI_Recv(
            /* data         = */ &number,
            /* count        = */ 1,
            /* datatype     = */ MPI_INT,
            /* source       = */ 0,
            /* tag          = */ 0,
            /* communicator = */ MPI_COMM_WORLD,
            /* status       = */ MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);
    }
    MPI_Finalize();
}
