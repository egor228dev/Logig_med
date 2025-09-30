#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

//Структура узла бинарного дерева
struct Node {
    int data;//Значение узла
    struct Node* left;//Указатель на левое поддерево
    struct Node* right;//Указатель на правое поддерево
};
struct Node* root = NULL;//Корень дерева

//Функция проверки, является ли строка числом
int is_number(const char* str) {
    //Проверка на пустую строку или NULL
    if (str == NULL || *str == '\0') return 0;

    //Проверяем каждый символ строки
    for (int i = 0; str[i] != '\0'; i++) {
        // Разрешаем цифры и минус только в начале числа
        if (!isdigit(str[i]) && !(i == 0 && str[i] == '-')) {
            return 0;//Не число
        }
    }
    return 1;//Строка является числом
}

//Функция создания дерева (рекурсивная)
struct Node* CreateTree(struct Node* r, int data)
{
    //Если достигли пустого узла - создаем новый
    if (r == NULL)
    {
        // Выделение памяти для нового узла
        r = (struct Node*)malloc(sizeof(struct Node));
        if (r == NULL)
        {
            printf("Ошибка выделения памяти");
            exit(0);
        }
        //Инициализация полей нового узла
        r->left = NULL;
        r->right = NULL;
        r->data = data;
        return r;
    }
    //Рекурсивная вставка в правое поддерево если данные больше текущего узла
    if (data > r->data)
        r->right = CreateTree(r->right, data);
    //Рекурсивная вставка в левое поддерево если данные меньше или равны
    else
        r->left = CreateTree(r->left, data);
    return r;
}

//Функция вывода дерева в консоль (вертикальный вывод)
void print_tree(struct Node* r, int l)
{
    //Пустое дерево
    if (r == NULL)
    {
        return;
    }
    //Рекурсивный вывод правого поддерева
    print_tree(r->right, l + 1);
    //Вывод отступов для визуализации уровня узла
    for (int i = 0; i < l; i++)
    {
        printf("  ");
    }
    //Вывод данных текущего узла
    printf("%d\n", r->data);
    //Рекурсивный вывод левого поддерева
    print_tree(r->left, l + 1);
}

//Функция поиска элемента в дереве (рекурсивная)
struct Node* search(struct Node* r, int key)
{
    //Пустое дерево или найденный элемент
    if (r == NULL || r->data == key)
        return r;
    //Рекурсивный поиск в правом поддереве если ключ больше
    if (key > r->data)
        return search(r->right, key);
    //Рекурсивный поиск в левом поддереве если ключ меньше
    else
        return search(r->left, key);
}

//Функция подсчёта числа вхождений элемента в дерево
int count_occurrences(struct Node* r, int key)
{
    //Пустое дерево
    if (r == NULL)
        return 0;
    int count = 0;
    //Если нашли совпадение - увеличиваем счетчик
    if (r->data == key)
        count = 1;
    //Рекурсивный подсчет в левом и правом поддеревьях
    return count + count_occurrences(r->left, key) + count_occurrences(r->right, key);
}

int main()
{
    setlocale(LC_ALL, "");
    char input[100];// Буфер для ввода пользователя
    int D;// Числовое значение введенных данных
    root = NULL;// Инициализация корня дерева
    printf("Вводите числа для построения дерева.\n");
    printf("Для окончания ввода введите stop\n\n");

    //Построение дерева
    while (1)
    {
        printf("Введите число: ");
        //Чтение ввода пользователя
        if (scanf("%99s", input) != 1) {
            printf("Ошибка ввода.\n");
            continue;
        }

        //Проверка на команду остановки ввода
        if (strcmp(input, "stop") == 0) {
            printf("Построение дерева окончено\n\n");
            break;
        }

        //Проверка, является ли ввод числом
        if (!is_number(input)) {
            printf("Ошибка: введите число или stop для завершения.\n");
            continue;
        }

        //Преобразование строки в целое число
        D = atoi(input);
        //Добавление элемента в дерево
        root = CreateTree(root, D);
    }

    //Вывод построенного дерева
    printf("Построенное дерево:\n");
    print_tree(root, 0);// Начальный уровень = 0
    printf("\n");

    //Поиск элементов в дереве
    while (1) {
        printf("Введите значение для поиска (или stop для завершения): ");
        if (scanf("%99s", input) != 1) {
            printf("Ошибка ввода.\n");
            continue;
        }

        //Проверка на команду остановки поиска
        if (strcmp(input, "stop") == 0) {
            break;
        }

        //Проверка корректности ввода
        if (!is_number(input)) {
            printf("Ошибка: введите число или stop для завершения.\n");
            continue;
        }

        int search_key = atoi(input);// Ключ для поиска
        struct Node* result = search(root, search_key);// Поиск в дереве

        //Вывод результата поиска
        if (result != NULL)
            printf("Элемент %d найден в дереве.\n", search_key);
        else
            printf("Элемент %d не найден в дереве.\n", search_key);
    }

    //Подсчет вхождений элементов
    while (1) {
        printf("Введите значение для подсчета вхождений (или stop для завершения): ");
        if (scanf("%99s", input) != 1) {
            printf("Ошибка ввода.\n");
            continue;
        }

        //Проверка на команду остановки подсчета
        if (strcmp(input, "stop") == 0) {
            break;
        }

        //Проверка корректности ввода
        if (!is_number(input)) {
            printf("Ошибка: введите число или stop для завершения.\n");
            continue;
        }

        int count_key = atoi(input);//Ключ для подсчета
        //Подсчет количества вхождений элемента
        int occurrences = count_occurrences(root, count_key);
        printf("Элемент %d встречается в дереве %d раз(а).\n", count_key, occurrences);
    }
    printf("Программа завершена. Нажмите Enter для выхода...");
    getchar();
    getchar();
    return 0;
}
