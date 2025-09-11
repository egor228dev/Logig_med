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
    //Установка кодировки консоли для русского языка
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    int n;//Кол-во студентов
    printf("Введите количество студентов: ");
    scanf("%d", &n);

    //Очистка буфера ввода после scanf
    while (getchar() != '\n');

    //Выделение динамической памяти для массива студентов
    struct student* students = (struct student*)malloc(n * sizeof(struct student));
    if (students == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    //Заполнение данных о студентах
    for (int i = 0; i < n; i++) {
        printf("\nСтудент %d:\n", i + 1);

        //Ввод фамилии
        printf("Фамилия: ");
        fgets(students[i].famil, sizeof(students[i].famil), stdin);
        students[i].famil[strcspn(students[i].famil, "\n")] = '\0';//Удаление символа новой строки

        //Ввод имени
        printf("Имя: ");
        fgets(students[i].name, sizeof(students[i].name), stdin);
        students[i].name[strcspn(students[i].name, "\n")] = '\0';

        //Ввод факультета
        printf("Факультет: ");
        fgets(students[i].facult, sizeof(students[i].facult), stdin);
        students[i].facult[strcspn(students[i].facult, "\n")] = '\0';

        //Ввод номера зачетной книжки
        printf("Номер зачетной книжки: ");
        scanf("%d", &students[i].Nomzach);
        while (getchar() != '\n');//Очистка буфера ввода
    }

    int choice;
    while (1) {
        printf("\nВыберите действие:\n");
        printf("1 - Поиск по фамилии\n");
        printf("2 - Поиск по имени\n");
        printf("3 - Поиск по факультету\n");
        printf("4 - Поиск по номеру зачетной книжки\n");
        printf("5 - Выход из программы\n");
        printf("6 - Удалить студента\n");
        printf("7 - Добавить студента\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); //Очистка буфера ввода

        char search_str[20];//Переменная для строкового поиска
        int search_int;//Переменная для числового поиска
        int found = 0;//Флаг найденного студента
        char* field = NULL;//Указатель на поле для поиска

        //Обработка выбора пользователя
        switch (choice) {
        case 1: 
        case 2: 
        case 3:
            printf("Введите строку для поиска: ");
            fgets(search_str, sizeof(search_str), stdin);
            search_str[strcspn(search_str, "\n")] = '\0';
            //Поиск по всем студентам
            for (int i = 0; i < n; i++) {
                //Выбор поля для поиска в зависимости от выбора пользователя
                switch (choice) {
                case 1: field = students[i].famil; break;
                case 2: field = students[i].name; break;
                case 3: field = students[i].facult; break;
                }
                //Поиск подстроки в выбранном поле
                if (field != NULL && strstr(field, search_str) != NULL) {
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
            while (getchar() != '\n');
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
            free(students);//Освобождение памяти
            return 0;

        case 6:
            printf("Введите номер студента для удаления (1-%d): ", n);
            int index_to_remove;
            scanf("%d", &index_to_remove);
            while (getchar() != '\n');//Очистка буфера ввода

            //Проверка корректности введенного номера
            if (index_to_remove >= 1 && index_to_remove <= n) {
                //Сдвиг элементов массива для удаления студента
                for (int i = index_to_remove - 1; i < n - 1; i++) {
                    students[i] = students[i + 1];
                }
                n--;//Уменьшение количества студентов
                //Перераспределение памяти
                students = (struct student*)realloc(students, n * sizeof(struct student));
                printf("Студент удален.\n");
            }
            else {
                printf("Некорректный номер студента.\n");
            }
            break;

        case 7:
            n++;//Увеличение количества студентов
            // Перераспределение памяти для нового студента
            students = (struct student*)realloc(students, n * sizeof(struct student));
            if (students == NULL) {
                printf("Ошибка выделения памяти при добавлении студента!\n");
                return 1;
            }

            //Ввод данных нового студента
            printf("\nВвод данных нового студента:\n");
            printf("Фамилия: ");
            fgets(students[n - 1].famil, sizeof(students[n - 1].famil), stdin);
            students[n - 1].famil[strcspn(students[n - 1].famil, "\n")] = '\0';

            printf("Имя: ");
            fgets(students[n - 1].name, sizeof(students[n - 1].name), stdin);
            students[n - 1].name[strcspn(students[n - 1].name, "\n")] = '\0';

            printf("Факультет: ");
            fgets(students[n - 1].facult, sizeof(students[n - 1].facult), stdin);
            students[n - 1].facult[strcspn(students[n - 1].facult, "\n")] = '\0';

            printf("Номер зачетной книжки: ");
            scanf("%d", &students[n - 1].Nomzach);
            while (getchar() != '\n');//Очистка буфера ввода
            printf("Студент добавлен.\n");
            break;

        default:
            printf("Неверный выбор!\n");
        }
        if (!found && choice >= 1 && choice <= 4) {
            printf("Студент не найден!\n");
        }
    }

    free(students);//Освобождение памяти
    return 0;
}