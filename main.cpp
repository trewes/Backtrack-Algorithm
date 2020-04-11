#include <iostream>
#include <vector>
#include <string>

#include "graph.h"
#include "backtracking.h"

int main(int argc, char* argv[])
{
    Graph::DirType type = Graph::undirected;
                                              //graphs can be given at runtime as arguments or specified at compile time
    Graph g = (argc > 1) ? Graph(argv[1], type): Graph("test1.txt", type);
    Graph h = (argc > 2) ? Graph(argv[2], type): Graph("test2.txt", type);

    std::cout<<"Begin."<<std::endl;
    std::vector<Graph::NodeId> phi;
    phi = isomorphic(g, h);                                                  //Search for an isomorphism between g and h

    std::cout<<"Finished."<<std::endl;
    std::vector<std::string> answer{"No", "Yes"};
    std::cout<<"Isomorphic: "<<answer[not phi.empty()]<<"."<<std::endl;
    for (int i: phi) {
		std::cout << i << " ";                                                                   //print the isomorphism
	}std::cout<<std::endl;

    std::cout<<"End of program."<<std::endl;
    return 0;
}