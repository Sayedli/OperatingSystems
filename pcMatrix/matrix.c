/*
 *  Matrix routines
 *  Supports generation of random R x C matrices
 *  And operations on them
 * 
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 */

#include "matrix.h"
#include <stdlib.h>

Matrix *AllocMatrix(int r, int c) {
    Matrix *mat = (Matrix *)malloc(sizeof(Matrix));
    mat->rows = r;
    mat->cols = c;
    mat->data = (int **)malloc(r * sizeof(int *));
    for (int i = 0; i < r; i++) {
        mat->data[i] = (int *)malloc(c * sizeof(int));
    }
    return mat;
}

void FreeMatrix(Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
}

void GenMatrix(Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = rand() % 10 + 1;
        }
    }
}

Matrix *GenMatrixRandom() {
    int rows = rand() % 10 + 1;
    int cols = rand() % 10 + 1;
    Matrix *mat = AllocMatrix(rows, cols);
    GenMatrix(mat);
    return mat;
}

int AvgElement(Matrix *mat) {
    int sum = 0;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            sum += mat->data[i][j];
        }
    }
    return sum / (mat->rows * mat->cols);
}

int SumMatrix(Matrix *mat) {
    int sum = 0;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            sum += mat->data[i][j];
        }
    }
    return sum;
}

Matrix *MatrixMultiply(Matrix *m1, Matrix *m2) {
    if (m1->cols != m2->rows) {
        return NULL;
    }
    Matrix *result = AllocMatrix(m1->rows, m2->cols);
    for (int i = 0; i < m1->rows; i++) {
        for (int j = 0; j < m2->cols; j++) {
            result->data[i][j] = 0;
            for (int k = 0; k < m1->cols; k++) {
                result->data[i][j] += m1->data[i][k] * m2->data[k][j];
            }
        }
    }
    return result;
}

void DisplayMatrix(Matrix *mat, FILE *stream) {
    for (int i = 0; i < mat->rows; i++) {
        fprintf(stream, "|");
        for (int j = 0; j < mat->cols; j++) {
            fprintf(stream, "%3d ", mat->data[i][j]);
        }
        fprintf(stream, "|\n");
    }
}

Matrix *GenMatrixBySize(int row, int col) {
    Matrix *mat = AllocMatrix(row, col);
    GenMatrix(mat);
    return mat;
}
