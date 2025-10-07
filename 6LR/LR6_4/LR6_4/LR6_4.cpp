#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

//Функция для генерации матрицы смежности
void generateAdjacencyMatrix(int** matrix, int vertices) {
    for (int i = 0; i < vertices; i++) {
        for (int j = i + 1; j < vertices; j++) {
            //Генерируем случайное значение 0 или 1 для ребра между вершинами i и j
            int value = rand() % 2;
            //Заполняем матрицу симметрично (граф неориентированный)
            matrix[i][j] = value;
            matrix[j][i] = value;
        }
        //Обнуляем главную диагональ
        matrix[i][i] = 0;
    }
}

//Функция для вывода матрицы смежности
//Выводит матрицу в удобочитаемом формате с заголовками строк и столбцов
void printMatrix(int** matrix, int vertices, const char* name) {
    printf("%s (%dx%d):\n", name, vertices, vertices);
    //Вывод заголовков столбцов (номера вершин, начиная с 1)
    printf("   ");
    for (int j = 0; j < vertices; j++) {
        printf("%2d ", j + 1);
    }
    printf("\n");

    //Вывод самой матрицы
    for (int i = 0; i < vertices; i++) {
        printf("%2d ", i + 1);//Номер строки (вершины)
        for (int j = 0; j < vertices; j++) {
            printf("%2d ", matrix[i][j]);//Значение элемента матрицы
        }
        printf("\n");
    }
    printf("\n");
}

// Функция для создания матрицы
int** createMatrix(int vertices) {
    //Выделяем память для массива указателей на строки
    int** matrix = (int**)malloc(vertices * sizeof(int*));
    for (int i = 0; i < vertices; i++) {
        //Выделяем память для каждой строки и инициализируем нулями
        matrix[i] = (int*)calloc(vertices, sizeof(int));
    }
    return matrix;
}

// Функция для освобождения памяти матрицы
void freeMatrix(int** matrix, int vertices) {
    for (int i = 0; i < vertices; i++) {
        free(matrix[i]);//Освобождаем каждую строку
    }
    free(matrix);//Освобождаем массив указателей
}

//Операция декартова произведения графов G = G1 Х G2
//Каждая вершина - это пара вершин из исходных графов
int** cartesianProduct(int** matrix1, int vertices1, int** matrix2, int vertices2, int* resultVertices) {
    //Количество вершин в результате: V1 X V1
    *resultVertices = vertices1 * vertices2;
    int** result = createMatrix(*resultVertices);

    //Проходим по всем парам вершин (u,v) где u из G1, v из G2
    for (int u1 = 0; u1 < vertices1; u1++) {
        for (int v1 = 0; v1 < vertices2; v1++) {
            //u2,v2 - вторая вершина в декартовом произведении
            for (int u2 = 0; u2 < vertices1; u2++) {
                for (int v2 = 0; v2 < vertices2; v2++) {
                    //Индексы вершин в результирующем графе
                    //Используем формулу: индекс = u * vertices2 + v
                    int i = u1 * vertices2 + v1;
                    int j = u2 * vertices2 + v2;
                    // Две вершины (u1,v1) и (u2,v2) смежны в декартовом произведении если:
                    // 1) u1 = u2 и (v1,v2) - ребро в G2 (горизонтальные связи), ИЛИ
                    // 2) v1 = v2 и (u1,u2) - ребро в G1 (вертикальные связи)
                    if ((u1 == u2 && matrix2[v1][v2] == 1) || (v1 == v2 && matrix1[u1][u2] == 1)) {
                        result[i][j] = 1;
                    }
                }
            }
        }
    }
    return result;
}

//Функция для вывода матрицыи
void printCartesianMatrix(int** matrix, int vertices1, int vertices2, const char* name) {
    int totalVertices = vertices1 * vertices2;
    printf("%s (%dx%d):\n", name, totalVertices, totalVertices);
    printf("    ");
    for (int u = 0; u < vertices1; u++) {
        for (int v = 0; v < vertices2; v++) {
            printf("(%d,%d) ", u + 1, v + 1);
        }
    }
    printf("\n");

    //Вывод матрицы построчно
    for (int u1 = 0; u1 < vertices1; u1++) {
        for (int v1 = 0; v1 < vertices2; v1++) {
            int i = u1 * vertices2 + v1;
            printf("(%d,%d)", u1 + 1, v1 + 1);//Метка строки
            //Вывод всех элементов строки
            for (int u2 = 0; u2 < vertices1; u2++) {
                for (int v2 = 0; v2 < vertices2; v2++) {
                    int j = u2 * vertices2 + v2;
                    printf("%2d    ", matrix[i][j]);
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}

//Функция для вывода списка смежности
void printCartesianAdjacencyList(int** matrix, int vertices1, int vertices2) {
    int totalVertices = vertices1 * vertices2;
    printf("Списки смежности декартова произведения:\n");
    //Для каждой вершины в декартовом произведении
    for (int u1 = 0; u1 < vertices1; u1++) {
        for (int v1 = 0; v1 < vertices2; v1++) {
            int i = u1 * vertices2 + v1;
            printf("Вершина (%d,%d): ", u1 + 1, v1 + 1);
            int hasNeighbors = 0;//Флаг наличия соседей
            //Ищем всех соседей текущей вершины
            for (int u2 = 0; u2 < vertices1; u2++) {
                for (int v2 = 0; v2 < vertices2; v2++) {
                    int j = u2 * vertices2 + v2;
                    //Если есть ребро и это не та же самая вершина
                    if (matrix[i][j] == 1 && i != j) {
                        printf("(%d,%d) ", u2 + 1, v2 + 1);
                        hasNeighbors = 1;
                    }
                }
            }
            //Если соседей нет, выводим соответствующее сообщение
            if (!hasNeighbors) {
                printf("Нет соседей");
            }
            printf("\n");
        }
    }
    printf("\n");
}

int main() {
    setlocale(LC_ALL, "ru");
    srand(time(NULL));
    int vertices1, vertices2;
    printf("Введите количество вершин для графа G1: ");
    scanf("%d", &vertices1);
    printf("Введите количество вершин для графа G2: ");
    scanf("%d", &vertices2);
    //Проверка корректности введенных данных
    if (vertices1 <= 0 || vertices2 <= 0) {
        printf("Ошибка: количество вершин должно быть положительным числом.\n");
        return 1;
    }
    //Создаем и генерируем матрицы для G1 и G2
    int** matrix1 = createMatrix(vertices1);
    int** matrix2 = createMatrix(vertices2);

    //Заполняем матрицы случайными значениями
    generateAdjacencyMatrix(matrix1, vertices1);
    generateAdjacencyMatrix(matrix2, vertices2);

    //Вывод исходных графов
    printf("\nИсходные графы\n");
    printMatrix(matrix1, vertices1, "Матрица смежности G1");
    printMatrix(matrix2, vertices2, "Матрица смежности G2");

    //Выполняем декартово произведение
    int resultVertices;
    int** result = cartesianProduct(matrix1, vertices1, matrix2, vertices2, &resultVertices);

    //Вывод информации о результате
    printf("\nДекартово произведение G1 X G2\n");
    printf("Количество вершин в результате: %d x %d = %d\n\n", vertices1, vertices2, resultVertices);

    //Выводим матрицу смежности результата
    printCartesianMatrix(result, vertices1, vertices2, "Матрица смежности декартова произведения");

    //Выводим списки смежности для наглядности
    printCartesianAdjacencyList(result, vertices1, vertices2);

    //Освобождение памяти
    freeMatrix(matrix1, vertices1);
    freeMatrix(matrix2, vertices2);
    freeMatrix(result, resultVertices);
    return 0;
}