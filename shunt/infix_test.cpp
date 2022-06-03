#include "shunt.hpp"
#include <iostream>
#include <string>

int main(void)
{
	std::string infix;
    std::cout<<"Enter infix expression. +, -, *, /, ^, and () are allowed.\n";
	std::getline(std::cin, infix);
    std::cout<<"Infix: "<<infix<<std::endl;
    std::cout<<"Postfix: <<"<<infix2postfix(infix)<<std::endl;
	std::cout<<"Evaluates to: "<<postfixEvaluator(infix2postfix(infix))<<std::endl;
	return 0;
}
