/*
Daniel James
10. Intermediate Code Generator
Reg no: 13400022
Date: 04-04-16
*/

#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>

using namespace std;

ifstream fin("input");
ofstream fout("output");
stack<char> operators;
stack<string> operands;
int newInd;

void processStack() {
    string s=string(1,operators.top())+operands.top();
    stringstream ss;
    operands.pop(); operators.pop();
    ss<<newInd++;
    s="t"+ss.str()+string(1,'=')+operands.top()+s;
    operands.pop();
    fout<<s<<endl;
    operands.push(s.substr(0,2));
}

int prec(char c) {
    switch(c) {
        case '+':       return 1;
        case '-':       return 1;
        case '*':       return 2;
        case '/':       return 2;
        case '%':       return 2;
        case '^':       return 3;
        case '(':       return 4;
        case ')':       return 4;
        default :       return 0;
    }
}

int main() {
    string line;
    newInd=0;
    getline(fin,line);
    for (int i=2;i<line.length();i++) {
        if (prec(line[i])==0) {
            operands.push(string(1,line[i]));
        } else {
            if ( line[i]==')' ) {
                while ( !operators.empty() && operators.top()!='(' ) processStack();
                if (operators.top()=='(') operators.pop();
            } else if (    operators.empty() ||
                        operators.top()=='(' ||
                        prec(operators.top())<prec(line[i]) ) {
                operators.push(line[i]);
            } else {
                while ( !operators.empty() &&
                        operators.top()!='(' &&
                        prec(operators.top())>=prec(line[i]) ) processStack();
            }
        }
    }
        while (!operators.empty()&&operands.size()>=2) processStack();
        fout<<line[0]<<"="<<operands.top()<<endl; return 0;
}

/********************

INPUT
-----
d=a+b*(b+d)+(b+d)-5

OUTPUT
------
t0=b+d
t1=b*t0
t2=a+t1
t3=b+d
t4=t3-5
d=t4

********************/