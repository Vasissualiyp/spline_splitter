#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    float x;
    float y;
    float z;
} Vec;

float h1(float t) {
    float result = 2.0 * t*t*t - 3.0 * t*t + 1.0;
	return result;
}

float h2(float t) {
    float result = - 2.0 * t*t*t + 3.0 * t*t;
	return result;
}

float h3(float t) {
    float result = t*t*t - 2.0 * t*t + t;
	return result;
}

float h4(float t) {
    float result = t*t*t - t*t;
	return result;
}


float dh1(float t) {
    float result = 6.0 * t*t - 6.0 * t;
	return result;
}

float dh2(float t) {
    float result = - 6.0 * t*t + 6.0 * t;
	return result;
}

float dh3(float t) {
    float result = 3.0 *t*t - 4.0 * t + 1.0;
	return result;
}

float dh4(float t) {
    float result = 3.0 * t*t - 2.0 * t;
	return result;
}

float** getCubicSpline(Vec* p0, Vec* p1, Vec* t0, Vec* t1, int dim, int n_spline) {
	/* Creates an array for coordinates of cubic spline. 
	 * Arguments:
	 *   p0 (Vec*) - 1st point
	 *   p1 (Vec*) - 2nd point
	 *   t0 (Vec*) - tangent vector to the 1st point
	 *   t1 (Vec*) - tangent vector to the 2nd point
	 *   dim (int) - dimensionality of simulation 
	 *   n_spline (int) - number of points in the spline
	 *
	 * Returns:
	 *   arr (array) - spline
	 *
	 */
	// Allocate the spline array
    float** arr = (float**)malloc(dim * sizeof(float*)); 
	for (int i = 0; i < 3; ++i) {
        arr[i] = (float*)malloc(n_spline * sizeof(float)); // Allocate n_spline columns for each row
    }
	float t, fi, fn_spline, x, y, z;
	fn_spline = (float)n_spline;

	float x0 = p0->x;
	float y0 = p0->y;
	float z0 = p0->z;
	float x1 = p1->x;
	float y1 = p1->y;
	float z1 = p1->z;

	float tx0 = t0->x;
	float ty0 = t0->y;
	float tz0 = t0->z;
	float tx1 = t1->x;
	float ty1 = t1->y;
	float tz1 = t1->z;

	for (int i = 0; i < n_spline; ++i) {
		fi = (float)i;
	    t = i / fn_spline;
		x = h1(t) * x0 + h2(t) * x1 + h3(t) * tx0 + h4(t) * tx1;
		y = h1(t) * y0 + h2(t) * y1 + h3(t) * ty0 + h4(t) * ty1;
		z = h1(t) * z0 + h2(t) * z1 + h3(t) * tz0 + h4(t) * tz1;
		arr[0][i] = x;
		arr[1][i] = y;
		arr[2][i] = z;
	}

    return arr;
}

void writeSplineToCSV(const char* filename, float** arr, int n_spline) {
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		perror("Failed to open file");
		return;
	}

    for (int i = 0; i < n_spline; ++i) {
        float x = arr[0][i];
        float y = arr[1][i];
        float z = arr[2][i];
        fprintf(file, "%.6f,%.6f,%.6f\n", x, y, z); // Write as CSV
    }
    
}

int main(int argc, char *argv[]) {
    int n_spline = 1000;
	char filename[] = "output.csv";

	Vec p0 = {0.0, 0.0, 0.0};
	Vec p1 = {1.0, 0.0, 0.0};
	Vec t0 = {0.0, 1.0, 0.0};
	Vec t1 = {0.0, -1.0, 0.0};
	float** arr;

    arr = getCubicSpline(&p0, &p1, &t0, &t1, 3, n_spline);
	writeSplineToCSV(filename, arr, n_spline);
    return 0;
}
