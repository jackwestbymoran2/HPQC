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

/* root process (rank 0) handles ping-pong loop */
void root_pingpong(int num_pings)
{
    int counter = 0;
    int msg;
    int partner = 1;

    double start_time, end_time;

    MPI_Status status;

    /* start timing */
    start_time = MPI_Wtime();

    while (counter < num_pings)
    {
        /* send ping */
        MPI_Send(&counter, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);

        /* receive pong */
        MPI_Recv(&msg, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, &status);

        counter = msg;
    }

    /* end timing */
    end_time = MPI_Wtime();

    double elapsed_time = end_time - start_time;
    double avg_time = elapsed_time / (2.0 * num_pings);

    printf("final counter = %d\n", counter);
    printf("elapsed time = %f seconds\n", elapsed_time);
    printf("average time per send/recv = %e seconds\n", avg_time);
}

/* client process (rank 1) responds to root */
void client_pingpong(int num_pings)
{
    int counter;
    int partner = 0;

    MPI_Status status;

    for (int i = 0; i < num_pings; i++)
    {
        /* receive ping */
        MPI_Recv(&counter, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, &status);

        /* increment counter */
        counter++;

        /* send pong back */
        MPI_Send(&counter, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
    }
}

int main(int argc, char **argv)
{
    int rank, size;

    /* check arguments */
    if (argc != 2)
    {
        printf("usage: mpirun -np 2 ./pingpong <num_pings>\n");
        return 1;
    }

    int num_pings = atoi(argv[1]);

    /* initialise mpi */
    init_mpi(argc, argv, &rank, &size);

    /* must use exactly 2 processes */
    if (size != 2)
    {
        if (rank == 0)
            printf("error: must run with exactly 2 processes\n");

        MPI_Finalize();
        return 1;
    }

    /* run correct role */
    if (rank == 0)
    {
        root_pingpong(num_pings);
    }
    else if (rank == 1)
    {
        client_pingpong(num_pings);
    }

    MPI_Finalize();
    return 0;
}
