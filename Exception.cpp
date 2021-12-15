#include "Exception.h"

const char* Exception::what() const noexcept
{
	return "Error: ";
}

SyntaxErr::SyntaxErr()
{
	err_msg = std::string(Exception::what()) + "Syntax Error In Command Input" ;
}

const char* SyntaxErr::what() const noexcept
{
	return err_msg.c_str();
}

NameErr::NameErr()
{
	err_msg = std::string(Exception::what()) + "Illegal Graph/Vertex Name";
}

const char* NameErr::what() const noexcept
{
	return err_msg.c_str();
}

GraphErr::GraphErr()
{
	err_msg = std::string(Exception::what()) + "Graph Doesnt Exist";
}

const char* GraphErr::what() const noexcept
{
	return err_msg.c_str();
}

EdgeErr::EdgeErr()
{
	err_msg = std::string(Exception::what()) + "Cannot Place Edge";
}

const char* EdgeErr::what() const noexcept
{
	return err_msg.c_str();
}
