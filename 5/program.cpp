/*
Daniel James
5. Macro Processor
Reg no: 13400022
Date: 22-02-16
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

using namespace std;

ifstream fin("input");
ofstream fout("output");
map<string,string> macros;
map<string,string> macHead;
string line,s1,s2,s3,mc;

int main() {
    while (getline(fin,line)) {
        stringstream ML(line);
        s1=s2="";
        ML>>s1>>s2>>s3;
        if ( s2=="MACRO" ) {
            mc=s1;
            macHead[mc]=s3;
            while ( getline(fin,line) ) {
                stringstream s(line);
                s>>s1>>s2;
                if (s1=="MEND") break;
                macros[mc]+=(line+"\n");
            }
        } else if (macros[s2]!="") {
            stringstream split(s3),head(macHead[s2]);
            string temp,hT,str=macros[s2];
            getline(split,temp,',');
            getline(head,hT,',');
            while (temp!="") {
                for (int i=0;i<str.length()-hT.length()+1;i++) {
                    if (str.substr(i,hT.length())==hT) {
                        str=str.substr(0,i)+temp+str.substr(i+hT.length(),str.length()-i-hT.length());
                    }
                }
                hT="";
                temp="";
                getline(split,temp,',');
                getline(head,hT,',');
            }
            fout<<str;
        } else {
            fout<<line<<endl;
        }
    }
    return 0;
}

/********************

INPUT
-----
COPY START 0
RDBUFF MACRO &INDEV,&BUFADR,&RECLTH
 CLEAR X
 CLEAR A
 CLEAR S
 +LDT #4096
 TD =X'&INDEV'
 JEQ *-3
 RD =X'&INDEV'
 COMPR A,S
 JEQ *+11
 STCH &BUFADR,X
 TIXR T
 JLT *-19
 STX &RECLTH
 MEND 
FIRST STL RETADR
CLOOP RDBUFF F1,BUFFER,LENGTH
 LDA LENGTH
 COMP #0
TEMP RDBUFF F2,RETADR,THREE
EOF BYTE C'EOF'
THREE WORD 3
RETADR RESW 1
BUFFER RESB 4096

OUTPUT
------
COPY START 0
FIRST STL RETADR
 CLEAR X
 CLEAR A
 CLEAR S
 +LDT #4096
 TD =X'F1'
 JEQ *-3
 RD =X'F1'
 COMPR A,S
 JEQ *+11
 STCH BUFFER,X
 TIXR T
 JLT *-19
 STX LENGTH
 LDA LENGTH
 COMP #0
 CLEAR X
 CLEAR A
 CLEAR S
 +LDT #4096
 TD =X'F2'
 JEQ *-3
 RD =X'F2'
 COMPR A,S
 JEQ *+11
 STCH RETADR,X
 TIXR T
 JLT *-19
 STX THREE
EOF BYTE C'EOF'
THREE WORD 3
RETADR RESW 1
BUFFER RESB 4096

********************/