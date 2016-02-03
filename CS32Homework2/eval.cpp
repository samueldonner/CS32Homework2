#include "Map.h"
#include <iostream>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool precedence( char current, char other )
{
    if(current=='/' || current =='*')
    {
        if( other == '-' || other== '+' )
        {
            return false;
        }
    }
    return true;
        
}

int calculatePostfix(string postfix, const Map& values) //DUMB
{
    if(postfix.size()==1) //if there are no operators, simply return the value that it maps to
    {
        int tempValue=0;
        values.get(postfix[0], tempValue);
        return tempValue;
    }
    
    stack<int> cStack; //create an integer stack for piling on operands
    
    for(int i=0; i<postfix.size(); i++)
    {
        char token = postfix[i];
        
        if(isalpha(token) && islower(token)) //if the token is a variable
        {
            
            int tempValue=0;
            values.get(token, tempValue);
            cStack.push(tempValue); //push in the integer that the variable maps to
        }
        else //token is an operator
        {
            int operand2=cStack.top();
            cStack.pop();
            int operand1=cStack.top();
            cStack.pop();
            
            int tempAns=0;
            
            switch(token)
            {
                case '-':
                    tempAns=operand1-operand2;
                    break;
                case '+':
                    tempAns=operand1+operand2;
                    break;
                case '*':
                    tempAns=operand1*operand2;
                    break;
                case '/':
                    tempAns=operand1/operand2;
                    break;
            }
            cStack.push(tempAns);
        }
    }
    
    //cerr << "The integer answer is: " << int(cStack.top()) << endl;
    
    return int(cStack.top());
}

bool dividesByZero(string postfix, const Map& values) //DUMB
{
    if(postfix.size()==1) //if there are no operators, simply return the value that it maps to
        return false;
    
    stack<int> cStack; //create an integer stack for piling on operands
    
    for(int i=0; i<postfix.size(); i++)
    {
        char token = postfix[i];
        
        if(isalpha(token) && islower(token))
        {
            int tempValue=0;
            values.get(token, tempValue);
            cStack.push(tempValue); //push in the integer that the variable maps to
        }
        else //char is an operator
        {
            int operand2=cStack.top();
            cStack.pop();
            int operand1=cStack.top();
            cStack.pop();
            
            int tempAns=0;
            
            switch(token)
            {
                case '-':
                    tempAns=operand1-operand2;
                    break;
                case '+':
                    tempAns=operand1+operand2;
                    break;
                case '*':
                    tempAns=operand1*operand2;
                    break;
                case '/':
                    if(operand2==0)
                        return true;
                    else
                        tempAns=operand1/operand2;
                    break;
            }
            cStack.push(tempAns);
        }
    }
    
    return false;
}

bool isInfix( string infix)
{
    if( infix == "" )
    {
        return false; // if there are no characters return 1;
    }
    char okOperators[7] = {'+','-','/','*',' ','(',')'}; //ok characters
    for(int i = 0; i < infix.size(); i++)
    {
        if(isalpha(infix[i]) && !islower(infix[i])) // if char is a letter and is lowercase
        {
            return false;
        }
        if(!isalpha(infix[i])) // if char is not a letter
        {
            int operatorMatch = 0;
            for(int j = 0; j < 7; j++)
            {
                if(infix[i] == okOperators[j])
                {
                    operatorMatch++;
                }
            }
            if(operatorMatch==0)
            {
                return false; // if a non-letter does not equal an ok operator
            }
        }
    }// end for loop
    
    int letterCount = 0;
    int okOperatorCount = 0;
    int rightParentheses = 0;
    int leftParentheses = 0;
    
    for( int i = 0; i < infix.size(); i++)
    {
        if(isalpha(infix[i]))
        {
            letterCount++;
        } // count letters
        if(infix[i] =='(')
        {
            leftParentheses++;
        } // count right paren
        if(infix[i] ==')')
        {
            rightParentheses++;
        } // count left paren
        if(infix[i] == '+' || infix[i] == '-' || infix[i] == '/' || infix[i] == '*')
        {
            okOperatorCount++;
        } // count +-/*
    }
    
    if( leftParentheses != rightParentheses )
    {
        return false;
    } // return 1 if an open parentheses is not accompanied by a close parentheses
    
    if(letterCount != okOperatorCount+1)
    {
        return false;
    } //there should be one more letter than operator
    
    return true;
}

string infixToPostfix(string infix)
{
    string postfix = "";
    stack<char> infixStack;
    
    for( int i=0; i<infix.size(); i++)
    {
        if( isalpha(infix[i]) && islower(infix[i]))
        {
            postfix+=infix[i];
        }
        else if( infix[i] == '(' )
        {
            infixStack.push(infix[i]);
        }
        else if( infix[i] == ')' )
        {
            while(!infixStack.empty() && infixStack.top()!= '(' )
            {
                postfix+=infixStack.top();
                infixStack.pop();
            }
            infixStack.pop();
        }
        else if( infix[i] == '+' || infix[i] == '-' || infix[i] == '/' || infix[i] == '*' )
        {
            while( !infixStack.empty() && infixStack.top()!='(' && precedence(infix[i],infixStack.top()))
            {
                postfix+=infixStack.top();
                infixStack.pop();
            }
            infixStack.push(infix[i]);
        }
    }
    
    while(!infixStack.empty())
    {
        postfix+=infixStack.top();
        infixStack.pop();
    }
    
    return postfix;
}

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    if(!isInfix(infix))
    {
        return 1;
    }
    
    postfix = infixToPostfix(infix);
    
    
    
    for(int i = 0; i < infix.size(); i++)
    {
        char temp = infix[i];
        if( isalpha(temp) && islower(temp) && !values.contains(temp))
        {
            return 2;
        }
    }
    
    if(dividesByZero(postfix,values))
    {
        return 3;
    }
    
    result = calculatePostfix(postfix,values);
    
    
    return 0; // function completeted
}

int main()
{
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    
    assert(evaluate("a+ e", m, pf, answer) == 0  &&
           pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
    // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  &&
           pf == "ab*"  &&  answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
           pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
           pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    cout << "Passed all tests" << endl;
}
