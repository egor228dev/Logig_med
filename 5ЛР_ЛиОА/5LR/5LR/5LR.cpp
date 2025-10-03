#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

//Функция для вывода матрицы смежности
void printMatrix(int V, int** matrix) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
}

//Функция для вывода матрицы инцидентности
void printIncidenceMatrix(int V, int E, int** matrix) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < E; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
}

//Функция для определения степени вершины из матрицы инцидентности
int getDegreeFromIncidence(int V, int E, int** matrix, int vertex) {
    int degree = 0;
    for (int j = 0; j < E; j++) {
        degree += matrix[vertex][j];
    }
    return degree;
}

//Функция для определения степени вершины из матрицы смежности
int getDegree(int V, int** matrix, int vertex) {
    int degree = 0;
    for (int i = 0; i < V; i++) {
        if (i == vertex) {
            //Петля учитывается дважды
            degree += 2 * matrix[vertex][i];
        }
        else {
            //Обычное ребро
            degree += matrix[vertex][i];
        }
    }
    return degree;
}

//Функция для подсчета количества ребер
int countEdges(int V, int** adjacencyMatrix) {
    int edges = 0;
    for (int i = 0; i < V; i++) {
        for (int j = i; j < V; j++) {
            if (adjacencyMatrix[i][j] == 1) {
                edges++;
            }
        }
    }
    return edges;
}

//Функция для выделения памяти под матрицу
int** allocateMatrix(int rows, int cols) {
    int** matrix = (int**)malloc(rows * sizeof(int*));
    if (matrix == NULL) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            printf("Ошибка выделения памяти!\n");
            exit(1);
        }
    }
    return matrix;
}

//Функция для освобождения памяти матрицы
void freeMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

//Функция для инициализации матрицы нулями
void initializeMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = 0;
        }
    }
}

//Функция для генерации случайной матрицы смежности
void generateAdjacencyMatrix(int V, int** matrix) {
    srand(time(NULL)); //Инициализация генератора случайных чисел
    //Сначала заполняем нулями
    initializeMatrix(matrix, V, V);
    //Заполняем матрицу случайными значениями (0 или 1)
    for (int i = 0; i < V; i++) {
        for (int j = i; j < V; j++) {//Заполняем только верхний треугольник включая диагональ
            if (i == j) {
                //Главная диагональ - иногда добавляем петли
                matrix[i][j] = (rand() % 100) < 30 ? 1 : 0;
            }
            else {
                //Вне диагонали - добавляем ребра (40% вероятность)
                matrix[i][j] = (rand() % 100) < 40 ? 1 : 0;
                //Обеспечиваем симметричность для неориентированного графа
                matrix[j][i] = matrix[i][j];
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "ru");

    int V;// количество вершин
    int** adjacencyMatrix = NULL;
    int** incidenceMatrix = NULL;

    //Ввод количества вершин
    printf("Введите количество вершин графа: ");
    while (scanf("%d", &V) != 1 || V <= 0) {
        printf("Ошибка! Введите положительное целое число: ");
        while (getchar() != '\n');// Очистка буфера ввода
    }

    //Выделение памяти для матрицы смежности
    adjacencyMatrix = allocateMatrix(V, V);

    //Генерация случайной матрицы смежности
    generateAdjacencyMatrix(V, adjacencyMatrix);
    printf("\nМатрица смежности графа G:\n");
    printMatrix(V, adjacencyMatrix);
    printf("\n");

    //Подсчет количества ребер
    int edges = countEdges(V, adjacencyMatrix);
    printf("Размер графа G:\n");
    printf("Количество вершин: %d\n", V);
    printf("Количество ребер: %d\n", edges);
    printf("\n");

    //Поиск специальных вершин
    printf("Специальные вершины:\n");

    //Изолированные вершины (степень 0)
    printf("Изолированные вершины (степень 0): ");
    int hasIsolated = 0;
    for (int i = 0; i < V; i++) {
        if (getDegree(V, adjacencyMatrix, i) == 0) {
            printf("%d ", i + 1);
            hasIsolated = 1;
        }
    }
    if (!hasIsolated) printf("нет");
    printf("\n");

    //Концевые вершины (степень 1)
    printf("Концевые вершины (степень 1): ");
    int hasEnd = 0;
    for (int i = 0; i < V; i++) {
        if (getDegree(V, adjacencyMatrix, i) == 1) {
            printf("%d ", i + 1);
            hasEnd = 1;
        }
    }
    if (!hasEnd) printf("нет");
    printf("\n");

    //Доминирующие вершины (степень V-1)
    printf("Доминирующие вершины (степень %d): ", V - 1);
    int hasDominant = 0;
    for (int i = 0; i < V; i++) {
        if (getDegree(V, adjacencyMatrix, i) == V - 1) {
            printf("%d ", i + 1);
            hasDominant = 1;
        }
    }
    if (!hasDominant) printf("нет");
    printf("\n");

    //Вершины с петлями
    printf("Вершины с петлями: ");
    int hasLoopVertices = 0;
    for (int i = 0; i < V; i++) {
        if (adjacencyMatrix[i][i] == 1) {
            printf("%d ", i + 1);
            hasLoopVertices = 1;
        }
    }
    if (!hasLoopVertices) printf("нет");
    printf("\n\n");

    //Построение матрицы инцидентности
    int E = edges;// количество ребер
    if (E > 0) {
        //Выделение памяти для матрицы инцидентности
        incidenceMatrix = allocateMatrix(V, E);
        initializeMatrix(incidenceMatrix, V, E);

        //Заполнение матрицы инцидентности
        int edgeCount = 0;
        for (int i = 0; i < V; i++) {
            for (int j = i; j < V; j++) { // j начинается с i для учета петель
                if (adjacencyMatrix[i][j] == 1) {
                    if (i == j) {
                        //Петля - вершина инцидентна ребру дважды
                        incidenceMatrix[i][edgeCount] = 2;
                    }
                    else {
                        //Обычное ребро
                        incidenceMatrix[i][edgeCount] = 1;
                        incidenceMatrix[j][edgeCount] = 1;
                    }
                    edgeCount++;
                }
            }
        }

        printf("Матрица инцидентности графа G:\n");
        printf("(2 означает петлю, 1 - обычное ребро)\n");
        printIncidenceMatrix(V, E, incidenceMatrix);
        printf("\n");

        //Специальные вершины через матрицу инцидентности
        printf("Специальные вершины (через матрицу инцидентности):\n");
        printf("Изолированные вершины (степень 0): ");
        hasIsolated = 0;
        for (int i = 0; i < V; i++) {
            if (getDegreeFromIncidence(V, E, incidenceMatrix, i) == 0) {
                printf("%d ", i + 1);
                hasIsolated = 1;
            }
        }
        if (!hasIsolated) printf("нет");
        printf("\n");
        printf("Концевые вершины (степень 1): ");
        hasEnd = 0;
        for (int i = 0; i < V; i++) {
            if (getDegreeFromIncidence(V, E, incidenceMatrix, i) == 1) {
                printf("%d ", i + 1);
                hasEnd = 1;
            }
        }
        if (!hasEnd) printf("нет");
        printf("\n");
        printf("Доминирующие вершины (степень %d): ", V - 1);
        hasDominant = 0;
        for (int i = 0; i < V; i++) {
            if (getDegreeFromIncidence(V, E, incidenceMatrix, i) == V - 1) {
                printf("%d ", i + 1);
                hasDominant = 1;
            }
        }
        if (!hasDominant) printf("нет");
        printf("\n");

        //Освобождение памяти матрицы инцидентности
        freeMatrix(incidenceMatrix, V);
    }
    else {
        printf("Граф не содержит ребер, матрица инцидентности не строится.\n");
    }

    //Вывод степеней всех вершин
    printf("\nСтепени вершин графа:\n");
    for (int i = 0; i < V; i++) {
        int degree = getDegree(V, adjacencyMatrix, i);
        printf("Вершина %d: степень %d\n", i + 1, degree);
    }
    printf("\n");

    //Освобождение памяти матрицы смежности
    freeMatrix(adjacencyMatrix, V);
    return 0;
}
