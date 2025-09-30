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
        //Выделение памяти для нового узла
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

//Функция поиска минимального значения в дереве
struct Node* find_min(struct Node* r) {
    if (r == NULL) return NULL;
    while (r->left != NULL) {
        r = r->left;
    }
    return r;
}

//Функция удаления узла из дерева
struct Node* delete_node(struct Node* r, int key) {
    if (r == NULL) return NULL;
    //Поиск узла для удаления
    if (key < r->data) {
        r->left = delete_node(r->left, key);
    }
    else if (key > r->data) {
        r->right = delete_node(r->right, key);
    }
    else {
        //Узел найден - удаляем
        //У узла нет потомков или только один потомок
        if (r->left == NULL) {
            struct Node* temp = r->right;
            free(r);
            return temp;
        }
        else if (r->right == NULL) {
            struct Node* temp = r->left;
            free(r);
            return temp;
        }
        //У узла два потомка
        //Находим минимальный элемент в правом поддереве
        struct Node* temp = find_min(r->right);
        //Заменяем данные текущего узла на найденный минимальный
        r->data = temp->data;
        //Удаляем найденный минимальный элемент из правого поддерева
        r->right = delete_node(r->right, temp->data);
    }
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
    //Меню операций
    while (1) {
        printf("\nВыберите операцию:\n");
        printf("1 - Поиск элемента\n");
        printf("2 - Подсчет вхождений элемента\n");
        printf("3 - Удаление элемента\n");
        printf("4 - Вывод дерева\n");
        printf("5 - Выход\n");
        printf("Введите номер операции: ");
        if (scanf("%99s", input) != 1) {
            printf("Ошибка ввода.\n");
            continue;
        }
        char operation_choice[10];
        strcpy(operation_choice, input);//Сохраняем выбор операции
        if (strcmp(operation_choice, "5") == 0) {
            break;
        }
        if (strcmp(operation_choice, "4") == 0) {
            printf("\nТекущее дерево:\n");
            print_tree(root, 0);
            continue;
        }
        if (strcmp(operation_choice, "1") == 0 || strcmp(operation_choice, "2") == 0 || strcmp(operation_choice, "3") == 0) {
            printf("Введите значение: ");
            if (scanf("%99s", input) != 1) {
                printf("Ошибка ввода.\n");
                continue;
            }
            //Проверка корректности ввода
            if (!is_number(input)) {
                printf("Ошибка: введите число\n");
                continue;
            }
            int value = atoi(input);
            if (strcmp(operation_choice, "1") == 0) {
                //Поиск элемента
                struct Node* result = search(root, value);
                if (result != NULL)
                    printf("Элемент %d найден в дереве.\n", value);
                else
                    printf("Элемент %d не найден в дереве.\n", value);

            }
            else if (strcmp(operation_choice, "2") == 0) {
                //Подсчет вхождений
                int occurrences = count_occurrences(root, value);
                printf("Элемент %d встречается в дереве %d раз(а).\n", value, occurrences);

            }
            else if (strcmp(operation_choice, "3") == 0) {
                //Удаление элемента
                if (search(root, value) != NULL) {
                    root = delete_node(root, value);
                    printf("Элемент %d удален из дерева.\n", value);
                    printf("\nОбновленное дерево:\n");
                    print_tree(root, 0);
                }
                else {
                    printf("Элемент %d не найден в дереве. Удаление невозможно.\n", value);
                }
            }
        }
        else {
            printf("Неверный выбор операции. Попробуйте снова.\n");
        }
    }
    printf("Программа завершена. Нажмите Enter для выхода...");
    getchar();
    getchar();
    return 0;
}