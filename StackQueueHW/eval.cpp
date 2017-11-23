#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

int precendence(char a, char b) {
	if (a == b) return 0;
	if (a == '|' && b == '&') return -1;
	if (a == '&' && b == '|') return 1;
	if (a == '!' && (b == '&' || b == '|')) return 1;
	if (b == '!' && (a == '&' || a == '|')) return -1;
	exit(2);
	return 10;
}

int evaluate(string infix, const bool values[], string& postfix, bool& result)
// Evaluates a boolean expression
// Postcondition: If infix is a syntactically valid infix boolean
//   expression, then postfix is set to the postfix form of that
//   expression, result is set to the value of the expression (where
//   in that expression, each digit k represents element k of the
//   values array), and the function returns zero.  If infix is not a
//   syntactically valid expression, the function returns 1.  (In that
//   case, postfix may or may not be changed, but result must be
//   unchanged.)
{
	postfix = "";
	
	stack<char> operatorStack;
	char prev = 'X';
	for (int i = 0; i < infix.size(); i++) {
		
		switch (infix[i]) {
		case ' ':
		case '	':
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (isdigit(prev)) return 1;
			if (prev == ')') return 1;
			postfix += infix[i];
			break;
		case '(':
		case '!':
			if (isdigit(prev)) return 1;
			operatorStack.push(infix[i]);
			break;
		case ')':
			if (operatorStack.empty()) return 1;
			while (operatorStack.top() != '(') {
				postfix += operatorStack.top();
				operatorStack.pop();
				if (operatorStack.empty()) return 1;
			}
			operatorStack.pop(); //remove the '('
			break;
		case '&':
		case '|':
			if (prev == '|' || prev == '&') return 1;
			while (!operatorStack.empty() && operatorStack.top() != '(' && precendence(infix[i], operatorStack.top()) <= 0) {
				postfix += operatorStack.top();
				operatorStack.pop(); //todo
			}
			operatorStack.push(infix[i]);
			break;
		default:
			return 1; //invalid char
		}
		if (infix[i] != ' ' && infix[i] != '	') prev = infix[i];
		
	}
	while (!operatorStack.empty()) {
		postfix += operatorStack.top();
		operatorStack.pop();
	}

	if (postfix.length() == 0) return 1;
	if (postfix.find('(') != string::npos) return 1; //contains (


	stack<bool> operandStack;
	for (int i = 0; i < postfix.size(); i++) {
		switch (postfix[i]) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			operandStack.push(values[ postfix[i] -'0'] ); //convert char to  value it represents
			break;
		case '!': {
			if (operandStack.empty()) return 1;
			bool b = !operandStack.top();
			operandStack.pop();
			operandStack.push(b);
			break; }
		case '&': {
			if (operandStack.empty()) return 1;
			bool b1 = operandStack.top();
			operandStack.pop();
			if (operandStack.empty()) return 1;
			bool b2 = operandStack.top();
			operandStack.pop();
			operandStack.push(b1&&b2);
			break; }
		case '|': {
			if (operandStack.empty()) return 1;
			bool b1 = operandStack.top();
			operandStack.pop();
			if (operandStack.empty()) return 1;
			bool b2 = operandStack.top();
			operandStack.pop();
			operandStack.push(b1 || b2);
			break; }
		
		}
	
	
	}
	
	result = operandStack.top();
	

	return 0;


	
}


int main2()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf;
	bool answer;
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("4||5", ba, pf, answer) == 1);
	assert(evaluate("4&|5", ba, pf, answer) == 1);
	assert(evaluate("4&&5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("(2))", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2&(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("6&(7|7))", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate("4  |	  !3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "43!03&&|" && !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer); 
	cout << "Passed all tests" << endl; 
}



int main()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf;
	bool answer;
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()5", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "43!03&&|" && !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);
	cout << "Passed all tests" << endl;

	return 0;
}

