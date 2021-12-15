#include "graph.h"
#include "gcalc.h"
#include "Exception.h"
#include <iostream>
#include <fstream> // file stream for opening and closing files

using namespace std;

int main(int argc, char** argv) {
	
	if (argc == 1) {
		Gcalc gcalc = Gcalc();
		string input_line;
		
		while (true) {
			cout << "Gcalc> ";
			if (getline(std::cin, input_line).eof()) { //scan the user input
				exit(0); //found EOF
			}

			//tokenize the input
			input_line = gcalc.addSpacesForTokenization(input_line);
			std::vector<string> input_vec = gcalc.tokenizeInputString(input_line, ' '); //tokenize by space delim

			//take vector of strings and execute according command
			try {
				gcalc.parser(input_vec, std::cout);
			}
			catch (const std::exception & e) {
				std::cout << e.what() << endl;
			}
		}
	}
	if(argc == 3) { //batch mode
		std::ifstream in(argv[1]); //creating input file
		std::ofstream out(argv[2]); //creatinf output file

		//good(): Returns true if none of the stream's error state flags (eofbit, failbit and badbit) is set.
		if (out.good() == false)
			std::cerr << "Error: an error has occured when creating the output file " << endl;
		if (in.good() == false) {
			out << "Error: an error has occured when finding/opening the input file" << endl; //write the error to the file
			std::cout << "Error: an error has occured when finding/opening the input file" << endl; //write the error to the cmd
		}

		Gcalc gcalc = Gcalc();
		while (in.good())
		{
			string input_line;
			std::getline(in, input_line);

			//tokenize the input
			input_line = gcalc.addSpacesForTokenization(input_line);
			std::vector<string> input_vec = gcalc.tokenizeInputString(input_line, ' '); //tokenize by space delim

			//take vector of strings and execute according command
			try {
				gcalc.parser(input_vec, out);  //write output to the output file
			}
			catch (const std::exception & e) {
				out << e.what() << endl; //write errors to the output file
			}
		}

	}
	else { //invalid argument count
		cout << "Error: invalid number of arguments" << endl;
	}
	return 0;
}


