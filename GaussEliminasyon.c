#include <stdio.h>
#include <stdlib.h>
void getInputMatrix(float **matrix, int n, int m) {
    int i,j;
	printf("Matrisin elemanlarýný giriniz:\n");
    for ( i = 0; i < n; i++) {
        for ( j = 0; j < m; j++) {
            printf("[%d][%d]elemaný giriniz: ", i+1, j+1);
            scanf("%f", &matrix[i][j]);
        }
    }
}
void birimMatris(float **matrix, int n){
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(i==j){
				*(*(matrix+i)+j)=1;
			}else *(*(matrix+i)+j)=0;
		}
	}
}
void printMatrix(float **matrix, int n, int m){
		int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			printf("%.2f ", matrix[i][j]);
		}printf("\n");
}
}
void swapRows(float **matrix, int row1, int row2, int n) {
    int i;
	for ( i = 0; i < n; i++) {
        float temp = matrix[row1][i];
        matrix[row1][i] = matrix[row2][i];
        matrix[row2][i] = temp;
    }
}
void multiplyRow(float **matrix, int row, float scalar, int n) {
	int i;
    for ( i = 0; i < n; i++) {
        matrix[row][i] *= scalar;
    }
}
void addMultipleOfRow(float **matrix, int sourceRow, int destinationRow, float multiple, int n) {
   int i;
    for ( i = 0; i < n; i++) {
        matrix[destinationRow][i] += multiple * matrix[sourceRow][i];
    }
}
float** yerAc(int n, int m){
	int i;
	float **matrix = (float **)calloc(n ,sizeof(float *));
    if (matrix == NULL) {
        printf("Bellek tahsis edilemedi.\n");
        exit(1);
    }
    for ( i = 0; i < n; i++) {
        matrix[i] = (float *)calloc(m ,sizeof(float));
        if (matrix[i] == NULL) {
            printf("Bellek tahsis edilemedi.\n");
            exit(1);
        }
    }
    return matrix;
}
void sonucYazdir(float* cevap, int n){
	int i;
	for(i=0;i<n;i++){
		printf("x%d: %f\n", i+1, cevap[i]);
	}
}
void GaussEliminasyon(float **matrix, int n, float*cevap) {
 
    // Girilen matrisi birim matrise cevirmek icin elementer satir islemleri kullanma
    int i = 0,j;
    while (i < n) {  
            // kosegendeki elemaný 1 yapalým
            float scalar = 1.0f / matrix[i][i];
            multiplyRow(matrix, i, scalar, n+1);
// i=0   j=1
            // Eliminate non-zero elements below the diagonal
            for (j = i+1; j < n; j++) {
                if (matrix[j][i]!=0) {
                    float multiple = (-1)*matrix[j][i];
                    addMultipleOfRow(matrix, i, j, multiple, n+1);
                }
            }
        i++;
    }
    printMatrix(matrix, n, n+1);
     for (i = n - 1; i >= 0; i--) {
        cevap[i] = matrix[i][n];
        for (j = i + 1; j < n; j++) {
            cevap[i] -= matrix[i][j] * cevap[j];
        }

    }
	sonucYazdir(cevap, n);
    
}


int main(){
	 int n;
	 printf("Kaç tane deklem gireceðinizi yazýnýz.\n");
        scanf("%d",&n);
	float **GEmatrix= yerAc(n,n+1);
    float* sonuclar= calloc(n, sizeof(float));
    getInputMatrix(GEmatrix, n,n+1);
    GaussEliminasyon(GEmatrix, n, sonuclar);
    sonucYazdir(sonuclar,  n);
	return 0;
}
