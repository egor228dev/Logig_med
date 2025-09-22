#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>
#include <wchar.h>
#include <ctype.h>
#include <string.h>

//Структура элемента очереди
struct node {
    wchar_t inf[256];// Название объекта
    struct node* next;// Указатель на следующий элемент в очереди
};
struct node* front = NULL;// Указатель на начало очереди
struct node* rear = NULL;// Указатель на конец очереди

//Функция очистки буфера ввода от лишних символов
void clear_input_buffer() {
    int c;
    //Читаем все символы до конца строки
    while ((c = getchar()) != '\n' && c != EOF);
}

//Функция проверки, является ли строка корректным целым числом
int is_number(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0; // Пустая строка - не число
    }
    char* endptr;
    //Преобразовываем строку в число
    strtol(str, &endptr, 10);
    //Если преобразование прошло до конца строки - это корректное число
    return *endptr == '\0';
}

//Функция создания нового элемента очереди
struct node* get_struct(void) {
    //Выделяем память под новый элемент
    struct node* p = (struct node*)malloc(sizeof(struct node));
    if (p == NULL) {
        wprintf(L"Ошибка при распределении памяти\n");
        exit(1);
    }

    //Ввод названия объекта
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

//Добавление элемента в конец очереди
void enqueue(void) {
    struct node* p = get_struct();
    if (p == NULL) return;
    if (rear == NULL) {
        //Если очередь пуста - новый элемент становится и началом и концом
        front = rear = p;
    }
    else {
        //Добавляем элемент в конец очереди
        rear->next = p;
        rear = p; // Обновляем указатель на конец очереди
    }
    wprintf(L"Элемент добавлен в очередь: %ls\n", p->inf);
}

//Удаление элемента из начала очереди
void dequeue(void) {
    if (front == NULL) {
        wprintf(L"Очередь пуста\n");
        return;
    }
    //Сохраняем указатель на удаляемый элемент
    struct node* temp = front;
    //Перемещаем начало очереди на следующий элемент
    front = front->next;
    //Если после удаления очередь стала пустой
    if (front == NULL) {
        rear = NULL;// Обнуляем указатель на конец
    }
    wprintf(L"Удален элемент: %ls\n", temp->inf);
    free(temp);// Освобождаем память удаленного элемента
}

//Просмотр первого элемента очереди без удаления
void peek(void) {
    if (front == NULL) {
        wprintf(L"Очередь пуста\n");
        return;
    }
    wprintf(L"Первый элемент: %ls\n", front->inf);
}

//Просмотр всей очереди
void display(void) {
    struct node* current = front;
    if (current == NULL) {
        wprintf(L"Очередь пуста\n");
        return;
    }
    wprintf(L"Содержимое очереди:\n");
    //Проходим по всем элементам очереди и выводим их
    int index = 1;
    while (current != NULL) {
        wprintf(L"%d. %ls (длина: %zu символов)\n", index, current->inf, wcslen(current->inf));
        current = current->next;
        index++;
    }
}

//Функция сортировки очереди по длине имени
void sort_by_name_length(void) {
    if (front == NULL || front->next == NULL) {
        wprintf(L"Очередь пуста или содержит только один элемент - сортировка не требуется\n");
        return;
    }
    //Временная переменная для указания на отсортированную часть
    struct node* sorted = NULL;
    struct node* current = front;
    while (current != NULL) {
        //Сохраняем следующий элемент перед изменением связей
        struct node* next = current->next;
        //Вставляем current в отсортированный список
        if (sorted == NULL || wcslen(current->inf) <= wcslen(sorted->inf)) {
            //Вставляем в начало
            current->next = sorted;
            sorted = current;
        }
        else {
            //Ищем место для вставки
            struct node* temp = sorted;
            while (temp->next != NULL &&
                wcslen(current->inf) > wcslen(temp->next->inf)) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    //Обновляем front и rear
    front = sorted;
    //Находим новый конец очереди
    rear = front;
    if (rear != NULL) {
        while (rear->next != NULL) {
            rear = rear->next;
        }
    }
    wprintf(L"Очередь отсортирована по длине имени (по возрастанию)\n");
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    SetConsoleOutputCP(CP_UTF8);// Для вывода
    SetConsoleCP(CP_UTF8);// Для ввода
    int choice;
    char input[256];
    wprintf(L"Реализация очереди (FIFO)\n");
    do {
        wprintf(L"\nМеню:\n");
        wprintf(L"1. Добавить элемент\n");
        wprintf(L"2. Удалить элемент\n");
        wprintf(L"3. Просмотреть первый элемент\n");
        wprintf(L"4. Просмотреть всю очередь\n");
        wprintf(L"5. Сортировать по длине имени\n");
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
            enqueue();
            break;
        case 2:
            dequeue();
            break;
        case 3:
            peek();
            break;
        case 4:
            display();
            break;
        case 5:
            sort_by_name_length();
            break;
        case 6:
            wprintf(L"Выход...\n");
            break;
        default:
            wprintf(L"Неверный выбор. Введите число от 1 до 6.\n");
        }
    } while (choice != 6);// Продолжаем пока не выбран выход

    //Очистка памяти - удаление всех элементов очереди
    while (front != NULL) {
        dequeue();
    }
    return 0;
}