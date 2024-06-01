#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITER 100

void printMatrix(float **mat, int n) {
    printf("Matris:\n");
    int i,j;
    for ( i = 0; i < n; i++) {
        for ( j = 0; j < n + 1; j++) {
            printf("%.2f\t", mat[i][j]);
        }
        printf("\n");
    }
}

void gaussSeidel(float **mat, int n, float epsilon) {
    float *x = (float *)malloc(n * sizeof(float));
    float *prevX = (float *)malloc(n * sizeof(float));
	int i,j;
    printf("Baþlangýç deðerlerini girin:\n");
    for ( i = 0; i < n; i++) {
        printf("x%d: ", i + 1);
        scanf("%f", &x[i]);
        prevX[i] = x[i];
    }

    float error;
    int iter = 0;
    do {
        for ( i = 0; i < n; i++) {
            prevX[i] = x[i];
        }

        for ( i = 0; i < n; i++) {
            float sum = 0.0;
            for ( j = 0; j < n; j++) {
                if (j != i) {
                    sum += mat[i][j] * x[j];
                }
            }
            x[i] = (mat[i][n] - sum) / mat[i][i];
        }

        error = 0.0;
        for ( i = 0; i < n; i++) {
            error += fabs(x[i] - prevX[i]);
        }
        iter++;

        printf("Ýterasyon %d, Hata = %f\n", iter, error); // Hata deðerini yazdýr
    } while (error > epsilon && iter < MAX_ITER);

    if (iter == MAX_ITER) {
        printf("Yeterli yakýnsama saðlanamadý.\n");
    } else {
        printf("Gauss-Seidel yöntemiyle %d iterasyonda sonuca ulaþýldý.\n", iter);
        printf("Çözüm:\n");
        for ( i = 0; i < n; i++) {
            printf("x%d = %.4f\n", i + 1, x[i]);
        }
    }

    free(x);
    free(prevX);
}

int main() {
    int n;
    printf("Matris boyutunu girin: ");
    scanf("%d", &n);
	int i,j;
    // Matris oluþturma
    float **mat = (float **)malloc(n * sizeof(float *));
    for ( i = 0; i < n; i++) {
        mat[i] = (float *)malloc((n + 1) * sizeof(float));
        printf("%d. denklem katsayýlarýný ve eþitliklerini sýrasýyla girin:\n", i + 1);
        for ( j = 0; j < n + 1; j++) {
            scanf("%f", &mat[i][j]);
        }
    }

    float epsilon;
    printf("Hata deðerini girin: ");
    scanf("%f", &epsilon);

    printMatrix(mat, n);
    gaussSeidel(mat, n, epsilon);

    // Bellek temizleme
    for ( i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
    return 0;
}

