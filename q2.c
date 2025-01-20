#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define TOLERANCE 1e-9
#define EPS 1e-6

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

void read_size(int *n) {
    FILE *f = fopen("inputfiles/input.in", "r");
    if (f == NULL) {
        printf("Error: Cannot open input file.\n");
        exit(EXIT_FAILURE);
    }

    fscanf(f, "%d", n);
    fclose(f);
}

void read_matrix(int n, double **mat) {
    char filename[100];
    snprintf(filename, sizeof(filename), "inputfiles/mat_%06d.in", n);

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: Cannot open input matrix file.\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            fscanf(f, "%lf,", &mat[i][j]);
        }
    }

    fclose(f);
}

void read_vector(int n, double *vec, int vec_num) {
    char filename[50];
    sprintf(filename, "inputfiles/vec_%06d_%06d.in", n, vec_num);

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: Cannot open input vector file.\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<n; i++) {
        fscanf(f, "%lf,", &vec[i]);
    }

    fclose(f);
}


bool check_is_eigenvector(int n, double **mat, double *vec, int vecnum, double *eigenvalue) {
    double *y = (double *)malloc(n * sizeof(double));

    // Compute y = mat * vec
    for (int i = 0; i < n; i++) {
        y[i]  = 0;
        for (int j = 0; j < n; j++) {
            y[i] += mat[i][j] * vec[j];
        }
    }

    // Compute lambda 
    int valid_lambda = 0;
    double min_val = INFINITY, max_val = -INFINITY;
    for (int i = 0; i < n; i++) {
        if (fabs(vec[i]) < TOLERANCE) continue; // Avoid division by zero

        double lambda = y[i] / vec[i];

        if (!valid_lambda) {
            valid_lambda = 1;
            *eigenvalue = lambda;
            min_val = max_val = lambda;
        } else {
            min_val = min(min_val, lambda);
            max_val = max(max_val, lambda);
        }
    }

    char vec_filename[50];
    sprintf(vec_filename, "vec_%06d_%06d.in", n, vecnum);

    bool is_eigenvector;
    if (valid_lambda && (max_val - min_val) < EPS) {
        is_eigenvector = true;
        printf("%s : Yes : %lf\n", vec_filename, *eigenvalue);
    } else {
        is_eigenvector = false;
        printf("%s : Not an eigenvector\n", vec_filename);
    }

    free(y);

    return is_eigenvector;
}

void append_result_to_file(int n, int vec_num, double eigenvalue) {
    char filename[50];
    sprintf(filename, "inputfiles/vec_%06d_%06d.in", n, vec_num);

    FILE *f = fopen(filename, "a");
    if (f == NULL) {
        printf("Error: Cannot open output file.\n");
        return;
    }

    fprintf(f, "%.15f\n", eigenvalue);
    fclose(f);
}

int main() {
    // Read n
    int n;
    read_size(&n);

    // Read matrix
    double **mat = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        mat[i] = (double *)malloc(n * sizeof(double));
    }
    read_matrix(n, mat);

    // Read vectors and check if eigenvector
    for (int i = 1; i < 5; i++) {
        double *vec = (double *)malloc(n * sizeof(double));
        read_vector(n, vec, i);

        double eigenvalue;
        bool is_eigenvector = check_is_eigenvector(n, mat, vec, i, &eigenvalue);

        if (is_eigenvector) {
            append_result_to_file(n, i, eigenvalue);
        }

        free(vec);
    }

    // Free matrix memory
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);

    return 0;
}
