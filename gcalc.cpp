#include "gcalc.h"

Graph Gcalc::defNewGraph(vector<string> tokens, unsigned int begin)
{
	vector<string>::iterator first_ver_pos, last_ver_pos, first_couple, end, itr_begin = tokens.begin();
	bool separator_found = false;
	Graph graph("", set<Vertex>());
	unsigned int counter = 0;

	while (counter != begin) {
		itr_begin++;
		counter++;
	}
	//locate special key posisions
	for (vector<string>::iterator itr = itr_begin ; itr != tokens.end(); itr++ ) {
		if (*itr == "{")
		{
			first_ver_pos = itr + 1;
		}

		if (*itr == "|") {
			separator_found = true;
			last_ver_pos = itr - 1;
			first_couple = itr + 1;
		}
		
		if (*itr == "}") {
			if (!separator_found)
				last_ver_pos = itr - 1;
			
			end = itr;
			break;
		}
	}

	//add vertices 
	bool comma = true;
	for (auto itr = first_ver_pos; itr != (last_ver_pos + 1); itr++) {
		if (*itr == ",") {
			comma = true;
			continue;
		}
		else {
			if (checkVertexName(*itr) == gcalcreturn::ILLEGAL_NAME || comma == false) {
				throw NameErr();
			}
			else {
				graph.addVertex(*itr);
				comma = false;
			}
		}
	}

	//add edges
	if (separator_found) {
		bool open = false;
		comma = false;
		string from, to;
		for (auto itr = first_couple; itr != end; itr++) {
			if (*itr == "<") {
				open = true;
				continue;
			}

			if (*itr == ">") {
				graph.addNeighbour(from, to);
				open = false;
				continue;
			}

			if (*itr == ",") {
				if (open) {
					comma = true;
				}
				continue;
			}

			if (open) {
				if (!comma) { //vertex before comma
					from = *itr;
				}

				else { //vertex after comma, add to neighbours of one before comma
					to = *itr;
					comma = false;
				}
			}
		}
	}
	return graph;
}

void Gcalc::saveGraph(Graph g, const std::string& filename)
{
	std::ofstream outfile(filename, std::ios_base::binary);
	if (!outfile) {
		throw "Can not open file for write";
	}
	unsigned int num_of_vertices = g.getVertices().size();
	unsigned int num_of_edges = g.getNumOfEdges();
	
	outfile.write((const char*)&(num_of_vertices), sizeof(unsigned int));
	outfile.write((const char*)&(num_of_edges), sizeof(unsigned int));
	
	for (auto vertex : g.getVertices()) {
		const std::string& name = vertex.getName();
		const unsigned int length = vertex.getName().length();
		outfile.write((const char*)&(length), sizeof(unsigned int));
		outfile.write((const char*)&(name[0]), length*sizeof(char));
	}
	for (auto vertex : g.getVertices()) {
		for (auto j : vertex.getNeighbours()) {
			const std::string first_name = vertex.getName();
			const unsigned int first_length = vertex.getName().length();
			const std::string second_name = j;
			const unsigned int second_length = (j).length();
			outfile.write((const char*)&(first_length), sizeof(unsigned int));
			outfile.write((const char*)&(first_name[0]), first_name.size());
			outfile.write((const char*)&(second_length), sizeof(unsigned int));
			outfile.write((const char*)&(second_name[0]), second_name.size());
		}
	}
	outfile.close();

}

Graph Gcalc::loadGraph(const std::string& filename)
{
	std::ifstream infile(filename, std::ios_base::binary);
	if (!infile) {
		cout << "Error: an error has occured when finding/opening the input file" << endl;
		exit(0);
	}
	Graph g("",set<Vertex>());
	unsigned int num_of_vertices;
	unsigned int num_of_edges;
	infile.read((char*)&num_of_vertices, sizeof(unsigned int));
	infile.read((char*)&num_of_edges, sizeof(unsigned int));

	while (num_of_vertices != 0) {
		unsigned int length;
		std::string vername;
		infile.read((char*)&length, sizeof(unsigned int));
		vername.resize(length);
		infile.read((char*)&vername[0], length);
		g.addVertex(vername);
		num_of_vertices--;
	}
	while (num_of_edges != 0) {
		unsigned int first_length;
		std::string first_name;
		unsigned int second_length;
		std::string second_name;
		infile.read((char*)&first_length, sizeof(unsigned int));
		first_name.resize(first_length);
		infile.read((char*)&first_name[0], first_length);
		infile.read((char*)&second_length, sizeof(unsigned int));
		second_name.resize(second_length);
		infile.read((char*)&second_name[0], second_length);
		g.addNeighbour(first_name, second_name);
		num_of_edges--;
	}
	infile.close();
	return g;
}


void Gcalc::whoCmd(ostream& os)
{
	for (std::set<Graph>::iterator itr = graphs.begin(); itr != graphs.end(); ++itr) {
		os << itr->getName() << endl;
	}
}

void Gcalc::deleteCmd(vector<string> input){
	//example: delete ( graph_name )
	if (input.size() != 4 ) {
		throw SyntaxErr();
	}
	if (input[1] != "(" || input[3] != ")") {
		throw SyntaxErr();
	}
	
	for (std::set<Graph>::iterator itr = graphs.begin(); itr != graphs.end(); ++itr) {
		if (itr->getName() == input[2]) {
			graphs.erase(*itr);
			return;
		}
	}

	//graph not found
	throw GraphErr();
}

void Gcalc::resetCmd()
{
	graphs.clear();
}

void Gcalc::printCmd(vector<string> input, ostream& os)
{
	if (input.size() < 4) {
		throw SyntaxErr();
	}
	if (input[1] != "(" || *(input.end() - 1) != ")") {
		throw SyntaxErr();
	}
	input.erase(input.begin()); //remove print command
	os << convertInfixToPostfixAndRPN(input);
}

void Gcalc::quitCmd()
{
	exit(0);
}


gcalcreturn Gcalc::checkVertexName(string& input)
{
	string legalchars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"; //characters allowed in name
	auto first_illegal = input.find_first_not_of(legalchars);
	auto last_illegal = input.find_last_not_of(legalchars);
	auto illegal = input.find_first_not_of(legalchars);

	if (isdigit(input[0])) { //checks if first character of the name is a number 1234567890
		return gcalcreturn::ILLEGAL_NAME;
	}

	if (first_illegal != std::string::npos && input[first_illegal] != '[') { //if found illegal character (] and ; included)
		return gcalcreturn::ILLEGAL_NAME;
	}

	if (last_illegal != std::string::npos && input[last_illegal] != ']') {//if found illegal (included [ ;)
		return gcalcreturn::ILLEGAL_NAME;
	}

	//iterate over string
	while (illegal != string::npos) {
		if (input[illegal] != '[' || input[illegal] != ']' || input[illegal] != ';') {
			return gcalcreturn::ILLEGAL_NAME;
		}
		illegal = input.find_first_not_of(legalchars, illegal + 1);
	}
	if (checkifParanthesisAreBalanced(input, '[', ']') == gcalcreturn::BALANCED) //[] is a legal name
			return gcalcreturn::LEGAL_NAME;
	else {
		return  gcalcreturn::ILLEGAL_NAME;
	}
}

gcalcreturn Gcalc::checkGraphName(string& input)
{
	string legalchars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"; //characters allowed in name
	std::size_t first_illegal = input.find_first_not_of(legalchars);

	if (isdigit(input[0])) {//checks if first character of the name is a number 1234567890
		return  gcalcreturn::ILLEGAL_NAME;
	}
	if (first_illegal != std::string::npos) { //if found illegal character
		return  gcalcreturn::ILLEGAL_NAME;
	}

	return  gcalcreturn::LEGAL_NAME;
}


gcalcreturn Gcalc::checkifParanthesisAreBalanced(string& input, char begin_char, char end_char)
{
	int parath_ctr = 0;
	for (unsigned int i = 0; i < input.length(); i++) {
		if (input[i] == begin_char) {
			parath_ctr++;
		}
		if (input[i] == end_char) {
			if (parath_ctr == 0) {
				return  gcalcreturn::IMBALANCED;
			}
			parath_ctr--;
		}
	}
	if (parath_ctr == 0) {
		return gcalcreturn::BALANCED;
	}

	return  gcalcreturn::IMBALANCED;
} 


std::string Gcalc::addSpacesForTokenization(string& input_str)
{
	string temp = input_str;
	int counter = 0;
	for (int i = 0; i < input_str.length(); i++){
		if ((input_str[i] == '+') || (input_str[i] == '-') || (input_str[i] == '*') || (input_str[i] == '^') || (input_str[i] == '=') ||
			(input_str[i] == '{') || (input_str[i] == '}') || (input_str[i] == '!') || (input_str[i] == '<') || (input_str[i] == '>') ||
			(input_str[i] == ',') || (input_str[i] == '|') || (input_str[i] == '(') || (input_str[i] == ')')) {

			temp.insert(i+counter, " "); //insert before the operator
			temp.insert(i + counter + 2, " "); //insert after operator
			counter+=2; //we add 2 spaces each time
		}
	}
	return temp;
}

std::vector<string> Gcalc::tokenizeInputString(string& input, char delim)
{
	vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	end = input.find(delim, start);
	while (end != std::string::npos) {
		tokens.push_back(input.substr(start, end - start));
		start = input.find_first_not_of(delim, end);
		end = input.find(delim, start);
	}

	if (start != std::string::npos) {
		tokens.push_back(input.substr(start));
		
	}

	return tokens;
}

void Gcalc::parser(vector<string> input, ostream& os)
{
	if (input[0] == "save") {
		if (input.size() < 4) { throw SyntaxErr(); }
	
		if (input[1] != "(" || *(input.end() - 1) != ")") { throw SyntaxErr(); }

		input.erase(input.begin()); //remove save command
		input.erase(input.begin()); //remove (
		string file_name = *(input.end() - 2);
		input.erase(input.end() - 1);  //remove ) command
		input.erase(input.end() - 1); //remove file_name
		input.erase(input.end() - 1); //remove comma
		saveGraph(convertInfixToPostfixAndRPN(input),file_name);
		return;
	}

	if (input[0] == "who") {
		if (input.size() > 1) { throw SyntaxErr(); }
		whoCmd(os);
		return;
	}

	if (input[0] == "reset") {
		if (input.size() > 1) { throw SyntaxErr(); }
		resetCmd();
		return;
	}

	if (input[0] == "print") {
		printCmd(input,os);
		return;
	}

	if (input[0] == "quit") {
		if (input.size() > 1) { throw SyntaxErr(); }
		quitCmd();
		return;
	}

	if (input[0] == "delete") {
		deleteCmd(input);
		return;
	}
	
	//input is "empty line" or "enter character \n"
	if (input.size() == 1 && input[0] == "") {
		return;
	}
	//found syntax err
	if (input.size() < 2) {
		throw SyntaxErr();
	}

	
	//defining a graph from expression + deals with "load"
	if (input[1] == "=") {
		if (checkGraphName(input[0]) == gcalcreturn::ILLEGAL_NAME) {
			throw NameErr();
		}

		Graph new_graph(input[0],set<Vertex>());

		//define new graph from expression 
		input.erase(input.begin()); //remove name
		input.erase(input.begin()); //remove "=" 
		new_graph = convertInfixToPostfixAndRPN(input); //        <---- shunting-yard + RPN

		if (graphs.find(new_graph) != graphs.end()) {//if found remove and replace later
			graphs.erase(graphs.find(new_graph));
		}
		graphs.insert(new_graph); //replace or insert if not found 

		return;
	}

	throw SyntaxErr();
}

Graph Gcalc::convertInfixToPostfixAndRPN(vector<string> input)
{
	map<string, Graph> temp_container = map<string, Graph>(); //a map which will hold the graphs we define by {} in the input
	vector<string>::iterator itr = input.begin();
	std::vector<string> output_queue;
	std::stack<string> operator_stack;
	int counter = 0; //dif name for temp container


	for (std::size_t i = 0; i < input.size(); i++) {
		if (input[i] == "{") {
			Graph new_g = defNewGraph(input, i);
			temp_container.insert({ "#" + to_string(counter),new_g }); //insert the new graph in the container with a "fake" name
			output_queue.push_back("#" + to_string(counter));
			counter++;
			while (input[i] != "}") {
				i++;
				if (i >= input.size()) {
					throw SyntaxErr();
				}
			}
			continue;
		}
		//define new graph from load
		if (input[i] == "load") {

			if (input[i + 1] != "(" || input[i + 3] != ")") { throw SyntaxErr(); }

			Graph new_g = loadGraph(input[i+2]); //send file name
			temp_container.insert({ "#" + to_string(counter),new_g }); //insert the new graph in the container with a "fake" name
			output_queue.push_back("#" + to_string(counter));
			counter++;
			i += 3; //skip the load command after dealing with it
			continue;
		}
		if (input[i] == "(") {
			operator_stack.push(input[i]);
			continue;
		}
		if (input[i] == ")") {
			while (operator_stack.top() != "(") {
				output_queue.push_back(operator_stack.top()); //push the operator to the output queue
				operator_stack.pop();
			}
			operator_stack.pop(); // pop the "(" 
			continue;
		}
		//is an operator
		if ((input[i] == "+") || (input[i] == "-") || (input[i] == "*") || (input[i] == "^")) {
			while (!operator_stack.empty() && (operator_stack.top() != "(")) {
				output_queue.push_back(operator_stack.top()); //push the operator to the output queue
				operator_stack.pop();
			}
			operator_stack.push(input[i]); //push the operator to the operator stack 
			continue;
		}
		if (input[i] == "!") {
			operator_stack.push(input[i]);
			continue;
		}

		//if found graph name
		output_queue.push_back(input[i]);
	}

	//no more tokens to read, pop all operators to output queue
	while (!operator_stack.empty()) {
		if (operator_stack.top() == "(") { //extra paranthesis found
			throw SyntaxErr();
		}
		output_queue.push_back(operator_stack.top()); //push the operator to the output queue
		operator_stack.pop();
	}
	//RPN 
	return RPN(output_queue,temp_container);
}

Graph Gcalc::RPN(vector<string> queue , map<string, Graph> graph_container)
{
	Graph graph("", set<Vertex>());
	Graph rhs("", set<Vertex>());
	Graph lhs("", set<Vertex>());
	string operation;
	stack<Graph> stk = stack<Graph>();

	for (vector<string>::iterator itr = queue.begin(); itr != queue.end(); itr++) {
		operation = *itr;

		switch (operation[0]) {
			case '+':
				if (stk.size() == 0 || stk.size() == 1) { throw SyntaxErr(); }
				rhs = stk.top();
				stk.pop();
				lhs = stk.top();
				stk.pop();
				stk.push(lhs + rhs);
				break;

			case '-':
				if (stk.size() == 0 || stk.size() == 1) { throw SyntaxErr(); }
				rhs = stk.top();
				stk.pop();
				lhs = stk.top();
				stk.pop();
				stk.push(lhs - rhs);
				break;


			case '*':
				if (stk.size() == 0 || stk.size() == 1) { throw SyntaxErr(); }
				rhs = stk.top();
				stk.pop();
				lhs = stk.top();
				stk.pop();
				stk.push(lhs * rhs);
				break;

			case '^':
				if (stk.size() == 0 || stk.size() == 1) { throw SyntaxErr(); }
				rhs = stk.top();
				stk.pop();
				lhs = stk.top();
				stk.pop();
				stk.push(lhs ^ rhs);
				break;

			case '!':
				if (stk.size() == 0) { throw SyntaxErr(); }
				rhs = stk.top();
				stk.pop();
				stk.push(!rhs);
				break;

			default: //graph name
				if (graph_container.find(*itr) != graph_container.end()) { // if found graph with fake name
					stk.push(graph_container.find(*itr)->second); //push graph to stack
				}
				else {//find in gcalc graph set
					if (graphs.find(Graph(*itr,set<Vertex>())) != graphs.end()) { //if found graph 
						stk.push(*graphs.find(Graph(*itr, set<Vertex>()))); //push graph to stack
					}
					else {
						throw GraphErr();
					}
				}
		}
	}
	
	if (stk.size() == 0 || stk.size() > 1) { throw SyntaxErr();  } //just for caution
	graph = stk.top();
	return graph;
}


