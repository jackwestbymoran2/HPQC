#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void init_mpi(int argc, char **argv, int *rank, int *size)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, rank);
    MPI_Comm_size(MPI_COMM_WORLD, size);
}

int main(int argc, char **argv)
{
    int rank, size;

    if (argc != 2)
    {
        if (rank == 0)
            printf("usage: mpirun -np <p> ./a.out <n>\n");
        return 1;
    }

    int n = atoi(argv[1]);

    init_mpi(argc, argv, &rank, &size);

    int *a = malloc(n * sizeof(int));
    int *b = malloc(n * sizeof(int));
    int *c = malloc(n * sizeof(int));

    if (rank == 0)
    {
        for (int i = 0; i < n; i++)
        {
            a[i] = i;
            b[i] = i;
        }
    }

    double start = MPI_Wtime();

    MPI_Bcast(a, n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n; i++)
        c[i] = a[i] + b[i];

    double end = MPI_Wtime();

    if (rank == 0)
    {
        printf("[bcast] n=%d time=%f seconds\n", n, end - start);
    }

    free(a); free(b); free(c);
    MPI_Finalize();
    return 0;
}
