#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <iomanip>
#include <limits>
#include <algorithm>
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
    //Конструктор - создает граф с заданным количеством вершин
    WeightedGraph(int n, bool directed = false) : vertices(n), isDirected(directed) {
        generateRandomGraph();
    }
    //Генерация случайного графа
    void generateRandomGraph() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> weightDis(1, 10);
        uniform_int_distribution<> edgeDis(0, 2);
        //Полностью пересоздаем матрицу с новым размером
        adjacencyMatrix.clear();// Очищаем старую матрицу
        adjacencyMatrix.resize(vertices, vector<int>(vertices, 0));
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (i == j) continue;
                if (!isDirected && i < j) {
                    if (edgeDis(gen) > 0) {
                        int weight = weightDis(gen);
                        adjacencyMatrix[i][j] = weight;
                        adjacencyMatrix[j][i] = weight;
                    }
                }
                else if (isDirected) {
                    if (edgeDis(gen) > 0) {
                        adjacencyMatrix[i][j] = weightDis(gen);
                    }
                }
            }
        }
    }

    // Вывод матрицы смежности
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

    // Поиск расстояний от вершины
    void findDistances(int startVertex) {
        vector<int> dist = dijkstraDistances(startVertex);
        cout << "Кратчайшие расстояния от вершины " << startVertex << ":\n";
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

    // Вычисление эксцентриситета вершины
    int calculateEccentricity(int vertex) {
        if (vertex < 0 || vertex >= vertices) {
            return -1;
        }
        vector<int> dist = dijkstraDistances(vertex);
        int eccentricity = -1;
        for (int i = 0; i < vertices; i++) {
            if (dist[i] != -1 && dist[i] > eccentricity) {
                eccentricity = dist[i];
            }
        }
        return eccentricity;
    }

    // Анализ графа - радиус, диаметр, центры
    void analyzeGraph() {
        vector<int> eccentricities(vertices);
        int radius = numeric_limits<int>::max();
        int diameter = -1;

        // Считаем эксцентриситеты всех вершин
        for (int i = 0; i < vertices; i++) {
            eccentricities[i] = calculateEccentricity(i);
            if (eccentricities[i] != -1) {
                radius = min(radius, eccentricities[i]);
                diameter = max(diameter, eccentricities[i]);
            }
        }

        // Находим центральные и периферийные вершины
        vector<int> centralVertices, peripheralVertices;
        for (int i = 0; i < vertices; i++) {
            if (eccentricities[i] == radius) centralVertices.push_back(i);
            if (eccentricities[i] == diameter) peripheralVertices.push_back(i);
        }

        // Вывод результатов
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

    // Геттеры и сеттеры
    int getVerticesCount() const { return vertices; }
    bool getIsDirected() const { return isDirected; }
    void setVertices(int n) {
        vertices = n;
        generateRandomGraph();
    }
    void setDirected(bool directed) { isDirected = directed; generateRandomGraph(); }
    void regenerateGraph() { generateRandomGraph(); }
};

int main() {
    setlocale(LC_ALL, "Russian");
    int vertices;
    cout << "Введите количество вершин графа: ";
    cin >> vertices;
    if (vertices <= 0) {
        cout << "Неверное количество вершин! Установлено значение по умолчанию: 6" << endl;
        vertices = 6;
    }
    WeightedGraph graph(vertices, false);
    int choice;
    do {
        cout << "\n1. Неориентированный взвешенный граф";
        cout << "\n2. Ориентированный взвешенный граф";
        cout << "\n3. Анализ графа";
        cout << "\n4. Изменить количество вершин";
        cout << "\n0. Выход";
        cout << "\nВыберите операцию: ";
        cin >> choice;

        int startVertex, newVertices;

        switch (choice) {
        case 1:
            if (graph.getIsDirected()) {
                graph.setDirected(false);
            }
            graph.printAdjacencyMatrix();
            cout << "Введите стартовую вершину для поиска расстояний: ";
            cin >> startVertex;
            if (startVertex >= 0 && startVertex < graph.getVerticesCount()) {
                graph.findDistances(startVertex);
            }
            else {
                cout << "Неверная вершина! Допустимый диапазон: 0-" << graph.getVerticesCount() - 1 << endl;
            }
            break;
        case 2:
            if (!graph.getIsDirected()) {
                graph.setDirected(true);
            }
            graph.printAdjacencyMatrix();
            cout << "Введите стартовую вершину для поиска расстояний: ";
            cin >> startVertex;
            if (startVertex >= 0 && startVertex < graph.getVerticesCount()) {
                graph.findDistances(startVertex);
            }
            else {
                cout << "Неверная вершина! Допустимый диапазон: 0-" << graph.getVerticesCount() - 1 << endl;
            }
            break;
        case 3:
            cout << "\nАнализ текущего графа:" << endl;
            graph.printAdjacencyMatrix();
            graph.analyzeGraph();
            break;
        case 4:
            cout << "Введите новое количество вершин: ";
            cin >> newVertices;
            if (newVertices > 0) {
                graph.setVertices(newVertices);
                graph.printAdjacencyMatrix();
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