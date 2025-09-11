#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "ru");//Подключаем русский
    int n;//Размер массива

    //Ввод размера массива с проверкой
    printf("Введите размер массива: ");
    while (scanf("%d", &n) != 1 || n <= 0) {
        printf("Ошибка! Введите целое положительное число: ");
        while (getchar() != '\n'); // Очистка буфера ввода
    }

    //Выделение памяти для динамического массива
    int* arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    //Ввод элементов массива с клавиатуры с проверкой
    printf("Введите %d элементов массива:\n", n);
    for (int i = 0; i < n; i++) {
        printf("Элемент %d: ", i + 1);

        //Проверка корректности ввода для каждого элемента
        while (scanf("%d", &arr[i]) != 1) {
            printf("Ошибка! Введите целое число для элемента %d: ", i + 1);
            while (getchar() != '\n');//Очистка буфера ввода
        }
    }

    //Вывод массива
    printf("Введенный массив: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    //Освобождение памяти
    free(arr);
    return 0;
}