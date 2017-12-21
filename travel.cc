#include <iostream>
#include <vector>
#include <queue>
#include <sstream>

struct Node;

struct Edge {
    Node *Start;
    Node *Destination;
    unsigned int Price;

    Edge(Node &start, Node &destination, unsigned int price) : Start(&start), Destination(&destination), Price(price) {}
};

struct Node {
    int Number;
    unsigned int PathPrice;
    bool IsKeyServer;
    std::vector <Edge> Edges;

    Node *PreviousNode;
};

class Graph {
public:

    void Load(std::string input);

    int Process();

private:
    int FindPath(Node *startNode, Node *targetNode, int pathStartPrice, std::vector<Node *> &path);

    Node *FindKeyServer();

    std::vector <Node> m_Nodes;
};

void Graph::Load(std::string input) {
    int numberOfNodes;
    int numberOfKeyServers;

    std::istringstream stream1(input);

    stream1 >> numberOfNodes >> numberOfKeyServers;

    m_Nodes.resize(numberOfNodes);
    for (int i = 0; i < numberOfNodes; ++i) {
        m_Nodes[i].Number = i;
        m_Nodes[i].IsKeyServer = false;
        m_Nodes[i].PathPrice = -1;
        m_Nodes[i].PreviousNode = nullptr;

        int nodeIndexFrom;
        int nodeIndexTo;
        int price;

        stream1 >> nodeIndexFrom >> nodeIndexTo >> price;

        m_Nodes[nodeIndexFrom].Edges.push_back(Edge(m_Nodes[nodeIndexFrom], m_Nodes[nodeIndexTo], price));
        m_Nodes[nodeIndexTo].Edges.push_back(Edge(m_Nodes[nodeIndexTo], m_Nodes[nodeIndexFrom], price));
    }


    for (int i = 0; i < numberOfKeyServers; ++i) {
        int keyServerIndex;
        stream1 >> keyServerIndex;

        m_Nodes[keyServerIndex].IsKeyServer = true;
    }
}

int Graph::Process() {
    unsigned int pathPrice = 0;

    Node *rootNode = FindKeyServer();

    Node *startNode = rootNode;
    startNode->IsKeyServer = false;

    while (true) {
        unsigned int newPathPrice = -1;
        int maxNumKeyServers = 0;
        Node *newStartNode = nullptr;
        std::vector < Node * > shortestPath;

        for (int i = 0; i < m_Nodes.size(); ++i) {
            if (m_Nodes[i].IsKeyServer) {
                std::vector < Node * > path;

                Node *targetNode = &m_Nodes[i];
                int price = FindPath(startNode, targetNode, pathPrice, path);

                for (int i = 0; i < m_Nodes.size(); ++i) {
                    m_Nodes[i].PathPrice = -1;
                    m_Nodes[i].PreviousNode = nullptr;
                }

                int numKeyServers = 0;
                for (int i = 0; i < path.size(); ++i) {
                    if (path[i]->IsKeyServer) {
                        numKeyServers += 1;
                    }
                }

                if (numKeyServers > maxNumKeyServers) {
                    maxNumKeyServers = numKeyServers;
                    newStartNode = targetNode;
                    newPathPrice = price;
                    shortestPath.clear();
                    std::swap(shortestPath, path);
                } else if (numKeyServers == maxNumKeyServers && price < newPathPrice) {
                    newStartNode = targetNode;
                    newPathPrice = price;
                    shortestPath.clear();
                    std::swap(shortestPath, path);
                }
            }
        }

        if (newStartNode == nullptr) {
            break;
        }

        for (int i = 0; i < shortestPath.size(); ++i) {
            if (shortestPath[i]->IsKeyServer) {
                shortestPath[i]->IsKeyServer = false;
            }

            //std::cout << shortestPath[i]->Number << " ";
        }

        //std::cout << std::endl;

        startNode = newStartNode;
        pathPrice = newPathPrice;
    }

    std::vector < Node * > path;
//    std::cout << FindPath(startNode, rootNode, pathPrice, path) << std::endl;
    return FindPath(startNode, rootNode, pathPrice, path);
}

int Graph::FindPath(Node *startNode, Node *targetNode, int pathStartPrice, std::vector<Node *> &path) {
    //std::cout << "From " << startNode->Number << " to " << targetNode->Number << std::endl;

    std::priority_queue < Node * > queue;
    queue.push(startNode);
    startNode->PathPrice = pathStartPrice;

    while (!queue.empty()) {
        Node *currentNode = queue.top();
        queue.pop();

        if (currentNode == targetNode) {
            //return;
        }

        for (int i = 0; i < currentNode->Edges.size(); i++) {
            Edge &edge = currentNode->Edges[i];
            int newPrice = currentNode->PathPrice + edge.Price;
            if (newPrice < edge.Destination->PathPrice) {
                edge.Destination->PathPrice = newPrice;
                edge.Destination->PreviousNode = currentNode;
                queue.push(edge.Destination);
            }
        }
    }

    //std::cout << "Path found, price is " << targetNode->PathPrice << std::endl;

    Node *currentNode = targetNode;
    while (currentNode != nullptr) {
        //if (currentNode->IsKeyServer)
        //{
        //	currentNode->IsKeyServer = false;
        //}

        path.push_back(currentNode);

        //std::cout << currentNode->Number << " ";
        currentNode = currentNode->PreviousNode;
    }

    //std::cout << std::endl;

    return targetNode->PathPrice;
}

Node *Graph::FindKeyServer() {
    Node *startNode = nullptr;
    for (int i = 0; i < m_Nodes.size(); i++) {
        Node &node = m_Nodes[i];
        if (node.IsKeyServer) {
            startNode = &node;
            break;
        }
    }

    return startNode;
}
