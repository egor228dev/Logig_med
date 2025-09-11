#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
int main() {
    setlocale(LC_ALL, "ru");//Подключаем русский
    int n;//Размер массива
    printf("Введите размер массива: ");
    scanf("%d", &n);

    //Выделение памяти для динамического массива
    int* arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    //Инициализация генератора случайных чисел
    srand(time(NULL));

    //Заполнение массива случайными числами
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100; //От 0 до 99
    }

    //Вывод массива
    printf("Сгенерированный массив: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    //Освобождение памяти
    free(arr);
    return 0;
}