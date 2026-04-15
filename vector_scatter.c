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

    int *a = malloc(n * sizeof(int));
    int *b = malloc(n * sizeof(int));

    int *local_a = malloc(chunk * sizeof(int));
    int *local_b = malloc(chunk * sizeof(int));
    int *local_c = malloc(chunk * sizeof(int));

    if (rank == 0)
    {
        for (int i = 0; i < n; i++)
        {
            a[i] = i;
            b[i] = i;
        }
    }

    double start = MPI_Wtime();

    MPI_Scatter(a, chunk, MPI_INT, local_a, chunk, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b, chunk, MPI_INT, local_b, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < chunk; i++)
        local_c[i] = local_a[i] + local_b[i];

    MPI_Gather(local_c, chunk, MPI_INT, a, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0)
    {
        printf("[scatter] n=%d time=%f seconds\n", n, end - start);
    }

    free(a); free(b);
    free(local_a); free(local_b); free(local_c);

    MPI_Finalize();
    return 0;
}
