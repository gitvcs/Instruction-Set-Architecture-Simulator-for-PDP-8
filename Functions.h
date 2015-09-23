/*********************************************************************************
 * File: 		Functions.h
 * Description: Implements a common set of functions which could be used anywhere.
 * Project: 	ECE 586 PDP-8 ISA Simulator
 * Version: 	0.3
 * Date:		01/24/2015
 * Author:		Prakhya, Doguparthi, Mutyala, Produtturi
 *********************************************************************************/

#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <cmath>
#include <algorithm>
#include <iostream>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

using namespace std;

extern ofstream myfile1;
extern int totalNumOfInst;
extern int totalNumOfCC;
extern int opcode0;
extern int opcode1;
extern int opcode2;
extern int opcode3;
extern int opcode4;
extern int opcode5;
extern int opcode6;
extern int opcode7;

int bin2Dec(int bit[]);
int oct2Dec(int octValue);
int dec2Oct(int value);
void createTraceFile(int memOP, int address);
int getAddFromPageAndOffset(int pageNum, int offsetNum);

/******************************************************************
* Take binary array as input and convert it to decimal
* Inputs: binary array (type: int)
* Output: decimal value (type: int)
*******************************************************************/

int bin2Dec(int bit[])
{
    int i = 11;
    int bin = 1;
    int total = 0;
    for (i; i >= 0; i--)
    {
        if (bit[i] == 1)
        {
            total = bin + total;
        }
        bin = (bin * 2);
    }
    return total;
}

/******************************************************************
* Convert octal value into decimal
* Inputs: Octal number (type: int)
* Output: Decimal number (type: int)
*******************************************************************/

int oct2Dec(int octValue)
{
	int result=0,index2=0,number=0;
	if(octValue < 10)
    {
        result = octValue;
    }
    else
    {
        while(octValue / 10)
        {
            number = octValue % 10;
            result += int((number * powl(8, index2)));
            index2++;
            octValue /= 10;
        }
        result += int((octValue * powl(8, index2)));
    }
    return result;
}

/******************************************************************
* Convert decimal value into octal
* Inputs: Decimal number (type: int)
* Output: Octal number (type: int)
*******************************************************************/

int dec2Oct(int value)
{
    int total = 1;
    int n = 0;
    while(value > 0)
    {
        total += int((pow(float(10),n++))*(value % 8));
        value /= 8;
    }
    //if (total < 8)
    //{
        return total-1;
    //}
    //else
    //{
    //    return total-1;
    //}
}

/******************************************************************
* Take contents and write it to Trace File
* Inputs: Memory Operation (0-Data read,1-Data Write,2-Instruction Read), address in octal
* Output: None
*******************************************************************/

void createTraceFile(int memOP, int address)
{
    if (myfile1.is_open())
    {
        myfile1 << memOP << " " << address << endl;
    }
    else
    {
        cout << "Unable to open file" << endl;
    }
}

/******************************************************************
* Take page number and offset number in decimal and get address in octal
* Inputs: Page number and Offset number in decimal (type: int)
* Output: Address in octal (type: int)
*******************************************************************/

int getAddFromPageAndOffset(int pageNum, int offsetNum)
{
    int decimal;
    string binaryP, binaryO;
    binaryP = std::bitset<5>(pageNum).to_string();
    binaryO = std::bitset<7>(offsetNum).to_string();
    binaryO = binaryP + binaryO;
    decimal = int(std::bitset<12>(binaryO).to_ulong());
    return dec2Oct(decimal);
}

int* twoscomplement(int accordataDec)
{
    int index1 = 0;
    int temp1Array[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
    static int accdataBinArray[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};
    while(accordataDec>0)
    {
        temp1Array[index1]=accordataDec%2;
        index1++;
        accordataDec=accordataDec/2;
    }
    for(int index1=0;index1<13;index1++)
    {
        accdataBinArray[index1]=temp1Array[12-index1];
    }
    for(int index2=0;index2<13;index2++)
    {
        if (accdataBinArray[index2])
        {
            accdataBinArray[index2]=0;
        }
        else
        {
            accdataBinArray[index2]=1;
        }
    }
    int i=12;
    int carry=1;
    while (i>=0)
    {
        if (accdataBinArray[i]==0 && carry==0)
        {
            accdataBinArray[i]=0;
        }
        else if (accdataBinArray[i]==0 && carry==1)
        {
            accdataBinArray[i]=1;
            carry=0;
        }
        else if (accdataBinArray[i]==1 && carry==0)
        {
            accdataBinArray[i]=1;
            carry=0;
        }
        else if (accdataBinArray[i]==1 && carry==1)
        {
            accdataBinArray[i]=0;
            carry=1;
        }
        else
        {
            cout <<"This should not happen";
        }
        i--;
    }
    return accdataBinArray;
}

/******************************************************************
* Take binary array as input and convert it to decimal
* Inputs: binary array (type: int)
* Output: decimal value (type: int)
*******************************************************************/

int bin2Dec1(int bit[])
{
    int i = 12;
    int bin = 1;
    int total = 0;
    for (i; i > 0; i--)
    {
        if (bit[i] == 1)
        {
            total = bin + total;
        }
        bin = (bin * 2);
    }
    return total;
}

void printStats()
{
    cout << "Total Number of Instructions Executed: " << totalNumOfInst << endl;
    cout << "Total Number of Clock Cycles Consumed: " << totalNumOfCC << endl;
    cout << "Total Number of AND Instructions Executed: " << opcode0 << endl;
    cout << "Total Number of TAD Instructions Executed: " << opcode1 << endl;
    cout << "Total Number of ISZ Instructions Executed: " << opcode2 << endl;
    cout << "Total Number of DCA Instructions Executed: " << opcode3 << endl;
    cout << "Total Number of JMS Instructions Executed: " << opcode4 << endl;
    cout << "Total Number of JMP Instructions Executed: " << opcode5 << endl;
    cout << "Total Number of I/O Instructions Executed: " << opcode6 << endl;
    cout << "Total Number of uOP Instructions Executed: " << opcode7 << endl;
}

#endif
