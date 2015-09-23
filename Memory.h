/*********************************************************************************
 * File: 		Memory.h
 * Description: Defines and initiates 4k memory. Defines methods to access memory. Like setWord(), getWord(), init().
 * Project: 	ECE 586 PDP-8 ISA Simulator
 * Version: 	0.3
 * Date:		01/24/2015
 * Author:		Prakhya, Doguparthi, Mutyala, Produtturi
 *********************************************************************************/

#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include "Functions.h"

using namespace std;

void setWord(int pageNum, int wordNum, int word);
int getWord(int pageNum, int wordNum, bool IorD);
void init();
void printMem();

/******************************************************************
* Define a memory of 4K words having 32 pages, 128 words/page and 2 integer vectors/word
*******************************************************************/

vector<int> Word(2);
vector< vector<int> > Page(128, Word);
vector< vector< vector<int> > > Memory(32, Page);

/******************************************************************
* Set given word at given address. Address in format of page number and word number in decimal
* Inputs: Page number (type: int), Word number (type: int) both in decimal, word to be loaded into memory,
          FDE - write this write operation into Trace File when it is not used for load memory
* Output: None
*******************************************************************/

void setWord(int pageNum, int wordNum, int wordToSet, bool FDE)
{
    Memory[pageNum][wordNum][0] = wordToSet;
    Memory[pageNum][wordNum][1] = 1;
    if (FDE)
    {
        createTraceFile(1, getAddFromPageAndOffset(pageNum, wordNum));
    }
}

/******************************************************************
* Get word at given address. Address in format of page number and word number both in decimal
* Input: Page number (type: int), Word number (type: int) both in decimal, IorD
         Write this operation to Trace File differently depending whether it's a read request for instruction or data.
* Output: Data at given address (type: int)
*******************************************************************/

int getWord(int pageNum, int wordNum, bool IorD)
{
    if (IorD)
    {
        createTraceFile(2, getAddFromPageAndOffset(pageNum, wordNum));
    }
    else
    {
        createTraceFile(0, getAddFromPageAndOffset(pageNum, wordNum));
    }
    return Memory[pageNum][wordNum][0];
}

/******************************************************************
* Initialize all bits in memory to 0
* Inputs: None
* Output: None
*******************************************************************/

void init()
{
	for (int index0=31;index0>=0;index0--)
	{
		for (int index1=127;index1>=0;index1--)
		{
		    Memory[index0][index1][0] = 0;
		    Memory[index0][index1][1] = 0;
		}
	}
}

/******************************************************************
* Print contents of memory for which valid bit is set
* Inputs: None
* Output: None
*******************************************************************/

void printMem()
{
    cout << "Page" << "\t" << "Offset" << "\t" << "Octal" << "\t" << "Decimal" << "\t" << "Octal"<< endl;
    for (int index0=31;index0>=0;index0--)
	{
		for (int index1=127;index1>=0;index1--)
		{
		    if (Memory[index0][index1][1]==1)
            {
                cout << index0 << "\t";
                cout << index1 << "\t";
				cout << getAddFromPageAndOffset(index0,index1) << "\t";
                cout << Memory[index0][index1][0] << "\t";
                cout << dec2Oct(Memory[index0][index1][0]) << endl;
            }
		}
	}
}

#endif
