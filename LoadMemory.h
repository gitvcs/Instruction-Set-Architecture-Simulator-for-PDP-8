/*********************************************************************************
 * File: 		LoadMemory.h
 * Description: Implements functions to load contents given in .OBJ file into defined 4K memory.
 * Project: 	ECE 586 PDP-8 ISA Simulator
 * Version: 	0.3
 * Date:		01/24/2015
 * Author:		Prakhya, Doguparthi, Mutyala, Produtturi
 *********************************************************************************/

#ifndef LOADMEMORY_H
#define LOADMEMORY_H

#include "Memory.h"

using namespace std;

int getPageOrOffsetNum(int address, bool pageOrOffset);
void loadMemory(string traceFile);

/******************************************************************
* Get page number or offset number for given address in decimal
* Inputs: Address (type: int) it is in decimal representation, pageOrOffset (type: bool) used to select page or offset
* Output: Page number or offset number (type: int) both in decimal
*******************************************************************/

int getPageOrOffsetNum(int address, bool pageOrOffset)
{
    int index=0, page=0, offset=0;
	int tempArray[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	int decimalArray[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	int decimalNum = address;
    while(decimalNum>0)
    {
        tempArray[index]=decimalNum%2;
        index++;
        decimalNum=decimalNum/2;
    }
    for(int index=0;index<12;index++)
    {
        decimalArray[index]=tempArray[11-index];
    }
    if(pageOrOffset)
    {
        for(int counter=4;counter>=0;counter--)
        {
            if(decimalArray[counter] == 1)
            {
                page += int(pow(float(2), (4-counter)));
            }
        }
        return page;
    }
    else
    {
        for(int counter=11;counter>=5;counter--)
        {
            if(decimalArray[counter] == 1)
            {
                offset += int(pow(float(2), (11-counter)));
            }
        }
        return offset;
    }
}

/******************************************************************
* Take file as input and load contents of it into memory
* Inputs: File (type: string)
* Output: None (Sets contents of memory at given addresses)
*******************************************************************/

void loadMemory(string traceFile)
{
    string trace, firstLine, secondLine, twoLines, addOrDataS;
    int count1 = 0, ADIntOct, ADIntDec, presentAddOrDataI;
    int pageNum = 0, offsetNum = 0;
    char local;
    ifstream myfile;
    myfile.open(traceFile.c_str());
    if (myfile.is_open())
    {
        while (getline(myfile,trace))
        {
            count1++;
            if (count1 == 1)
            {
                firstLine = trace.substr(0,3);
            }
            else
            {
                count1 = 0;
                secondLine = trace.substr(0,3);
                twoLines = firstLine + secondLine;
                local = twoLines[0];
                addOrDataS = twoLines.substr(1,2) + twoLines.substr(4,2);
                ADIntOct = atoi(addOrDataS.c_str());
                ADIntDec = oct2Dec(ADIntOct);
                if (local == '1')
                {
                    presentAddOrDataI = ADIntDec;
                }
                else
                {
                    pageNum = getPageOrOffsetNum(presentAddOrDataI, true);
                    offsetNum = getPageOrOffsetNum(presentAddOrDataI, false);
                    setWord(pageNum,offsetNum,ADIntDec,false);
                    presentAddOrDataI++;
                }
            }
        }
        myfile.close();
    }
    else
    {
        cout << "Unable to open file" << endl;
    }
}

#endif
