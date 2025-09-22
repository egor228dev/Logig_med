#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>
#include <wchar.h>
#include <ctype.h>

//Структура элемента приоритетной очереди
struct node {
    wchar_t inf[256];// Название объекта
    int priority;// Приоритет элемента
    struct node* next;// Указатель на следующий элемент
};

struct node* head = NULL;// Указатель на голову очереди

//Функция очистки буфера ввода
void clear_input_buffer() {
    int c;
    //Читаем символы из буфера до конца строки
    while ((c = getchar()) != '\n' && c != EOF);
}

//Функция проверки, является ли строка числом
int is_number(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;// Пустая строка - не число
    }
    char* endptr;
    //Пытаемся преобразовать строку в число
    strtol(str, &endptr, 10);
    //Если endptr указывает на конец строки - это число
    return *endptr == '\0';
}

//Функция проверки существования приоритета
int priority_exists(int priority) {
    struct node* current = head;
    while (current != NULL) {
        if (current->priority == priority) {
            return 1;// Приоритет уже существует
        }
        current = current->next;
    }
    return 0;// Приоритет не найден
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
    wprintf(L"Введите название объекта: ");
    clear_input_buffer();// Очищаем буфер перед чтением строки
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

    //Ввод приоритета с проверкой корректности
    char input[256];
    int valid_input = 0;
    int priority = 0;
    while (!valid_input) {
        wprintf(L"Введите приоритет: ");
        if (scanf("%255s", input) != 1) {
            wprintf(L"Ошибка ввода.\n");
            clear_input_buffer();
            continue;
        }

        //Проверка, что введено число
        if (!is_number(input)) {
            wprintf(L"Ошибка: введите целое число\n");
            clear_input_buffer();
            continue;
        }

        priority = atoi(input);// Преобразуем строку в число
        //Проверка, что число положительное
        if (priority <= 0) {
            wprintf(L"Ошибка: приоритет должен быть положительным числом.\n");
            clear_input_buffer();
            continue;
        }
        //Проверка, что приоритет не существует
        if (priority_exists(priority)) {
            wprintf(L"Ошибка: приоритет %d уже существует. Введите уникальный приоритет.\n", priority);
            clear_input_buffer();
            continue;
        }
        valid_input = 1;// Ввод корректен, выходим из цикла
    }
    p->priority = priority;// Устанавливаем приоритет
    p->next = NULL;// Следующий элемент пока отсутствует
    return p;
}

//Добавление элемента в соответствии с приоритетом
void spstore(void) {
    struct node* p = get_struct();
    if (p == NULL) return;
    if (head == NULL) {
        //Если список пуст - новый элемент становится головой
        head = p;
        return;
    }
    if (p->priority > head->priority) {
        //Если новый элемент имеет высший приоритет чем голова, вставляем его в начало списка
        p->next = head;
        head = p;
        return;
    }

    //Ищем место для вставки в соответствии с приоритетом
    struct node* current = head;
    while (current->next != NULL && current->next->priority >= p->priority) {
        current = current->next;
    }

    //Вставляем элемент на найденную позицию
    p->next = current->next;
    current->next = p;
}

//Просмотр содержимого приоритетной очереди
void review(void) {
    struct node* current = head;
    if (current == NULL) {
        wprintf(L"Список пуст\n");
        return;
    }
    wprintf(L"Содержимое приоритетной очереди:\n");
    int index = 1;
    //Проходим по всем элементам списка и выводим их с номерами
    while (current != NULL) {
        wprintf(L"%d. Имя: %ls, Приоритет: %d\n", index, current->inf, current->priority);
        current = current->next;
        index++;
    }
}

//Удаление элемента с наивысшим приоритетом (из начала очереди)
void delete_highest_priority(void) {
    if (head == NULL) {
        wprintf(L"Очередь пуста\n");
        return;
    }
    //Сохраняем указатель на удаляемый элемент
    struct node* temp = head;
    //Перемещаем голову на следующий элемент
    head = head->next;
    wprintf(L"Удален элемент: %ls (приоритет: %d)\n", temp->inf, temp->priority);
    free(temp);// Освобождаем память удаляемого элемента
}

//Функция удаления элемента по номеру (позиции)
void delete_by_position(void) {
    if (head == NULL) {
        wprintf(L"Очередь пуста\n");
        return;
    }
    //Сначала показываем текущее состояние очереди с номерами
    wprintf(L"Текущая очередь:\n");
    review();
    char input[256];
    int position = 0;
    int valid_input = 0;
    while (!valid_input) {
        wprintf(L"Введите номер элемента для удаления: ");
        if (scanf("%255s", input) != 1) {
            wprintf(L"Ошибка ввода.\n");
            clear_input_buffer();
            continue;
        }

        //Проверка, что введено число
        if (!is_number(input)) {
            wprintf(L"Ошибка: введите целое число\n");
            clear_input_buffer();
            continue;
        }
        position = atoi(input);
        //Проверка, что номер положительный
        if (position <= 0) {
            wprintf(L"Ошибка: номер должен быть положительным числом.\n");
            clear_input_buffer();
            continue;
        }

        valid_input = 1;
    }
    //Если удаляем первый элемент
    if (position == 1) {
        struct node* temp = head;
        head = head->next;
        wprintf(L"Удален элемент: %ls (приоритет: %d)\n", temp->inf, temp->priority);
        free(temp);
        return;
    }
    //Ищем элемент перед удаляемым
    struct node* current = head;
    int current_position = 1;
    while (current != NULL && current_position < position - 1) {
        current = current->next;
        current_position++;
    }

    //Проверяем, существует ли элемент для удаления
    if (current == NULL || current->next == NULL) {
        wprintf(L"Элемент с номером %d не найден.\n", position);
        return;
    }

    //Удаляем элемент
    struct node* temp = current->next;
    current->next = temp->next;
    wprintf(L"Удален элемент: %ls (приоритет: %d)\n", temp->inf, temp->priority);
    free(temp);
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    SetConsoleOutputCP(CP_UTF8);// Кодировка вывода
    SetConsoleCP(CP_UTF8);// Кодировка ввода
    int choice;
    char input[256];
    wprintf(L"Реализация приоритетной очереди\n");

    do {
        wprintf(L"\nМеню:\n");
        wprintf(L"1. Добавить элемент\n");
        wprintf(L"2. Просмотреть очередь\n");
        wprintf(L"3. Удалить элемент с высшим приоритетом\n");
        wprintf(L"4. Удалить элемент по номеру\n");
        wprintf(L"5. Выход\n");
        wprintf(L"Выберите действие: ");

        //Чтение выбора пользователя
        if (scanf("%255s", input) != 1) {
            clear_input_buffer();
            wprintf(L"Неверный ввод. Попробуйте снова.\n");
            continue;
        }

        //Проверка, что введено число
        if (!is_number(input)) {
            wprintf(L"Ошибка: введите число от 1 до 5. Попробуйте снова.\n");
            clear_input_buffer();
            continue;
        }

        choice = atoi(input);// Преобразуем ввод в число
        switch (choice) {
        case 1:
            spstore();
            break;
        case 2:
            review();
            break;
        case 3:
            delete_highest_priority();
            break;
        case 4:
            delete_by_position();
            break;
        case 5:
            wprintf(L"Выход...\n");
            break;
        default:
            wprintf(L"Неверный выбор. Введите число от 1 до 5.\n");
        }
    } while (choice != 5);// Продолжаем пока не выбран выход
    //Очистка памяти - удаление всех элементов очереди
    while (head != NULL) {
        delete_highest_priority();
    }
    return 0;
}