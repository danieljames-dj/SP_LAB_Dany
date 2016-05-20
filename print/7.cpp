/*
Daniel James
7. Operator Precedence
Reg no: 13400022
Date: 14-03-16
*/

#include <iostream>
#include <stack>
#include <cstdlib>

using namespace std;

int prec(char c) {
    switch(c) {
        case '+':    return 1;
        case '-':    return 1;
        case '*':    return 2;
        case '/':    return 2;
        case '%':    return 2;
        case '^':    return 3;
        case '(':    return 4;
        case ')':    return 4;
        default:     return 0;
    }
}

int endErrorProg(string expression,int i) {
    cout<<expression<<" : Rejected with error "<<i<<endl;
    exit(0);
}

int main() {
    string expression;
    cin>>expression;
    stack<char> operators,operands;
    for ( int i=0 ; i<expression.length() ; i++ ) {
        if (prec(expression[i])==0) {
            if (i>0&&prec(expression[i-1])==0) endErrorProg(expression,1001);
            operands.push(expression[i]);
        } else {
            if ( expression[i]==')' ) {
                while ( !operators.empty() && operators.top()!='(' ) {
                    operators.pop();
                    if (operands.size()<2) endErrorProg(expression,1002);
                    operands.pop();
                }
                if (operators.empty()) endErrorProg(expression,1003);
                else operators.pop();
            } else if (    operators.empty() ||
                    prec(operators.top())<prec(expression[i]) ||
                    operators.top()=='(' ) {
                operators.push(expression[i]);
            } else {
                while (    !operators.empty() &&
                        operators.top()!='(' &&
                        prec(operators.top())>=prec(expression[i]) ) {
                    if (operands.size()<2) endErrorProg(expression,1004);
                    operands.pop();
                    operators.pop();
                }
                operators.push(expression[i]);
            }
        }
    }
    if (operands.size()-operators.size()!=1) endErrorProg(expression,1005);
    while ( !operators.empty() ) {
        if ( prec(operators.top())>3 ) endErrorProg(expression,1006);
        operators.pop();
    }
    cout<<expression<<" : Accepted\n";
    return 0;
}

/********************

OUTPUT
------
x+2--
x+2-- : Rejected with error 1004
x+t*r+(a-b)
x+t*r+(a-b) : Accepted

********************/