#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

//Структура для узла списка смежности (узел-связь с 1 вершиной)
typedef struct Node {
    int vertex;// Номер вершины, с которой есть связь
    struct Node* next;// Указатель на следующий узел в списке
} Node;

//Структура для стека (нерекурсивный обход)
typedef struct {
    int* data;// Динамический массив для хранения элементов стека
    int top;// Индекс верхнего элемента
    int capacity;// Вместимость стека
} Stack;

int* visited;// Динамический массив для отслеживания посещенных вершин
Node** adjacencyList;// Динамический массив указателей на списки смежности
int** graph_matrix;// Динамическая матрица смежности
int num_vertices;// Количество вершин в графе

//Создание стека
Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->data = (int*)malloc(capacity * sizeof(int));
    stack->top = -1;// Стек пуст
    stack->capacity = capacity;// Сохраняем максимальную вместимость
    return stack;
}

//Освобождение памяти, занятой стеком
void freeStack(Stack* stack) {
    free(stack->data);
    free(stack);
}

//Добавление элемента в стек
void push(Stack* stack, int vertex) {
    //Проверяем, не переполнен ли стек
    if (stack->top < stack->capacity - 1) {
        stack->data[++stack->top] = vertex;// Увеличиваем top и записываем вершину в массив
    }
}

//Извлечение элемента из стека
int pop(Stack* stack) {
    //Проверяем, не пуст ли стек
    if (stack->top >= 0) {
        return stack->data[stack->top--];// Возвращаем верхний элемент и уменьшаем top
    }
    return -1;// Если стек пуст
}

//Проверка пустоты стека
int isEmpty(Stack* stack) {
    return stack->top == -1;
}

//Инициализация графа
void initializeGraph(int n) {
    num_vertices = n;// Сохраняем количество вершин
    //Выделение памяти для матрицы смежности
    graph_matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        graph_matrix[i] = (int*)malloc(n * sizeof(int));
        //Инициализируем все элементы матрицы нулями (нет связей)
        for (int j = 0; j < n; j++) {
            graph_matrix[i][j] = 0;
        }
    }

    //Выделение памяти для списков смежности
    //Создаем массив указателей на Node
    adjacencyList = (Node**)malloc(n * sizeof(Node*));
    for (int i = 0; i < n; i++) {
        adjacencyList[i] = NULL;// Пустые списки
    }
    //Выделение памяти для массива посещенных вершин
    visited = (int*)malloc(n * sizeof(int));
}

//Освобождение памяти, занятой графом
void freeGraph() {
    //Освобождение матрицы смежности
    if (graph_matrix != NULL) {
        for (int i = 0; i < num_vertices; i++) {
            free(graph_matrix[i]);
        }
        free(graph_matrix);
        graph_matrix = NULL;
    }

    //Освобождение списков смежности
    if (adjacencyList != NULL) {
        for (int i = 0; i < num_vertices; i++) {
            Node* current = adjacencyList[i];
            //Проходим по всему списку и освобождаем все узлы
            while (current != NULL) {
                Node* temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(adjacencyList);
        adjacencyList = NULL;
    }
    //Освобождение массива посещенных вершин
    if (visited != NULL) {
        free(visited);
        visited = NULL;
    }
}

//Функция добавления ребра в список см.
void addEdge(int src, int dest) {
    //Создаем новый узел для направления src -> dest
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = dest;// Указываем вершину назначения
    newNode->next = adjacencyList[src];// Вставляем новый узел в начало списка
    adjacencyList[src] = newNode;// Обновляем голову списка для вершины src

    //Для неориентированного графа добавляем обратное ребро dest -> src
    newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = src;// Указываем исходную вершину как назначение
    newNode->next = adjacencyList[dest];// Вставляем в начало списка для dest
    adjacencyList[dest] = newNode;// Обновляем голову списка для вершины dest
}

//Функция генерации графа
void generateRandomGraph() {
    //Очистка существующих данных матрицы смежности
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            graph_matrix[i][j] = 0;
        }
    }

    //Очистка существующих списков смежности
    for (int i = 0; i < num_vertices; i++) {
        Node* current = adjacencyList[i];
        //Удаляем все узлы в списке для вершины i
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
        adjacencyList[i] = NULL;
    }

    //Генерация случайных ребер
    for (int i = 0; i < num_vertices; i++) {
        for (int j = i + 1; j < num_vertices; j++) {
            //С вероятностью 50% создаем ребро
            if (rand() % 2 == 1) {
                graph_matrix[i][j] = 1;
                graph_matrix[j][i] = 1;
                addEdge(i, j);
            }
        }
    }
}

//Рекурсивная функция обхода в глубину (матрица) - прямой порядок
void DFS_matrix_recursive(int start_vertex) {
    visited[start_vertex] = 1;// Помечаем текущую вершину как посещенную
    printf("%d ", start_vertex + 1);// Выводим номер вершины (начиная с 1)
    //Проверяем все вершины на связь с текущей - прямой порядок
    for (int i = 0; i < num_vertices; i++) {
        //Если есть связь и вершина еще не посещена
        if (graph_matrix[start_vertex][i] == 1 && !visited[i]) {
            DFS_matrix_recursive(i);// Рекурсивно посещаем связанную вершину
        }
    }
}

//Нерекурсивный обход в глубину (матрица) - Прямой порядок
void DFS_matrix_nonRecursive(int start_vertex) {
    Stack* stack = createStack(num_vertices);// Создаем стек для хранения вершин
    int* visited_local = (int*)malloc(num_vertices * sizeof(int));// Локальный массив посещенных вершин
    //Инициализируем массив посещенных вершин
    for (int i = 0; i < num_vertices; i++) {
        visited_local[i] = 0;// Вершина не посещена
    }
    push(stack, start_vertex);// Добавляем стартовую вершину в стек
    visited_local[start_vertex] = 1;// Помечаем как посещенную
    printf("Порядок обхода в глубину (матрица, нерекурсивный): ");
    //Пока стек не пуст, продолжаем обход
    while (!isEmpty(stack)) {
        int current_vertex = pop(stack);// Извлекаем вершину из стека
        printf("%d ", current_vertex + 1);// Выводим номер вершины

        //Добавляем в стек все смежные непосещенные вершины
        //Прямой порядок (от 0 до n-1) для согласованности со списками
        for (int i = 0; i < num_vertices; i++) {
            if (graph_matrix[current_vertex][i] == 1 && !visited_local[i]) {
                push(stack, i);// Добавляем смежную вершину в стек
                visited_local[i] = 1;// Помечаем как посещенную
            }
        }
    }
    printf("\n");
    free(visited_local);
    freeStack(stack);
}

//Рекурсивная функция обхода в глубину (для списков смежности)
void DFS_adjacency_list_recursive(int vertex) {
    visited[vertex] = 1;// Помечаем текущую вершину как посещенную
    printf("%d ", vertex + 1);// Выводим номер вершины
    Node* temp = adjacencyList[vertex];// Получаем голову списка смежности для текущей вершины
    //Проходим по всем смежным вершинам
    while (temp != NULL) {
        int adjVertex = temp->vertex;// Получаем номер смежной вершины
        //Если смежная вершина еще не посещена
        if (!visited[adjVertex]) {
            DFS_adjacency_list_recursive(adjVertex);// Рекурсивно посещаем ее
        }
        temp = temp->next;// Переходим к следующей смежной вершине
    }
}

//Нерекурсивный обход в глубину для списков смежности
void DFS_adjacency_list_nonRecursive(int start_vertex) {
    Stack* stack = createStack(num_vertices);// Создаем стек для хранения вершин
    int* visited_local = (int*)malloc(num_vertices * sizeof(int));// Массив посещенных
    //Инициализируем массив посещенных вершин
    for (int i = 0; i < num_vertices; i++) {
        visited_local[i] = 0;// Вершина не посещена
    }
    push(stack, start_vertex);// Добавляем стартовую вершину в стек
    visited_local[start_vertex] = 1;// Помечаем как посещенную

    printf("Порядок обхода в глубину (списки смежности, нерекурсивный): ");

    //Пока стек не пуст, продолжаем обход
    while (!isEmpty(stack)) {
        int current_vertex = pop(stack);// Извлекаем вершину из стека
        printf("%d ", current_vertex + 1);// Выводим номер вершины

        //Добавляем смежные вершины в обратном порядке (чтобы соответствовать матрице)
        Stack* temp_stack = createStack(num_vertices);
        Node* temp = adjacencyList[current_vertex];// Получаем список смежных вершин

        //Сначала собираем все смежные вершины во временный стек
        while (temp != NULL) {
            int adjVertex = temp->vertex;// Получаем номер смежной вершины
            // Если вершина еще не посещена
            if (!visited_local[adjVertex]) {
                push(temp_stack, adjVertex);// Добавляем во временный стек
                visited_local[adjVertex] = 1;// Помечаем как посещенную
            }
            temp = temp->next;// Переходим к следующей смежной вершине
        }

        //Переносим из временного стека в основной стек в ПРЯМОМ порядке
        //Это обеспечит тот же порядок обхода, что и в матрице
        while (!isEmpty(temp_stack)) {
            push(stack, pop(temp_stack));
        }
        freeStack(temp_stack);
    }
    printf("\n");

    free(visited_local);// Освобождаем локальный массив посещенных вершин
    freeStack(stack);// Освобождаем основной стек
}

//Функция для вывода матрицы смежности
void printMatrix() {
    printf("\nМатрица смежности:\n");
    printf("   ");
    for (int i = 0; i < num_vertices; i++) {
        printf("%2d ", i + 1);
    }
    printf("\n");
    for (int i = 0; i < num_vertices; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < num_vertices; j++) {
            printf("%2d ", graph_matrix[i][j]);
        }
        printf("\n");
    }
}

//Функция вывода списков смежности
void printAdjacencyList() {
    printf("\nСписки смежности:\n");
    for (int i = 0; i < num_vertices; i++) {
        printf("Вершина %d: ", i + 1);
        Node* temp = adjacencyList[i];
        while (temp != NULL) {
            printf("%d ", temp->vertex + 1);
            temp = temp->next;
        }
        printf("\n");
    }
}

//Инициализации массива посещенных вершин
void initializeVisited() {
    for (int i = 0; i < num_vertices; i++) {
        visited[i] = 0;
    }
}

void displayMenu() {
    printf("1 - Пересоздать граф\n");
    printf("2 - Рекурсивный обход (матрица)\n");
    printf("3 - Нерекурсивный обход (матрица)\n");
    printf("4 - Рекурсивный обход (списки смежности)\n");
    printf("5 - Нерекурсивный обход (списки смежности)\n");
    printf("0 - Выход из программы\n");
    printf("Ваш выбор: ");
}

int main() {
    setlocale(LC_ALL, "ru");
    srand(time(NULL));
    int choice;
    int start_vertex;
    int graph_initialized = 0;
    while (1) {
        if (!graph_initialized) {
            printf("Введите количество вершин графа: ");
            scanf("%d", &num_vertices);
            if (num_vertices <= 0) {
                printf("Ошибка: количество вершин должно быть натуральным числом\n");
                continue;
            }
            initializeGraph(num_vertices);
            generateRandomGraph();
            graph_initialized = 1;
            printf("Граф с %d вершинами успешно создан!\n", num_vertices);
            printMatrix();
            printAdjacencyList();
        }
        displayMenu();
        scanf("%d", &choice);
        switch (choice) {
        case 0:
            //Выход из программы
            freeGraph();
            return 0;

        case 1:
            // Пересоздание графа
            freeGraph();
            graph_initialized = 0;
            break;

        case 2:
            //Рекурсивный обход в глубину (матрица)
            printf("Введите начальную вершину (1-%d): ", num_vertices);
            scanf("%d", &start_vertex);
            if (start_vertex < 1 || start_vertex > num_vertices) {
                printf("Ошибка: неверный номер вершины!\n");
                break;
            }
            initializeVisited();
            printf("Порядок обхода в глубину (матрица, рекурсивный): ");
            DFS_matrix_recursive(start_vertex - 1);
            printf("\n");
            break;

        case 3:
            // Нерекурсивный обход в глубину (матрица)
            printf("Введите начальную вершину (1-%d): ", num_vertices);
            scanf("%d", &start_vertex);
            if (start_vertex < 1 || start_vertex > num_vertices) {
                printf("Ошибка: неверный номер вершины!\n");
                break;
            }
            DFS_matrix_nonRecursive(start_vertex - 1);
            break;

        case 4:
            //Рекурсивный обход в глубину (списки смежности)
            printf("Введите начальную вершину (1-%d): ", num_vertices);
            scanf("%d", &start_vertex);
            if (start_vertex < 1 || start_vertex > num_vertices) {
                printf("Ошибка: неверный номер вершины!\n");
                break;
            }
            initializeVisited();
            printf("Порядок обхода в глубину (списки смежности, рекурсивный): ");
            DFS_adjacency_list_recursive(start_vertex - 1);
            printf("\n");
            break;

        case 5:
            //Нерекурсивный обход в глубину (списки смежности)
            printf("Введите начальную вершину (1-%d): ", num_vertices);
            scanf("%d", &start_vertex);
            if (start_vertex < 1 || start_vertex > num_vertices) {
                printf("Ошибка: неверный номер вершины!\n");
                break;
            }
            DFS_adjacency_list_nonRecursive(start_vertex - 1);
            break;

        default:
            // Некорректный выбор
            printf("Ошибка: неверный выбор! Пожалуйста, выберите от 0 до 5.\n");
            break;
        }
    }
    return 0;
}