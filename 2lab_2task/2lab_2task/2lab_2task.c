#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>

//Объявления функций
char* get_type_name(int type);
void shell(int* items, int count);
void qs_iterative(int* items, int n);
int compare(const void* a, const void* b);
void generate_data(int* arr, int n, int type);
void test_sort(char* name, void (*sort_func)(int*, int), int* arr, int n, int type);
void test_qsort(int* arr, int n, int type);

//Функция возвращает название типа данных для вывода
char* get_type_name(int type) {
    switch (type) {
    case 0: return "Случайные";
    case 1: return "Возрастающая";
    case 2: return "Убывающая";
    case 3: return "Смешанная";
    default: return "Неизвестно";
    }
}

//Реализация сортировки Шелла
void shell(int* items, int count) {
    int i, j, gap, k;
    int x, a[5] = { 9, 5, 3, 2, 1 }; //Последовательность промежутков
    for (k = 0; k < 5; k++) {
        gap = a[k]; //Текущий промежуток
        for (i = gap; i < count; ++i) {
            x = items[i]; //Запоминаем текущий элемент
            //Сдвигаем элементы, пока не найдем правильную позицию
            for (j = i - gap; (x < items[j]) && (j >= 0); j = j - gap)
                items[j + gap] = items[j];
            items[j + gap] = x; //Вставляем элемент на правильное место
        }
    }
}

// Итеративная версия быстрой сортировки-без рекурсии
void qs_iterative(int* items, int n) {
    //Выделяем память для стека-границы подмассивов
    int* stack = (int*)malloc(n * 2 * sizeof(int));
    int top = -1; //Вершина стека

    //Помещаем в стек начальные границы всего массива
    stack[++top] = 0;
    stack[++top] = n - 1;

    //Пока стек не пуст
    while (top >= 0) {
        //Извлекаем границы текущего подмассива
        int right = stack[top--];
        int left = stack[top--];
        int i = left, j = right;
        //Выбираем опорный элемент (середина массива)
        int pivot = items[(left + right) / 2];

        //Разделение массива относительно опорного элемента
        do {
            while (items[i] < pivot) i++; //Ищем элемент больше опорного слева
            while (pivot < items[j]) j--; //Ищем элемент меньше опорного справа
            if (i <= j) {
                //Обмен элементов
                int temp = items[i];
                items[i] = items[j];
                items[j] = temp;
                i++; j--;
            }
        } while (i <= j);

        //Помещаем в стек границы левого подмассива (если он есть)
        if (left < j) {
            stack[++top] = left;
            stack[++top] = j;
        }
        //Помещаем в стек границы правого подмассива (если он есть)
        if (i < right) {
            stack[++top] = i;
            stack[++top] = right;
        }
    }
    free(stack);
}

//Функция сравнения для стандартной qsort
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b); //Сравнение двух чисел
}

//Генерация тестовых данных разных типов
void generate_data(int* arr, int n, int type) {
    switch (type) {
    case 0: //Случайные числа
        for (int i = 0; i < n; i++) arr[i] = rand() % 10000;
        break;
    case 1: //Возрастающая последовательность
        for (int i = 0; i < n; i++) arr[i] = i;
        break;
    case 2: //Убывающая последовательность
        for (int i = 0; i < n; i++) arr[i] = n - i;
        break;
    case 3: //Смешанный тип: первая половина возрастает, вторая убывает
        for (int i = 0; i < n / 2; i++) arr[i] = i;
        for (int i = n / 2; i < n; i++) arr[i] = n - i;
        break;
    }
}

//Тестирование алгоритма сортировки
void test_sort(char* name, void (*sort_func)(int*, int), int* arr, int n, int type) {
    //Создаем временную копию массива для тестирования
    int* temp = (int*)malloc(n * sizeof(int));
    if (temp == NULL) return; 
    memcpy(temp, arr, n * sizeof(int)); //Копируем исходные данные

    //Засекаем время
    clock_t start = clock();
    if (strcmp(name, "Быстрая") == 0) {
        qs_iterative(temp, n);
    }
    else {
        sort_func(temp, n);
    }
    clock_t end = clock();

    //Вычисляем время в секундах и выводим результат
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%-10s | %-12s | %7d | %10.5f\n",
        name, get_type_name(type), n, time_taken);
    free(temp);
}

//Тестирование стандартной функции qsort
void test_qsort(int* arr, int n, int type) {
    //Создаем временную копию массива
    int* temp = (int*)malloc(n * sizeof(int));
    if (temp == NULL) return;
    memcpy(temp, arr, n * sizeof(int));

    //Измеряем время выполнения qsort
    clock_t start = clock();
    qsort(temp, n, sizeof(int), compare);
    clock_t end = clock();

    //Выводим результаты
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%-10s | %-12s | %7d | %10.5f\n",
        "qsort", get_type_name(type), n, time_taken);

    free(temp);
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    //Размеры массивов для тестирования
    int sizes[] = { 1000, 5000, 10000, 50000, 100000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    printf("Алгоритм   | Тип данных   | Размер  | Время(сек)\n");
    printf("------------------------------------------------\n");

    //Тестирование для каждого размера массива
    for (int size_idx = 0; size_idx < num_sizes; size_idx++) {
        int n = sizes[size_idx];
        int* arr = (int*)malloc(n * sizeof(int));
        if (arr == NULL) continue;

        //Тестирование для каждого типа данных
        for (int type = 0; type < 4; type++) {
            generate_data(arr, n, type);
            test_sort("Шелла", shell, arr, n, type);
            test_sort("Быстрая", NULL, arr, n, type);
            test_qsort(arr, n, type);
            printf("------------------------------------------------\n");
        }
        free(arr);
    }
    return 0;
}