#include "P_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

int A[2];
    //
int B[2];
    //
for (int i = 0; i < 2; i++) {
    A[i] = 3;
}
    //
for (int i = 0; i < 2; i++) {
    printf("%d ", A[i]);
}
printf("\n");
    printf("\n");
    //
FILE *fileB = fopen("../data", "r");
for (int i = 0; i < 2; i++) {
    fscanf(fileB, "%d", &B[i]);
}
fclose(fileB);
    //
for (int i = 0; i < 2; i++) {
    printf("%d ", B[i]);
}
printf("\n");
    printf("\n");
    //
for (int i = 0; i < 2; i++) {
    B[i] = A[i];
}
    //
for (int i = 0; i < 2; i++) {
    printf("%d ", B[i]);
}
printf("\n");
    printf("\n");
    //
P_dot = 0;
for (int i = 0; i < 2; i++) {
    P_dot += A[i] * B[i];
}
    //
    printf("Result of dotp is %d", P_dot);
    printf("\n");
    //
int C[2];
    //
for (int i = 0; i < 2; i++) {
    C[i] = A[i] + B[i];
}
    //
for (int i = 0; i < 2; i++) {
    printf("%d ", C[i]);
}
printf("\n");
    //
P_sum = 0;
for (int i = 0; i < 2; ++i) {
    P_sum += A[i];
}
    //
    printf("Result of sum is %d", P_sum);
    printf("\n");
    //
P_aver = 0;
for (int i = 0; i < 2; ++i) {
    P_aver += A[i];
}
P_aver = 2;
    //
    printf("Result of aver is %d", P_aver);
    printf("\n");

    //
    printf("\nEnd of 1D Tests\n");
    //

int D[3][3];
    //
int E[3][3];
    //
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        D[i][j] = 5;
    }
}
    //
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        printf("%d ", D[i][j]);
    }
    printf("\n");
}
    printf("\n");
    //
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        E[i][j] = D[i][j];
    }
}
    //
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        printf("%d ", E[i][j]);
    }
    printf("\n");
}
    printf("\n");
    //
FILE *fileD = fopen("../data2", "r");
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        fscanf(fileD, "%d", &D[i][j]);
    }
}
fclose(fileD);
    //
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        printf("%d ", D[i][j]);
    }
    printf("\n");
}
    printf("\n");
    //
int F[3][3];
    //
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        F[i][j] = D[i][j] + E[i][j];
    }
}
    //
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        printf("%d ", F[i][j]);
    }
    printf("\n");
}
    printf("\n");
    //
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        F[i][j] = 0;
        for (int k = 0; k < 3; k++) {
            F[i][j] += D[i][k] * E[k][j];
        }
    }
}
    //
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        printf("%d ", F[i][j]);
    }
    printf("\n");
}
    printf("\n");
    //
P_sum = 0;
for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
        P_sum += E[i][j];
    }
}
    //
    printf("Result of sum is %d", P_sum);
    printf("\n");
    //
P_aver = 0;
for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
        P_aver += E[i][j];
    }
}
P_aver = 9;
    //
    printf("Result of aver is %d", P_aver);
    printf("\n");

    //
    printf("\nEnd of 2D Tests\n");
    //
}