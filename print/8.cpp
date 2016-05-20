/*
Daniel James
8. Recursive Descent Parser
Reg no: 13400022
Date: 21-03-16
*/

#include <iostream>

using namespace std;

void e();
void ed();
void t();
void td();
void f();
string str;
int ptr;
bool error=false;

void e() {
    t();
    ed();
}

void ed() {
    if (str[ptr]=='+') {
        ptr++;
        t();
        ed();
    }
}

void t() {
    f();
    td();
}

void td() {
    if (str[ptr]=='*') {
        ptr++;
        f();
        td();
    }
}

void f() {
    if (str[ptr]=='(') {
        ptr++;
        e();
        if (str[ptr]==')') {
            ptr++;
        }
    } else if (str[ptr]=='x') {
        ptr++;
    } else {
        error=true;
    }
}

int main() {
    cin>>str;
    ptr=0;
    e();
    if (!error && ptr==str.length()) cout<<str<<" : Accepted\n";
    else cout<<str<<" : Rejected\n";
    return 0;
}

/********************

OUTPUT
------
x+x*(x+x)
x+x*(x+x) : Accepted
xx+x
xx+x : Rejected

********************/