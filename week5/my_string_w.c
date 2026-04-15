#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* struct to hold runtime parameters */
typedef struct {
    int cycles;
    int samples;
    char output_file[256];
} params_t;

/* initialises a vector with a constant value */
void init_vector(double vector[], int size, double initial)
{
    for (int i = 0; i < size; i++)
    {
        vector[i] = initial;
    }
}

/* prints vector values to screen */
void print_vector(double vector[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d, %lf\n", i, vector[i]);
    }
}

/* parses and validates command line arguments */
params_t check_args(int argc, char **argv)
{
    params_t p;

    if (argc != 4)
    {
        fprintf(stderr, "error: incorrect usage\n");
        fprintf(stderr, "usage: %s <cycles> <samples> <output_file>\n", argv[0]);
        exit(-1);
    }

    p.cycles = atoi(argv[1]);
    p.samples = atoi(argv[2]);

    snprintf(p.output_file, sizeof(p.output_file), "%s", argv[3]);

    if (p.cycles <= 0 || p.samples <= 0)
    {
        fprintf(stderr, "error: cycles and samples must be positive integers\n");
        exit(-1);
    }

    return p;
}

/* main program */
int main(int argc, char **argv)
{
    params_t p = check_args(argc, argv);

    int cycles = p.cycles;
    int samples = p.samples;
    char *outfile = p.output_file;

    /* allocate vectors */
    double *vector = (double *)malloc(samples * sizeof(double));

    if (vector == NULL)
    {
        fprintf(stderr, "error: memory allocation failed\n");
        exit(-1);
    }

    /* initialise vector */
    init_vector(vector, samples, 0.0);

    /* open output file */
    FILE *fp = fopen(outfile, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "error: cannot open output file\n");
        free(vector);
        exit(-1);
    }

    /* simple wave update loop (placeholder structure) */
    for (int c = 0; c < cycles; c++)
    {
        for (int i = 1; i < samples - 1; i++)
        {
            vector[i] = 0.5 * (vector[i - 1] + vector[i + 1]);
        }
    }

    /* write results */
    for (int i = 0; i < samples; i++)
    {
        fprintf(fp, "%d %lf\n", i, vector[i]);
    }

    fclose(fp);
    free(vector);

    return 0;
}
