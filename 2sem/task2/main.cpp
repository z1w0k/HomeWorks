#include "Graph.h"
#include <iostream>
#include <cstring>

void runTests() {
    try {
        std::cout << "\nЗапуск тестов" << std::endl;
        
        std::cout << "\nТест 1: Конструктор по умолчанию" << std::endl;
        Graph g1;
        std::cout << "Граф создан: " << g1 << std::endl;
        
        std::cout << "\nТест 2: Добавление вершин" << std::endl;
        g1.addVertex(1);
        g1.addVertex(2);
        g1.addVertex(3);
        std::cout << "После добавления вершин 1,2,3: " << g1 << std::endl;
        
        std::cout << "\nТест 3: Добавление ребер" << std::endl;
        g1.addEdge(1, 2, "a");
        g1.addEdge(2, 3, "abc");
        std::cout << "После добавления ребер: " << g1 << std::endl;
        
        std::cout << "\nТест 4: Конструктор из строки" << std::endl;
        Graph g2("((1,2,3,4),((1,'a',2),(2,'abc',3)))");
        std::cout << "Граф из строки: " << g2 << std::endl;
        
        std::cout << "\nТест 5: Конструктор копирования" << std::endl;
        Graph g3(g1);
        std::cout << "Оригинал: " << g1 << std::endl;
        std::cout << "Копия: " << g3 << std::endl;
        
        std::cout << "\nТест 6: Оператор присваивания" << std::endl;
        Graph g4;
        g4 = g1;
        std::cout << "Присвоенный граф: " << g4 << std::endl;
        
        std::cout << "\nТест 7: Оператор + (объединение)" << std::endl;
        Graph g5;
        g5.addVertex(5);
        g5.addVertex(6);
        g5.addEdge(5, 6, "xy");
        Graph g6 = g1 + g5;
        std::cout << "Результат объединения: " << g6 << std::endl;
        
        std::cout << "\nТест 8: Оператор + (добавление вершины)" << std::endl;
        Graph g7 = g1 + 4;
        std::cout << "После добавления вершины 4: " << g7 << std::endl;
        
        std::cout << "\nТест 9: Оператор + (добавление ребра)" << std::endl;
        Graph g8 = g1 + "(3,'c',1)";
        std::cout << "После добавления ребра (3,'c',1): " << g8 << std::endl;
        
        std::cout << "\nТест 10: Оператор - (удаление вершины)" << std::endl;
        Graph g9 = g1 - 2;
        std::cout << "После удаления вершины 2: " << g9 << std::endl;
        
        std::cout << "\nТест 11: Оператор - (удаление ребра)" << std::endl;
        Graph g10 = g1 - "(1,'a',2)";
        std::cout << "После удаления ребра (1,'a',2): " << g10 << std::endl;
        
        std::cout << "\nТест 12: Метод removeVertex" << std::endl;
        Graph g11(g1);
        g11.removeVertex(2);
        std::cout << "После removeVertex(2): " << g11 << std::endl;
        
        std::cout << "\nТест 13: Метод removeEdge" << std::endl;
        Graph g12(g1);
        g12.removeEdge(1, 2, "a");
        std::cout << "После removeEdge(1,2,'a'): " << g12 << std::endl;
        
        std::cout << "\nТест 14: deleteVertex (синоним delete[])" << std::endl;
        Graph g13(g1);
        g13.deleteVertex(2);
        std::cout << "После deleteVertex(2): " << g13 << std::endl;
        
        std::cout << "\nТест 15: Вывод исходящих ребер" << std::endl;
        g1.printOutgoingEdges(1);
        
        std::cout << "\nТест 16: Проверка достижимости" << std::endl;
        std::cout << "Достижима ли 1 из 1? " << (g1.isReachable(1, 1) ? "Да" : "Нет") << std::endl;
        std::cout << "Достижима ли 3 из 1? " << (g1.isReachable(1, 3) ? "Да" : "Нет") << std::endl;
        std::cout << "Достижима ли 1 из 3? " << (g1.isReachable(3, 1) ? "Да" : "Нет") << std::endl;
        
        std::cout << "\nТест 17: Проверка наличия" << std::endl;
        std::cout << "Содержит вершину 1? " << (g1.contains(1) ? "Да" : "Нет") << std::endl;
        std::cout << "Содержит вершину 5? " << (g1.contains(5) ? "Да" : "Нет") << std::endl;
        
        std::cout << "\nТест 18: Добавление с переменным числом аргументов" << std::endl;
        Graph g14;
        g14.add(5, "1", "2", "3", "(1,'a',2)", "(2,'b',3)");
        std::cout << "После add(5, ...): " << g14 << std::endl;
        
        std::cout << "\nТест 19: Удаление с переменным числом аргументов" << std::endl;
        Graph g15(g14);
        g15.remove(2, "2", "(1,'a',2)");
        std::cout << "После remove(2, ...): " << g15 << std::endl;
        
        std::cout << "\nТест 20: Использование LoopCount" << std::endl;
        try {
            Graph g16;
            g16.addVertex(-1);
        } catch (const Graph::Error& e) {
            e.print();
        }

        Graph g20;
        g20.addVertex(1);
        g20.addVertex(2);
        g20.addVertex(3);
        g20.addVertex(4);
        g20.addVertex(5);
        g20.addVertex(6);
        g20.addEdge(1, 2, "a");
        g20.addEdge(2, 3, "b");
        g20.addEdge(3, 1, "c");
        g20.addEdge(4, 5, "d");
        g20.addEdge(5, 6, "e");
        g20.addEdge(6, 4, "f");
        std::cout << "Граф: " << g20 << std::endl;
        std::cout << "Количество циклов: " << g20.LoopCount() << std::endl;


        std::cout << "\nTecnt 21: Использование LoopCount" << std::endl;
        Graph g21;
        g21.addVertex(1);
        g21.addVertex(2);
        g21.addVertex(3);
        g21.addVertex(4);
        g21.addEdge(1, 2, "a");
        g21.addEdge(2, 3, "b");
        g21.addEdge(3, 1, "c");
        g21.addEdge(4, 4, "self");
        std::cout << "Граф: " << g21 << std::endl;
        std::cout << "Количество циклов: " << g21.LoopCount() << std::endl;


        std::cout << "\nВсе тесты завершены" << std::endl;
        
    } catch (const Graph::Error& e) {
        e.print();
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        runTests();
        return 0;
    }
    
    
    Graph graph;
    std::string cmd;
    
    std::cout << "\nКоманды:" << std::endl;
    std::cout << "  addV <id>                    - добавить вершину" << std::endl;
    std::cout << "  addE <откуда> <куда> [метка] - добавить ребро" << std::endl;
    std::cout << "  addM <в1> <в2> ... <р1> ...  - добавить несколько" << std::endl;
    std::cout << "  removeV <id>                 - удалить вершину" << std::endl;
    std::cout << "  removeE <откуда> <куда> [метка] - удалить ребро" << std::endl;
    std::cout << "  removeM ...                  - удалить несколько" << std::endl;
    std::cout << "  print                        - показать граф" << std::endl;
    std::cout << "  reach <откуда> <куда>        - проверить достижимость" << std::endl;
    std::cout << "  outgoing <вершина>           - показать исходящие ребра" << std::endl;
    std::cout << "  contains <вершина>           - проверить наличие вершины" << std::endl;
    std::cout << "  union                        - объединить с тестовым графом" << std::endl;
    std::cout << "  quit                         - выход" << std::endl;
    std::cout << "  loopcount                    - подсчитать количество циклов" << std::endl;
    
    while (true) {
        std::cout << "\n> ";
        std::cin >> cmd;
        
        if (cmd == "quit" || cmd == "exit") {
            break;
        }
        else if (cmd == "addV") {
            int id;
            std::cin >> id;
            graph.addVertex(id);
            std::cout << "Вершина " << id << " добавлена" << std::endl;
        }
        else if (cmd == "addE") {
            int from, to;
            char label[100] = "";
            std::cin >> from >> to;
            if (std::cin.peek() != '\n') {
                std::cin >> label;
            }
            graph.addEdge(from, to, label);
            std::cout << "Ребро " << from << "->" << to << " добавлено" << std::endl;
        }
        else if (cmd == "addM") {
            char input[1024];
            std::cin.getline(input, 1024);
            
            int count = 0;
            for (int i = 0; input[i]; i++) {
                if (input[i] == ' ') count++;
            }
            count++;
            
            char** args = new char*[count];
            char* token = strtok(input, " ");
            int i = 0;
            while (token && i < count) {
                args[i] = new char[strlen(token) + 1];
                strcpy(args[i], token);
                token = strtok(nullptr, " ");
                i++;
            }
            
            switch (count) {
                case 1: graph.add(1, args[0]); break;
                case 2: graph.add(2, args[0], args[1]); break;
                case 3: graph.add(3, args[0], args[1], args[2]); break;
                case 4: graph.add(4, args[0], args[1], args[2], args[3]); break;
                case 5: graph.add(5, args[0], args[1], args[2], args[3], args[4]); break;
                default: std::cout << "Слишком много аргументов" << std::endl;
            }
            
            for (int j = 0; j < i; j++) delete[] args[j];
            delete[] args;
            
            std::cout << "Элементы добавлены" << std::endl;
        }
        else if (cmd == "removeV") {
            int id;
            std::cin >> id;
            graph.removeVertex(id);
            std::cout << "Вершина " << id << " удалена" << std::endl;
        }
        else if (cmd == "removeE") {
            int from, to;
            char label[100] = "";
            std::cin >> from >> to;
            if (std::cin.peek() != '\n') {
                std::cin >> label;
            }
            graph.removeEdge(from, to, label);
            std::cout << "Ребро удалено" << std::endl;
        }
        else if (cmd == "removeM") {
            char input[1024];
            std::cin.getline(input, 1024);
            
            int count = 0;
            for (int i = 0; input[i]; i++) {
                if (input[i] == ' ') count++;
            }
            count++;
            
            char** args = new char*[count];
            char* token = strtok(input, " ");
            int i = 0;
            while (token && i < count) {
                args[i] = new char[strlen(token) + 1];
                strcpy(args[i], token);
                token = strtok(nullptr, " ");
                i++;
            }
            
            switch (count) {
                case 1: graph.remove(1, args[0]); break;
                case 2: graph.remove(2, args[0], args[1]); break;
                case 3: graph.remove(3, args[0], args[1], args[2]); break;
                case 4: graph.remove(4, args[0], args[1], args[2], args[3]); break;
                case 5: graph.remove(5, args[0], args[1], args[2], args[3], args[4]); break;
                default: std::cout << "Слишком много аргументов" << std::endl;
            }
            
            for (int j = 0; j < i; j++) delete[] args[j];
            delete[] args;
            
            std::cout << "Элементы удалены" << std::endl;
        }
        else if (cmd == "print") {
            std::cout << graph << std::endl;
        }
        else if (cmd == "reach") {
            int from, to;
            std::cin >> from >> to;
            std::cout << from << " -> " << to << " : " 
                      << (graph.isReachable(from, to) ? "достижима" : "не достижима") << std::endl;
        }
        else if (cmd == "outgoing") {
            int vertex;
            std::cin >> vertex;
            graph.printOutgoingEdges(vertex);
        }
        else if (cmd == "contains") {
            int id;
            std::cin >> id;
            std::cout << "Вершина " << id << ": " 
                      << (graph.contains(id) ? "существует" : "не существует") << std::endl;
        }
        else if (cmd == "union") {
            Graph testGraph;
            testGraph.addVertex(10);
            testGraph.addVertex(20);
            testGraph.addEdge(10, 20, "test");
            Graph result = graph + testGraph;
            std::cout << "Результат объединения: " << result << std::endl;
            std::cout << "Примечание: исходный граф не изменился" << std::endl;
        }
        else if (cmd == "loopcount") {
            std::cout << "Колличество циклов: " << graph.LoopCount() << std::endl;
        }
        else {
            std::cout << "Неизвестная команда. Используйте: addV, addE, addM, removeV, removeE, removeM, "
                      << "print, reach, outgoing, contains, union, quit" << std::endl;
        }
    }
    
    return 0;
}
