#pragma once

#ifndef GCALC_H
#define GCALC_H


using namespace std;

#include <iostream>
#include <set>
#include <string>
#include <cctype>
#include <vector>
#include <stack>
#include <map>
#include <fstream>
#include "Exception.h"
#include "graph.h"

enum class gcalcreturn { LEGAL_NAME, ILLEGAL_NAME , BALANCED , IMBALANCED };

class Gcalc
{	
	set<Graph>  graphs;

	public:
		Gcalc() :graphs(std::set<Graph>()) {}; //constructs with an empty set of graphs
		~Gcalc() = default; //destructor

		/*adds spaces to string in order to make tokenizing easier later on */
		std::string addSpacesForTokenization(string& input_str);

		/*tokenization: splitting a string by delimiters*/
		std::vector<string> tokenizeInputString(string& input, char delim);

		/*shell commands*/
		void whoCmd(ostream& os);   //who command
		void quitCmd();  //quit command
		void resetCmd(); //reset command
		void deleteCmd(vector<string> input); //delete command
		void printCmd(vector<string> input, ostream& os); //print command

		/*takes vector of strings and executes commands*/
		void parser(vector<string> input, ostream& os);

		/*takes an infix vector of tokens and converts to a postfix vector of tokens then executes RPN algorithm
		  returns a graph to be updated later on.
		*/
		Graph convertInfixToPostfixAndRPN(vector<string> input);
		Graph RPN(vector<string> queue , map<string, Graph>graph_container);

		/*checks if the string input is legal
		 - graph/vertex names
		 - regular expressions: after/before each '[' must have ']' also ';' must be between '[' ']' 
		 */
		gcalcreturn checkVertexName(string& input);
		gcalcreturn checkGraphName(string& input);
		gcalcreturn checkifParanthesisAreBalanced(string& input, char begin_char, char end_char); //regular explession
		
		/*defines new graph and sets the vertices and edges from the tokenized string input between brackets { }*/
		Graph defNewGraph(vector<string> tokens, unsigned int begin);

		/*save + load graphs from file*/
		void saveGraph(Graph g, const std::string& filename);
		Graph loadGraph(const std::string& filename);
};

#endif // !GCALC_H