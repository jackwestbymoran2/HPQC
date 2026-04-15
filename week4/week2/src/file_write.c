#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *fp;
    const char *filename = "../data/output.txt";
    fp = fopen(filename, "w");
    if (!fp) {
        perror("Unable to open file");
        return 1;
    }

    clock_t start = clock();

    // write 10,000 lines to file
    for (int i = 0; i < 10000; i++) {
        fprintf(fp, "Line %d: The quick brown fox jumps over the lazy dog.\n", i);
    }

    clock_t end = clock();
    fclose(fp);

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time to write file: %f seconds\n", elapsed);

    return 0;
}
