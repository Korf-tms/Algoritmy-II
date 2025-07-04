#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <format>

using std::vector, std::pair, std::set;

// prvni cislo je vaha, vnirtni pair je dvojice vrcholu
using Edge = pair<int, pair<int, int>>;
using Graph = vector<Edge>;

set<Edge> primMinimalSpanningTree(const Graph& graph, const int numberOfVertices){
    set<int> verticesInMST;
    set<Edge> edgesInMST;
    std::priority_queue<Edge, vector<Edge>, std::greater<>> mstQueue;
    int startingVertex = graph[0].second.first;

    verticesInMST.insert(startingVertex);
    // "zbytecne" prochazi i hrany, ktere k nicemu nejsou
    for(const auto& currentEdge : graph){
        if(currentEdge.second.first == startingVertex or currentEdge.second.second == startingVertex){
            mstQueue.push(currentEdge);
        }
    }

    for(int i = 0; i < numberOfVertices - 1; i++){
        Edge minEdge = mstQueue.top();
        mstQueue.pop();
        // zjisti, jestli hrana vede z kostry do vrcholu, ktery v kostre neni
        while (verticesInMST.count(minEdge.second.first) + verticesInMST.count(minEdge.second.second) != 1)
        {
            minEdge = mstQueue.top();
            mstQueue.pop();
        }
        edgesInMST.insert(minEdge);
        // chci vedet, ktery vrchol pridavam
        int newIndex;
        if(verticesInMST.count(minEdge.second.first) == 0){
            newIndex = minEdge.second.first;
        }else{
            newIndex = minEdge.second.second;
        }
        verticesInMST.insert(newIndex);
        for(const auto& currentEdge : graph){
            if(currentEdge.second.first == newIndex or currentEdge.second.second == newIndex){
                if(verticesInMST.count(currentEdge.second.first) + verticesInMST.count(currentEdge.second.second)){
                    mstQueue.push(currentEdge);
                }
            }
        }
    }
    return edgesInMST;
}

// pricare hloupe napocitani vrcholu grafu
int countVertices(const Graph& graph){
    int counter = 0;
    set<int> vertices;
    for(const auto& [weigth, edge] : graph){
        auto [vertex1, vertex2] = edge;
        if(vertices.count(vertex1) == 0){
            counter += 1;
            vertices.insert(vertex1);
        }
        if(vertices.count(vertex2) == 0){
            counter += 1;
            vertices.insert(vertex2);
        }
    }
    return counter;
}


void printEdgeToLevitinGraph(const Edge& edge){
    char v1 = edge.second.first - 1 + 'a';
    char v2 = edge.second.second - 1 + 'a';
    std::cout << std::format("Edge: {} {} Weight: {}\n", v1, v2, edge.first);
}

int main() {
    // Levitin page 321
    Graph inputData = {{3, {1, 2}},
	    		{1, {2, 3}},
			{6, {3, 4}},
			{8, {4, 5}},
			{2, {5, 6}},
			{6, {1, 6}},
			{5, {1, 6}},
			{4, {2, 6}},
			{4, {3, 6}},
			{5, {4, 6}}};
    auto res = primMinimalSpanningTree(inputData, countVertices(inputData));
    for(const auto& edge : res){
        printEdgeToLevitinGraph(edge);
    }
    return 0;
}
