#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *fp;
    const char *filename = "../data/output.txt";
    fp = fopen(filename, "r");
    if (!fp) {
        perror("Unable to open file");
        return 1;
    }

    char buffer[256];
    clock_t start = clock();

    // read all lines
    while (fgets(buffer, sizeof(buffer), fp)) {
        // optionally do something with buffer
    }

    clock_t end = clock();
    fclose(fp);

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time to read file: %f seconds\n", elapsed);

    return 0;
}
