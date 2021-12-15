#include "graph.h"
#include "Exception.h"

//copy contructor
Graph::Graph(const Graph& graph)
{
	this->name = graph.name;
	this->vertices = graph.vertices;
}

void Graph::addVertex(string name)
{
	Vertex new_ver(name, set<string>());

	if (vertices.find(new_ver) != vertices.end()) {
		throw NameErr();
	}
	//no need to check if name is legal
	vertices.insert(new_ver);
}

void Graph::addNeighbour(string from, string to)
{
	Vertex temp(from, set<string>());
	Vertex temp2(to, set<string>());
	set<Vertex>::iterator found_from = vertices.find(temp);
	if (found_from != vertices.end()){//if found from
		if(vertices.find(temp2) == vertices.end()){//if didnt find to
			throw NameErr();
		}
		if (from == to) {
			throw NameErr();
		}
		//to override the const flag in found
		temp = *found_from;
		temp.addNeighbour(to);
		vertices.erase(found_from);
		vertices.insert(temp);

	}else { //if didnt find from
		throw NameErr();
	}
}

void Graph::removeVertex(Vertex vertex)
{
	vertices.erase(vertex);
}

void Graph::setName(string& new_name)
{
	name = new_name;
}

set<Vertex> Graph::getVertices()
{
	return vertices;
}

Graph& Graph::operator=(const Graph& rhs_graph)
{
	if (&rhs_graph != this) { //to make sure we're not doing g1=g1 
		/*this->name = rhs_graph.name; removed to prevent overwriting the name*/
		this->vertices = rhs_graph.vertices;
	}
	return *this;
}

//add all vertices to the union graph - iterate over all vertices and insert them in vertices set in union graph
//for each vertex, use the vertex union function and determine all union neighbours
Graph Graph::operator+(const Graph& rhs_graph)
{
	string union_name("");//empty, wont be used later anyways in operator =
	std::set<Vertex> uni_vertices_set; //empty set of vertices
	Graph union_graph(union_name, uni_vertices_set);

	for (std::set<Vertex>::iterator itr = (rhs_graph.vertices).begin(); itr != (rhs_graph.vertices).end(); ++itr) {
		union_graph.vertices.insert(*itr); //inserted all vertices of rhs vertex
	}

	for (std::set<Vertex>::iterator itr = (this->vertices).begin(); itr != (this->vertices).end(); ++itr) {
		std::set<Vertex>::iterator tmp = rhs_graph.vertices.find(*itr);
		if (tmp != rhs_graph.vertices.end()) { //if found
			union_graph.vertices.erase(*tmp);
			union_graph.vertices.insert(*tmp + *itr);
		}
		else {
			union_graph.vertices.insert(*itr); //insert all vertices of lhs vertex
		}
	}
	return union_graph;
}

Graph Graph::operator^(Graph const& rhs_graph)
{
	string inter_graph_name("");
	std::set<Vertex> inter_vertices;
	Graph inter_graph(inter_graph_name, inter_vertices);

	for (std::set<Vertex>::iterator itr = vertices.begin(); itr != vertices.end(); ++itr) {
		inter_graph.vertices.insert(*itr); //copy all neighbours of lhs
	}

	//eliminate the incommon vertices
	//iterate over the lhs set, erase vertices of graph if dont exists in rhs vertices set too 
	for (std::set<Vertex>::iterator itr = vertices.begin(); itr != vertices.end(); ++itr) {
		if (rhs_graph.vertices.find(*itr) == rhs_graph.vertices.end()) { //if not found before end, erase vertex
			inter_graph.vertices.erase(*itr);
		}
		else {//found
			std::set<Vertex>::iterator tmp = rhs_graph.vertices.find(*itr);
			inter_graph.vertices.erase(*tmp);
			inter_graph.vertices.insert(*tmp ^ *itr); //update the intersection edges 
		}
	}
	return inter_graph;
}

Graph Graph::operator-(const Graph& rhs_graph)
{
	
	Graph diff_graph("", set<Vertex>());

	for (std::set<Vertex>::iterator itr = vertices.begin(); itr != vertices.end(); ++itr) {
		diff_graph.vertices.insert(*itr); //copy all vertices of lhs
	}

	//(eliminate the common vertices)
	//iterate over the lhs set, erase vertices of graph if exists in rhs neighbour too 
	for (std::set<Vertex>::iterator itr = vertices.begin(); itr != vertices.end(); ++itr) {
		if (rhs_graph.vertices.find(*itr) != rhs_graph.vertices.end()) { //if found before end, erase vertex
			diff_graph.vertices.erase(*itr);
			
			for (std::set<Vertex>::iterator itr2 = vertices.begin(); itr2 != vertices.end(); ++itr2) {
				if (diff_graph.vertices.find(*itr2) != diff_graph.vertices.end()) { //
					Vertex ver_in_diff = *diff_graph.vertices.find(*itr2);
					diff_graph.vertices.erase(ver_in_diff);
					diff_graph.vertices.insert(ver_in_diff - *itr); //update the diff edges 
				}
			}
		}
	}
	return diff_graph;
}

Graph Graph::operator*(const Graph& rhs_graph)
{
	string name("");
	std::set<Vertex> prod_vertices = set<Vertex>();
	Graph prod_graph(name, prod_vertices);

	for (std::set<Vertex>::iterator lhs_itr = vertices.begin(); lhs_itr != vertices.end(); ++lhs_itr) {
		for (std::set<Vertex>::iterator rhs_itr = (rhs_graph.vertices).begin(); rhs_itr != (rhs_graph.vertices).end(); ++rhs_itr) {
			prod_graph.vertices.insert(*lhs_itr * *rhs_itr);
		}
	}
	return prod_graph;
}

Graph Graph::operator!()
{
	Graph comp_graph(*this); //name doesnt matter, vertices dont change with the complementary graph
	for (std::set<Vertex>::iterator itr = vertices.begin(); itr != vertices.end(); ++itr) {
		Vertex temp = itr->setComplementNeighbours(vertices);
		comp_graph.vertices.erase(*itr);
		comp_graph.vertices.insert(temp);
	}
	return comp_graph;
}
std::string Graph::getName() const
{
	return name;
}

int Graph::getNumOfEdges()
{
	int number = 0;
	for (auto itr : vertices) {
		number += itr.getNeighbours().size();
	}

	return number;
}

//copy constructor
Vertex::Vertex(const Vertex& vertex) {
	this->name = vertex.name;
	this->neighbours = vertex.neighbours;
}

void Vertex::addNeighbour(string neighbour_name)
{
	auto found = neighbours.find(neighbour_name);

	if (found == neighbours.end()) //if didnt find goal vertex
		neighbours.insert(neighbour_name);
	else 
		throw EdgeErr();
}

void Vertex::removeNeighbour(string neighbour_name)
{
	neighbours.erase(neighbour_name);
}

Vertex Vertex::setComplementNeighbours(set<Vertex> og_neighbours) const
{
	Vertex temp(*this);

	for (std::set<Vertex>::iterator itr = og_neighbours.begin(); itr != og_neighbours.end(); ++itr) {
		if (temp.neighbours.find(itr->name) != temp.neighbours.end()) { //if found
			temp.neighbours.erase(itr->name);
		}
		else {
			if (itr->name != name)
				temp.neighbours.insert(itr->name);
		}
	}
	return temp;
}

std::set<string> Vertex::getNeighbours()
{
	return neighbours;
}

string Vertex::getName() const
{
	return name;
}

Vertex& Vertex::operator=(const Vertex& rhs_vertex)
{
	if (&rhs_vertex != this) {
		this->name = rhs_vertex.name;
		this->neighbours = rhs_vertex.neighbours;
	}
	return *this;
}

//returns elemnt union_vertex 
Vertex Vertex::operator+(const Vertex& curr_vertex) const
{
	string union_vertix(curr_vertex.name);
	std::set<string> union_neighbours;
	Vertex uni_ver(union_vertix, union_neighbours);

	//iterate of the vertex given from a specific graph, and insert its neighbours (mark and add the corresponding edges)
	//in the current union vertex's neighbours set
	for (std::set<string>::iterator itr = (curr_vertex.neighbours).begin(); itr != (curr_vertex.neighbours).end(); ++itr) {
		uni_ver.neighbours.insert(*itr); //inserted all neighbours of rhs vertex
	}

	for (std::set<string>::iterator itr = (this->neighbours).begin(); itr != (this->neighbours).end(); ++itr) {
		uni_ver.neighbours.insert(*itr); //insert all neighbours of lhs vertex
	}

	return uni_ver;
}

Vertex Vertex::operator^(const Vertex& curr_vertex) const
{
	string inter_vertix(curr_vertex.name); //rhs ver name is the same as lhs
	std::set<string> inter_neighbours;
	Vertex inter_ver(inter_vertix, inter_neighbours);

	for (std::set<string>::iterator itr = neighbours.begin(); itr != neighbours.end(); ++itr) {
		inter_ver.neighbours.insert(*itr); //copy all neighbours of lhs
	}

	//iterate over the lhs(this) set, erase neighbour of vertex if dont exists in rhs neighbour too (eliminate the edges)
	for (std::set<string>::iterator itr = neighbours.begin(); itr != neighbours.end(); ++itr) {
		if (curr_vertex.neighbours.find(*itr) == curr_vertex.neighbours.end()) { //if not found before end, erase edge
			inter_ver.neighbours.erase(*itr);
		}
	}
	return inter_ver;
}

Vertex Vertex::operator-(const Vertex& curr_vertex) const
{
	std::set<string> diff_neighbours = std::set<std::string>();
	Vertex diff_ver(name, diff_neighbours);

	for (std::set<string>::iterator itr = neighbours.begin(); itr != neighbours.end(); ++itr) {
		diff_ver.neighbours.insert(*itr); //copy all neighbours of lhs
	}

	//Vertex diff_ver(*this);
	// eliminate the edge
	std::set<std::string>::iterator found = diff_ver.neighbours.find(curr_vertex.name);
	if (found != diff_ver.neighbours.end()) { //if found before end, erase edge
		diff_ver.neighbours.erase(curr_vertex.name);
	}

	return diff_ver;
}


Vertex Vertex::operator*(const Vertex& curr_vertex) const
{
	string prod_vertix('[' + name + ';' + curr_vertex.name + ']'); //[name;ver.name]
	std::set<string> prod_neighbours;
	Vertex prod_ver(prod_vertix, prod_neighbours);

	for (std::set<string>::iterator lhs_itr = (this->neighbours).begin(); lhs_itr != (this->neighbours).end(); ++lhs_itr) {
		for (std::set<string>::iterator rhs_itr = (curr_vertex.neighbours).begin(); rhs_itr != (curr_vertex.neighbours).end(); ++rhs_itr) {
			string couple = '[' + *lhs_itr + ';' + *rhs_itr + ']';
			prod_ver.neighbours.insert(couple);
		}
	}
	return prod_ver;
}


bool Vertex::operator==(const Vertex& rhs_vertex) const
{
	if (this->name == rhs_vertex.name) return true;
	return false;
}

bool Vertex::operator!=(const Vertex& rhs_vertex) const
{
	return !(*this == rhs_vertex);
}

bool Vertex::operator<(const Vertex& rhs_vertex) const
{
	return name < rhs_vertex.name;
}

bool Vertex::operator>(const Vertex& rhs_vertex) const
{
	return name > rhs_vertex.name;
}

bool Vertex::operator<=(const Vertex& rhs_vertex) const
{
	return !(*this > rhs_vertex);
}

bool Vertex::operator>=(const Vertex& rhs_vertex) const
{
	return !(*this < rhs_vertex);
}

ostream& operator<<(ostream& os, Vertex vertex)
{
	for (std::set<string>::iterator itr = vertex.neighbours.begin(); itr != vertex.neighbours.end(); ++itr) {
		os << vertex.name << " " << *itr << endl;
	}
	return os;
}

ostream& operator<<(ostream& os, Graph graph)
{
	for (std::set<Vertex>::iterator itr = graph.vertices.begin(); itr != graph.vertices.end(); ++itr) {
		os << itr->getName() << endl;
	}

	os << "$" << endl;

	for (std::set<Vertex>::iterator itr = graph.vertices.begin(); itr != graph.vertices.end(); ++itr) {
		os << *itr; //prints neighbours
	}
	return os;
}

bool Graph::operator==(const Graph& rhs_graph) const {
	return name == rhs_graph.name;
}

bool Graph::operator!=(const Graph& rhs_graph) const {
	return !(rhs_graph == *this);
}

bool Graph::operator<(const Graph& rhs_graph) const {
	return name < rhs_graph.name;
}

bool Graph::operator>(const Graph& rhs_graph) const {
	return rhs_graph < *this;
}

bool Graph::operator<=(const Graph& rhs_graph) const {
	return !(rhs_graph < *this);
}

bool Graph::operator>=(const Graph& rhs_graph) const {
	return !(*this < rhs_graph);
}
