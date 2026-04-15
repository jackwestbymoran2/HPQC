#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/* custom sum operation (scalar safe version) */
void my_sum(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
    int *in = (int *)invec;
    int *inout = (int *)inoutvec;

    for (int i = 0; i < *len; i++)
    {
        inout[i] += in[i];
    }
}

int main(int argc, char **argv)
{
    int rank, size;

    if (argc != 2)
    {
        if (rank == 0)
            printf("usage: mpirun -np p ./a.out n\n");
        return 1;
    }

    int n = atoi(argv[1]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk = n / size;

    int *data = malloc(chunk * sizeof(int));

    for (int i = 0; i < chunk; i++)
        data[i] = 1;

    /* compute local sum FIRST */
    int local_sum = 0;
    for (int i = 0; i < chunk; i++)
        local_sum += data[i];

    MPI_Op my_op;
    MPI_Op_create((MPI_User_function *)my_sum, 1, &my_op);

    int custom_result = 0;
    int builtin_result = 0;

    MPI_Barrier(MPI_COMM_WORLD);

    double start1 = MPI_Wtime();

    MPI_Reduce(&local_sum, &custom_result, 1, MPI_INT,
               my_op, 0, MPI_COMM_WORLD);

    double end1 = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);

    double start2 = MPI_Wtime();

    MPI_Reduce(&local_sum, &builtin_result, 1, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);

    double end2 = MPI_Wtime();

    if (rank == 0)
    {
        printf("\nresults:\n");
        printf("custom reduce = %d\n", custom_result);
        printf("builtin reduce = %d\n", builtin_result);

        printf("\nperformance:\n");
        printf("custom time = %f\n", end1 - start1);
        printf("builtin time = %f\n", end2 - start2);
    }

    MPI_Op_free(&my_op);
    free(data);
    MPI_Finalize();
    return 0;
}
