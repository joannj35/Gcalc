#pragma once

#include <iostream>

class Exception : public std::exception {
	protected:
		std::string err_msg;
	public:
		const char* what() const noexcept override;
};

class SyntaxErr : public Exception {
	public:
		explicit SyntaxErr(); //c'tor
		const char* what() const noexcept override;
};

class NameErr : public Exception {
	public:
		explicit NameErr(); //c'tor
		const char* what() const noexcept override;

};

class GraphErr : public Exception {
	public:
		explicit GraphErr(); //c'tor
		const char* what() const noexcept override;

};

class EdgeErr : public Exception {
	public:
		explicit EdgeErr(); //c'tor
		const char* what() const noexcept override;

};