#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#define V 6 // количество вершин
#define E 5 // количество ребер

//Функция для вывода матрицы
void printMatrix(int matrix[V][V]) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

//Функция для вывода матрицы инцидентности
void printIncidenceMatrix(int matrix[V][E]) {
    //Строки матрицы (вершины)
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < E; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

//Функция для определения степени вершины из матрицы инцидентности
int getDegreeFromIncidence(int matrix[V][E], int vertex) {
    int degree = 0;
    for (int j = 0; j < E; j++) {
        degree += matrix[vertex][j];
    }
    return degree;
}
//Функция для определения степени вершины
int getDegree(int matrix[V][V], int vertex) {
    int degree = 0;
    for (int i = 0; i < V; i++) {
        degree += matrix[vertex][i];
    }
    return degree;
}

int main() {
    setlocale(LC_ALL, "ru");
    
    //Исходная матрица смежности для построения матрицы инцидентности
    int adjacencyMatrix[V][V] = {
        {0, 1, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 0},
        {1, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 0}
    };
    printf("Матрица смежности графа G:\n");
    printMatrix(adjacencyMatrix);
    printf("\n");


    //Размер графа (количество вершин и ребер)
    int edges = 0;
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            edges += adjacencyMatrix[i][j];
        }
    }
    printf("Размер графа G:\n");
    printf("Количество вершин: %d\n", V);
    printf("Количество ребер: %d\n", edges);
    printf("\n");

    //Поиск специальных вершин
    printf("Специальные вершины:\n");
    printf("Изолированные вершины (степень 0): ");
    int hasIsolated = 0;
    for (int i = 0; i < V; i++) {
        if (getDegree(adjacencyMatrix, i) == 0) {
            printf("%d ", i + 1);
            hasIsolated = 1;
        }
    }
    if (!hasIsolated) printf("нет");
    printf("\n");
    printf("Концевые вершины (степень 1): ");
    int hasEnd = 0;
    for (int i = 0; i < V; i++) {
        if (getDegree(adjacencyMatrix, i) == 1) {
            printf("%d ", i + 1);
            hasEnd = 1;
        }
    }
    if (!hasEnd) printf("нет");
    printf("\n");
    printf("Доминирующие вершины (степень %d): ", V - 1);
    int hasDominant = 0;
    for (int i = 0; i < V; i++) {
        if (getDegree(adjacencyMatrix, i) == V - 1) {
            printf("%d ", i + 1);
            hasDominant = 1;
        }
    }
    if (!hasDominant) printf("нет");
    printf("\n");
    printf("\n");
    //Построение матрицы инцидентности
    int incidenceMatrix[V][E];
    //Инициализация матрицы инцидентности нулями
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < E; j++) {
            incidenceMatrix[i][j] = 0;
        }
    }
    //Заполнение матрицы инцидентности
    int edgeCount = 0;
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (adjacencyMatrix[i][j] == 1) {
                incidenceMatrix[i][edgeCount] = 1;
                incidenceMatrix[j][edgeCount] = 1;
                edgeCount++;
            }
        }
    }
    printf("Матрица инцидентности графа G:\n");
    printIncidenceMatrix(incidenceMatrix);
    printf("\n");

    //Размер графа через матрицу инцидентности
    printf("Размер графа G:\n");
    printf("Количество вершин: %d\n", V);
    printf("Количество ребер: %d\n", E);
    printf("\n");

    //Специальные вершины через матрицу инцидентности
    printf("Специальные вершины:\n");
    printf("Изолированные вершины (степень 0): ");
    int hasIsolated1 = 0;
    for (int i = 0; i < V; i++) {
        if (getDegreeFromIncidence(incidenceMatrix, i) == 0) {
            printf("v%d ", i + 1);
            hasIsolated1 = 1;
        }
    }
    if (!hasIsolated) printf("нет");
    printf("\n");
    printf("Концевые вершины (степень 1): ");
    int hasEnd1 = 0;
    for (int i = 0; i < V; i++) {
        if (getDegreeFromIncidence(incidenceMatrix, i) == 1) {
            printf("%d ", i + 1);
            hasEnd1 = 1;
        }
    }
    if (!hasEnd) printf("нет");
    printf("\n");
    printf("Доминирующие вершины (степень %d): ", V - 1);
    int hasDominant1 = 0;
    for (int i = 0; i < V; i++) {
        if (getDegreeFromIncidence(incidenceMatrix, i) == V - 1) {
            printf("%d ", i + 1);
            hasDominant1 = 1;
        }
    }
    if (!hasDominant1) printf("нет");
    printf("\n");
    return 0;
}
