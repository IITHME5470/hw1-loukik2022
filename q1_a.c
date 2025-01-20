#include <stdio.h>
#include <stdlib.h>

void print_to_file(int n, double **A, int format_flag) {
    char filename[100];
    sprintf(filename, "outputfiles/array_%06d_%s.out", n, format_flag ? "bin" : "asc");

    FILE *f = fopen(filename, format_flag ? "wb" : "w");
    if (f == NULL) {
        printf("Error: Cannot open output file.\n");
        exit(EXIT_FAILURE);
    }

    if(format_flag) {
        // print in binary
        for (int i = 0; i < n; i++) {
            fwrite(A[i], sizeof(double), n, f);
        }
    }
    else {
        // print in asci
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fprintf(f, "%.15f ", A[i][j]);
            }
            fprintf(f, "\n");    
        }
    }

    fclose(f);
}

int main() {
    // read n and format_flag from input.in 
    FILE *f = fopen("inputfiles/input.in", "r");
    if (f == NULL) {
        printf("Error: Cannot open input file.\n");
        exit(EXIT_FAILURE);
    }

    int n, format_flag;
    fscanf(f, "%d %d", &n, &format_flag);
    fclose(f);

    // allocate memory for A[n][n] and fill with i+j
    double **A = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        A[i] = (double *)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            A[i][j] = i + j;
        }
    }

    print_to_file(n, A, format_flag);

    // memory deallocation
    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}