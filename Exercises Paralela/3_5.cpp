#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    int my_rank, comm_sz;
    int n;
    std::vector<double> A, x, local_A, local_y;
    std::vector<double> y;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (my_rank == 0) {
        std::cout << "Ingrese el tamaño de la matriz (n): ";
        std::cin >> n;

        if (n % comm_sz != 0) {
            std::cerr << "El tamaño de la matriz debe ser divisible por el número de procesos." << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        A.resize(n * n);
        std::cout << "Ingrese los elementos de la matriz:" << std::endl;
        for (int i = 0; i < n * n; ++i) {
            std::cin >> A[i];
        }

        x.resize(n);
        std::cout << "Ingrese los elementos del vector:" << std::endl;
        for (int i = 0; i < n; ++i) {
            std::cin >> x[i];
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_n = n / comm_sz;
    local_A.resize(local_n * n);
    local_y.resize(local_n);

    MPI_Scatter(A.data(), local_n * n, MPI_DOUBLE, local_A.data(), local_n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Bcast(x.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; ++i) {
        local_y[i] = 0.0;
        for (int j = 0; j < n; ++j) {
            local_y[i] += local_A[i * n + j] * x[j];
        }
    }

    if (my_rank == 0) {
        y.resize(n);
    }

    MPI_Gather(local_y.data(), local_n, MPI_DOUBLE, y.data(), local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        std::cout << "El resultado de la multiplicación es:" << std::endl;
        for (double val : y) {
            std::cout << val << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}

