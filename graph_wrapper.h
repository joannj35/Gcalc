#ifndef GRAPH_W_H
#define GRAPH_W_H

#include "graph.h"#include <string>
#include <iostream>

using std::cout;using std::string;
using std::endl;

Graph* create()
{
	Graph* g = new Graph();
	if(g == nullptr)
		cout << "Error: allocating new graph failed!" << endl;
	return g;
}

void destroy(Graph* g)
{
	if(!g)
	{
		cout << "Error: invalid graph argument!" << endl;
		return;
	}
	delete g;
}

Graph* addVertex(Graph* g, char* vertex)
{
	if(!g)
	{
		cout << "Error: invalid graph argument!" << endl;
		return nullptr;
	}	string v(vertex);
	try {
		g->addVertex(v);
	}
	catch (const std::exception & e) {
		std::cout << e.what() << endl;
	}
	
	return g;
}

Graph* addEdge(Graph* g, char* v1, char* v2)
{
	if(!g)
	{
		cout << "Error: invalid graph argument!" << endl;
		return nullptr;
	}	string _v1(v1);	string _v2(v2);
	try {
		g->addNeighbour(_v1, _v2);
	}
	catch (const std::exception & e) {
		std::cout << e.what() << endl;
	}
	return g;
}

void disp(Graph* g)
{
		if(!g)
	{
		cout << "Error: invalid graph argument!" << endl;
		return;
	}
	cout << *g;
}

Graph* graphUnion(Graph* graph_in1, Graph* graph_in2, Graph* graph_out)
{
	if(!graph_in1)
	{
		cout << "Error: invalid graph argument! (1)" << endl;
		return nullptr;
	}
	if(!graph_in2)
	{
		cout << "Error: invalid graph argument! (2)" << endl;
		return nullptr;
	}
	if(!graph_out)
	{
		cout << "Error: invalid graph argument! (3)" << endl;
		return nullptr;
	}
	
	*graph_out = (*graph_in1) + (*graph_in2);
	return graph_out;
}

Graph* graphIntersection(Graph* graph_in1, Graph* graph_in2, Graph* graph_out)
{
	if(!graph_in1)
	{
		cout << "Error: invalid graph argument! (1)" << endl;
		return nullptr;
	}
	if(!graph_in2)
	{
		cout << "Error: invalid graph argument! (2)" << endl;
		return nullptr;
	}
	if(!graph_out)
	{
		cout << "Error: invalid graph argument! (3)" << endl;
		return nullptr;
	}
	
	*graph_out = (*graph_in1) ^ (*graph_in2);
	return graph_out;
}

Graph* graphDifference(Graph* graph_in1, Graph* graph_in2, Graph* graph_out)
{
	if(!graph_in1)
	{
		cout << "Error: invalid graph argument! (1)" << endl;
		return nullptr;
	}
	if(!graph_in2)
	{
		cout << "Error: invalid graph argument! (2)" << endl;
		return nullptr;
	}
	if(!graph_out)
	{
		cout << "Error: invalid graph argument! (3)" << endl;
		return nullptr;
	}
	
	*graph_out = (*graph_in1) - (*graph_in2);
	return graph_out;
}

Graph* graphProduct(Graph* graph_in1, Graph* graph_in2, Graph* graph_out)
{
	if(!graph_in1)
	{
		cout << "Error: invalid graph argument! (1)" << endl;
		return nullptr;
	}
	if(!graph_in2)
	{
		cout << "Error: invalid graph argument! (2)" << endl;
		return nullptr;
	}
	if(!graph_out)
	{
		cout << "Error: invalid graph argument! (3)" << endl;
		return nullptr;
	}
	
	*graph_out = (*graph_in1) * (*graph_in2);
	return graph_out;
}

Graph* graphComplement(Graph* graph_in, Graph* graph_out)
{
	if(!graph_in)
	{
		cout << "Error: invalid graph argument! (1)" << endl;
		return nullptr;
	}
	if(!graph_out)
	{
		cout << "Error: invalid graph argument! (2)" << endl;
		return nullptr;
	}
	
	*graph_out = !(*graph_in);
	return graph_out;
}

#endif