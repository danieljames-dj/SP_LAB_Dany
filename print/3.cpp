/*
Daniel James
3. Absolute Loader
Reg no: 13400022
Date: 08-02-16
*/

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

using namespace std;

int main() {
    map <int,string> memoryMap;
    string curLine,curAddress,programName;
    int locctr,startAddress;
    ifstream fin("input");
    while ( getline(fin,curLine) ) {
        if ( curLine!="" ) {
            if ( curLine[0]=='H' ) {
                programName=curLine.substr(2,6);
            } else if ( curLine[0]=='T' ) {
                stringstream ss;
                ss<<hex<<curLine.substr(2,6);
                ss>>locctr;
                for (int i=12;i<curLine.length();i++) {
                    if ( curLine[i]=='^' )
                        curLine=curLine.substr(0,i)+curLine.substr(i+1,curLine.length()-i);
                }
                for (int i=12;i<curLine.length();i+=2) {
                    memoryMap[locctr++]=curLine.substr(i,2);
                }
            } else if ( curLine[0]=='E' ) {
                stringstream ss;
                ss<<hex<<curLine.substr(2,6);
                ss>>startAddress;
            } else {
                cout<<"INVALID\n";
                return 0;
            }
        }
    }
    cout<<"Program Name : "<<programName<<"\n\nMemory Map\n";
    for (map<int,string>::iterator mIt=memoryMap.begin();mIt!=memoryMap.end();mIt++)
        cout<<hex<<mIt->first<<":"<<mIt->second<<endl;
    cout<<"\nStarting Address : "<<startAddress<<endl;
    return 0;
}

/********************

INPUT
-----
H^  COPY^001000^000016
T^001000^0d^000000^6B0000^500000^454f46^F0
T^001001^02^100d
T^00100d^03^000001
T^001007^02^1010
T^001004^02^1013
T^001013^03^000002
E^001000

OUTPUT
------
Program Name :   COPY

Memory Map
1000:00
1001:10
1002:0d
1003:6B
1004:10
1005:13
1006:50
1007:10
1008:10
1009:45
100a:4f
100b:46
100c:F0
100d:00
100e:00
100f:01
1013:00
1014:00
1015:02

Starting Address : 1000

********************/