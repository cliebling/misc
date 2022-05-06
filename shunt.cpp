#include <iostream>
#include <string>
#include <stack>
#include <queue>

/*
 * Implementation of the "shunting yard" algorithm
 * Author: C. Liebling
 * 2022-05-06
 */

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
		case '^':
		case '(':
		case ')':
			return true;
		break;
		default:
			return false;
		break;
	}
}

char precedent(char op)
{
	switch(op)
	{
		case '+':
		case '-':
			return 1;
			break;
		case '*':
		case '/':
			return 2;
			break;
		case '^':
			return 3;
		default:
			return 0;
			break;
	}

}

bool isLeftAssociative(char op)
{
	switch(op)
	{
		case '+':
		case '-':
		case '*':
		case '/':
			return true;
			break;
		case '^':
			return false;
			break;
		default:
			return true;
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
	std::string stackTop;
	tmpNumber = "";
	for(i = 0; i < infix.length(); i++)
	{
		if(isOperator(infix[i]))
		{
			if(infix[i] == '(')
			{
				tmpOperator = infix[i];
				opstack.push(tmpOperator);
				tmpOperator = "";
			}
			else if(infix[i] == ')')
			{
				if(opstack.empty())
				{
					/* unbalanced parenthesis */
					break;
				}
				stackTop = opstack.top();
				while(stackTop[0] != '(')
				{
					output.push(opstack.top());
					opstack.pop();
					stackTop = opstack.top();
				}
				opstack.pop();
			}
			else
			{
				if(opstack.empty())
				{
					tmpOperator = infix[i];
					opstack.push(tmpOperator);
					tmpOperator = "";
				}
				else
				{
					stackTop = opstack.top(); 
					if(opstack.empty())
					{
						tmpOperator = infix[i];
						opstack.push(tmpOperator);
						tmpOperator = "";
					}
					else if((precedent(infix[i]) > precedent(stackTop[0]))
						|| ((precedent(infix[i]) ==  precedent(stackTop[0]))
						&&  !isLeftAssociative(stackTop[0] )))
					{
						tmpOperator = infix[i];
						opstack.push(tmpOperator);
						tmpOperator = "";
					}
					else if((precedent(infix[i]) < precedent(stackTop[0]))
						|| ((precedent(infix[i]) == precedent(stackTop[0]))
						&& isLeftAssociative(stackTop[0])))
					{
						while((precedent(infix[i]) <= precedent(stackTop[0]))
						|| ((precedent(infix[i]) == precedent(stackTop[0]))
						&& isLeftAssociative(stackTop[0])))
						{
							if(opstack.empty())
							{
								break;
							}
							output.push(opstack.top());
							opstack.pop();
							if(opstack.empty())
							{
								break;
							}
							stackTop = opstack.top();
						}
						tmpOperator = infix[i];
						opstack.push(tmpOperator);
						tmpOperator = "";
					}
				}
			}
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
