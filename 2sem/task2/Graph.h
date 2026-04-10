#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <cstring>
#include <cstdarg>

class Graph {
private:
    struct Vertex {
        int id;
        Vertex* next;
        
        Vertex(int i) : id(i), next(nullptr) {}
    };
    
    struct Edge {
        int from;
        int to;
        char* label;
        Edge* next;
        
        Edge(int f, int t, const char* l) : from(f), to(t), next(nullptr) {
            label = new char[strlen(l) + 1];
            strcpy(label, l);
        }
        
        ~Edge() {
            delete[] label;
        }
    };
    
    Vertex* vertices;
    Edge* edges;
    int vertexCount;
    int edgeCount;
    
    Vertex* findVertex(int id) {
        Vertex* curr = vertices;
        while (curr) {
            if (curr->id == id) return curr;
            curr = curr->next;
        }
        return nullptr;
    }
    
    Edge* findEdge(int from, int to, const char* label) {
        Edge* curr = edges;
        while (curr) {
            if (curr->from == from && curr->to == to && 
                strcmp(curr->label, label) == 0) return curr;
            curr = curr->next;
        }
        return nullptr;
    }
    
    void clear() {
        while (vertices) {
            Vertex* temp = vertices;
            vertices = vertices->next;
            delete temp;
        }
        while (edges) {
            Edge* temp = edges;
            edges = edges->next;
            delete temp;
        }
        vertexCount = 0;
        edgeCount = 0;
    }
    
    void copyFrom(const Graph& other) {
        clear();
        
        Vertex* otherV = other.vertices;
        while (otherV) {
            addVertex(otherV->id);
            otherV = otherV->next;
        }
        
        Edge* otherE = other.edges;
        while (otherE) {
            addEdge(otherE->from, otherE->to, otherE->label);
            otherE = otherE->next;
        }
    }
    
    bool isReachableDFS(int current, int target, bool* visited, int maxVertices) {
        if (current == target) return true;
        
        for (int i = 0; i < maxVertices; i++) {
            if (visited[i]) continue;
            
            Edge* e = edges;
            while (e) {
                if (e->from == current) {
                    visited[i] = true;
                    if (isReachableDFS(e->to, target, visited, maxVertices)) {
                        return true;
                    }
                    visited[i] = false;
                }
                e = e->next;
            }
        }
        return false;
    }
    
public:
    class Error {
    private:
        char message[256];
        int lineNum;
    public:
        Error(const char* msg, int line) {
            strncpy(message, msg, 255);
            message[255] = '\0';
            lineNum = line;
        }
        
        void print() const {
            std::cerr << "Error at line " << lineNum << ": " << message << std::endl;
        }
    };
    
    Graph() : vertices(nullptr), edges(nullptr), vertexCount(0), edgeCount(0) {}
    
    Graph(const char* str) : vertices(nullptr), edges(nullptr), vertexCount(0), edgeCount(0) {
        if (!str) return;
        
        const char* p = str;
        
        if (*p == '(') p++;
        if (*p == '(') p++;
        
        while (*p && *p != ')') {
            if (*p >= '0' && *p <= '9') {
                int id = 0;
                while (*p >= '0' && *p <= '9') {
                    id = id * 10 + (*p - '0');
                    p++;
                }
                addVertex(id);
            } else {
                p++;
            }
        }
        
        if (*p == ')') p++;
        if (*p == ',') p++;
        if (*p == '(') p++;
        
        while (*p && *p != ')') {
            if (*p == '(') {
                p++;
                int from = 0, to = 0;
                char label[256] = "";
                
                while (*p >= '0' && *p <= '9') {
                    from = from * 10 + (*p - '0');
                    p++;
                }
                if (*p == ',') p++;
                if (*p == '\'') {
                    p++;
                    int i = 0;
                    while (*p && *p != '\'') {
                        label[i++] = *p;
                        p++;
                    }
                    label[i] = '\0';
                    p++;
                }
                if (*p == ',') p++;
                while (*p >= '0' && *p <= '9') {
                    to = to * 10 + (*p - '0');
                    p++;
                }
                if (*p == ')') p++;
                
                addEdge(from, to, label);
            } else {
                p++;
            }
        }
    }
    
    Graph(const Graph& other) : vertices(nullptr), edges(nullptr), vertexCount(0), edgeCount(0) {
        copyFrom(other);
    }
    
    ~Graph() {
        clear();
    }
    
    Graph& operator=(const Graph& other) {
        if (this != &other) {
            copyFrom(other);
        }
        return *this;
    }
    
    void addVertex(int id) {
        if (id < 0) {
            throw Error("ID должен быть положительным", __LINE__);
        }
        if (findVertex(id)) return;
        
        Vertex* v = new Vertex(id);
        v->next = vertices;
        vertices = v;
        vertexCount++;
    }
    
    void addEdge(int from, int to, const char* label = "") {
        if (from < 0 || to < 0) {
            throw Error("Должны быть положительные числа", __LINE__);
        }
        
        if (!findVertex(from)) addVertex(from);
        if (!findVertex(to)) addVertex(to);
        
        if (findEdge(from, to, label)) return;
        
        Edge* e = new Edge(from, to, label);
        e->next = edges;
        edges = e;
        edgeCount++;
    }
    
    void add(int count, ...) {
        va_list args;
        va_start(args, count);
        
        for (int i = 0; i < count; i++) {
            const char* arg = va_arg(args, const char*);
            
            char* endptr;
            long val = strtol(arg, &endptr, 10);
            if (*endptr == '\0') {
                addVertex((int)val);
            } else if (arg[0] == '(') {
                int from, to;
                char label[256] = "";
                if (sscanf(arg, "(%d,'%255[^']',%d)", &from, label, &to) == 3) {
                    addEdge(from, to, label);
                }
            }
        }
        
        va_end(args);
    }
    
    Graph operator+(const Graph& other) const {
        Graph result(*this);
        
        Vertex* otherV = other.vertices;
        while (otherV) {
            result.addVertex(otherV->id);
            otherV = otherV->next;
        }
        
        Edge* otherE = other.edges;
        while (otherE) {
            result.addEdge(otherE->from, otherE->to, otherE->label);
            otherE = otherE->next;
        }
        
        return result;
    }
    
    Graph operator+(int vertex) const {
        Graph result(*this);
        result.addVertex(vertex);
        return result;
    }
    
    Graph operator+(const char* edgeStr) const {
        Graph result(*this);
        int from, to;
        char label[256] = "";
        if (sscanf(edgeStr, "(%d,'%255[^']',%d)", &from, label, &to) == 3) {
            result.addEdge(from, to, label);
        }
        return result;
    }
    
    Graph operator-(int vertex) const {
        Graph result(*this);
        result.removeVertex(vertex);
        return result;
    }
    
    Graph operator-(const char* edgeStr) const {
        Graph result(*this);
        int from, to;
        char label[256] = "";
        if (sscanf(edgeStr, "(%d,'%255[^']',%d)", &from, label, &to) == 3) {
            result.removeEdge(from, to, label);
        }
        return result;
    }
    
    void removeVertex(int id) {
        Edge* curr = edges;
        Edge* prev = nullptr;
        while (curr) {
            if (curr->from == id || curr->to == id) {
                Edge* toDel = curr;
                if (prev) {
                    prev->next = curr->next;
                    curr = curr->next;
                } else {
                    edges = curr->next;
                    curr = edges;
                }
                delete toDel;
                edgeCount--;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
        
        Vertex* vCurr = vertices;
        Vertex* vPrev = nullptr;
        while (vCurr) {
            if (vCurr->id == id) {
                if (vPrev) {
                    vPrev->next = vCurr->next;
                } else {
                    vertices = vCurr->next;
                }
                delete vCurr;
                vertexCount--;
                break;
            }
            vPrev = vCurr;
            vCurr = vCurr->next;
        }
    }
    
    void removeEdge(int from, int to, const char* label = "") {
        Edge* curr = edges;
        Edge* prev = nullptr;
        
        while (curr) {
            if (curr->from == from && curr->to == to && 
                strcmp(curr->label, label) == 0) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    edges = curr->next;
                }
                delete curr;
                edgeCount--;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    
    void remove(int count, ...) {
        va_list args;
        va_start(args, count);
        
        for (int i = 0; i < count; i++) {
            const char* arg = va_arg(args, const char*);
            
            char* endptr;
            long val = strtol(arg, &endptr, 10);
            if (*endptr == '\0') {
                removeVertex((int)val);
            } else if (arg[0] == '(') {
                int from, to;
                char label[256] = "";
                if (sscanf(arg, "(%d,'%255[^']',%d)", &from, label, &to) == 3) {
                    removeEdge(from, to, label);
                }
            }
        }
        
        va_end(args);
    }
    
    void deleteVertex(int id) {
        removeVertex(id);
    }
    
    void operator delete[](void* ptr) {
        ::operator delete[](ptr);
    }
    
    bool isReachable(int from, int to) const {
        if (from == to) return true;
        
        const int MAX = 100;
        int* stack = new int[MAX];
        bool* visited = new bool[MAX];
        for (int i = 0; i < MAX; i++) visited[i] = false;
        
        int top = 0;
        stack[top++] = from;
        visited[from] = true;
        
        while (top > 0) {
            int current = stack[--top];
            
            Edge* e = edges;
            while (e) {
                if (e->from == current) {
                    if (e->to == to) {
                        delete[] stack;
                        delete[] visited;
                        return true;
                    }
                    if (!visited[e->to]) {
                        visited[e->to] = true;
                        if (top < MAX) {
                            stack[top++] = e->to;
                        }
                    }
                }
                e = e->next;
            }
        }
        
        delete[] stack;
        delete[] visited;
        return false;
    }
    
    void printOutgoingEdges(int vertexId) const {
        std::cout << "Выходящие ребра из вершины " << vertexId << ":" << std::endl;
        
        Edge* e = edges;
        bool found = false;
        while (e) {
            if (e->from == vertexId) {
                std::cout << "  -> " << e->to;
                if (strlen(e->label) > 0) {
                    std::cout << " (пометка: '" << e->label << "')";
                }
                std::cout << std::endl;
                found = true;
            }
            e = e->next;
        }
        
        if (!found) {
            std::cout << "  Нет выходящих ребер" << std::endl;
        }
    }
    
    bool contains(int id) const {
        Vertex* v = vertices;
        while (v) {
            if (v->id == id) return true;
            v = v->next;
        }
        return false;
    }
    
    int getVertexCount() const { return vertexCount; }
    int getEdgeCount() const { return edgeCount; }

    int LoopCount() const {
        if (vertexCount == 0) return 0;
        
        int* verts = new int[vertexCount];
        int vCount = 0;
        Vertex* v = vertices;
        while (v) {
            verts[vCount++] = v->id;
            v = v->next;
        }
        
        auto findIndex = [&](int id) -> int {
            for (int i = 0; i < vCount; i++) {
                if (verts[i] == id) return i;
            }
            return -1;
        };
        
        bool* globalVisited = new bool[vCount];
        for (int i = 0; i < vCount; i++) globalVisited[i] = false;
        
        int cycleCount = 0;
        
        for (int i = 0; i < vCount; i++) {
            if (!globalVisited[i]) {
                bool* reachableFrom = new bool[vCount];
                for (int j = 0; j < vCount; j++) reachableFrom[j] = false;
                
                int* stack = new int[vCount];
                int top = 0;
                stack[top++] = i;
                reachableFrom[i] = true;
                
                while (top > 0) {
                    int currentIdx = stack[--top];
                    int currentId = verts[currentIdx];
                    
                    Edge* e = edges;
                    while (e) {
                        if (e->from == currentId) {
                            int nextIdx = findIndex(e->to);
                            if (nextIdx >= 0 && !reachableFrom[nextIdx]) {
                                reachableFrom[nextIdx] = true;
                                if (top < vCount) {
                                    stack[top++] = nextIdx;
                                }
                            }
                        }
                        e = e->next;
                    }
                }
                
                bool* canReachTo = new bool[vCount];
                for (int j = 0; j < vCount; j++) canReachTo[j] = false;
                
                top = 0;
                stack[top++] = i;
                canReachTo[i] = true;
                
                while (top > 0) {
                    int currentIdx = stack[--top];
                    int currentId = verts[currentIdx];
                    
                    Edge* e = edges;
                    while (e) {
                        if (e->to == currentId) {
                            int prevIdx = findIndex(e->from);
                            if (prevIdx >= 0 && !canReachTo[prevIdx]) {
                                canReachTo[prevIdx] = true;
                                if (top < vCount) {
                                    stack[top++] = prevIdx;
                                }
                            }
                        }
                        e = e->next;
                    }
                }
                
                bool* component = new bool[vCount];
                int compSize = 0;
                for (int j = 0; j < vCount; j++) {
                    component[j] = reachableFrom[j] && canReachTo[j];
                    if (component[j]) {
                        compSize++;
                        globalVisited[j] = true;
                    }
                }
                
                bool hasEdge = false;
                Edge* e = edges;am
                while (e) {
                    int fromIdx = findIndex(e->from);
                    int toIdx = findIndex(e->to);
                    if (fromIdx >= 0 && toIdx >= 0 && component[fromIdx] && component[toIdx]) {
                        hasEdge = true;
                        break;
                    }
                    e = e->next;
                }
                
                if (hasEdge && compSize > 1) {
                    cycleCount++;
                } else if (hasEdge && compSize == 1) {
                    for (int j = 0; j < vCount; j++) {
                        if (component[j]) {
                            Edge* e2 = edges;
                            while (e2) {
                                if (e2->from == verts[j] && e2->to == verts[j]) {
                                    cycleCount++;
                                    break;
                                }
                                e2 = e2->next;
                            }
                            break;
                        }
                    }
                }
                
                delete[] reachableFrom;
                delete[] canReachTo;
                delete[] component;
                delete[] stack;
            }
        }
        
        delete[] verts;
        delete[] globalVisited;
        
        return cycleCount;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
        os << "(";
        
        int* verts = new int[g.vertexCount];
        int count = 0;
        Vertex* v = g.vertices;
        while (v) {
            verts[count++] = v->id;
            v = v->next;
        }
        
        os << "(";
        for (int i = count - 1; i >= 0; i--) {
            os << verts[i];
            if (i > 0) os << ",";
        }
        os << "),";
        
        os << "(";
        const int MAX_EDGES = 100;
        struct EdgeInfo {
            int from;
            int to;
            const char* label;
        };
        EdgeInfo* edgeInfos = new EdgeInfo[MAX_EDGES];
        int eCount = 0;
        Edge* e = g.edges;
        while (e && eCount < MAX_EDGES) {
            edgeInfos[eCount].from = e->from;
            edgeInfos[eCount].to = e->to;
            edgeInfos[eCount].label = e->label;
            eCount++;
            e = e->next;
        }
        
        for (int i = eCount - 1; i >= 0; i--) {
            os << "(" << edgeInfos[i].from << ",'" << edgeInfos[i].label << "'," << edgeInfos[i].to << ")";
            if (i > 0) os << ",";
        }
        os << ")";
        os << ")";
        
        delete[] verts;
        delete[] edgeInfos;
        
        return os;
    }
};

#endif
