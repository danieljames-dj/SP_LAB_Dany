/*
Daniel James
4. Relocating Loader
Reg no: 13400022
Date: 15-02-16
*/

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

int main() {
    map <int,string> memoryMap;
    string curLine,curAddress,programName,startAddressStr;
    int locctr,startAddress,bMask;
    ifstream fin("input");
    cout<<"Enter starting address : ";
    cin>>startAddressStr;
    stringstream s;
    s<<hex<<startAddressStr;
    s>>startAddress;
    while ( getline(fin,curLine) ) {
        if ( curLine!="" ) {
            if ( curLine[0]=='H' ) {
                programName=curLine.substr(2,6);
                stringstream ss;
                ss<<hex<<curLine.substr(9,6);
                ss>>locctr;
                startAddress-=locctr;
            } else if ( curLine[0]=='T' ) {
                stringstream ss;
                ss<<hex<<curLine.substr(2,6)<<" "<<hex<<curLine.substr(12,3);
                ss>>locctr>>bMask;
                for (int i=12;i<curLine.length();i++) {
                    if ( curLine[i]=='^' )
                        curLine[i]=' ';
                }
                string record;
                stringstream recordStream(curLine.substr(16,curLine.length()-16));
                while (getline(recordStream,record,' ')) {
                    if ((bMask&0x800)==0x800) {
                        stringstream a;
                        a<<hex<<strtol(record.c_str(),NULL,16)+startAddress;
                        record=a.str();
                    }
                    bMask=bMask<<1;
                    for (int i=0;i<record.length();i+=2) {
                        memoryMap[startAddress+locctr++]=record.substr(i,2);
                    }
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
    cout<<"\nStarting Address : "<<s.str()<<endl;
    return 0;
}

/********************

INPUT
-----
H^  COPY^000000^000016
T^000000^0d^e00^10000d^6B0013^500010^454f46^F0
T^00000d^03^000^000001
T^000013^03^000^000002
E^000000

OUTPUT
------
Enter starting address : 2000
Program Name :   COPY

Memory Map
2000:10
2001:20
2002:0d
2003:6b
2004:20
2005:13
2006:50
2007:20
2008:10
2009:45
200a:4f
200b:46
200c:F0
200d:00
200e:00
200f:01
2013:00
2014:00
2015:02

Starting Address : 2000

********************/