#include <iostream>
#include <string>
#include <stack>
#include <queue>


bool isNumOrPoint(char c)
{
	if((c >= '0') && (c <= '9'))
	{
		return true;
	}
	else if(c == '.')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isOperator(char c)
{
	switch(c) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '(':
		case ')':
			return true;
		break;
		default:
			return false;
		break;
	}
}


std::string infix2postfix(std::string infix)
{
	int32_t i;
	std::queue<std::string> output;
	std::stack<std::string> opstack;
	std::string tmpNumber;
	std::string tmpOperator;
	std::string postfix;
	tmpNumber = "";
	for(i = 0; i < infix.length(); i++)
	{
		if(isOperator(infix[i]))
		{
			tmpOperator = infix[i];
			opstack.push(tmpOperator);
			tmpOperator = "";
		}
		if(isNumOrPoint(infix[i]))
		{
			while(isNumOrPoint(infix[i]))
			{
				tmpNumber += infix[i];
				i++;
			}
			output.push(tmpNumber);
			tmpNumber = "";
		}
	}
	while(!opstack.empty())
	{
		output.push(opstack.top());
		opstack.pop();
	}
	while(!output.empty())
	{
		postfix += output.front();
		output.pop();
		postfix += " ";
	}
	return postfix;
}

int main(void)
{
	std::string infix;
	std::getline(std::cin, infix);
	std::cout<<infix2postfix(infix)<<std::endl;
	return 0;
}
