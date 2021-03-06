#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <cmath>
#include <string.h>
#include "shunt.h"

/*
 * Implementation of the "shunting yard" algorithm
 * Author: C. Liebling
 * 2022-05-06
 *
 *
 * This implements the "shunting yard" algorithm for:
 * Addition
 * Subtraction
 * Multiplication
 * Division
 * Exponentiation
 * of:
 * Integers and floating point, with the result presented
 * as a single-precision floating point value.
 *
 * It is a relatively naive algorithm and does not handle
 * anything exotic. This is a product of time constraints.
 *
 * Formatting may contain spaces or not, e.g.
 * 3 + 6 / 7
 * 3+6/7
 * ( 3 + 7 ) ^ 2.5
 * (3+7)^2.5
 *
 * Known issues:
 * 1. This implementation does not handle unary negation,
 * i.e. "-6 + 3" does not work.
 * 2. Floating point numbers work, but they are limited to
 * single precision.
 * 3. Incorrectly formatted numbers will cause problems, e.g.
 * 123.456.789 or 123a2.4 (the latter will become two numbers,
 * 123 and 2.4 without an operator
 *
 */

/*
 * Function: isNumOrPoint
 * Checks if a character is 0-9 or .
 */
static bool isNumOrPoint(char c)
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

/* Function: isOperator
 * Checks if a character is an arithmetic operator
 */
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

/* Function: precedent
 * Allows the comparison of operator precedent
 */
static char precedent(char op)
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

/* 
 * Function: isLeftAssociative
 * Checks if an arithmetic operator is left associative
 */
static bool isLeftAssociative(char op)
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

/*
 * Function: infix2postfix
 * Argument(s):
 * std::string infix: A string representing an infix
 * arithmetic expression consisting of integer 
 * or floating point numbers and the operators
 * +, -, *, /, and ^
 * Returns:
 * A string showing the expression in postfix
 */

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
						{	/* No operators left? */
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
			/* Process numbers */
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
		/* build result string */
		postfix += output.front();
		output.pop();
		postfix += " ";
	}
	return postfix;
}

static float operate(float a, float b, char op)
{
	switch(op) {
		case '+':
			return a + b;
			break;
		case '-':
			return a - b;
			break;
		case '*':
			return a * b;
			break;
		case '/':
			return a / b;
			break;
		case '^':
			return pow(a, b);
			break;
		default:
			return a;
	}
}

/* Function: postfixEvaluator
 * Evaluates a postfix formatted string
 * Supports floating point
 */
float postfixEvaluator(std::string postfix)
{
	std::stack<float> operands;
	std::string tmpNumber;
	float opA;
	float opB;
	float result;
	result = 0.0f;
	for(int i = 0; i < postfix.length(); i++)
	{
		if(isNumOrPoint(postfix[i]))
		{
			while(isNumOrPoint(postfix[i]))
			{
				tmpNumber += postfix[i];
				i++;
			}
			operands.push(std::stof(tmpNumber, NULL));
			tmpNumber.clear();
		}
		else if(isOperator(postfix[i]))
		{
			opB = operands.top();
			operands.pop();
			opA = operands.top();
			operands.pop();
			operands.push(operate(opA, opB, postfix[i]));
		}
	}
	result = operands.top();
	return result;
}


extern "C" float c_postfixEvaluator(char *postfix)
{
    std::string postfixString(postfix);
    return postfixEvaluator(postfixString);
}


extern "C" int c_infix2postfix(char *infix, char *postfix, size_t buffer_length)
{
    if((infix == NULL) || (postfix == NULL))
    {
        return -1;
    }
    std::string infixString(infix);
    std::string postfixString;
    if(infixString.length() > MAX_BUFFER_LENGTH)
    {
        return -1;
    }
    postfixString = infix2postfix(infixString);
    strncpy(postfix, postfixString.c_str(), MAX_BUFFER_LENGTH);
    return 0;
}


/*
int main(void)
{
	std::string infix;
	std::getline(std::cin, infix);
	std::cout<<infix2postfix(infix)<<std::endl;
	std::cout<<"Evaluates to: "<<postfixEvaluator(infix2postfix(infix))<<std::endl;
	return 0;
}
*/
