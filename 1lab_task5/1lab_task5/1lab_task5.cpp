#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

//Структура student
struct student {
    char famil[20];
    char name[20];
    char facult[20];
    int Nomzach;
};

int main() {
    //Установка кодировки для Windows
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    int n;//Кол-во студентов в б.д.
    printf("Введите количество студентов: ");
    scanf("%d", &n);

    //Очистка буфера после scanf
    while (getchar() != '\n');

    //Выделение памяти для массива структур
    struct student* students = (struct student*)malloc(n * sizeof(struct student));
    if (students == NULL) {
        printf("Некорректный ввод!\n");
        return 1;
    }

    //Ввод данных студентов
    for (int i = 0; i < n; i++) {
        printf("\nСтудент %d:\n", i + 1);

        printf("Фамилия: ");
        fgets(students[i].famil, sizeof(students[i].famil), stdin);
        // Удаляем символ новой строки
        students[i].famil[strcspn(students[i].famil, "\n")] = '\0';

        printf("Имя: ");
        fgets(students[i].name, sizeof(students[i].name), stdin);
        students[i].name[strcspn(students[i].name, "\n")] = '\0';

        printf("Факультет: ");
        fgets(students[i].facult, sizeof(students[i].facult), stdin);
        students[i].facult[strcspn(students[i].facult, "\n")] = '\0';

        printf("Номер зачетной книжки: ");
        scanf("%d", &students[i].Nomzach);

        // Очистка буфера после scanf
        while (getchar() != '\n');
    }

    //Поиск студента
    int choice;
    printf("\nПоиск студента по:\n");
    printf("1 - Фамилия\n");
    printf("2 - Имя\n");
    printf("3 - Факультет\n");
    printf("4 - Номер зачетной книжки\n");
    printf("5 - Выход из программы\n");
    printf("Выберите критерий поиска: ");
    scanf("%d", &choice);

    //Очистка буфера после scanf
    while (getchar() != '\n');

    char search_str[20];//Фамилия Имя Факультет (19 символов + нулевой терминатор)
    int search_int;//Номер зачетной книжки
    int found = 0;//Индикатор(Найден/Ненайден)

    switch (choice) {
    case 1:
        printf("Введите фамилию: ");
        fgets(search_str, sizeof(search_str), stdin);
        search_str[strcspn(search_str, "\n")] = '\0';

        for (int i = 0; i < n; i++) {
            if (strcmp(students[i].famil, search_str) == 0) {
                printf("Найден: %s %s, факультет %s, номер %d\n",
                    students[i].famil, students[i].name,
                    students[i].facult, students[i].Nomzach);
                found = 1;
            }
        }
        break;

    case 2:
        printf("Введите имя: ");
        fgets(search_str, sizeof(search_str), stdin);
        search_str[strcspn(search_str, "\n")] = '\0';

        for (int i = 0; i < n; i++) {
            if (strcmp(students[i].name, search_str) == 0) {
                printf("Найден: %s %s, факультет %s, номер %d\n",
                    students[i].famil, students[i].name,
                    students[i].facult, students[i].Nomzach);
                found = 1;
            }
        }
        break;

    case 3:
        printf("Введите факультет: ");
        fgets(search_str, sizeof(search_str), stdin);
        search_str[strcspn(search_str, "\n")] = '\0';

        for (int i = 0; i < n; i++) {
            if (strcmp(students[i].facult, search_str) == 0) {
                printf("Найден: %s %s, факультет %s, номер %d\n",
                    students[i].famil, students[i].name,
                    students[i].facult, students[i].Nomzach);
                found = 1;
            }
        }
        break;

    case 4:
        printf("Введите номер зачетной книжки: ");
        scanf("%d", &search_int);

        for (int i = 0; i < n; i++) {
            if (students[i].Nomzach == search_int) {
                printf("Найден: %s %s, факультет %s, номер %d\n",
                    students[i].famil, students[i].name,
                    students[i].facult, students[i].Nomzach);
                found = 1;
            }
        }
        break;

    case 5:
        printf("Выход из программы...\n");
        free(students);
        return 0;//Завершение программы

    default:
        printf("Неверный выбор!\n");
    }
    //Проверка наличия студента в б.д.
    if (!found && choice >= 1 && choice <= 4) {
        printf("Студент не найден!\n");
    }

    //Освобождение памяти
    free(students);
    return 0;
}