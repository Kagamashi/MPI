#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    
    //Inicjalizacja środowiska MPI
    MPI_Init(NULL, NULL);
    
    //*Rank* i rozmiar komunikatora
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    printf("World rank = %d \n", world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int token;
    // Otrzymanie wiadomości od najniższego procesu i wysłanie do procesu najwyższego
    // Trzeba uważać na sytuację, gdy jest się pierwszym procesem do przeciwdziałania *deadlock*
    if (world_rank != 0) { 
        MPI_Recv(&token, 
                 1, 
                 MPI_INT, 
                 world_rank - 1, //odebranie od procesu z *rank* o 1 niższym
                 0, 
                 MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        printf("Process %d received token %d from process %d\n", 
            world_rank, token, world_rank - 1);
    }
    else {
        // Ustawienie wartości tokena jeśli jest się procesem 0
        token = -1;
    }

    MPI_Send(&token, 
             1, 
             MPI_INT, 
             (world_rank + 1) % world_size, // wysłanie do procesu
             0,
             MPI_COMM_WORLD);

    // Proces 0 może odbierać od ostatniego procesu. To upewnia nas że przynajmniej jedno MPI_Send zostanie zainicjowane przed wszystkimi MPI_Recvs (aby uniknać deadlocku)
    // deadlock - dwie różne akcje czekają na siebie nawzajem, więc żadna z nich nie może się zakończyć
    if (world_rank == 0) {
        MPI_Recv(&token, 
                 1, 
                 MPI_INT, 
                 world_size - 1, 
                 0, 
                 MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        printf("Process %d received token %d from process %d\n", 
            world_rank, token, world_size - 1);
    }

    MPI_Finalize();
}
