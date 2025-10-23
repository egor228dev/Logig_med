#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <random>
#include <iomanip>
#include <limits>
#include <chrono>
using namespace std;
using namespace std::chrono;
//Класс для представления графа
class Graph {
private:
    vector<vector<int>> adjacencyMatrix;// Матрица смежности графа
    vector<vector<int>> adjacencyList;// Списки смежности графа
    int vertices;// Количество вершин в графе
    //Метод для построения списков смежности на основе матрицы смежности
    void buildAdjacencyListFromMatrix() {
        adjacencyList.clear();// Очищаем существующие списки
        adjacencyList.resize(vertices);// Изменяем размер под количество вершин
        //Проходим по всем элементам матрицы смежности
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                //Если есть ребро между вершинами i и j
                if (adjacencyMatrix[i][j] == 1) {
                    //Добавляем вершину j в список смежности вершины i
                    adjacencyList[i].push_back(j);
                }
            }
        }
    }
public:
    //Конструктор класса Graph
    Graph(int n) : vertices(n) {
        generateRandomGraph();
    }
    //Метод для генерации случайного неориентированного графа
    void generateRandomGraph() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 1);
        //Инициализация матрицы смежности нулями
        adjacencyMatrix.resize(vertices, vector<int>(vertices, 0));
        //Генерация неориентированного графа
        for (int i = 0; i < vertices; i++) {
            for (int j = i + 1; j < vertices; j++) {
                int edge = dis(gen);
                adjacencyMatrix[i][j] = edge;
                adjacencyMatrix[j][i] = edge;
            }
        }
        //Построение списков смежности на основе сгенерированной матрицы
        buildAdjacencyListFromMatrix();
    }
    //Метод для вывода матрицы смежности
    void printAdjacencyMatrix() {
        cout << "\nМатрица смежности:\n";
        //Вывод заголовка
        cout << "   ";
        for (int i = 0; i < vertices; i++) {
            cout << setw(3) << i;
        }
        cout << "\n";
        //Вывод самой матрицы
        for (int i = 0; i < vertices; i++) {
            cout << setw(3) << i;
            for (int j = 0; j < vertices; j++) {
                cout << setw(3) << adjacencyMatrix[i][j];
            }
            cout << "\n";
        }
    }
    //Метод для вывода списков смежности
    void printAdjacencyList() {
        cout << "\nСписки смежности:\n";
        for (int i = 0; i < vertices; i++) {
            cout << i << ": ";
            for (int neighbor : adjacencyList[i]) {
                cout << neighbor << " ";
            }
            cout << "\n";
        }
    }
    //BFS обход в ширину через матрицу смежности
    vector<int> bfsDistanceMatrix(int startVertex) {
        //Засекаем время
        auto start = high_resolution_clock::now();
        //Вектор для хранения расстояний, инициализирован -1 (непосещенные вершины)
        vector<int> dist(vertices, -1);
        queue<int> q;//Очередь для BFS
        //Устанавливаем расстояние до стартовой вершины 0 и добавляем её в очередь
        dist[startVertex] = 0;
        q.push(startVertex);
        //Пока очередь не пуста
        while (!q.empty()) {
            int current = q.front();//Берем первую вершину из очереди
            q.pop();//Удаляем её из очереди
            //Проверяем всех соседей текущей вершины через матрицу смежности
            for (int i = 0; i < vertices; i++) {
                //Если есть ребро и вершина еще не посещена
                if (adjacencyMatrix[current][i] == 1 && dist[i] == -1) {
                    dist[i] = dist[current] + 1;// Обновляем расстояние
                    q.push(i);// Добавляем вершину в очередь для дальнейшего обхода
                }
            }
        }
        //Останавливаем таймер
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        cout << "Время выполнения BFS (матрица): " << duration.count() << " мкс\n";
        return dist;//Возвращаем вектор расстояний
    }
    //BFS через списки смежности
    vector<int> bfsDistanceList(int startVertex) {
        auto start = high_resolution_clock::now();
        vector<int> dist(vertices, -1);
        queue<int> q;
        dist[startVertex] = 0;
        q.push(startVertex);
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            //Используем списки смежности для получения соседей
            for (int neighbor : adjacencyList[current]) {
                if (dist[neighbor] == -1) {
                    dist[neighbor] = dist[current] + 1;
                    q.push(neighbor);
                }
            }
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        cout << "Время выполнения BFS (списки): " << duration.count() << " мкс\n";
        return dist;
    }
    //DFS обход в глубину через матрицу смежности
    vector<int> dfsDistanceMatrix(int startVertex) {
        auto start = high_resolution_clock::now();
        vector<int> dist(vertices, -1);
        stack<int> s;// Стек для DFS
        dist[startVertex] = 0;
        s.push(startVertex);
        while (!s.empty()) {
            int current = s.top();// Берем вершину с вершины стека
            s.pop();// Удаляем её из стека
            //Проверяем соседей через матрицу смежности
            for (int i = 0; i < vertices; i++) {
                if (adjacencyMatrix[current][i] == 1 && dist[i] == -1) {
                    dist[i] = dist[current] + 1;
                    s.push(i);// Добавляем в стек для дальнейшего обхода
                }
            }
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        cout << "Время выполнения DFS (матрица): " << duration.count() << " мкс\n";
        return dist;
    }

    //DFS через списки смежности
    vector<int> dfsDistanceList(int startVertex) {
        auto start = high_resolution_clock::now();
        vector<int> dist(vertices, -1);
        stack<int> s;
        dist[startVertex] = 0;
        s.push(startVertex);
        while (!s.empty()) {
            int current = s.top();
            s.pop();
            //Используем списки смежности для получения соседей
            for (int neighbor : adjacencyList[current]) {
                if (dist[neighbor] == -1) {
                    dist[neighbor] = dist[current] + 1;
                    s.push(neighbor);
                }
            }
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        cout << "Время выполнения DFS (списки): " << duration.count() << " мкс\n";
        return dist;
    }
    //Вывод расстояний
    void printDistances(const vector<int>& dist, int startVertex) {
        cout << "\nРасстояния от вершины " << startVertex << ":\n";
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
};
int main() {
    setlocale(LC_ALL, "Russian");
    int vertices;
    cout << "Введите количество вершин графа: ";
    cin >> vertices;
    if (vertices <= 0) {
        cout << "Неверное количество вершин! Установлено значение по умолчанию: 5" << endl;
        vertices = 5;
    }
    //Создание графа с заданным количеством вершин
    Graph graph(vertices);
    int choice;// Переменная для выбора пользователя
    do
    {
        cout << "\n1. Установить количество вершин";
        cout << "\n2. Показать матрицу смежности";
        cout << "\n3. Показать списки смежности";
        cout << "\n4. BFS - расстояния (матрица смежности)";
        cout << "\n5. BFS - расстояния (списки смежности)";
        cout << "\n6. DFS - расстояния (матрица смежности)";
        cout << "\n7. DFS - расстояния (списки смежности)";
        cout << "\n0. Выход";
        cout << "\nВыберите операцию: ";
        cin >> choice;
        switch (choice) {
        case 1: {
            cout << "Введите количество вершин: ";
            cin >> vertices;
            if (vertices > 0) {
                graph = Graph(vertices);
            }
            else {
                cout << "Неверное количество вершин!\n";
            }
            break;
        }
        case 2: {
            graph.printAdjacencyMatrix();
            break;
        }
        case 3: {
            graph.printAdjacencyList();
            break;
        }
        case 4: {
            int startVertex;
            cout << "Введите начальную вершину: ";
            cin >> startVertex;
            if (startVertex >= 0 && startVertex < vertices) {
                vector<int> dist = graph.bfsDistanceMatrix(startVertex);
                graph.printDistances(dist, startVertex);
            }
            else {
                cout << "Неверная вершина!\n";
            }
            break;
        }
        case 5: {
            int startVertex;
            cout << "Введите начальную вершину: ";
            cin >> startVertex;
            if (startVertex >= 0 && startVertex < vertices) {
                vector<int> dist = graph.bfsDistanceList(startVertex);
                graph.printDistances(dist, startVertex);
            }
            else {
                cout << "Неверная вершина!\n";
            }
            break;
        }
        case 6: {
            int startVertex;
            cout << "Введите начальную вершину: ";
            cin >> startVertex;
            if (startVertex >= 0 && startVertex < vertices) {
                vector<int> dist = graph.dfsDistanceMatrix(startVertex);
                graph.printDistances(dist, startVertex);
            }
            else {
                cout << "Неверная вершина!\n";
            }
            break;
        }
        case 7: {
            int startVertex;
            cout << "Введите начальную вершину: ";
            cin >> startVertex;
            if (startVertex >= 0 && startVertex < vertices) {
                vector<int> dist = graph.dfsDistanceList(startVertex);
                graph.printDistances(dist, startVertex);
            }
            else {
                cout << "Неверная вершина!\n";
            }
            break;
        }
        case 0: {
            break;
        }
        default: {
            cout << "Неверный выбор!\n";
            break;
        }
        }
    } while (choice != 0);
    return 0;
}