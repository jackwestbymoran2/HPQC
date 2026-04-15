#include <stdio.h>
#include <mpi.h>
#include <stdio.h>
#include <mpi.h>

/* initialise mpi */
void init_mpi(int argc, char **argv, int *rank, int *size)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, rank);
    MPI_Comm_size(MPI_COMM_WORLD, size);
}

/* root process receives messages and times each receive */
void root_receive(int size, int tag)
{
    int recv_message;
    MPI_Status status;

    double start_time, end_time;

    for (int source = 1; source < size; source++)
    {
        /* start timing before receive */
        start_time = MPI_Wtime();

        /* receive message from worker */
        MPI_Recv(&recv_message, 1, MPI_INT, source, tag,
                 MPI_COMM_WORLD, &status);

        /* end timing after receive */
        end_time = MPI_Wtime();

        /* print received value and timing */
        printf("[rank 0] received %d from rank %d | time = %f seconds\n",
               recv_message, source, end_time - start_time);
    }
}

/* worker processes send messages and time each send */
void worker_send(int rank, int tag)
{
    int send_message = rank * 10;
    int dest = 0;

    double start_time, end_time;

    /* start timing before send */
    start_time = MPI_Wtime();

    /* send message to root */
    MPI_Send(&send_message, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);

    /* end timing after send */
    end_time = MPI_Wtime();

    /* print sent value and timing */
    printf("[rank %d] sent %d to rank %d | time = %f seconds\n",
           rank, send_message, dest, end_time - start_time);
}

int main(int argc, char **argv)
{
    int rank, size;
    int tag = 0;

    /* initialise mpi environment */
    init_mpi(argc, argv, &rank, &size);

    /* ensure at least 2 processes */
    if (size > 1)
    {
        /* root process handles receives */
        if (rank == 0)
        {
            root_receive(size, tag);
        }
        /* worker processes send data */
        else
        {
            worker_send(rank, tag);
        }
    }
    else
    {
        /* safety check for insufficient processes */
        printf("unable to communicate with less than 2 processes. size = %d\n", size);
    }

    /* finalise mpi environment */
    MPI_Finalize();

    return 0;
}
