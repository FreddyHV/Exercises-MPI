#include <mpi.h>
#include <iostream>
#include <vector>

int Find_bin(float data, const std::vector<float>& bin_maxes, int bin_count, float min_meas);

int main(int argc, char* argv[]) {
    int bin_count, data_count, local_data_count, bin;
    float min_meas = 0.0, max_meas = 5.0, bin_width;
    int my_rank, comm_sz;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    std::vector<float> bin_maxes, data;
    std::vector<int> bin_counts, loc_bin_counts;

    if (my_rank == 0) {
        data_count = 20;
        bin_count = 5;
        data = {1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 3.2, 0.3, 
                 4.9, 2.4, 3.1, 4.4, 3.9, 0.4, 4.2, 4.5, 4.9, 0.9};

        bin_width = (max_meas - min_meas) / bin_count;
        bin_maxes.resize(bin_count);
        bin_counts.resize(bin_count, 0);

        for (int i = 0; i < bin_count; ++i) {
            bin_maxes[i] = min_meas + bin_width * (i + 1);
        }

        local_data_count = data_count / comm_sz;
    }

    MPI_Bcast(&bin_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&local_data_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&min_meas, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&bin_width, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (my_rank != 0) {
        bin_maxes.resize(bin_count);
    }
    MPI_Bcast(bin_maxes.data(), bin_count, MPI_FLOAT, 0, MPI_COMM_WORLD);

    std::vector<float> local_data(local_data_count);
    loc_bin_counts.resize(bin_count, 0);

    MPI_Scatter(data.data(), local_data_count, MPI_FLOAT, local_data.data(), local_data_count, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for (float val : local_data) {
        bin = Find_bin(val, bin_maxes, bin_count, min_meas);
        loc_bin_counts[bin]++;
    }

    if (my_rank == 0) {
        MPI_Reduce(MPI_IN_PLACE, bin_counts.data(), bin_count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    } else {
        MPI_Reduce(loc_bin_counts.data(), bin_counts.data(), bin_count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    if (my_rank == 0) {
        std::cout << "Histograma:\n";
        for (int i = 0; i < bin_count; ++i) {
            std::cout << "Bin " << i << ": " << bin_counts[i] << std::endl;
        }
    }

    MPI_Finalize(); 
    return 0;
}

int Find_bin(float data, const std::vector<float>& bin_maxes, int bin_count, float min_meas) {
    int bin = 0;
    while (bin < bin_count - 1 && data >= bin_maxes[bin]) {
        ++bin;
    }
    return bin;
}
