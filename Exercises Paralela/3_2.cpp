#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    int my_rank, comm_sz;
    long long number_tosses;
    long long number_in_circle = 0;
    long long total_in_circle = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (my_rank == 0) {
        std::cout << "Ingrese el número total de lanzamientos: ";
        std::cin >> number_tosses;
    }

    MPI_Bcast(&number_tosses, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    srand(time(NULL) + my_rank);

    long long local_tosses = number_tosses / comm_sz;
    for (long long toss = 0; toss < local_tosses; toss++) {
        double x = static_cast<double>(rand()) / RAND_MAX * 2.0 - 1.0;
        double y = static_cast<double>(rand()) / RAND_MAX * 2.0 - 1.0;
        double distance_squared = x * x + y * y;

        if (distance_squared <= 1.0) {
            number_in_circle++;
        }
    }

    MPI_Reduce(&number_in_circle, &total_in_circle, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        double pi_estimate = 4.0 * total_in_circle / number_tosses;
        std::cout << "Valor estimado de π: " << pi_estimate << std::endl;
    }

    MPI_Finalize();
    return 0;
}
