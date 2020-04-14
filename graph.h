#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <vector>


/*
 * This is essentially the graph class from Hougardys webpage. I removed the weight attribute of
 * edges and adapted the constructor to be able to read in graphs given as adjacency matrix, for that reason
 * slightly altered add_edge too. And there is a function is_edge(v,w) returning a boolean of whether there is an
 * edge between or from v to w.
 * It's assumed that given graphs are simple and loops throw an error while parallel edges are ignored at least for
 * adjacency matrices. Graphs can be given in two formats:
 *      Format 1:               //adjacency list
 *      num_nodes               //number of nodes of the graph
 *      node1_1 node1_2         //meaning there is and edge between node1_1 and node1_2
 *      node2_1 node2_2
 *      ...
 *
 *      Format 2:               //adjacency matrix
 *      num_nodes               //number of nodes of the graph
 *      10001..1101             //the first row of the matrix, filled with 0 or 1
 *                              //further rows like the second one
 *
 */


class Graph {
public:
  using NodeId = int;

  class Neighbor {
  public:
        explicit Neighbor(Graph::NodeId n);
        Graph::NodeId id() const;
  private:
        Graph::NodeId _id;
    };

  class Node {
  public:
        void add_neighbor(Graph::NodeId nodeid);
        const std::vector<Neighbor> & adjacent_nodes() const;
  private:
        std::vector<Neighbor> _neighbors;
    };

  enum DirType {directed, undirected};
  Graph(NodeId num_nodes, DirType dirtype);
  Graph(char const* filename, DirType dirtype);
  bool is_edge(Graph::NodeId v, Graph::NodeId w) const;

  void add_nodes(NodeId num_new_nodes);
  enum Format {list, matrix};
  void add_edge(NodeId tail, NodeId head, Format format = list);

  NodeId num_nodes() const;
  const Node & get_node(NodeId) const;
  void print() const;

  const DirType dirtype;
  static constexpr NodeId INVALIDNODE=-1;

private:
  std::vector<Node> _nodes;
};

#endif  //GRAPH_H
