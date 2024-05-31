#include <stdio.h>

#define MAX_POINTS 10 // Maximum number of data points

// Struct definition for data points
struct Data {
    double x;
    double y;
};

void getInputs(struct Data data[], int *n);
double calculateDifference(struct Data data[], int n, int i, int j);
double interpolate(struct Data data[], int n, double xi);

int main() {
    struct Data data[MAX_POINTS];
    double xi, result;
    int n;

    // Get inputs from the user
    getInputs(data, &n);

    // Get the value to interpolate
    printf("Enter the value of x for interpolation: ");
    scanf("%lf", &xi);

    // Interpolate
    result = interpolate(data, n, xi);

    // Display result
    printf("Interpolated value at x = %.2lf is %.4lf\n", xi, result);

    return 0;
}

// Function to get inputs (x and y values)
void getInputs(struct Data data[], int *n) {
    printf("Enter the number of data points: ");
    scanf("%d", n);
int i;
    printf("Enter the data points:\n");
    for ( i = 0; i < *n; i++) {
        printf("x[%d] = ", i);
        scanf("%lf", &data[i].x);
        printf("y[%d] = ", i);
        scanf("%lf", &data[i].y);
    }
}

// Function to calculate differences
double calculateDifference(struct Data data[], int n, int i, int j) {
    if (j == 0)
        return data[i].y;
    else
        return (calculateDifference(data, n, i + 1, j - 1) - calculateDifference(data, n, i, j - 1)) / (data[i + j].x - data[i].x);
}

// Function to interpolate
double interpolate(struct Data data[], int n, double xi) {
    double result = 0;
    int i,j;
    for (i = 0; i < n; i++) {
        double term = calculateDifference(data, n, 0, i);
        for ( j = 0; j < i; j++) {
            term *= (xi - data[j].x);
        }
        result += term;
    }
    return result;
}

