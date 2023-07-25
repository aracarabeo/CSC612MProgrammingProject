//Charlyne Arajoy Carabeo CSC612M
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern void SIMDstencilVector(size_t ARRAY_SIZE, int* h_out, int* h_in);
extern void stencilVectorx86(size_t ARRAY_SIZE, int* h_out, int* h_in);
//C function to stencilVector
void stencilVector(const int* X, int* Y, size_t n) {
    int counter = 0;
    for (int i = 3; i < n - 3; i++) {
        Y[counter] = X[i - 3] + X[i - 2] + X[i - 1] + X[i] + X[i + 1] + X[i + 2] + X[i + 3];
        counter++;
    }

}

int main() {
    const size_t ARRAY_SIZE = 1 << 28; //1<<24 1<<30
    const size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(int)*2; //x2 cause 64 bit integer

    // declare array
    int* in, * out;
    in = (int*)malloc(ARRAY_BYTES);
    out = (int*)malloc(ARRAY_BYTES);

    if (in == NULL || out == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // timer variables
    clock_t start, end;

    // init input array
    for (unsigned int i = 0; i < ARRAY_SIZE; i++) {
        in[i] = (rand() % 10) + 1; //assign input (random number 1-10)
    }
    for (unsigned int i = 0; i < ARRAY_SIZE - 6; i++) {
        out[i] = 0;
    }

    // Flush out cache
    stencilVector(in, out, ARRAY_SIZE);

    // time here
    start = clock();
    for (int i = 0; i < 30; i++) {
        stencilVector(in, out, ARRAY_SIZE);
    }
    end = clock();
    double time_taken = ((double)(end - start)) * 1e6 / CLOCKS_PER_SEC;
    printf("C function will take %f microseconds for array size %zu\n", time_taken, ARRAY_SIZE);

    // error checking
    size_t error_count = 0;
    int counter = 0;
    for (unsigned int i = 3; i < ARRAY_SIZE - 3; i++) {
        if (in[i - 3] + in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2] + in[i + 3] != out[counter]) {
            error_count++;
        }
        counter++;
    }
    printf("Error count (C program) = %zu\n", error_count);


    //x86-64 SIMD YMM
    //initialize back out[i] to 0
    for (unsigned int i = 0; i < ARRAY_SIZE - 6; i++) {
        out[i] = 0;
    }

    // Flush out cache
     SIMDstencilVector(ARRAY_SIZE, out, in);

    // time here
    start = clock();
     for (int i = 0; i < 30; i++) {
        SIMDstencilVector(ARRAY_SIZE, out, in);
     }
    end = clock();
    time_taken = ((double)(end - start)) * 1e6 / CLOCKS_PER_SEC;
    printf("x86 SIMD ISA function will take %f microseconds for array size %zu\n", time_taken, ARRAY_SIZE);

    // error checking
    error_count = 0;
    counter = 0;
    for (unsigned int i = 3; i < ARRAY_SIZE - 3; i++) {
        if (in[i - 3] + in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2] + in[i + 3] != out[counter]) {
           // printf("in[i-3]: %d,in[i-2]: %d, in[i-1]: %d,in[i]: %d,in[i+1]: %d,in[i+2]: %d,in[i+3]: %d\n", in[i - 3], in[i - 2], in[i - 1], in[i], in[i+1], in[i+2], in[i + 3]);
            printf("SIMD: %d, Expected Output: %d\n", out[counter], in[i - 3] + in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2] + in[i + 3]);
            error_count++;
        }
        counter++;
    }
    printf("Error count (SIMD program) = %zu\n", error_count);

    //x86-64 version
     // initialize back out[i] to 0
    for (unsigned int i = 0; i < ARRAY_SIZE - 6; i++) {
        out[i] = 0;
    }

    // Flush out cache
    stencilVectorx86(ARRAY_SIZE, out, in);

    // time here
    start = clock();
    for (int i = 0; i < 30; i++) {
        stencilVectorx86(ARRAY_SIZE, out, in);
    }
    end = clock();
    time_taken = ((double)(end - start)) * 1e6 / CLOCKS_PER_SEC;
    printf("x86_64 function will take %f microseconds for array size %zu\n", time_taken, ARRAY_SIZE);

    // error checking
    error_count = 0;
    for (int i = 3; i < ARRAY_SIZE - 3; i++) {
        if (in[i - 3] + in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2] + in[i + 3] != out[i]) {
            printf("Non-SIMD: %d, Expected Output: %d\n", out[counter], in[i - 3] + in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2] + in[i + 3]);
            error_count++;
        }
    }
    printf("Error count (x86_64 program) = %zu\n", error_count);
    free(in);
    free(out);
    return 0;
}
