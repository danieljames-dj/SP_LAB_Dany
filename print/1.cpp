/*
Daniel James
1. One Pass Assembler
Reg no: 13400022
Date: 11-01-16
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <cstdlib>

using namespace std;

void writeText(string textRec[],int& lineNo,int lineSize[],string& curCode,string& temp,string& curAddress) {
    if (lineSize[lineNo]+curCode.length()/2>30) {
        stringstream s;
        s<<setw(2)<<setfill('0')<<hex<<lineSize[lineNo];
        textRec[lineNo]=temp+s.str()+textRec[lineNo];
        lineNo++;
        temp="T^00"+curAddress+"^";
    }
    textRec[lineNo]=textRec[lineNo]+"^"+curCode;
    lineSize[lineNo]+=curCode.length()/2;
}

int main() {
    ifstream fin("optab");
    ofstream fout("output");
    map <string,string> symtab,optab;
    int lineNo=0,lineSize[100];
    for (int i=0;i<100;i++) lineSize[i]=0;
    string headRec,textRec[100],endRec,curLine,label,opcode,operand,curCode,temp,curAddress;
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
    fin.open("source");
    int startAddress,locctr;
    int textRecCount=0;
    getline(fin,curLine);
    stringstream ss(curLine);
    getline(ss,label,' ');
    getline(ss,opcode,' ');
    getline(ss,operand,' ');
    if ( opcode=="START" ) {
        stringstream str;
        str<<setw(6)<<setfill('0')<<hex<<operand;
        str>>locctr;
        startAddress=locctr;
        headRec="H^"+label+"^"+str.str()+"^";
        endRec="E^"+str.str();
        temp="T^"+str.str()+"^";
    }
    while ( getline(fin,curLine) ) {
        int curPtr=fin.tellg();
        char c;
        fin>>c;
        if ( !fin.eof() ) fin.seekg(curPtr,ios::beg);
        stringstream ss(curLine);
        getline(ss,label,' ');
        getline(ss,opcode,' ');
        getline(ss,operand,' ');
        if ( label[0]=='.' ) continue;
        stringstream str;
        str<<setw(4)<<setfill('0')<<hex<<locctr;
        str>>curAddress;
        if ( label!="" ) {
            string labelValue=symtab[label];
            if ( labelValue=="" ) {
                symtab[label]=curAddress;
            } else if ( labelValue[0]=='n' ) {
                stringstream s;
                s<<setw(2)<<setfill('0')<<hex<<lineSize[lineNo];
                textRec[lineNo]=temp+s.str()+textRec[lineNo];
                lineNo++;
                for (int i=1;i<labelValue.length();i+=4) {
                    string s=labelValue.substr(i,4);
                    textRec[lineNo]="T^00"+s+"^02^"+curAddress;
                    lineNo++;
                }
                temp="T^00"+curAddress+"^";
                symtab[label]=curAddress;
            }
        }
        if (optab[opcode]!="") {
            curCode=optab[opcode];
            if (symtab[operand]=="") {
                curCode+="0000";
                stringstream s;
                s<<setw(4)<<setfill('0')<<hex<<locctr+1;
                symtab[operand]="n"+s.str();
            } else if (symtab[operand][0]=='n') {
                curCode+="0000";
                stringstream s;
                s<<setw(4)<<setfill('0')<<hex<<locctr+1;
                symtab[operand]+=s.str();
            } else {
                curCode+=symtab[operand];
            }
            writeText(textRec,lineNo,lineSize,curCode,temp,curAddress);
            locctr+=3;
        } else if (opcode=="WORD") {
            stringstream s;
            s<<setw(6)<<setfill('0')<<hex<<operand;
            s>>curCode;
            writeText(textRec,lineNo,lineSize,curCode,temp,curAddress);
            locctr+=3;
        } else if (opcode=="RESW") {
            locctr+=3*atoi(operand.c_str());
        } else if (opcode=="RESB") {
            locctr+=atoi(operand.c_str());
        } else if (opcode=="BYTE") {
            int len=operand.length()-3;
            string byteStr=operand.substr(2,len);
            if (operand[0]=='X') {
                curCode=byteStr;
                writeText(textRec,lineNo,lineSize,curCode,temp,curAddress);
                locctr+=byteStr.length()/2;
            } else {
                curCode="";
                for (int i=0;i<byteStr.length();i++) {
                    int ascii=byteStr[i];
                    stringstream s;
                    s<<hex<<ascii;
                    curCode+=s.str();
                }
                writeText(textRec,lineNo,lineSize,curCode,temp,curAddress);
                locctr+=len;
            }

        } else {
            curCode="XXXXXX";
            writeText(textRec,lineNo,lineSize,curCode,temp,curAddress);
            locctr+=3;
        }
    }
    if (lineSize[lineNo]>0) {
        stringstream s;
        s<<setw(2)<<setfill('0')<<hex<<lineSize[lineNo];
        textRec[lineNo]=temp+s.str()+textRec[lineNo];
        lineNo++;
    }
    stringstream s;
    s<<setw(6)<<setfill('0')<<hex<<locctr-startAddress;
    headRec+=s.str();
    fout<<headRec<<endl;
    for (int i=0;i<lineNo;i++) {
        fout<<textRec[i]<<endl;
    }
    fout<<endRec<<endl;
    return 0;
}

/********************

OPTAB
-----
LDA 00
STA 50
ADD 6B

SOURCE
------
COPY START 1000
 LDA ALPHA
 STA ALPHA
 ADD BETA
 STA GAMMA
EOF BYTE C'EOF'
. this is a comment
HEX BYTE X'F0'
ALPHA WORD 1
BETA WORD 2
GAMMA RESW 4096

OUTPUT
------
H^COPY^001000^003016
T^001000^10^000000^500000^6B0000^500000^454f46^F0
T^001001^02^1010
T^001004^02^1010
T^001010^03^000001
T^001007^02^1013
T^001013^03^000002
T^00100a^02^1016
E^001000

********************/