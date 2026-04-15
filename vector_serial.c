#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <vector_size>\n", argv[0]);
        return 1;
    }

    long long N = atoll(argv[1]);

    double sum = 0.0;

    clock_t start = clock();

    for (long long i = 0; i < N; i++) {
        sum += (double)i * (double)i;
    }

    clock_t end = clock();
    double cpu_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Serial sum = %.5f\n", sum);
    printf("Internal CPU time = %f seconds\n", cpu_time);

    return 0;
}
