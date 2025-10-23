#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <string>
#include <cstring>
#include <functional>
using namespace std;

//Класс для работы со взвешенными графами
class WeightedGraph {
private:
    vector<vector<int>> adjacencyMatrix;
    int vertices;// Количество вершин в графе
    bool isDirected;// Флаг
    //Поиск кратчайших расстояний от начальной вершины до всех остальных
    vector<int> dijkstraDistances(int startVertex) {
        if (startVertex < 0 || startVertex >= vertices) {
            return vector<int>(vertices, -1);
        }
        //Инициализация массива расстояний (изначально все расстояния бесконечны)
        vector<int> dist(vertices, numeric_limits<int>::max());
        //Массив для отслеживания посещенных вершин
        vector<bool> visited(vertices, false);
        //Приоритетная очередь для хранения пар (расстояние, вершина)
        //greater обеспечивает минимальную кучу - наименьшее расстояние-первое
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        dist[startVertex] = 0;
        pq.push({ 0, startVertex });
        while (!pq.empty()) {
            //Извлекаем вершину с наименьшим расстоянием из очереди
            int current = pq.top().second;
            pq.pop();
            //Если вершина уже обработана, пропускаем ее
            if (visited[current]) continue;
            //Помечаем вершину как посещенную
            visited[current] = true;
            //Обходим всех соседей текущей вершины
            for (int i = 0; i < vertices; i++) {
                //Проверяем: есть ли ребро и не посещена ли соседняя вершина
                if (adjacencyMatrix[current][i] > 0 && !visited[i]) {
                    //Вычисляем новое расстояние через текущую вершину
                    int newDist = dist[current] + adjacencyMatrix[current][i];
                    //Если найден более короткий путь до соседней вершины
                    if (newDist < dist[i]) {
                        //Обновляем расстояние
                        dist[i] = newDist;
                        //Добавляем обновленную вершину в очередь
                        pq.push({ newDist, i });
                    }
                }
            }
        }
        //Заменяем непосещенные вершины на -1
        for (int i = 0; i < vertices; i++) {
            if (dist[i] == numeric_limits<int>::max()) {
                dist[i] = -1;
            }
        }
        return dist;
    }

public:
    //Конструктор: создает граф с n вершинами
    WeightedGraph(int n, bool directed = false) : vertices(n), isDirected(directed) {
        generateRandomGraph(); // Генерируем случайный граф
    }
    //Генерация случайного взвешенного графа
    void generateRandomGraph() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> weightDis(1, 10);// Распределение весов 1-10
        uniform_int_distribution<> edgeDis(0, 2);
        //Инициализируем матрицу смежности нулями
        adjacencyMatrix.clear();
        adjacencyMatrix.resize(vertices, vector<int>(vertices, 0));
        //Заполняем матрицу смежности
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (i == j) continue;
                if (!isDirected && i < j) {
                    //Для неориентированного графа обрабатываем только верхний треугольник
                    if (edgeDis(gen) > 0) {
                        int weight = weightDis(gen);// Генерируем случайный вес
                        adjacencyMatrix[i][j] = weight;// Устанавливаем вес для ребра i->j
                        adjacencyMatrix[j][i] = weight;// И для j->i (симметрично)
                    }
                }
                else if (isDirected) {
                    //Для ориентированного графа обрабатываем все пары вершин
                    if (edgeDis(gen) > 0) {
                        adjacencyMatrix[i][j] = weightDis(gen);// Устанавливаем вес для ребра i->j
                    }
                }
            }
        }
    }
    //Вывод матрицы смежности на экран
    void printAdjacencyMatrix() {
        cout << "\nМатрица смежности " << (isDirected ? "ориентированного" : "неориентированного") << " графа:\n";
        cout << "   ";
        for (int i = 0; i < vertices; i++) {
            cout << setw(4) << i;
        }
        cout << "\n";
        for (int i = 0; i < vertices; i++) {
            cout << setw(3) << i;
            for (int j = 0; j < vertices; j++) {
                cout << setw(4) << adjacencyMatrix[i][j];
            }
            cout << "\n";
        }
    }
    //Поиск и вывод расстояний от заданной вершины до всех остальных
    void findDistances(int startVertex) {
        vector<int> dist = dijkstraDistances(startVertex);// Получаем расстояния через BFS
        cout << "Расстояния от вершины " << startVertex << ":\n";
        for (int i = 0; i < vertices; i++) {
            cout << "Вершина " << i << ": ";
            if (dist[i] == -1) {
                cout << "недостижима";
            }
            else {
                cout << dist[i];
            }
            cout << "\n";
        }
    }
    //Вычисление эксцентриситета вершины (максимальное расстояние до других вершин)
    int calculateEccentricity(int vertex) {
        if (vertex < 0 || vertex >= vertices) {
            return -1;
        }

        vector<int> dist = dijkstraDistances(vertex);// Получаем расстояния от вершины
        int eccentricity = -1;// Начальное значение эксцентриситета
        //Находим максимальное расстояние среди достижимых вершин
        for (int i = 0; i < vertices; i++) {
            if (dist[i] != -1 && dist[i] > eccentricity) {
                eccentricity = dist[i];
            }
        }
        return eccentricity;
    }
    //Полный анализ графа: радиус, диаметр, центральные и периферийные вершины
    void analyzeGraph() {
        vector<int> eccentricities(vertices);// Вектор для хранения эксцентриситетов всех вершин
        int radius = numeric_limits<int>::max();// Радиус графа (минимальный эксцентриситет)
        int diameter = -1;// Диаметр графа (максимальный эксцентриситет)
        //Вычисляем эксцентриситеты всех вершин
        for (int i = 0; i < vertices; i++) {
            eccentricities[i] = calculateEccentricity(i);
            if (eccentricities[i] != -1) {
                radius = min(radius, eccentricities[i]);// Обновляем радиус
                diameter = max(diameter, eccentricities[i]);// Обновляем диаметр
            }
        }
        //Находим центральные и периферийные вершины
        vector<int> centralVertices, peripheralVertices;
        for (int i = 0; i < vertices; i++) {
            if (eccentricities[i] == radius) {
                centralVertices.push_back(i);// Вершины с эксцентриситетом = радиусу
            }
            if (eccentricities[i] == diameter) {
                peripheralVertices.push_back(i);// Вершины с эксцентриситетом = диаметру
            }
        }
        //Вывод результатов анализа
        cout << "Эксцентриситеты вершин:\n";
        for (int i = 0; i < vertices; i++) {
            cout << "Вершина " << i << ": " << eccentricities[i] << "\n";
        }
        cout << "\nРадиус графа: " << radius << endl;
        cout << "Диаметр графа: " << diameter << endl;
        cout << "Центральные вершины: ";
        for (int v : centralVertices) cout << v << " ";
        cout << endl;
        cout << "Периферийные вершины: ";
        for (int v : peripheralVertices) cout << v << " ";
        cout << endl;
    }
    //Геттеры для доступа к приватным полям
    int getVerticesCount() const { return vertices; }
    bool getIsDirected() const { return isDirected; }
    //Сеттеры для изменения параметров графа
    void setVertices(int n) { vertices = n; generateRandomGraph(); }
    void setDirected(bool directed) { isDirected = directed; generateRandomGraph(); }
};

//Обработка параметров командной строки
void processCommandLine(int argc, char* argv[], int& vertices, bool& isDirected, bool& doAnalysis, int& startVertex, bool& interactiveMode) {
    // Устанавливаем значения по умолчанию
    vertices = 6;
    isDirected = false;
    doAnalysis = false;
    startVertex = -1;
    interactiveMode = true;
    //Если нет параметров - используем интерактивный режим
    if (argc == 1) {
        return;
    }
    //Обрабатываем переданные параметры
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 && i + 1 < argc) {
            vertices = atoi(argv[i + 1]);// Количество вершин
            i++;// Пропускаем следующий аргумент (значение)
        }
        else if (strcmp(argv[i], "-d") == 0) {
            isDirected = true;// Ориентированный граф
        }
        else if (strcmp(argv[i], "-u") == 0) {
            isDirected = false;// Неориентированный граф
        }
        else if (strcmp(argv[i], "-a") == 0) {
            doAnalysis = true;// Выполнить анализ
            interactiveMode = false;// Переходим в неинтерактивный режим
        }
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            startVertex = atoi(argv[i + 1]);// Стартовая вершина
            interactiveMode = false;// Переходим в неинтерактивный режим
            i++;// Пропускаем следующий аргумент (значение)
        }
    }
    //Если указаны параметры для неинтерактивного режима
    if (doAnalysis || startVertex != -1) {
        interactiveMode = false;
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    //Переменные для хранения параметров
    int vertices;
    bool isDirected;
    bool doAnalysis;
    int startVertex;
    bool interactiveMode;
    //Обрабатываем параметры командной строки
    processCommandLine(argc, argv, vertices, isDirected, doAnalysis, startVertex, interactiveMode);
    //Проверяем корректность количества вершин
    if (vertices <= 0) {
        cout << "Неверное количество вершин! Установлено значение по умолчанию: 6" << endl;
        vertices = 6;
    }
    //Создаем граф с заданными параметрами
    WeightedGraph graph(vertices, isDirected);
    //Неинтерактивный режим (если указаны параметры командной строки)
    if (!interactiveMode) {
        graph.printAdjacencyMatrix();// Всегда выводим матрицу смежности
        //Если запрошен анализ графа
        if (doAnalysis) {
            graph.analyzeGraph();
        }
        //Если указана стартовая вершина для поиска расстояний
        if (startVertex != -1) {
            if (startVertex >= 0 && startVertex < graph.getVerticesCount()) {
                graph.findDistances(startVertex);
            }
            else {
                cout << "Неверная стартовая вершина: " << startVertex << endl;
            }
        }
        return 0;
    }
    //Интерактивный режим
    int choice;
    int newVertices;
    do {
        cout << "\n1. Неориентированный взвешенный граф";
        cout << "\n2. Ориентированный взвешенный граф";
        cout << "\n3. Анализ графа";
        cout << "\n4. Изменить количество вершин";
        cout << "\n0. Выход";
        cout << "\nВыберите операцию: ";
        cin >> choice;
        switch (choice) {
        case 1:
            if (graph.getIsDirected()) {
                graph.setDirected(false);// Устанавливаем неориентированный граф только если нужно
            }
            graph.printAdjacencyMatrix();// Выводим матрицу
            cout << "Введите стартовую вершину для поиска расстояний: ";
            cin >> startVertex;
            if (startVertex >= 0 && startVertex < graph.getVerticesCount()) {
                graph.findDistances(startVertex);// Ищем расстояния
            }
            else {
                cout << "Неверная вершина! Допустимый диапазон: 0-" << graph.getVerticesCount() - 1 << endl;
            }
            break;
        case 2:
            if (!graph.getIsDirected()) {
                graph.setDirected(true);// Устанавливаем ориентированный граф только если нужно
            }
            graph.printAdjacencyMatrix();// Выводим матрицу
            cout << "Введите стартовую вершину для поиска расстояний: ";
            cin >> startVertex;
            if (startVertex >= 0 && startVertex < graph.getVerticesCount()) {
                graph.findDistances(startVertex);// Ищем расстояния
            }
            else {
                cout << "Неверная вершина! Допустимый диапазон: 0-" << graph.getVerticesCount() - 1 << endl;
            }
            break;
        case 3:
            graph.printAdjacencyMatrix();// Выводим матрицу
            graph.analyzeGraph();// Выполняем анализ графа
            break;
        case 4:
            cout << "Введите новое количество вершин: ";
            cin >> newVertices;
            if (newVertices > 0) {
                graph.setVertices(newVertices);// Изменяем количество вершин
                graph.printAdjacencyMatrix();// Выводим новую матрицу
            }
            else {
                cout << "Неверное количество вершин!" << endl;
            }
            break;
        case 0:
            break;
        default:
            cout << "Неверный выбор!" << endl;
        }
    } while (choice != 0);
    return 0;
}