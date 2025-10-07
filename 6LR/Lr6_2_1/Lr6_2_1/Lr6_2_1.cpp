#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

//Функция для генерации матрицы смежности
void generateAdjacencyMatrix(int** matrix, int vertices) {
    //Проходим по всем вершинам матрицы
    for (int i = 0; i < vertices; i++) {
        for (int j = i + 1; j < vertices; j++) {
            //Генерируем случайное значение 0 или 1 для ребра
            int value = rand() % 2;
            //Заполняем симметрично
            matrix[i][j] = value;
            matrix[j][i] = value;
        }
        matrix[i][i] = 0;
    }
}

//Функция для вывода матрицы смежности
void printMatrix(int** matrix, int vertices) {
    //Выводим заголовок с размерами матрицы
    printf("Матрица смежности (%dx%d):\n", vertices, vertices);

    //Выводим номера столбцов
    printf("   ");
    for (int j = 0; j < vertices; j++) {
        printf("%2d ", j + 1);//Нумерация вершин с 1 для удобства
    }
    printf("\n");

    //Выводим саму матрицу
    for (int i = 0; i < vertices; i++) {
        printf("%2d ", i + 1);//Номер строки
        for (int j = 0; j < vertices; j++) {
            printf("%2d ", matrix[i][j]);//Значение элемента матрицы
        }
        printf("\n");
    }
    printf("\n");
}

//Функция для копирования матрицы
int** copyMatrix(int** source, int vertices) {
    //Выделяем память для новой матрицы
    int** copy = (int**)malloc(vertices * sizeof(int*));
    for (int i = 0; i < vertices; i++) {
        //Выделяем память для каждой строки
        copy[i] = (int*)malloc(vertices * sizeof(int));
        //Копируем значения из исходной матрицы
        for (int j = 0; j < vertices; j++) {
            copy[i][j] = source[i][j];
        }
    }
    return copy;
}

//Операция отождествления вершин
int** identifyVertices(int** matrix, int vertices, int v1, int v2, int* newVertices) {
    //Проверка корректности введенных вершин
    if (v1 < 1 || v1 > vertices || v2 < 1 || v2 > vertices || v1 == v2) {
        printf("Ошибка: некорректные номера вершин!\n");
        *newVertices = vertices;
        return copyMatrix(matrix, vertices);//Возвращаем копию исходной матрицы
    }

    //Преобразуем в индексы (от 0 до v1)
    int idx1 = v1 - 1;
    int idx2 = v2 - 1;
    //Оставляем вершину с меньшим индексом
    int keep_idx = (idx1 < idx2) ? idx1 : idx2;
    int remove_idx = (idx1 < idx2) ? idx2 : idx1;
    //Новая матрица будет на одну вершину меньше
    *newVertices = vertices - 1;
    //Выделяем память для новой матрицы и инициализируем нулями
    int** newMatrix = (int**)malloc(*newVertices * sizeof(int*));
    for (int i = 0; i < *newVertices; i++) {
        newMatrix[i] = (int*)calloc(*newVertices, sizeof(int));
    }
    //Заполняем новую матрицу
    for (int i = 0; i < vertices; i++) {
        if (i == remove_idx) continue;//Пропускаем удаляемую вершину
        //Вычисляем новый индекс для текущей вершины
        int new_i = (i < remove_idx) ? i : i - 1;
        for (int j = 0; j < vertices; j++) {
            if (j == remove_idx) continue;//Пропускаем удаляемую вершину
            //Вычисляем новый индекс для текущего столбца
            int new_j = (j < remove_idx) ? j : j - 1;
            if (i == keep_idx || j == keep_idx) {
                //Для оставляемой вершины объединяем связи
                if (i == keep_idx && j == keep_idx) {
                    newMatrix[new_i][new_j] = 0;// Петли нет
                }
                else if (i == keep_idx) {
                    //Объединяем связи из обеих вершин (логическое ИЛИ)
                    newMatrix[new_i][new_j] = (matrix[keep_idx][j] || matrix[remove_idx][j]) ? 1 : 0;
                }
                else if (j == keep_idx) {
                    //Объединяем связи в обе вершины (логическое ИЛИ)
                    newMatrix[new_i][new_j] = (matrix[i][keep_idx] || matrix[i][remove_idx]) ? 1 : 0;
                }
            }
            else {
                //Просто копируем значение для остальных вершин
                newMatrix[new_i][new_j] = matrix[i][j];
            }
        }
    }
    return newMatrix;
}

//Операция стягивания ребра
int** contractEdge(int** matrix, int vertices, int v1, int v2, int* newVertices) {
    //Проверка корректности введенных вершин
    if (v1 < 1 || v1 > vertices || v2 < 1 || v2 > vertices || v1 == v2) {
        printf("Ошибка: некорректные номера вершин!\n");
        *newVertices = vertices;
        return copyMatrix(matrix, vertices);
    }
    int idx1 = v1 - 1;
    int idx2 = v2 - 1;
    //Проверяем, существует ли ребро между вершинами
    if (matrix[idx1][idx2] == 0) {
        printf("Ошибка: ребро между вершинами %d и %d не существует!\n", v1, v2);
        *newVertices = vertices;
        return copyMatrix(matrix, vertices);
    }
    //Стягивание ребра аналогично отождествлению вершин
    return identifyVertices(matrix, vertices, v1, v2, newVertices);
}

//Операция расщепления вершины
int** splitVertex(int** matrix, int vertices, int v, int* newVertices) {
    //Проверка корректности введенной вершины
    if (v < 1 || v > vertices) {
        printf("Ошибка: некорректный номер вершины!\n");
        *newVertices = vertices;
        return copyMatrix(matrix, vertices);
    }
    int idx = v - 1;
    *newVertices = vertices + 1;
    //Создаем новую матрицу
    int** newMatrix = (int**)malloc(*newVertices * sizeof(int*));
    for (int i = 0; i < *newVertices; i++) {
        newMatrix[i] = (int*)calloc(*newVertices, sizeof(int));
    }
    //Копируем исходную матрицу без изменений
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            newMatrix[i][j] = matrix[i][j];
        }
    }
    int newVertexIdx = vertices; // Новая вершина имеет индекс vertices (последняя)
    //Соединяем исходную и новую вершину (ребро между ними)
    newMatrix[idx][newVertexIdx] = 1;
    newMatrix[newVertexIdx][idx] = 1;
    //На диагонали нет петель
    for (int i = 0; i < *newVertices; i++) {
        newMatrix[i][i] = 0;
    }
    return newMatrix;
}

//Функция для освобождения памяти
void freeMatrix(int** matrix, int vertices) {
    // Освобождаем память для каждой строки
    for (int i = 0; i < vertices; i++) {
        free(matrix[i]);
    }
    //Освобождаем память для массива указателей
    free(matrix);
}

int main() {
    setlocale(LC_ALL, "ru");
    srand(time(NULL));
    int vertices;
    printf("Введите количество вершин для графа: ");
    scanf("%d", &vertices);

    //Проверка корректности ввода
    if (vertices <= 0) {
        printf("Ошибка: количество вершин должно быть положительным числом.\n");
        return 1;
    }

    //Выделение памяти и генерация исходной матрицы
    int** matrix = (int**)malloc(vertices * sizeof(int*));
    for (int i = 0; i < vertices; i++) {
        matrix[i] = (int*)malloc(vertices * sizeof(int));
    }

    //Генерируем и выводим исходную матрицу
    generateAdjacencyMatrix(matrix, vertices);
    printf("\nИсходный граф\n");
    printMatrix(matrix, vertices);
    int choice;
    int v1, v2, v;
    int newVertices;
    do {
        printf("\nОперации над графом \n");
        printf("1 - Отождествление вершин\n");
        printf("2 - Стягивание ребра\n");
        printf("3 - Расщепление вершины\n");
        printf("0 - Выход\n");
        printf("Выберите операцию: ");
        scanf("%d", &choice);
        //Обработка выбора пользователя
        switch (choice) {
        case 1: {
            //Операция отождествления вершин
            printf("Введите номера вершин для отождествления (через пробел): ");
            scanf("%d %d", &v1, &v2);
            int** result = identifyVertices(matrix, vertices, v1, v2, &newVertices);
            printf("\nРезультат отождествления вершин %d и %d:\n", v1, v2);
            printMatrix(result, newVertices);
            freeMatrix(result, newVertices);
            break;
        }
        case 2: {
            //Операция стягивания ребра
            printf("Введите номера вершин ребра для стягивания (через пробел): ");
            scanf("%d %d", &v1, &v2);
            int** result = contractEdge(matrix, vertices, v1, v2, &newVertices);
            printf("\nРезультат стягивания ребра (%d, %d):\n", v1, v2);
            printMatrix(result, newVertices);
            freeMatrix(result, newVertices);
            break;
        }
        case 3: {
            // Операция расщепления вершины
            printf("Введите номер вершины для расщепления: ");
            scanf("%d", &v);
            int** result = splitVertex(matrix, vertices, v, &newVertices);
            printf("\nРезультат расщепления вершины %d:\n", v);
            printMatrix(result, newVertices);
            freeMatrix(result, newVertices);
            break;
        }
        case 0:
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор!\n");
        }
    } while (choice != 0);
    //Освобождение памяти исходной матрицы
    freeMatrix(matrix, vertices);
    return 0;
}