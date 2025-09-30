#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

//Структура узла бинарного дерева
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};
struct Node* root = NULL;// Корень дерева

//Функция проверки, является ли строка числом
int is_number(const char* str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i]) && !(i == 0 && str[i] == '-')) {
            return 0;// Не число
        }
    }
    return 1;// Строка является числом
}

//Функция создания дерева (рекурсивная)
struct Node* CreateTree(struct Node* r, int data) {
    if (r == NULL) {
        r = (struct Node*)malloc(sizeof(struct Node));
        if (r == NULL) {
            printf("Ошибка выделения памяти");
            exit(0);
        }
        r->left = NULL;
        r->right = NULL;
        r->data = data;
        return r;
    }
    if (data > r->data)
        r->right = CreateTree(r->right, data);
    else
        r->left = CreateTree(r->left, data);
    return r;
}

//Функция вывода дерева в консоль (вертикальный вывод)
void print_tree(struct Node* r, int l) {
    if (r == NULL) {
        return;
    }
    print_tree(r->right, l + 1);
    for (int i = 0; i < l; i++) {
        printf("  ");
    }
    printf("%d\n", r->data);
    print_tree(r->left, l + 1);
}

//Функция поиска уровня последнего повторения элемента
int find_last_occurrence_level(struct Node* r, int key, int level) {
    if (r == NULL) return -1;
    int level_found = -1;
    //Сначала ищем в правом поддереве
    int right_level = find_last_occurrence_level(r->right, key, level + 1);
    if (right_level != -1) level_found = right_level;
    //Если не нашли в правом, ищем здесь
    if (r->data == key) level_found = level;
    //Если нашли здесь, проверяем левое поддерево
    int left_level = find_last_occurrence_level(r->left, key, level + 1);
    if (left_level != -1) level_found = left_level;
    return level_found;
}

int main() {
    setlocale(LC_ALL, "");
    char input[100];
    int D;
    root = NULL;
    printf("Вводите числа для построения дерева.\n");
    printf("Для окончания ввода введите stop\n\n");

    while (1) {
        printf("Введите число: ");
        if (scanf("%99s", input) != 1) {
            printf("Ошибка ввода.\n");
            continue;
        }

        if (strcmp(input, "stop") == 0) {
            printf("Построение дерева окончено\n\n");
            break;
        }

        if (!is_number(input)) {
            printf("Ошибка: введите число или stop для завершения.\n");
            continue;
        }

        D = atoi(input);
        root = CreateTree(root, D);
    }

    printf("Построенное дерево:\n");
    print_tree(root, 0);
    printf("\n");

    while (1) {
        printf("Введите значение для поиска уровня последнего вхождения (или stop для завершения): ");
        if (scanf("%99s", input) != 1) {
            printf("Ошибка ввода.\n");
            continue;
        }

        if (strcmp(input, "stop") == 0) {
            break;
        }

        if (!is_number(input)) {
            printf("Ошибка: введите число или stop для завершения.\n");
            continue;
        }

        int search_key = atoi(input);
        int level = find_last_occurrence_level(root, search_key, 0);

        if (level != -1)
            printf("Последнее вхождение элемента %d находится на уровне %d.\n", search_key, level);
        else
            printf("Элемент %d не найден в дереве.\n", search_key);
    }
    printf("Программа завершена. Нажмите Enter для выхода...");
    getchar();
    getchar();
    return 0;
}
