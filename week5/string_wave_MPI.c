#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

/* holds runtime parameters */
typedef struct {
    int cycles;
    int samples;
    char output_file[256];
} params_t;


/* parse command line arguments */
params_t parse_args(int argc, char **argv)
{
    params_t p;

    if (argc != 4)
    {
        fprintf(stderr, "usage: %s <cycles> <samples> <output_file>\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    p.cycles = atoi(argv[1]);
    p.samples = atoi(argv[2]);
    snprintf(p.output_file, sizeof(p.output_file), "%s", argv[3]);

    if (p.cycles <= 0 || p.samples <= 0)
    {
        fprintf(stderr, "error: cycles and samples must be positive\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    return p;
}


/* main program */
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* parse args */
    params_t p = parse_args(argc, argv);

    int global_n = p.samples;
    int cycles = p.cycles;

    /* compute local domain size */
    int local_n = global_n / size;

    /* allocate local arrays (+2 for ghost cells) */
    double *current = (double *)malloc((local_n + 2) * sizeof(double));
    double *next = (double *)malloc((local_n + 2) * sizeof(double));

    if (!current || !next)
    {
        fprintf(stderr, "memory allocation failed\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    /* initialise */
    for (int i = 0; i < local_n + 2; i++)
    {
        current[i] = 0.0;
        next[i] = 0.0;
    }

    /* initial condition (simple pulse in middle rank 0 only) */
    if (rank == 0)
    {
        current[local_n / 2] = 1.0;
    }

    /* neighbours */
    int left = rank - 1;
    int right = rank + 1;

    for (int c = 0; c < cycles; c++)
    {
        /* exchange ghost cells */
        MPI_Status status;

        /* send left, receive right ghost */
        if (right < size)
        {
            MPI_Sendrecv(&current[local_n], 1, MPI_DOUBLE, right, 0,
                         &current[local_n + 1], 1, MPI_DOUBLE, right, 0,
                         MPI_COMM_WORLD, &status);
        }
        else
        {
            current[local_n + 1] = 0.0;
        }

        /* send right, receive left ghost */
        if (left >= 0)
        {
            MPI_Sendrecv(&current[1], 1, MPI_DOUBLE, left, 1,
                         &current[0], 1, MPI_DOUBLE, left, 1,
                         MPI_COMM_WORLD, &status);
        }
        else
        {
            current[0] = 0.0;
        }

        /* update interior points */
        for (int i = 1; i <= local_n; i++)
        {
            next[i] = 0.5 * (current[i - 1] + current[i + 1]);
        }

        /* swap pointers */
        double *tmp = current;
        current = next;
        next = tmp;
    }

    /* gather results at root */
    double *global = NULL;

    if (rank == 0)
    {
        global = (double *)malloc(global_n * sizeof(double));
    }

    MPI_Gather(&current[1], local_n, MPI_DOUBLE,
               global, local_n, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    /* write output (root only) */
    if (rank == 0)
    {
        FILE *fp = fopen(p.output_file, "w");

        if (!fp)
        {
            fprintf(stderr, "error opening output file\n");
            MPI_Abort(MPI_COMM_WORLD, -1);
        }

        for (int i = 0; i < global_n; i++)
        {
            fprintf(fp, "%d %lf\n", i, global[i]);
        }

        fclose(fp);
        free(global);
    }

    free(current);
    free(next);

    MPI_Finalize();
    return 0;
}
