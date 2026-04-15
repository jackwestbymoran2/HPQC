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

    int *recvbuf = NULL;

    if (rank == 0)
        recvbuf = malloc(size * sizeof(int));

    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();

    int local_sum = 0;
    for (int i = 0; i < chunk; i++)
        local_sum += data[i];

    MPI_Gather(&local_sum, 1, MPI_INT,
               recvbuf, 1, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        int total = 0;

        for (int i = 0; i < size; i++)
            total += recvbuf[i];

        double end = MPI_Wtime();

        printf("[gather] sum=%d time=%f\n", total, end - start);

        free(recvbuf);
    }

    free(data);
    MPI_Finalize();
    return 0;
}
