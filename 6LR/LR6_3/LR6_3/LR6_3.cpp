#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

//Функция для генерации матрицы смежности
void generateAdjacencyMatrix(int** matrix, int vertices) {
    //Проходим по всем вершинам графа
    for (int i = 0; i < vertices; i++) {
        //Начинаем с j = i+1 чтобы избежать дублирования (граф неориентированный)
        for (int j = i + 1; j < vertices; j++) {
            //Генерируем случайное значение 0 или 1 для наличия ребра
            int value = rand() % 2;
            //Заполняем матрицу симметрично
            matrix[i][j] = value;
            matrix[j][i] = value;
        }
        //Устанавливаем 0 на главной диагонали
        matrix[i][i] = 0;
    }
}

//Функция для вывода матрицы смежности на экран
void printMatrix(int** matrix, int vertices, const char* name) {
    //Выводим название матрицы и ее размеры
    printf("%s (%dx%d):\n", name, vertices, vertices);
    //Выводим заголовок с номерами столбцов
    printf("   ");
    for (int j = 0; j < vertices; j++) {
        printf("%2d ", j + 1);// +1 для нумерации вершин с 1
    }
    printf("\n");

    //Выводим саму матрицу построчно
    for (int i = 0; i < vertices; i++) {
        printf("%2d ", i + 1);// Номер строки (вершины)
        for (int j = 0; j < vertices; j++) {
            printf("%2d ", matrix[i][j]);// Значение элемента матрицы
        }
        printf("\n");
    }
    printf("\n");
}

//Функция для создания матрицы заданного размера
int** createMatrix(int vertices) {
    //Выделяем память для массива указателей на строки матрицы
    int** matrix = (int**)malloc(vertices * sizeof(int*));
    //Для каждой строки выделяем память и инициализируем нулями
    for (int i = 0; i < vertices; i++) {
        matrix[i] = (int*)calloc(vertices, sizeof(int)); // calloc инициализирует нулями
    }
    return matrix;
}

//Функция для освобождения памяти, занятой матрицей
void freeMatrix(int** matrix, int vertices) {
    //Освобождаем память каждой строки матрицы
    for (int i = 0; i < vertices; i++) {
        free(matrix[i]);
    }
    //Освобождаем память массива указателей
    free(matrix);
}

//Функция для создания глубокой копии матрицы
int** copyMatrix(int** source, int vertices) {
    //Создаем новую матрицу того же размера
    int** copy = createMatrix(vertices);
    //Копируем все элементы из исходной матрицы
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            copy[i][j] = source[i][j];
        }
    }
    return copy;
}

// а) Операция объединения графов (ИЛИ)
int** unionGraphs(int** matrix1, int** matrix2, int vertices) {
    //Создаем результирующую матрицу
    int** result = createMatrix(vertices);
    //Проходим по всем элементам матриц
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            result[i][j] = (matrix1[i][j] || matrix2[i][j]) ? 1 : 0;
        }
    }

    return result;
}

// б) Операция пересечения графов (И)
int** intersectionGraphs(int** matrix1, int** matrix2, int vertices) {
    //Создаем результирующую матрицу
    int** result = createMatrix(vertices);
    //Проходим по всем элементам матриц
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            result[i][j] = (matrix1[i][j] && matrix2[i][j]) ? 1 : 0;
        }
    }
    return result;
}

// в) Сложение по модулю 2
int** ringSumGraphs(int** matrix1, int** matrix2, int vertices) {
    //Создаем результирующую матрицу
    int** result = createMatrix(vertices);
    //Проходим по всем элементам матриц
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            //Ребро существует, если оно есть в G1 или в G2, но не в обоих
            //1 если значения разные, 0 если одинаковые
            result[i][j] = (matrix1[i][j] != matrix2[i][j]) ? 1 : 0;
        }
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "ru");
    srand(time(NULL));
    int vertices;
    printf("Введите количество вершин для графов G1 и G2: ");
    scanf("%d", &vertices);

    //Проверяем корректность введенного значения
    if (vertices <= 0) {
        printf("Ошибка: количество вершин должно быть положительным числом.\n");
        return 1; // Возвращаем код ошибки
    }
    int** matrix1 = createMatrix(vertices);
    int** matrix2 = createMatrix(vertices);

    //Заполняем матрицы случайными значениями
    generateAdjacencyMatrix(matrix1, vertices);
    generateAdjacencyMatrix(matrix2, vertices);
    //Выводим исходные графы
    printf("\nИсходные графы\n");
    printMatrix(matrix1, vertices, "Матрица смежности G1");
    printMatrix(matrix2, vertices, "Матрица смежности G2");
    int choice;// Переменная для хранения выбора пользователя
    do {
        printf("\nОперации над графами\n");
        printf("1 - Объединение G1 U G2\n");
        printf("2 - Пересечение G1 n G2\n");
        printf("3 - Сложение по модулю (2) G1 (+) G2\n");
        printf("0 - Выход\n");
        printf("Выберите операцию: ");
        scanf("%d", &choice);

        //Указатель на результирующую матрицу
        int** result = NULL;
        //Название операции для вывода
        const char* operationName = "";
        switch (choice) {
        case 1: {
            result = unionGraphs(matrix1, matrix2, vertices);
            operationName = "Результат объединения G1 U G2";
            break;
        }
        case 2: {
            result = intersectionGraphs(matrix1, matrix2, vertices);
            operationName = "Результат пересечения G1 n G2";
            break;
        }
        case 3: {
            result = ringSumGraphs(matrix1, matrix2, vertices);
            operationName = "Результат сложения по модулю (2) G1 (+) G2";
            break;
        }
        case 0:
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор!\n");
        }

        //Если операция была выполнена
        if (result != NULL) {
            //Выводим результат операции
            printf("\n%s\n", operationName);
            printMatrix(result, vertices, "");
            //Освобождаем память, занятую результатом
            freeMatrix(result, vertices);
        }
    } while (choice != 0);
    //Освобождение памяти исходных матриц
    freeMatrix(matrix1, vertices);
    freeMatrix(matrix2, vertices);
    return 0;
}