#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <wchar.h>
#include <string.h>

//Структура элемента стека
struct node {
    wchar_t inf[256];// Название объекта
    struct node* next;// Указатель на следующий элемент в стеке
};
struct node* top = NULL;// Указатель на вершину стека

//Функция очистки буфера ввода от лишних символов
void clear_input_buffer() {
    int c;
    //Читаем все символы до конца строки
    while ((c = getchar()) != '\n' && c != EOF);
}

//Функция проверки, является ли строка корректным целым числом
int is_number(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;// Пустая строка - не число
    }
    char* endptr;
    //Пытаемся преобразовать строку в число
    strtol(str, &endptr, 10);
    //Если преобразование прошло до конца строки - это корректное число
    return *endptr == '\0';
}

//Функция создания нового элемента стека
struct node* get_struct(void) {
    //Выделяем память под новый элемент
    struct node* p = (struct node*)malloc(sizeof(struct node));
    if (p == NULL) {
        wprintf(L"Ошибка при распределении памяти\n");
        exit(1);
    }

    //Ввод названия объекта с проверкой уникальности
    int valid_input = 0;
    while (!valid_input) {
        wprintf(L"Введите название объекта: ");
        clear_input_buffer(); // Очищаем буфер перед чтением строки
        if (fgetws(p->inf, 256, stdin) == NULL) {
            wprintf(L"Ошибка ввода\n");
            free(p);
            return NULL;
        }

        //Удаление символа новой строки из введенной строки
        size_t len = wcslen(p->inf);
        if (len > 0 && p->inf[len - 1] == L'\n') {
            p->inf[len - 1] = L'\0';
        }
        valid_input = 1;// Ввод корректен, выходим из цикла
    }
    p->next = NULL;// Новый элемент пока не ссылается на следующий
    return p;
}

//Добавление элемента на вершину стека 
void push(void) {
    struct node* p = get_struct();
    if (p == NULL) return;
    //Новый элемент становится новой вершиной стека
    p->next = top;// Связываем новый элемент с предыдущей вершиной
    top = p;// Обновляем указатель на вершину стека
    wprintf(L"Элемент добавлен в стек: %ls\n", p->inf);
}

//Удаление элемента с вершины стека
void pop(void) {
    if (top == NULL) {
        wprintf(L"Стек пуст\n");
        return;
    }
    //Сохраняем указатель на удаляемый элемент (вершину стека)
    struct node* temp = top;
    //Перемещаем вершину стека на следующий элемент
    top = top->next;
    wprintf(L"Удален элемент: %ls\n", temp->inf);
    free(temp);// Освобождаем память удаленного элемента
}

//Просмотр верхнего элемента стека без удаления
void peek_stack(void) {
    if (top == NULL) {
        wprintf(L"Стек пуст\n");
        return;
    }
    wprintf(L"Верхний элемент: %ls\n", top->inf);
}

//Просмотр всего стека (от вершины к основанию)
void display_stack(void) {
    struct node* current = top;
    if (current == NULL) {
        wprintf(L"Стек пуст\n");
        return;
    }
    wprintf(L"Содержимое стека (сверху вниз):\n");
    //Проходим по всем элементам стека и выводим их
    while (current != NULL) {
        wprintf(L"%ls\n", current->inf);
        current = current->next;
    }
}

//Проверка пустоты стека
int is_empty(void) {
    return top == NULL;// Возвращает 1 если стек пуст, 0 если нет
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    SetConsoleOutputCP(CP_UTF8);// Для вывода
    SetConsoleCP(CP_UTF8);// Для ввода
    int choice;
    char input[256];
    wprintf(L"Реализация стека (LIFO)\n");

    do {
        wprintf(L"\nМеню:\n");
        wprintf(L"1. Добавить элемент\n");
        wprintf(L"2. Удалить элемент\n");
        wprintf(L"3. Просмотреть верхний элемент\n");
        wprintf(L"4. Просмотреть весь стек\n");
        wprintf(L"5. Проверить пустоту стека\n");
        wprintf(L"6. Выход\n");
        wprintf(L"Выберите действие: ");

        //Чтение выбора пользователя
        if (scanf("%255s", input) != 1) {
            clear_input_buffer();
            wprintf(L"Неверный ввод. Попробуйте снова.\n");
            continue;
        }

        //Проверка, что введено число
        if (!is_number(input)) {
            wprintf(L"Ошибка: введите число от 1 до 6. Попробуйте снова.\n");
            clear_input_buffer();
            continue;
        }
        choice = atoi(input);// Преобразуем строку в число
        switch (choice) {
        case 1:
            push();
            break;
        case 2:
            pop();
            break;
        case 3:
            peek_stack();
            break;
        case 4:
            display_stack();
            break;
        case 5:
            if (is_empty()) {
                wprintf(L"Стек пуст\n");
            }
            else {
                wprintf(L"Стек не пуст\n");
            }
            break;
        case 6:
            wprintf(L"Выход...\n");
            break;
        default:
            wprintf(L"Неверный выбор. Введите число от 1 до 6.\n");
        }
    } while (choice != 6);// Продолжаем пока не выбран выход
    //Очистка памяти - удаление всех элементов стека
    while (top != NULL) {
        pop();
    }
    return 0;
}