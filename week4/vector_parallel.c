#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0)
            printf("Usage: %s <vector_size>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    long long N = atoll(argv[1]);

    long long chunk = N / size;
    long long start = rank * chunk;
    long long end = start + chunk;

    double local_sum = 0.0;

    double start_time = MPI_Wtime();

    for (long long i = start; i < end; i++) {
        local_sum += (double)i * (double)i;
    }

    double total_sum = 0.0;

    MPI_Reduce(&local_sum, &total_sum, 1,
               MPI_DOUBLE, MPI_SUM,
               0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Parallel sum = %.5f\n", total_sum);
        printf("MPI time = %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
