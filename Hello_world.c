#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	//Inicjalizacja środowiska MPI
	MPI_Init(NULL, NULL);

	//Pobranie liczby procesów
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	//Pobranie *rank* procesora
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	//Pobranie nazwy procesora
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	//Wydrukowanie wiadomości
	printf("Hello world from processor %s, rank %d out of %d processor\n", 
		processor_name, world_rank, world_size);

	//Finalizacja środowiska MPI
	MPI_Finalize();
}
