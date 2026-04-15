#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/* initialise mpi */
void init_mpi(int argc, char **argv, int *rank, int *size)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, rank);
    MPI_Comm_size(MPI_COMM_WORLD, size);
}

/* root process */
void root_pingpong(int *buffer, int count, int num_pings)
{
    int partner = 1;
    MPI_Status status;

    double start_time = MPI_Wtime();
    double end_time;

    for (int i = 0; i < num_pings; i++)
    {
        MPI_Send(buffer, count, MPI_INT, partner, 0, MPI_COMM_WORLD);
        MPI_Recv(buffer, count, MPI_INT, partner, 0, MPI_COMM_WORLD, &status);
    }

    end_time = MPI_Wtime();

    double elapsed = end_time - start_time;
    double avg_time = elapsed / (2.0 * num_pings);

    printf("size = %d ints | total = %f s | avg = %e s\n",
           count, elapsed, avg_time);
}

/* client process */
void client_pingpong(int *buffer, int count, int num_pings)
{
    int partner = 0;
    MPI_Status status;

    for (int i = 0; i < num_pings; i++)
    {
        MPI_Recv(buffer, count, MPI_INT, partner, 0, MPI_COMM_WORLD, &status);
        MPI_Send(buffer, count, MPI_INT, partner, 0, MPI_COMM_WORLD);
    }
}

int main(int argc, char **argv)
{
    int rank, size;

    if (argc != 3)
    {
        printf("usage: mpirun -np 2 ./bandwidth_pingpong <num_pings> <num_ints>\n");
        return 1;
    }

    int num_pings = atoi(argv[1]);
    int num_ints = atoi(argv[2]);

    init_mpi(argc, argv, &rank, &size);

    if (size != 2)
    {
        if (rank == 0)
            printf("error: must use exactly 2 processes\n");

        MPI_Finalize();
        return 1;
    }

    /* allocate buffer */
    int *buffer = (int *)malloc(num_ints * sizeof(int));

    /* initialise buffer */
    for (int i = 0; i < num_ints; i++)
        buffer[i] = i;

    if (rank == 0)
        root_pingpong(buffer, num_ints, num_pings);
    else
        client_pingpong(buffer, num_ints, num_pings);

    free(buffer);
    MPI_Finalize();
    return 0;
}
