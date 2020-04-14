#include "graph.h"



void Graph::add_nodes(NodeId num_new_nodes) {
    _nodes.resize(num_nodes() + num_new_nodes);
}

Graph::Neighbor::Neighbor(Graph::NodeId n) : _id(n) {}

Graph::Graph(NodeId num, DirType dtype) : dirtype(dtype), _nodes(num) {}

void Graph::add_edge(NodeId tail, NodeId head, Format format) {
    if (tail >= num_nodes() or tail < 0 or head >= num_nodes() or head < 0) {
        throw std::runtime_error("Edge cannot be added due to undefined endpoint.");
    }
    _nodes[tail].add_neighbor(head);
    if (dirtype == Graph::undirected and format == list) {
        _nodes[head].add_neighbor(tail);
    }
}

void Graph::Node::add_neighbor(Graph::NodeId nodeid) {
    _neighbors.emplace_back(nodeid);
}

const std::vector<Graph::Neighbor> &Graph::Node::adjacent_nodes() const {
    return _neighbors;
}

Graph::NodeId Graph::num_nodes() const {
    return _nodes.size();
}

const Graph::Node &Graph::get_node(NodeId node) const {
    if (node < 0 or node >= static_cast<int>(_nodes.size())) {
        std::cout << node << std::endl;
        throw std::runtime_error("Invalid nodeid in Graph::get_node.");
    }
    return _nodes[node];
}

Graph::NodeId Graph::Neighbor::id() const {
    return _id;
}


void Graph::print() const {
    if (dirtype == Graph::directed) {
        std::cout << "Digraph ";
    } else {
        std::cout << "Undirected graph ";
    }
    std::cout << "with " << num_nodes() << " vertices, numbered 0,...,"
              << num_nodes() - 1 << ".\n";

    for (NodeId nodeid = 0; nodeid < num_nodes(); ++nodeid) {
        std::cout << "The following edges are ";
        if (dirtype == Graph::directed) {
            std::cout << "leaving";
        } else {
            std::cout << "incident to";
        }
        std::cout << " vertex " << nodeid << ":\n";
        for (auto neighbor: _nodes[nodeid].adjacent_nodes()) {
            std::cout << nodeid << " - " << neighbor.id() << "\n";
        }
    }
}

Graph::Graph(char const *filename, DirType dtype) : dirtype(dtype) {
    std::ifstream file(filename);
    if (not file) {
        throw std::runtime_error("Cannot open file.");
    }

    Graph::NodeId num = 0;
    std::string line;
    std::getline(file, line);
    std::stringstream ss(line);
    ss >> num;                                                         //read in the number of nodes from the first line
    if (not ss) {
        throw std::runtime_error("Invalid file format.");
    }
    add_nodes(num);
    line = "";
    std::getline(file, line);                                             //get second line and check the format
    if (line.empty()) {                                                                                    //empty graph
        return;
    }
    if (any_of(line.begin(), line.end(), isspace)) {       //second line contains a whitespace, format is adjacency list
        do {
            std::stringstream ss(line);
            Graph::NodeId head, tail;
            ss >> tail >> head;
            if (not ss) {
                throw std::runtime_error("Invalid file format.");
            }
            if (tail != head) {
                add_edge(tail, head);

            } else {
                throw std::runtime_error("Invalid file format: loops not allowed.");
            }
        } while (std::getline(file, line));
    } else {                                                                     //Format is that of an adjacency matrix
        for (int tail = 0; tail < num; tail++) {                        //iterate over the following n strings of size n
            std::stringstream ss(line);
            std::string edges;
            ss >> edges;

            if (not ss) {
                throw std::runtime_error("Invalid file format.");
            }
            for (int head = 0; head < num; head++) {
                if (edges[tail] =='1') {
                    throw std::runtime_error("Invalid file format: loops not allowed.");
                }
                if (edges[head] == '1') {
                    add_edge(tail, head, matrix);
                }
            }
            std::getline(file, line);
        }
    }
}


bool Graph::is_edge(Graph::NodeId v, Graph::NodeId w) const {
    if(v == INVALIDNODE or w == INVALIDNODE){
        throw std::runtime_error("Error, Invalid node.");
    }
    for (Graph::Neighbor neighbor: Graph::get_node(v).adjacent_nodes()) {
        if (neighbor.id() == w) {                                      //simply checks if one of the neighbors of v is w
            return true;
        }
    }
    return false;
}
