#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "ru");//Подключаем русский язык
    int n, i, j;//Размер, строка, столбец

    //Получение размера массива от пользователя
    printf("Введите размер квадратного массива: ");
    scanf("%d", &n);

    //Выделение памяти для двумерного массива
    int** matrix = (int**)malloc(n * sizeof(int*));
    if (matrix == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }
    for (i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
        if (matrix[i] == NULL) {
            printf("Ошибка выделения памяти!\n");
            //Освобождаем ранее выделенную память
            for (int k = 0; k < i; k++) {
                free(matrix[k]);
            }
            free(matrix);
            return 1;
        }
    }

    //Инициализация генератора случайных чисел
    srand(time(NULL));

    //Заполнение только верхнего треугольника (включая диагональ)
    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++) {//Начинаем с диагонали
            matrix[i][j] = rand() % 21 - 5;//От -5 до 15
            //Копируем значение в симметричную позицию (кроме диагонали)
            if (i != j) {
                matrix[j][i] = matrix[i][j];
            }
        }
    }

    //Вывод массива
    printf("Сгенерированный симметричный массив:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    //Освобождение выделенной памяти
    for (i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}