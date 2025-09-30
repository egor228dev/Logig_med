#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

//Структура узла бинарного дерева поиска
struct Node {
    int data;// Значение узла
    struct Node* left;// Указатель на левое поддерево (меньшие значения)
    struct Node* right;// Указатель на правое поддерево (большие значения)
};
struct Node* root = NULL;// Корень дерева

//Функция проверки, является ли строка числом
int is_number(const char* str) {
    //Проверка на пустую строку или NULL указатель
    if (str == NULL || *str == '\0') return 0;
    //Посимвольная проверка строки
    for (int i = 0; str[i] != '\0'; i++) {
        //Разрешаем цифры и минус
        if (!isdigit(str[i]) && !(i == 0 && str[i] == '-')) {
            return 0;// Обнаружен недопустимый символ
        }
    }
    return 1;// Строка содержит корректное число
}

//Рекурсивная функция поиска элемента в дереве
struct Node* search(struct Node* r, int key) {
    //Пустой узел или найденное значение
    if (r == NULL || r->data == key)
        return r;
    //Рекурсивный поиск в правом поддереве для больших значений
    if (key > r->data)
        return search(r->right, key);
    //Рекурсивный поиск в левом поддереве для меньших значений
    else
        return search(r->left, key);
}

//Функция создания дерева с защитой от дубликатов
struct Node* CreateTree(struct Node* r, int data) {
    //Создание нового узла при достижении пустой позиции
    if (r == NULL) {
        r = (struct Node*)malloc(sizeof(struct Node));
        if (r == NULL) {
            printf("Ошибка выделения памяти");
            exit(0);
        }
        //Инициализация полей нового узла
        r->data = data;
        r->left = NULL;
        r->right = NULL;
        return r;
    }
    //Вставка в правое поддерево для значений больше текущего узла
    if (data > r->data) {
        r->right = CreateTree(r->right, data);
    }
    //Вставка в левое поддерево для значений меньше текущего узла
    else if (data < r->data) {
        r->left = CreateTree(r->left, data);
    }
    //Если значение равно текущему узлу - дубликат игнорируется
    return r;
}

//Функция визуального вывода дерева
void print_tree(struct Node* r, int l) {
    //Пустое поддерево
    if (r == NULL) {
        return;
    }
    //Рекурсивный вывод правого поддерева
    print_tree(r->right, l + 1);
    //Формирование отступов
    for (int i = 0; i < l; i++) {
        printf("  ");
    }
    //Вывод значения текущего узла
    printf("%d\n", r->data);
    //Рекурсивный вывод левого поддерева
    print_tree(r->left, l + 1);
}

//Функция подсчета количества вхождений элемента в дерево
int count_occurrences(struct Node* r, int key) {
    //Пустое поддерево
    if (r == NULL)
        return 0;

    int count = 0;
    //Увеличение счетчика при совпадении значения
    if (r->data == key)
        count = 1;

    //Рекурсивный подсчет в левом и правом поддеревьях
    return count + count_occurrences(r->left, key) + count_occurrences(r->right, key);
}

int main() {
    setlocale(LC_ALL, "");
    char input[100];// Буфер для ввода пользователя
    int D;// Для введенных данных
    root = NULL;// Инициализация пустого дерева
    printf("Вводите числа для построения дерева.\n");
    printf("Для окончания ввода введите stop\n\n");

    //Построение бинарного дерева поиска
    while (1) {
        printf("Введите число: ");
        //Чтение ввода пользователя с ограничением длины
        if (scanf("%99s", input) != 1) {
            printf("Ошибка ввода.\n");
            continue;
        }

        //Проверка команды завершения ввода
        if (strcmp(input, "stop") == 0) {
            printf("Построение дерева окончено\n\n");
            break;
        }

        //Проверка что введено число
        if (!is_number(input)) {
            printf("Ошибка: введите число или stop для завершения.\n");
            continue;
        }

        //Преобразование строки в целое число
        D = atoi(input);
        //Проверка на дубликат перед добавлением
        if (search(root, D) != NULL) {
            printf("Ошибка: число %d уже существует в дереве. Введите другое число.\n", D);
            continue;
        }
        //Добавление уникального элемента в дерево
        root = CreateTree(root, D);
    }
    printf("Построенное дерево:\n");
    print_tree(root, 0);//Вывод начиная с нулевого уровня
    printf("\n");

    //Поиск элементов
    while (1) {
        printf("Введите значение для поиска (или stop для завершения): ");
        if (scanf("%99s", input) != 1) {
            printf("Ошибка ввода.\n");
            continue;
        }

        //Проверка команды завершения поиска
        if (strcmp(input, "stop") == 0) {
            break;
        }

        //Валидация ввода для поиска
        if (!is_number(input)) {
            printf("Ошибка: введите число или stop для завершения.\n");
            continue;
        }

        int search_key = atoi(input);//Ключ для поиска
        //Выполнение поиска в дереве
        struct Node* result = search(root, search_key);
        //Вывод результата поиска
        if (result != NULL)
            printf("Элемент %d найден в дереве.\n", search_key);
        else
            printf("Элемент %d не найден в дереве.\n", search_key);
    }

    //Подсчет количества вхождений элементов
    while (1) {
        printf("Введите значение для подсчета вхождений (или stop для завершения): ");
        if (scanf("%99s", input) != 1) {
            printf("Ошибка ввода.\n");
            continue;
        }

        //Проверка команды завершения подсчета
        if (strcmp(input, "stop") == 0) {
            break;
        }

        //Валидация ввода для подсчета
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