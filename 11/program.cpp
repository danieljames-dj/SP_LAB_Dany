/*
Daniel James
11. Machine Code Generator
Reg no: 13400022
Date: 11-04-16
*/

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

string oper(char c) {
    switch(c) {
        case '+': return "ADD ";
        case '-': return "SUB ";
        case '*': return "MUL ";
        case '/': return "DIV ";
    }
}

int main() {
    ifstream fin("input");
    ofstream fout("output");
    string line,result,op1,op2;
    char op;
    while (getline(fin,line)) {
        op2="";
        stringstream s(line);
        char temp;
        s>>result>>temp>>op1>>op>>op2;
        fout<<"MOV "<<result<<","<<op1<<endl;
        if (op2!="") {
            fout<<oper(op)<<result<<","<<op2<<endl;
        }
    }
    return 0;
}

/********************

INPUT
-----
t0 = b + d
t1 = b * t0
t2 = a + t1
t3 = b + d
t4 = t3 -5
d = t4

OUTPUT
------
MOV t0,b
ADD t0,d
MOV t1,b
MUL t1,t0
MOV t2,a
ADD t2,t1
MOV t3,b
ADD t3,d
MOV t4,t3
SUB t4,5
MOV d,t4

********************/