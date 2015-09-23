/*********************************************************************************
 * File:        FD.h
 * Description: Fetch and Decode a instruction and Calculate Effective Address from that instruction
 * Project: 	ECE 586 PDP-8 ISA Simulator
 * Version: 	0.3
 * Date:		01/29/2015
 * Author:		Prakhya, Doguparthi, Mutyala, Produtturi
 *********************************************************************************/

#ifndef FDE_H
#define FDE_H

#include <tuple>
#include "LoadMemory.h"

using namespace std;

//int totalNumOfCC;

int fetch(int pcDec);
tuple<int, int, int> decode(int mbDec);
tuple<int, int> calculateEffAddr(int mbDec, int opcode, int indirection, int zeroOrCP, int pcDec);

/******************************************************************
* Fetch contents at given memory location
* Inputs: Memory address in decimal (type: int)
* Output: Contents in decimal at given memory location (type: int)
*******************************************************************/

int fetch(int pcDec)
{
    int pageNum = getPageOrOffsetNum(pcDec, true);
    int offsetNum = getPageOrOffsetNum(pcDec, false);
    return getWord(pageNum,offsetNum,true);
}

/******************************************************************
* Decode a given instruction
* Inputs: Instruction in decimal (type: int)
* Output: Opcode, direct/indirect, Zero page/Current page (type: int)
*******************************************************************/

tuple<int, int, int> decode(int mbDec)
{
    int index = 0;
    int opcode = 8;
    int indirection = 0;
    int zeroOrCP = 0;
    int tempArray[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int decimalArray[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    while(mbDec>0)
    {
        tempArray[index]=mbDec%2;
        index++;
        mbDec=mbDec/2;
    }
    for(int index=0;index<12;index++)
    {
        decimalArray[index]=tempArray[11-index];
    }
    if ((decimalArray[0]==0) & (decimalArray[1]==0) & (decimalArray[2]==0))
        opcode = 0;
    else if ((decimalArray[0]==0) & (decimalArray[1]==0) & (decimalArray[2]==1))
        opcode = 1;
    else if ((decimalArray[0]==0) & (decimalArray[1]==1) & (decimalArray[2]==0))
        opcode = 2;
    else if ((decimalArray[0]==0) & (decimalArray[1]==1) & (decimalArray[2]==1))
        opcode = 3;
    else if ((decimalArray[0]==1) & (decimalArray[1]==0) & (decimalArray[2]==0))
        opcode = 4;
    else if ((decimalArray[0]==1) & (decimalArray[1]==0) & (decimalArray[2]==1))
        opcode = 5;
    else if ((decimalArray[0]==1) & (decimalArray[1]==1) & (decimalArray[2]==0))
        opcode = 6;
    else if ((decimalArray[0]==1) & (decimalArray[1]==1) & (decimalArray[2]==1))
        opcode = 7;
    if (decimalArray[3]==1)
        indirection = 1;
    if (decimalArray[4]==1)
        zeroOrCP = 1;
    return  make_tuple(opcode, indirection, zeroOrCP);
}

/******************************************************************
* Calculate effective address to fetch data
* Inputs: Instruction, PC in decimal, Opcode, direct/indirect, Zero page/Current page (type: int)
* Output: Page number and Offset number in decimal (type: int)
*******************************************************************/

tuple<int, int> calculateEffAddr(int mbDec, int opcode, int indirection, int zeroOrCP, int pcDec)
{
    int pageNo = 0;
    int offsetNo = 0;
    int eapageNo = 0;
    int eaoffsetNo = 0;
    int imbOct = 0;
    int imDec = 0;
    if ((zeroOrCP == 0) & (indirection == 0) & (opcode != 6) & (opcode != 7))
    {
        pageNo = 0;
        offsetNo = getPageOrOffsetNum(mbDec, false);
    }
    else if ((zeroOrCP == 1) & (indirection == 0) & (opcode != 6) & (opcode != 7))
    {
        pageNo = getPageOrOffsetNum(pcDec, true);
        offsetNo = getPageOrOffsetNum(mbDec, false);
    }
    else if ((zeroOrCP == 1) & (indirection == 1) & (opcode != 6) & (opcode != 7))
    {
        eapageNo = getPageOrOffsetNum(pcDec, true);
        eaoffsetNo = getPageOrOffsetNum(mbDec, false);
        imDec = getWord(eapageNo, eaoffsetNo, false);
        pageNo = getPageOrOffsetNum(imDec, true);
        offsetNo = getPageOrOffsetNum(imDec, false);
        totalNumOfCC++;
    }
    else if ((zeroOrCP == 0) & (indirection == 1) & (opcode != 6) & (opcode != 7))
    {
        eapageNo = 0;
        eaoffsetNo = getPageOrOffsetNum(mbDec, false);
        imDec = getWord(eapageNo, eaoffsetNo, false);
		if ((eaoffsetNo >= oct2Dec(10)) & (eaoffsetNo <= oct2Dec(17)))
			imDec++;
        pageNo = getPageOrOffsetNum(imDec, true);
        offsetNo = getPageOrOffsetNum(imDec, false);
        totalNumOfCC++;
    }
    return  make_tuple(pageNo, offsetNo);
}

#endif
