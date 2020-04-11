#ifndef BACKTRACK_ALGORITHM_BACKTRACKING_H
#define BACKTRACK_ALGORITHM_BACKTRACKING_H

#include <vector>
#include <tuple>
#include <algorithm>

#include "graph.h"

typedef std::vector<Graph::NodeId> Map;
typedef Graph::NodeId Node;

/*
 *iso_check(first_graph, second_graph, phi, phi_inv)
 *
 * Parameter: first_graph and second_graph are two graphs and we want to see if
 *            phi and phi_inv are actually an isomorphism of these graphs.
 *
 * Returns: The truth value of whether the two graphs are isomorphic via the given map phi (and phi_inv)
 */
bool iso_check(const Graph &first_graph, const Graph &second_graph,
               const Map &phi, const Map &phi_inv);


/*
 * Adjacent_to_unmatched(graph, map) This looks for a not yet mapped vertex in the neighbourhood of all
 *                                    mapped vertices so far.
 *
 * Parameter: graph
 *            map Has the information about which vertices are mapped so far.
 *
 * Returns: If we find a mapped vertex v which has a unmapped neighbour w, it returns the tuple (w,v).
 *          If no such v,w exist (because the mapping is bijective but not an isomorphism or the graph
 *          is disconnected) the it returns (-1, -1)
 *
 */
std::tuple<Graph::NodeId, Graph::NodeId> adjacent_to_matched(const Graph &graph, const Map &map);

/*
 * try_match(first_graph, second_graph, phi, phi_inv, node_first_graph, node_second_graph)
 *
 * Parameter: two graphs and an incomplete mapping between them
 *            plus two nodes which we map to each other
 *
 * Returns: truth value of whether the tried pairing can be/is expanded to an isomorphism or not.
 */
bool try_match(const Graph &first_graph, const Graph &second_graph, Map &phi,
               Map &phi_inv, Node node_first_graph, Node node_second_graph);

/*
 * isomorphic(first_graph, second_graph)
 *
 * Parameter: Two graphs
 *
 * Returns: Either an isomorphism as a Node vector where Node i is sent to node vector[i]
 *          or if no isomorphism can be found, an empty vector.
 */
std::vector<Node> isomorphic(const Graph &first_graph, const Graph &second_graph);

/*
 * degree(graph, node)
 *
 * Returns: The degree of node in graph
 */
int degree(const Graph &graph, Node node);

/*
 * degree2(graph, node) Gives some more info about the neighbourhood of node
 *
 * Returns: The degree of all neighbours of node as a sorted vector
 */
std::vector<int> degree2(const Graph &graph, Node node);

/*
 * degree3(graph, node) Counts number of triangles going through a node
 *
 * Returns: The number of triangles going through node (Twice in the undirected case)
 */
int degree3(const Graph &graph, Node node);

/*
 * distinguish(first_graph, first_node, second_graph, second_node, method)
 *
 * Returns: Whether the two given nodes are distinguished by the given method. True if they can not be
 *          distinguished, false if the given method marks them as different.
 */
enum Method {by_degree, neighbours, triangles};
bool distinguish(const Graph &first_graph, Node first_node,
                 const Graph &second_graph, Node second_node, Method method = by_degree);

#endif //BACKTRACK_ALGORITHM_BACKTRACKING_H
