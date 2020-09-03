#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>

using namespace std;

int evaluate(string infix, const Map& values, string& postfix, int& result);

int main()
{
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = { 3,  -9,   6,   2,   4,   1 };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0 &&
        pf == "ae+" && answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("()", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("()o", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("y(     *o)", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("-a", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2 &&
        pf == "ab*" && answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0 &&
        pf == "yoau-*+" && answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3 &&
        pf == "oyy-/" && answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0 &&
        pf == "a" && answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0 &&
        pf == "a" && answer == 3);
    assert(evaluate("a*(e+i)/o", m, pf, answer) == 0 &&
        pf == "aei+*o/" && answer == -4);
    assert(evaluate("e", m, pf, answer) == 0 && pf == "e" && answer == -9);
    cout << "Passed all tests" << endl;
}

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    if (infix.size() <= 0 || infix[0] == '+' || infix[0] == '-' || infix[0] == '*' || infix[0] == '/') //check for empty infix strings or strings beginning with an operator
        return 1;

    //infix to postfix
    int operators = 0;
    int operands = 0;
    postfix = "";
    stack <char> operatorStack;
    for (int i = 0; i < infix.size(); i++)
    {
        if (!isupper(infix[i]) && !isdigit(infix[i]))
        {
            switch (infix[i])
            {
            case ' ':
                continue;
            case '(':
                operatorStack.push(infix[i]);
                break;
            case ')':
                for (int s = 0; s < operatorStack.size(); s++)
                {
                    if (operatorStack.top() == '(')
                        break;
                    postfix += operatorStack.top();
                    operators++;
                    operatorStack.pop();
                }
                if (operatorStack.empty() || postfix.size() == 0)
                    return 1;

                operatorStack.pop();
                break;
            case '+':
            case '-':
            case '*':
            case '/':
                for (int j = i - 1; j >= 0; j--) //checks that operators MUST follow a operand or ')' in the infix string
                {
                    if (infix[j] == '(' || infix[j] == '+' || infix[j] == '-' || infix[j] == '*' || infix[j] == '/')
                    {
                        postfix = "";
                        return 1;
                    }
                    else if (islower(infix[j]) || infix[j] == ')')
                        break;
                }
                while (!operatorStack.empty() && operatorStack.top() != '(' && infix[i] >= operatorStack.top())
                {
                    postfix += operatorStack.top();
                    operators++;
                    operatorStack.pop();
                }
                operatorStack.push(infix[i]);
                break;
            default:
                postfix += infix[i];
                operands++;
                break;
            }
        }
        else
        {
            postfix = "";
            return 1;
        }
    }

    while (!operatorStack.empty()) //add remaining operators in the stack to the postfix string
    {
        postfix += operatorStack.top();
        operators++;
        operatorStack.pop();
    }

    //check if postfix is syntactically valid
    if ((operators + 1) != operands) //number of operators should be 1 less than number of operands
        return 1;

    //calculate result
    stack <int> operandStack;
    for (int i = 0; i < postfix.size(); i++)
    {
        if (islower(postfix[i]))
        {
            int temp;
            if (!values.get(postfix[i], temp))
                return 2;
            operandStack.push(temp);
        }
        else //postfix[binary operator]
        {
            int operand2 = operandStack.top();
            operandStack.pop();

            int operand1 = operandStack.top();
            operandStack.pop();

            switch (postfix[i])
            {
            case '+':
                operandStack.push(operand1 + operand2);
                break;
            case '-':
                operandStack.push(operand1 - operand2);
                break;
            case '*':
                operandStack.push(operand1 * operand2);
                break;
            case '/':
                if (operand2 == 0)
                    return 3;
                operandStack.push(operand1 / operand2);
                break;
            }
        }
    }
    result = operandStack.top();
    return 0;
}
