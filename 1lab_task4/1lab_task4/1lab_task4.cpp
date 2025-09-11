#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
int main() {
    setlocale(LC_ALL, "ru");//Подключаем русский
    int rows, collumns;//Матрица(строки, столбцы)
    printf("Введите количество строк: ");
    scanf("%d", &rows);
    printf("Введите количество столбцов: ");
    scanf("%d", &collumns);

    //Выделение памяти для динамического двумерного массива
    int** arr = (int**)malloc(rows * sizeof(int*));
    if (arr == NULL) {
        printf("Ошибка создания матрицы, недопустимые значения!\n");
        return 1;
    }

    //Проверка выделения памяти
    for (int i = 0; i < rows; i++) {
        arr[i] = (int*)malloc(collumns * sizeof(int));
        if (arr[i] == NULL) {
            printf("Ошибка создания матрицы, недопустимые значения!\n");
            //Освобождаем уже выделенную память перед выходом
            for (int j = 0; j < i; j++) {
                free(arr[j]);
            }
            free(arr);
            return 1;
        }
    }

    //Инициализация генератора случайных чисел
    srand(time(NULL));

    //Заполнение массива случайными числами и вывод
    printf("Двумерный массив:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < collumns; j++) {
            arr[i][j] = rand() % 10; //От 0 до 9
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }

    //Вычисление суммы по столбцам
    printf("Суммы по столбцам:\n");
    for (int j = 0; j < collumns; j++) {
        int sum = 0;
        for (int i = 0; i < rows; i++) {
            sum += arr[i][j];
        }
        printf("Столбец %d: %d\n", j + 1, sum);
    }

    //Освобождение памяти
    for (int i = 0; i < rows; i++) {
        free(arr[i]);
    }
    free(arr);
    return 0;
}