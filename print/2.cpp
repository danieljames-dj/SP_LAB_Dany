/*
Daniel James
2. Two Pass Assembler
Reg no: 13400022
Date: 25-01-16
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <cstdlib>

using namespace std;

map <string,string> symtab,optab;
int programSize;

void optabFetch() {
    ifstream fin("optab");
    string curLine,opcode,operand;
    while ( getline(fin,curLine) ) {
        char c;
        fin>>c;
        if ( !fin.eof() ) fin.seekg(-1,ios::cur);
        stringstream ss(curLine);
        getline(ss,opcode,' ');
        getline(ss,operand,' ');
        optab[opcode]=operand;
    }
    fin.close();
}

int passOne() {
    ifstream fin("source");
    ofstream fout("intermediate");
    string curLine,label,opcode,operand;
    int startAddress,locctr;
    getline(fin,curLine);
    stringstream ss(curLine);
    getline(ss,label,' ');
    getline(ss,opcode,' ');
    getline(ss,operand,' ');
    if ( opcode=="START" ) {
        stringstream s;
        s<<setw(4)<<setfill('0')<<hex<<operand;
        s>>locctr;
        startAddress=locctr;
        fout<<s.str()<<" "<<curLine<<endl;
    } else {
        cout<<"ERROR:START NOT FOUND\n";
        return 0;
    }
    while (getline(fin,curLine)) {
        int curPtr=fin.tellg();
        char c;
        fin>>c;
        if ( !fin.eof() ) fin.seekg(curPtr,ios::beg);
        stringstream s(curLine);
        getline(s,label,' ');
        getline(s,opcode,' ');
        getline(s,operand,' ');
        if (label[0]=='.') continue;
        stringstream curAdd;
        curAdd<<setw(4)<<setfill('0')<<hex<<locctr;
        fout<<curAdd.str()<<" "<<curLine<<endl;
        if (label!="") {
            if (symtab[label]=="") {
                symtab[label]=curAdd.str();
            } else {
                cout<<"ERROR:LABEL REDECLARATION\n";
                return 0;
            }
        }
        if (optab[opcode]!="") {
            locctr+=3;
        } else if (opcode=="WORD") {
            locctr+=3;
        } else if (opcode=="RESW") {
            locctr+=3*atoi(operand.c_str());
        } else if (opcode=="RESB") {
            locctr+=atoi(operand.c_str());
        } else if (opcode=="BYTE") {
            if (operand[0]=='X') locctr+=(operand.length()-3)/2;
            else locctr+=operand.length()-3;
        } else {
            cout<<"ERROR:OPCODE NOT FOUND";
            return 0;
        }
    }
    programSize=locctr-startAddress;
    return 1;
}

void writeText(int x,int &locctr,int &startAddress,ofstream &fout,string &textRec,string &objCode) {
    if (locctr-startAddress+x>30 || x==0) {
        fout<<"T^"<<setw(6)<<setfill('0')<<hex<<startAddress<<"^"<<setw(2)<<setfill('0')<<hex<<locctr-startAddress<<textRec<<endl;
        textRec="";
        startAddress=locctr;
    }
    if (x!=0) {
        locctr+=x;
        textRec+=("^"+objCode);
    }
}

void passTwo() {
    ifstream fin("intermediate");
    ofstream fout("output");
    string curLine,curAddress,label,opcode,operand,textRec,endRec,objCode;
    int locctr,startAddress;
    getline(fin,curLine);
    stringstream ss(curLine);
    getline(ss,curAddress,' ');
    getline(ss,label,' ');
    getline(ss,opcode,' ');
    getline(ss,operand,' ');
    stringstream address;
    address<<hex<<curAddress;
    address>>locctr;
    startAddress=locctr;
    endRec="E^00"+curAddress+"\n";
    if (opcode=="START")
        fout<<"H^"<<label<<"^"<<setw(6)<<setfill('0')<<hex<<locctr<<"^"<<setw(6)<<setfill('0')<<hex<<programSize<<endl;
    while (getline(fin,curLine)) {
        char c;
        fin>>c;
        if ( !fin.eof() ) fin.seekg(-1,ios::cur);
        stringstream s(curLine);
        getline(s,curAddress,' ');
        getline(s,label,' ');
        getline(s,opcode,' ');
        getline(s,operand,' ');
        if (optab[opcode]!="") {
            objCode=optab[opcode]+symtab[operand];
            writeText(3,locctr,startAddress,fout,textRec,objCode);
        } else if (opcode=="RESB") {
            writeText(0,locctr,startAddress,fout,textRec,objCode);
            locctr+=atoi(operand.c_str());
            startAddress=locctr;
        } else if (opcode=="RESW") {
            writeText(0,locctr,startAddress,fout,textRec,objCode);
            locctr+=3*atoi(operand.c_str());
            startAddress=locctr;
        } else if (opcode=="WORD") {
            stringstream oc;
            oc<<setw(6)<<setfill('0')<<hex<<operand;
            oc>>objCode;
            writeText(3,locctr,startAddress,fout,textRec,objCode);
        } else {
            int len=operand.length()-3;
            string byteStr=operand.substr(2,len);
            if (operand[0]=='X') {
                objCode=byteStr;
                writeText(len/2,locctr,startAddress,fout,textRec,objCode);
            } else {
                objCode="";
                for (int i=0;i<byteStr.length();i++) {
                    int ascii=byteStr[i];
                    stringstream s;
                    s<<hex<<ascii;
                    objCode+=s.str();
                }
                writeText(len,locctr,startAddress,fout,textRec,objCode);
            }
        }
    }
    if (textRec!="") writeText(0,locctr,startAddress,fout,textRec,objCode);
    fout<<endRec;
}

int main() {
    optabFetch();
    if (passOne() ) passTwo();
    return 0;
}

/********************

SOURCE
------
COPY START 1000
 LDA ALPHA
 ADD BETA
 STA GAMMA
EOF BYTE C'EOF'
. this is a comment
HEX BYTE X'F0'
ALPHA WORD 1
BETA WORD 2
GAMMA RESW 4096

OPTAB
-----
LDA 00
STA 50
ADD 6B

INTERMEDIATE
------------
1000 COPY START 1000
1000  LDA ALPHA
1003  ADD BETA
1006  STA GAMMA
1009 EOF BYTE C'EOF'
100c HEX BYTE X'F0'
100d ALPHA WORD 1
1010 BETA WORD 2
1013 GAMMA RESW 4096

OUTPUT
------
H^COPY^001000^003013
T^001000^13^00100d^6B1010^501013^454f46^F0^000001^000002
E^001000

********************/