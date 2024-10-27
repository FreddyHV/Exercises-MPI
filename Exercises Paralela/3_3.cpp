#include <mpi.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    int my_rank, comm_sz;
    int local_value;
    int global_sum;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    local_value = my_rank + 1;

    int step;
    for (step = 1; step < comm_sz; step *= 2) {
        if (my_rank % (2 * step) == 0) {
            if (my_rank + step < comm_sz) {
                int received_value;
                MPI_Recv(&received_value, 1, MPI_INT, my_rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_value += received_value;
            }
        } else if (my_rank % (2 * step) == step) {
            MPI_Send(&local_value, 1, MPI_INT, my_rank - step, 0, MPI_COMM_WORLD);
        }
    }

    if (my_rank < comm_sz && my_rank % 2 == 0) {
        if (my_rank + 1 < comm_sz) {
            int received_value;
            MPI_Recv(&received_value, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_value += received_value;
        }
    } else if (my_rank % 2 == 1) {
        MPI_Send(&local_value, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
    }

    if (my_rank == 0) {
        global_sum = local_value;
        std::cout << "La suma global es: " << global_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}
