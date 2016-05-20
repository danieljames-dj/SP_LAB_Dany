/*
Daniel James
9. First and Follow
Reg no: 13400022
Date: 28-03-16
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

map<char,string> grammar;
char keys[20];
int keyCount=0,stH;
void follow(char);
char stack[50];

void first(char key,int folV=0) {
    string str="";
    stringstream s(grammar[key]);
    getline(s,str,'|');
    while (str!="") {
        if (grammar[str[0]]=="") {
            if (str[0]=='#' && folV==1) {
                follow(key);
            } else {
                stack[stH++]=str[0];
            }
        }
        else if ( str[0]!=key ) first(str[0]);
        str="";
        getline(s,str,'|');
    }
}

void follow(char key) {
    if (key==keys[0]) stack[stH++]='$';
    string str;
    char c;
    for (int i=0;i<keyCount;i++) {
        stringstream s(grammar[keys[i]]);
        getline(s,str,'|');
        while (str!="") {
            for (int j=0;j<str.length()-1;j++) {
                if (str[j]==key) {
                    if (grammar[str[j+1]]=="") stack[stH++]=str[j+1];
                    else first(str[j+1],1);
                }
            }
            char c=str[str.length()-1];
            if (c==key) {
                if (c!=keys[i]) follow(keys[i]);
            }
            str="";
            getline(s,str,'|');
        }
    }
}

int main() {
    ifstream fin("input");
    string line,str;
    char key;
    while (getline(fin,line)) {
        stringstream s(line);
        s>>key>>str>>str;
        grammar[key]=str;
        keys[keyCount++]=key;
    }
    for (int i=0;i<keyCount;i++) {
        cout<<"First of "<<keys[i]<<" are : ";
        stH=0;
        first(keys[i]);
        for (int j=0,k=0;j<stH;j++) {
            for (k=0;k<j;k++) {
                if (stack[k]==stack[j]) break;
            }
            if (k!=j) continue;
            cout<<stack[j]<<" ";
        }
        cout<<endl;
    }
    for (int i=0;i<keyCount;i++) {
        cout<<"Follow of "<<keys[i]<<" are : ";
        stH=0;
        follow(keys[i]);
        for (int j=0,k=0;j<stH;j++) {
            for (k=0;k<j;k++) {
                if (stack[k]==stack[j]) break;
            }
            if (k!=j || stack[j]=='#') continue;
            cout<<stack[j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}

/********************

INPUT
-----
E -> TD
D -> +TD|#
F -> (E)|x
T -> FS
S -> *FS|#

OUTPUT
------
First of E are : ( x 
First of D are : + # 
First of F are : ( x 
First of T are : ( x 
First of S are : * # 
Follow of E are : $ ) 
Follow of D are : $ ) 
Follow of F are : * + $ ) 
Follow of T are : + $ ) 
Follow of S are : + $ ) 

********************/