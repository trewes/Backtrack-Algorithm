#include "backtracking.h"


bool iso_check(const Graph &first_graph, const Graph &second_graph, const Map &phi, const Map &phi_inv) {

    // checks (v,w) edge of G1 iff (phi(v),phi(w)) edge of G2

    for (Node v = 0; v < first_graph.num_nodes(); v++) {
        for (Graph::Neighbor w: first_graph.get_node(v).adjacent_nodes()) {
            if (not second_graph.is_edge(phi[v], phi[w.id()])) {
                return false;
            }
        }
    }

    for (Node v = 0; v < second_graph.num_nodes(); v++) {
        for (Graph::Neighbor w: second_graph.get_node(v).adjacent_nodes()) {
            if (not first_graph.is_edge(phi_inv[v], phi_inv[w.id()])) {
                return false;
            }
        }
    }
    return true;
}


std::tuple<Node, Node> adjacent_to_matched(const Graph &graph, const Map &map) {
    for (Node v = 0; v < graph.num_nodes(); v++) {
        if (map[v] != -1) {
            for (Graph::Neighbor w :graph.get_node(v).adjacent_nodes()) {
                if (map[w.id()] == -1) {                                  //get unmatched neighbors w of matched nodes v
                    return std::make_tuple(w.id(), v);
                }
            }
        }
    }
    return std::make_tuple(-1, -1);
}


bool try_match(const Graph &first_graph, const Graph &second_graph,
               Map &phi, Map &phi_inv, Node node_first_graph, Node node_second_graph) {

    phi[node_first_graph] = node_second_graph;                    //try to build an isomorphism with the assumed mapping
    phi_inv[node_second_graph] = node_first_graph;


                                                //check if it now is a bijection and then further if also an isomorphism
    if (std::find(phi.begin(), phi.end(), -1) == phi.end()) {
        if (iso_check(first_graph, second_graph, phi, phi_inv)) {
            return true;                                                                          //found an isomorphism
        }
    }
                             //if the map is bijective then there are no unmapped vertices left and backtracking happens
    else {                                                                                  //else, search for a pairing
        Node v, v_prime;
        std::tie(v, v_prime) = adjacent_to_matched(first_graph, phi);
        if (v != -1) {
            //there exists an unmapped node that has a mapped neighbor
            //map that node and look at the neighborhood of the image node

            //v is unmatched, get a matched neighbor of v, that is v_prime
            //map that to node in G2 and look at neighbourhood
            //w_prime=phi[v_prime]
            //w = unmatched neighbor of w_prime. Match v and w.

            for (Graph::Neighbor w: second_graph.get_node(phi[v_prime]).adjacent_nodes()) {
                if (phi_inv[w.id()] == -1) {
                    if (distinguish(first_graph, v, second_graph, w.id(),
                                    triangles)) {          //small check for same degree
                        //assume the match of the two nodes and go on
                        if (try_match(first_graph, second_graph, phi, phi_inv, v, w.id())) {
                            return true;
                        }
                    }
                }
            }
        } else {
            //i.e. if v==-1, that means no v with the wanted conditions exist
            //note that in a connected graph this code is never reached

            //my decision process works with the neighbourhood of the (so far) mapped graph
            //and gives no hint what nodes to pair when the neighbor is empty (because the graph is disconnected)
            //can't do much but guess, try matching any two unmapped nodes.
            for (Node first_graph_node = 0; first_graph_node < first_graph.num_nodes(); first_graph_node++) {
                if (phi[first_graph_node] == -1) {
                    for (Node second_graph_node = 0; second_graph_node < first_graph.num_nodes(); second_graph_node++) {
                        if (phi_inv[second_graph_node] == -1) {
                            if (distinguish(first_graph, first_graph_node, second_graph, second_graph_node,
                                            triangles)) {
                                                                           //assume the match of the two nodes and go on
                                if (try_match(first_graph, second_graph, phi, phi_inv,
                                        first_graph_node, second_graph_node)) {
                                    return true;
                                }
                            }
                        }
                    }
                                                                 //tried matching the first unmmapped node to all of the
                    break;                                              //unmatched one in the second graph, that failed
                }
            }
        }
    }
    phi[node_first_graph] = -1;                                //the assumed mapping failed, reset mapping and backtrack
    phi_inv[node_second_graph] = -1;
    return false;
}


std::vector<Graph::NodeId> isomorphic(const Graph &first_graph, const Graph &second_graph) {

    if (first_graph.num_nodes() != second_graph.num_nodes()) {              //preliminary check for same number of nodes
        return std::vector<Graph::NodeId>();
    }

    Map phi(first_graph.num_nodes(), -1);                                         //mapping that will be built up
    Map phi_inv(first_graph.num_nodes(), -1);                                      // -1 means vertex is unmapped


                                  //we start by trying to match the first node to any of the nodes from the second graph
    auto degree_of_0 = triangle_count(first_graph, 0);
    for (Node possible_node = 0; possible_node < first_graph.num_nodes(); possible_node++) {

        if (degree_of_0 == triangle_count(second_graph, possible_node)) {//again, need same degree

            //std::cout << "Step: " << possible_node << std::endl;

            if (try_match(first_graph, second_graph, phi, phi_inv, 0, possible_node)) {
                return phi;                                                                       //found an isomorphism
            }
        }
    }
    return Map();                                   //No attempted mapping was successful, the graphs are not isomorphic
}

int degree(const Graph &graph, Node node) {
    return graph.get_node(node).adjacent_nodes().size();
}

std::vector<int> degree_neighbours(const Graph &graph, Node node) {
    std::vector<int> result;
    for (auto w: graph.get_node(node).adjacent_nodes()) {
        result.push_back(degree(graph, w.id()));
    }
    std::sort(result.begin(), result.end());
    return result;
}

int triangle_count(const Graph &graph, Node node) {
    int count = 0;
    for (auto v: graph.get_node(node).adjacent_nodes()) {
        for (auto w: graph.get_node(node).adjacent_nodes()) {
            if (graph.is_edge(v.id(), w.id())) {
                count++;                                                   //count the number of triangles like node,v,w
            }
        }
    }
    return count;
}

bool distinguish(const Graph &first_graph, Node first_node, const Graph &second_graph, Node second_node, Method method){
    if (degree(first_graph, first_node) != degree(second_graph, second_node)) {
        return false;
    } else if (method == neighbours) {
        return (degree_neighbours(first_graph, first_node) == degree_neighbours(second_graph, second_node));
    } else if (method == triangles) {
        return (triangle_count(first_graph, first_node) == triangle_count(second_graph, second_node));
    }
    return true;
}


