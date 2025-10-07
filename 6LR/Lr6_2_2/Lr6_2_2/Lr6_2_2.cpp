#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

//Структура для узла списка смежности
typedef struct Node {
    int vertex;// Номер смежной вершины
    struct Node* next;// Указатель на следующий узел
} Node;

//Структура для списка смежности графа
typedef struct {
    int vertices;// Количество вершин
    Node** adjLists;// Массив списков смежности
} Graph;

//Функция для создания нового узла
Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// Функция для создания графа
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->vertices = vertices;

    // Выделяем память для массива списков смежности
    graph->adjLists = (Node**)malloc(vertices * sizeof(Node*));

    // Инициализируем каждый список как пустой
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }

    return graph;
}

//Функция для добавления ребра в неориентированный граф
void addEdge(Graph* graph, int src, int dest) {
    //Добавляем ребро из src в dest (исходная -> целевая)
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    //Добавляем ребро из dest в src (граф неориентированный)
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

//Функция для генерации случайного графа
void generateRandomGraph(Graph* graph, int vertices) {
    //Генерируем случайные ребра
    for (int i = 0; i < vertices; i++) {
        for (int j = i + 1; j < vertices; j++) {
            if (rand() % 2 == 1) {// С вероятностью 50% добавляем ребро
                addEdge(graph, i, j);
            }
        }
    }
}

//Функция для вывода списков смежности
void printAdjacencyList(Graph* graph) {
    printf("Списки смежности:\n");
    for (int i = 0; i < graph->vertices; i++) {
        printf("Вершина %d: ", i + 1);
        Node* temp = graph->adjLists[i];
        if (temp == NULL) {
            printf("нет соседей");
        }
        else {
            while (temp) {
                printf("%d ", temp->vertex + 1);//нумерация с 1
                temp = temp->next;
            }
        }
        printf("\n");
    }
    printf("\n");
}

//Функция для копирования графа
Graph* copyGraph(Graph* graph) {
    Graph* copy = createGraph(graph->vertices);
    //Копируем каждый список смежности
    for (int i = 0; i < graph->vertices; i++) {
        Node* temp = graph->adjLists[i];
        Node** copyPtr = &(copy->adjLists[i]);// Указатель на текущий конец списка

        while (temp) {
            *copyPtr = createNode(temp->vertex);// Создаем копию узла
            copyPtr = &((*copyPtr)->next);// Перемещаем указатель на следующий элемент
            temp = temp->next;
        }
    }

    return copy;
}

//Функция для удаления вершины из списка смежности
void removeVertexFromList(Node** head, int v) {
    Node* temp = *head;
    Node* prev = NULL;
    //Ищем вершину в списке
    while (temp != NULL && temp->vertex != v) {
        prev = temp;
        temp = temp->next;
    }
    //Если вершина найдена, удаляем её
    if (temp != NULL) {
        if (prev == NULL) {
            *head = temp->next;// Удаляем первый элемент
        }
        else {
            prev->next = temp->next;// Удаляем из середины или конца
        }
        free(temp);
    }
}

//Функция для проверки наличия вершины в списке
int isVertexInList(Node* head, int v) {
    Node* temp = head;
    while (temp != NULL) {
        if (temp->vertex == v) {
            return 1;// Вершина найдена
        }
        temp = temp->next;
    }
    return 0;// Вершина не найдена
}

//Функция для освобождения памяти графа
void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->vertices; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            Node* toDelete = temp;
            temp = temp->next;
            free(toDelete);// Освобождаем каждый узел списка
        }
    }
    free(graph->adjLists);// Освобождаем массив указателей
    free(graph);// Освобождаем структуру графа
}

// а) Операция отождествления вершин
Graph* identifyVerticesList(Graph* graph, int v1, int v2) {
    //Проверка корректности входных данных
    if (v1 < 1 || v1 > graph->vertices || v2 < 1 || v2 > graph->vertices || v1 == v2) {
        printf("Ошибка: некорректные номера вершин!\n");
        return copyGraph(graph);// Возвращаем копию исходного графа
    }
    //Преобразуем номера вершин в индексы массива (начинаются с 0)
    int idx1 = v1 - 1;
    int idx2 = v2 - 1;
    //Определяем какую вершину оставляем, какую удаляем
    int keep_idx = (idx1 < idx2) ? idx1 : idx2;
    int remove_idx = (idx1 < idx2) ? idx2 : idx1;
    //Создаем новый граф с уменьшенным количеством вершин
    Graph* newGraph = createGraph(graph->vertices - 1);
    //Сначала собираем всех соседей объединенной вершины
    Node* combinedNeighbors = NULL;
    //Добавляем соседей вершины keep_idx
    Node* temp = graph->adjLists[keep_idx];
    while (temp != NULL) {
        //Исключаем петли и ссылки на удаляемую вершину
        if (temp->vertex != keep_idx && temp->vertex != remove_idx) {
            Node* newNode = createNode(temp->vertex);
            newNode->next = combinedNeighbors;
            combinedNeighbors = newNode;// Добавляем в начало списка
        }
        temp = temp->next;
    }

    //Добавляем соседей вершины remove_idx
    temp = graph->adjLists[remove_idx];
    while (temp != NULL) {
        if (temp->vertex != keep_idx && temp->vertex != remove_idx) {
            //Проверяем, нет ли уже такой вершины в списке
            if (!isVertexInList(combinedNeighbors, temp->vertex)) {
                Node* newNode = createNode(temp->vertex);
                newNode->next = combinedNeighbors;
                combinedNeighbors = newNode;
            }
        }
        temp = temp->next;
    }

    //Заполняем новый граф
    for (int i = 0; i < graph->vertices; i++) {
        if (i == remove_idx) continue;// Пропускаем удаляемую вершину
        int new_i = (i < remove_idx) ? i : i - 1;// Корректируем индекс
        Node* currentList = NULL;
        if (i == keep_idx) {
            //Для объединенной вершины используем собранный список соседей
            currentList = combinedNeighbors;
        }
        else {
            //Для остальных вершин копируем список, заменяя индексы
            temp = graph->adjLists[i];
            while (temp != NULL) {
                int old_j = temp->vertex;
                int new_j;
                //Заменяем ссылки на удаляемую вершину ссылками на оставляемую
                if (old_j == remove_idx) {
                    new_j = keep_idx;
                }
                else {
                    new_j = (old_j < remove_idx) ? old_j : old_j - 1;
                }
                //Добавляем ребро если это не петля и ребро еще не существует
                if (new_j != new_i && !isVertexInList(currentList, new_j)) {
                    Node* newNode = createNode(new_j);
                    newNode->next = currentList;
                    currentList = newNode;
                }
                temp = temp->next;
            }
        }
        newGraph->adjLists[new_i] = currentList;
    }
    return newGraph;
}

// б) Операция стягивания ребра
Graph* contractEdgeList(Graph* graph, int v1, int v2) {
    //Проверка корректности входных данных
    if (v1 < 1 || v1 > graph->vertices || v2 < 1 || v2 > graph->vertices || v1 == v2) {
        printf("Ошибка: некорректные номера вершин!\n");
        return copyGraph(graph);
    }
    int idx1 = v1 - 1;
    int idx2 = v2 - 1;
    //Проверяем существование ребра между вершинами
    if (!isVertexInList(graph->adjLists[idx1], idx2)) {
        printf("Ошибка: ребро между вершинами %d и %d не существует!\n", v1, v2);
        return copyGraph(graph);
    }
    //Стягивание ребра - это отождествление вершин + удаление петли
    Graph* result = identifyVerticesList(graph, v1, v2);
    //Удаляем петлю из объединенной вершины (если она есть)
    int keep_idx = (idx1 < idx2) ? idx1 : idx1 - 1;// Учитываем смещение индексов
    removeVertexFromList(&(result->adjLists[keep_idx]), keep_idx);
    return result;
}

// в) Операция расщепления вершины
Graph* splitVertexList(Graph* graph, int v) {
    // Проверка корректности введенной вершины
    if (v < 1 || v > graph->vertices) {
        printf("Ошибка: некорректный номер вершины!\n");
        return copyGraph(graph);
    }
    int idx = v - 1;// Преобразуем номер вершины в индекс
    //Создаем новый граф с +1 вершиной
    Graph* newGraph = createGraph(graph->vertices + 1);
    int newVertexIdx = graph->vertices;// Индекс новой вершины (последний)
    //Копируем все исходные списки смежности без изменений
    for (int i = 0; i < graph->vertices; i++) {
        Node* temp = graph->adjLists[i];
        while (temp != NULL) {
            int old_j = temp->vertex;
            //Копируем ребро без изменений
            if (!isVertexInList(newGraph->adjLists[i], old_j) && i != old_j) {
                Node* newNode = createNode(old_j);
                newNode->next = newGraph->adjLists[i];
                newGraph->adjLists[i] = newNode;
            }
            temp = temp->next;
        }
    }

    //Соединяем исходную вершину и новую вершину (ребро между ними)
    if (!isVertexInList(newGraph->adjLists[idx], newVertexIdx)) {
        Node* newNode1 = createNode(newVertexIdx);
        newNode1->next = newGraph->adjLists[idx];
        newGraph->adjLists[idx] = newNode1;

        Node* newNode2 = createNode(idx);
        newNode2->next = newGraph->adjLists[newVertexIdx];
        newGraph->adjLists[newVertexIdx] = newNode2;
    }

    return newGraph;
}

int main() {
    setlocale(LC_ALL, "ru");
    srand(time(NULL));// Инициализация генератора случайных чисел
    int vertices;
    printf("Введите количество вершин для графа: ");
    scanf("%d", &vertices);
    if (vertices <= 0) {
        printf("Ошибка: количество вершин должно быть положительным числом.\n");
        return 1;
    }

    //Создаем и генерируем граф
    Graph* graph = createGraph(vertices);
    generateRandomGraph(graph, vertices);
    printf("\nИсходный граф\n");
    printAdjacencyList(graph);
    int choice;
    int v1, v2, v;
    //Основной цикл меню
    do {
        printf("\nОперации над графом\n");
        printf("1 - Отождествление вершин\n");
        printf("2 - Стягивание ребра\n");
        printf("3 - Расщепление вершины\n");
        printf("0 - Выход\n");
        printf("Выберите операцию: ");
        scanf("%d", &choice);

        Graph* result = NULL;

        switch (choice) {
        case 1: {
            printf("Введите номера вершин для отождествления (через пробел): ");
            scanf("%d %d", &v1, &v2);
            result = identifyVerticesList(graph, v1, v2);
            printf("\nРезультат отождествления вершин %d и %d:\n", v1, v2);
            printAdjacencyList(result);
            break;
        }
        case 2: {
            printf("Введите номера вершин ребра для стягивания (через пробел): ");
            scanf("%d %d", &v1, &v2);
            result = contractEdgeList(graph, v1, v2);
            printf("\nРезультат стягивания ребра (%d, %d):\n", v1, v2);
            printAdjacencyList(result);
            break;
        }
        case 3: {
            printf("Введите номер вершины для расщепления: ");
            scanf("%d", &v);
            result = splitVertexList(graph, v);
            printf("\nРезультат расщепления вершины %d:\n", v);
            printAdjacencyList(result);
            break;
        }
        case 0:
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор!\n");
        }

        if (result != NULL) {
            freeGraph(result);// Освобождаем память результата операции
        }

    } while (choice != 0);

    //Освобождение памяти исходного графа
    freeGraph(graph);
    return 0;
}