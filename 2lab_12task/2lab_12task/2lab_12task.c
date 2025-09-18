#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

//Структура для хранения матрицы
typedef struct {
    int** data;
    int size;
} Matrix;

//Выделение памяти для матрицы
Matrix* create_matrix(int n) {
    Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
    if (matrix == NULL) return NULL;

    matrix->size = n;
    matrix->data = (int**)malloc(n * sizeof(int*));
    if (matrix->data == NULL) {
        free(matrix);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        matrix->data[i] = (int*)malloc(n * sizeof(int));
        if (matrix->data[i] == NULL) {
            // Освобождаем уже выделенную память в случае ошибки
            for (int j = 0; j < i; j++) {
                free(matrix->data[j]);
            }
            free(matrix->data);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

//Освобождение памяти матрицы
void free_matrix(Matrix* matrix) {
    if (matrix == NULL) return;

    for (int i = 0; i < matrix->size; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

//Инициализация матрицы случайными числами
void init_matrix_random(Matrix* matrix) {
    if (matrix == NULL) return;

    for (int i = 0; i < matrix->size; i++) {
        for (int j = 0; j < matrix->size; j++) {
            matrix->data[i][j] = rand() % 100 + 1;
        }
    }
}

//Инициализация матрицы нулями
void init_matrix_zero(Matrix* matrix) {
    if (matrix == NULL) return;

    for (int i = 0; i < matrix->size; i++) {
        for (int j = 0; j < matrix->size; j++) {
            matrix->data[i][j] = 0;
        }
    }
}

//Умножение матриц: C = A на B
void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* c) {
    if (a == NULL || b == NULL || c == NULL || a->size != b->size || a->size != c->size) {
        return;
    }

    int n = a->size;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c->data[i][j] = 0;
            for (int k = 0; k < n; k++) {
                c->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

//Основная функция тестирования
int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    int sizes[] = { 100, 200, 400, 1000, 2000, 4000, 10000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    printf("Размер матрицы | Время (сек)\n");
    printf("----------------------------\n");
    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];

        //Создание матриц
        Matrix* a = create_matrix(n);
        Matrix* b = create_matrix(n);
        Matrix* c = create_matrix(n);
        if (a == NULL || b == NULL || c == NULL) {
            printf("Ошибка выделения памяти для размера %d\n", n);
            free_matrix(a);
            free_matrix(b);
            free_matrix(c);
            continue;
        }

        //Инициализация матриц
        init_matrix_random(a);
        init_matrix_random(b);
        init_matrix_zero(c);

        //Замер времени только умножения
        clock_t start = clock();
        multiply_matrices(a, b, c);
        clock_t end = clock();
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("%13d | %10.4f\n", n, time_taken);

        //Освобождение памяти
        free_matrix(a);
        free_matrix(b);
        free_matrix(c);
    }

    return 0;
}