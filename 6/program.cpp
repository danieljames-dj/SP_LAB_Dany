/*
Daniel James
6. Lexical Analyser
Reg no: 13400022
Date: 07-03-16
*/

#include <iostream>
#include <fstream>
#include <cctype>
#include <map>
#define KW_SIZE 3

using namespace std;

int main() {
    ifstream fin("input");
    string line;
    string keywords[]={"include","int","cout"};
    map<char,string> symbol;
    symbol['#']="Preprocessor";
    symbol[';']="Delimiter";
    symbol['(']=symbol['{']="Open Brace";
    symbol[')']=symbol['}']="Close Brace";
    while (getline(fin,line)) {
        cout<<line<<endl;
        for (int i=0;i<line.length();i++) {
            if (line[i]==' ') {
                continue;
            } else if (isalpha(line[i])) {
                string str=string(1,line[i]);
                for (i++;i<line.length();i++) {
                    if (isalpha(line[i])) str+=string(1,line[i]);
                    else break;
                }
                i--;
                if (i+2<=line.length() && line[i+1]=='.'&&line[i+2]=='h') {
                    str+=".h";
                    cout<<"\t"<<str<<" -> Header file\n";
                    i+=2;
                    continue;
                }
                if (i+2<=line.length() && line[i+1]=='('&&line[i+2]==')') {
                    str+="()";
                    cout<<"\t"<<str<<" -> Function\n";
                    i+=2;
                    continue;
                }
                int j;
                for (j=0;j<KW_SIZE;j++) {
                    if (keywords[j]==str) {
                        cout<<"\t"<<str<<" -> Keyword\n";
                        break;
                    }
                }
                if (j==KW_SIZE) cout<<"\t"<<str<<" -> Variable\n";
            } else {
                if (line[i]=='"') {
                    string str=string(1,line[i]);
                    for (i++;i<line.length();i++) {
                        if (line[i]!='"') str+=string(1,line[i]);
                        else break;
                    }
                    str+=string(1,line[i]);
                    cout<<"\t"<<str<<" -> String Constant\n";
                } else if ( line[i]=='<' ) {
                    if (i+1<line.length()&&line[i+1]=='<') {
                        cout<<"\t<< -> Operator\n"; i++;
                    } else cout<<"\t< -> Operator\n";
                } else if ( line[i]=='>' ) {
                    if (i+1<line.length()&&line[i+1]=='>') {
                        cout<<"\t<< -> Operator\n"; i++;
                    } else cout<<"\t> -> Operator\n";
                }
                else if (symbol[line[i]]=="") cout<<"\t"<<line[i]<<" -> Operator\n";
                else cout<<"\t"<<line[i]<<" -> "<<symbol[line[i]]<<endl;
            }
        }
    }
    return 0;
}

/********************

INPUT
-----
#include<iostream.h>
int main()
{
int a;
int b;
int s;
s = b + a;
cout<<"sum : ";
cout<<s;
}

OUTPUT
------
#include<iostream.h>
    # -> Preprocessor
    include -> Keyword
    < -> Operator
    iostream.h -> Header file
    > -> Operator
int main()
    int -> Keyword
    main() -> Function
{
    { -> Open Brace
int a;
    int -> Keyword
    a -> Variable
    ; -> Delimiter
int b;
    int -> Keyword
    b -> Variable
    ; -> Delimiter
int s;
    int -> Keyword
    s -> Variable
    ; -> Delimiter
s = b + a;
    s -> Variable
    = -> Operator
    b -> Variable
    + -> Operator
    a -> Variable
    ; -> Delimiter
cout<<"sum : ";
    cout -> Keyword
    << -> Operator
    "sum : " -> String Constant
    ; -> Delimiter
cout<<s;
    cout -> Keyword
    << -> Operator
    s -> Variable
    ; -> Delimiter
}
    } -> Close Brace

********************/