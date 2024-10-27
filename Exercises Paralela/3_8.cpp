#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

std::vector<int> merge(const std::vector<int>& left, const std::vector<int>& right) {
    std::vector<int> result;
    int i = 0, j = 0;
    while (i < left.size() && j < right.size()) {
        if (left[i] < right[j]) {
            result.push_back(left[i]);
            ++i;
        } else {
            result.push_back(right[j]);
            ++j;
        }
    }
    while (i < left.size()) {
        result.push_back(left[i]);
        ++i;
    }
    while (j < right.size()) {
        result.push_back(right[j]);
        ++j;
    }
    return result;
}

int main(int argc, char* argv[]) {
    int my_rank, comm_sz;
    int n;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (my_rank == 0) {
        std::cout << "Ingrese el tamaño de la lista (n): ";
        std::cin >> n;
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_n = n / comm_sz;
    std::vector<int> local_list(local_n);

    srand(time(0) + my_rank);

    for (int i = 0; i < local_n; ++i) {
        local_list[i] = rand() % 100;
    }

    std::sort(local_list.begin(), local_list.end());

    if (my_rank == 0) {
        std::cout << "Listas locales ordenadas:" << std::endl;
    }

    for (int rank = 0; rank < comm_sz; ++rank) {
        if (my_rank == rank) {
            std::cout << "Proceso " << my_rank << ": ";
            for (int val : local_list) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    int step = 1;
    while (step < comm_sz) {
        if (my_rank % (2 * step) == 0) {
            if (my_rank + step < comm_sz) {
                int recv_size;
                MPI_Recv(&recv_size, 1, MPI_INT, my_rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                std::vector<int> recv_list(recv_size);
                MPI_Recv(recv_list.data(), recv_size, MPI_INT, my_rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_list = merge(local_list, recv_list);
            }
        } else {
            int send_size = local_list.size();
            MPI_Send(&send_size, 1, MPI_INT, my_rank - step, 0, MPI_COMM_WORLD);
            MPI_Send(local_list.data(), send_size, MPI_INT, my_rank - step, 0, MPI_COMM_WORLD);
            break;
        }
        step *= 2;
    }

    if (my_rank == 0) {
        std::cout << "\nLista global ordenada:" << std::endl;
        for (int val : local_list) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
