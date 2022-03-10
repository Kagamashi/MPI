#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    const int PING_PONG_LIMIT = 10; // Ustawienie limitu wymiany wiadomości

    // Inicjalizacja środowiska MPI
    MPI_Init(NULL, NULL);

    //Sprawdzenie liczby procesów i ich *rank*
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Zakładamy 2 procesy dla tego zadania
    if (world_size != 2) {
        fprintf(stderr, "World size must be two for %s\n", argv[0]); //stderr - błąd standardowy
        MPI_Abort(MPI_COMM_WORLD, 1); 
    }

    int ping_pong_count = 0; 
    int partner_rank = (world_rank + 1) % 2; // Określenie *rank* partnera 

    while (ping_pong_count < PING_PONG_LIMIT) { // Wymiana wiadomości między procesami trwa tak długo aż nie osiągniemy limitu
        if (world_rank == ping_pong_count % 2) { // Jeśli *rank* procesu = ping_pong_count % 2 => to będzie 0

            // Zwiększenie licznika o 1 przed wysłaniem wiadomości
            ping_pong_count++;
            MPI_Send(&ping_pong_count,  //wysłanie wiadomości
                     1, 
                     MPI_INT, 
                     partner_rank, 
                     0, 
                     MPI_COMM_WORLD);

            printf("%d sent and incremented ping_pong_count %d to %d\n",
                world_rank, ping_pong_count, partner_rank);
        }
        else {
            MPI_Recv(&ping_pong_count, 
                     1, 
                     MPI_INT, 
                     partner_rank, 
                     0, 
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            printf("%d received ping_pong_count %d from %d\n",
                world_rank, ping_pong_count, partner_rank);
        }
    }
    MPI_Finalize();
}
