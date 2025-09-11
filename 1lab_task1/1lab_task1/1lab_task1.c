#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
int main() {
    setlocale(LC_ALL, "ru");//подключаем русский
    int n;//Размер массива
    printf("Введите размер массива: ");
    scanf("%d", &n);

    // Выделение памяти для динамического массива
    int* arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) {//Если память не выделилась 
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    //Инициализация генератора случайных чисел
    srand(time(NULL));

    //Заполнение массива случайными числами и вывод
    printf("Массив: ");
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100; //от 0 до 100
        printf("%d ", arr[i]);
    }
    printf("\n");

    //Поиск минимального и максимального элементов
    int min = arr[0], max = arr[0]; //Предполагаем, что минимальный и при этом максимальный элмент стоит на 1 позиции массива
    for (int i = 1; i < n; i++) {
        if (arr[i] < min) min = arr[i];//Переопледеляем минимальный
        if (arr[i] > max) max = arr[i];//Переопределяем максимальный
    }
    printf("Минимальный элемент: %d\n", min);
    printf("Максимальный элемент: %d\n", max);
    printf("Разница: %d\n", max - min);

    //Освобождение памяти
    free(arr);
    return 0;
}