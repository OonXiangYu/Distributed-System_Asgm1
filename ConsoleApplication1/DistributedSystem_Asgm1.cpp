/* Name: Oon Xiang Yu */
/* Student Number: 3159517 */

// BSCH - Distributed Systems
// Assignment 1
// TODO: Student Name - Student Number

#include <iostream>
#include <fstream>
#include <mpi.h>
#include <string>
#include <cstdlib>

//Global variables
const int arraySize = 120;
int world_size, world_rank;
int numberArray[arraySize];

//Change to your own details
int yourStudentNumber = 3159517; //TODO change this to your actual student number
std::string yourFullName = "Oon Xiang Yu"; //TODO change this to your full name

int main(int argc, char** argv) {

	// initialise the MPI library
	MPI_Init(NULL, NULL);

	// determine the world size and current rank
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// Print my name and student number
	if (world_rank == 0) {
		std::cout << "Name: " << yourFullName << std::endl;
		std::cout << "Student Number: " << yourStudentNumber << std::endl;
	}

	// Check can world size evenly divide the array size
	if (arraySize % world_size != 0) {
		if (world_rank == 0) {
			std::cerr << "Error: world size does not evenly divide the array size" << std::endl;
		}
		MPI_Abort(MPI_COMM_WORLD, 1); // terminate program for all processes ( Explain in document )
		return 1;
	}

	// Populate the random number with my student number
	if (world_rank == 0) {

		srand(3159517);

		std::cout << " total array start ----- " << std::endl;
		for (unsigned int i = 0; i < arraySize; i++) {
			numberArray[i] = rand() % 1000; // produce number from 0 - 999
			std::cout << numberArray[i] << ",";
			if (i % 10 == 9) std::cout << std::endl;
		}
		std::cout << " total array end ----- " << std::endl;
	}

	int partitionSize = arraySize / world_size; // variable to determine how many number we need to assign into each node
	int* partition = new int[partitionSize];


	// Run the sctter operation and then display the contents
	MPI_Scatter(numberArray, partitionSize, MPI_INT, partition, partitionSize, MPI_INT, 0, MPI_COMM_WORLD);

	std::cout << "rank" << world_rank << " received partition: ";

	for (unsigned int i = 0; i < partitionSize; i++)
		std::cout << partition[i] << ",";
		std::cout << std::endl;

	// var that store the first element of partition
	int nodeNumber = partition[0];

	// Gather all nodeNumber back to Node 0
	int* gatheredNumbers = new int[world_size];
	MPI_Gather(&nodeNumber, 1, MPI_INT, gatheredNumbers, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// Node 0 prints all collected nodeNumbers
	if (world_rank == 0) {
		std::cout << "\nNode 0 collected the first number from each node:" << std::endl;
		for (int i = 0; i < world_size; ++i)
			std::cout << "From node " << i << ": " << gatheredNumbers[i] << std::endl;
	}

	// always finalise the MPI library
	MPI_Finalize();

}