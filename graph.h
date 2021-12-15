#pragma once

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <set>
#include <string>

using namespace std;

class Vertex {
	string name;
	set<string> neighbours; //edges

public:
	Vertex(const string& name, const std::set<string>& neighbours) : name(name), neighbours(neighbours) {}; // constructor
	~Vertex() = default; //destructor
	Vertex(const Vertex& vertex); //copy constructor

	void addNeighbour(string neighbour_name);
	void removeNeighbour(string neighbour_name);
	Vertex setComplementNeighbours(set<Vertex> og_neighbours) const;
	set<string> getNeighbours(); //returns the set of edges (obj)
	string getName() const;

	//operators
	bool operator==(const Vertex& rhs_vertex) const;
	bool operator!=(const Vertex& rhs_vertex) const;
	bool operator<(const Vertex& rhs_vertex) const;
	bool operator>(const Vertex& rhs_vertex) const;
	bool operator<=(const Vertex& rhs_vertex) const;
	bool operator>=(const Vertex& rhs_vertex) const;

	Vertex& operator=(const Vertex& rhs_vertex);
	Vertex operator+(const Vertex& curr_vertex) const; //union
	Vertex operator^(const Vertex& curr_vertex) const; //intersection
	Vertex operator-(const Vertex& curr_vertex) const; //difference
	Vertex operator*(Vertex const& curr_vertex) const; //product
	//Vertex operator!(); no need, implemented in graphs

	friend ostream& operator<<(ostream& os, Vertex vertex);

};

class Graph {
	string name;
	set<Vertex> vertices;

public:
	Graph(const string& name = "", const std::set<Vertex>& vertices = std::set<Vertex>()) : name(name), vertices(vertices) {}; // constructor
	~Graph() = default;  //destructor
	Graph(const Graph& graph); //copy constructor

	void addVertex(string name);
	void addNeighbour(string from, string to);
	void removeVertex(Vertex vertex);
	void setName(string& new_name);
	set<Vertex> getVertices();
	std::string getName() const;
	int getNumOfEdges();
	

	//operators
	bool operator==(const Graph& rhs_graph) const;
	bool operator!=(const Graph& rhs_graph) const;
	bool operator<(const Graph& rhs_graph) const;
	bool operator>(const Graph& rhs_graph) const;
	bool operator<=(const Graph& rhs_graph) const;
	bool operator>=(const Graph& rhs_graph) const;

	Graph& operator=(const Graph& rhs_graph);
	Graph operator+(const Graph& rhs_graph); //union
	Graph operator^(const Graph& rhs_graph); //intersection
	Graph operator-(const Graph& rhs_graph); //differernce
	Graph operator*(const Graph& rhs_graph); //product
	Graph operator!(); //complement


	friend ostream& operator<<(ostream& os, Graph graph);



};


#endif