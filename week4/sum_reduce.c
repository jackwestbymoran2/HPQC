#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, size;

    if (argc != 2)
    {
        printf("usage: mpirun -np <p> ./a.out <n>\n");
        return 1;
    }

    int n = atoi(argv[1]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk = n / size;
    int *data = malloc(chunk * sizeof(int));

    for (int i = 0; i < chunk; i++)
        data[i] = rank + 1;

    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();

    int local_sum = 0;
    for (int i = 0; i < chunk; i++)
        local_sum += data[i];

    int total_sum = 0;

    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0)
    {
        printf("[reduce] sum=%d time=%f\n", total_sum, end - start);
    }

    free(data);
    MPI_Finalize();
    return 0;
}
