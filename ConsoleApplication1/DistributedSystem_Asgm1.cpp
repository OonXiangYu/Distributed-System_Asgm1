/* Name: Oon Xiang Yu */
/* Student Number: 3159517 */

/* Includes */
#include <iostream>
#include <string>
#include <mpi.h>

int main(int argc, char** argv) {

	// initialise the MPI library
	MPI_Init(NULL, NULL);

	// determine the world size and current rank
	int world_size, world_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// student number
	std::string name = "Oon Xiang Yu";
	int studNum = 3159517;

	// amount of data we need
	int numOfData = 120;

	// arrays that we need to communicating data
	int *total_array = new int[numOfData];
	int* partition = new int[numOfData / world_size];

	//if we are process zero then create an array of 40 random integers
	if (world_rank == 0) {
		std::cout << " total array start ----- " << std::endl;
		for (unsigned int i = 0; i < numOfData; i++) {
			total_array[i] = rand() % 10;
			std::cout << total_array[i] << ",";
			if (i % 10 == 9) std::cout << std::endl;
		}
	std::cout << " total array end ----- " << std::endl;
	}

	// run the sctter operation and then display the contents of all 4 nodes
	MPI_Scatter(total_array, numOfData / world_size, MPI_INT, partition, numOfData / world_size, MPI_INT, 0, MPI_COMM_WORLD);

	std::cout << "rank" << world_rank << " received partition: ";

	for (unsigned int i = 0; i < numOfData / world_size; i++)
		std::cout << partition[i] << ",";
	std::cout << std::endl;

	// always finalise the MPI library
	MPI_Finalize();

	// clear up memory before we finish
	delete[] total_array;
	delete[] partition;
}