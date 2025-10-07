#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
//Функция для генерации матрицы смежности
void generateAdjacencyMatrix(int** matrix, int vertices) {
    for (int i = 0; i < vertices; i++) {
        for (int j = i + 1; j < vertices; j++) {
            //Случайное значение 0 или 1
            int value = rand() % 2;
            matrix[i][j] = value;
            matrix[j][i] = value;// Граф неориентированный
        }
        matrix[i][i] = 0;
    }
}

//Функция для вывода матрицы смежности
void printMatrix(int** matrix, int vertices) {
    printf("Матрица смежности:\n");
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//Функция для преобразования матрицы смежности в список смежности
void matrixToList(int** matrix, int vertices) {
    printf("Список смежности:\n");
    //Строки матрицы
    for (int i = 0; i < vertices; i++) {
        printf("Вершина %d: ", i + 1);
        //Флаг для отслеживания наличия соседей 
        int hasNeighbors = 0;
        //Столбцы матрицы
        for (int j = 0; j < vertices; j++) {
            if (matrix[i][j] == 1 && i != j) {
                printf("%d ", j + 1);
                //Устанавливаем флаг, что у вершины есть хотя бы один сосед
                hasNeighbors = 1;
            }
        }
        if (!hasNeighbors) {
            printf("Нет соседей");
        }
        printf("\n");
    }
    printf("\n");
}

//Функция для освобождения памяти
void freeMatrix(int** matrix, int vertices) {
    for (int i = 0; i < vertices; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    setlocale(LC_ALL, "ru");
    srand(time(NULL));// Инициализация генератора случайных чисел
    int vertices1, vertices2;

    //Ввод количества вершин для первого графа
    printf("Введите количество вершин для графа G1: ");
    scanf("%d", &vertices1);

    //Ввод количества вершин для второго графа
    printf("Введите количество вершин для графа G2: ");
    scanf("%d", &vertices2);

    //Проверка корректности ввода
    if (vertices1 <= 0 || vertices2 <= 0) {
        printf("Ошибка: количество вершин должно быть вещественным числом.\n");
        return 1;
    }

    //Выделение памяти для матрицы M1
    int** M1 = (int**)malloc(vertices1 * sizeof(int*));
    for (int i = 0; i < vertices1; i++) {
        M1[i] = (int*)malloc(vertices1 * sizeof(int));
    }

    //Выделение памяти для матрицы M2
    int** M2 = (int**)malloc(vertices2 * sizeof(int*));
    for (int i = 0; i < vertices2; i++) {
        M2[i] = (int*)malloc(vertices2 * sizeof(int));
    }

    //Генерация и вывод матрицы M1
    printf("\nГраф G1\n");
    generateAdjacencyMatrix(M1, vertices1);
    printMatrix(M1, vertices1);

    //Преобразование и вывод списка смежности для G1
    matrixToList(M1, vertices1);

    //Генерация и вывод матрицы M2
    printf("Граф G2\n");
    generateAdjacencyMatrix(M2, vertices2);
    printMatrix(M2, vertices2);

    //Преобразование и вывод списка смежности для G2
    matrixToList(M2, vertices2);

    //Освобождение памяти
    freeMatrix(M1, vertices1);
    freeMatrix(M2, vertices2);
    return 0;
}