#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>

using namespace std;

struct Vertex{
	int id;
	int degree;
	vector<int> nbr;
};


class Graph{
public:
	Graph();
	~Graph();

	// Graph information
	int vertices_num_;
	unsigned edges_num_;
	int max_deg_;
	vector<Vertex*> vertices_;
	vector<int> id_new_to_old_;
};

#endif