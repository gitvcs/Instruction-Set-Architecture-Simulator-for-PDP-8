/*********************************************************************************
 * File: 		Execute.h
 * Description: Test loadMemory() method of LoadMemory.h
 * Project: 	ECE 586 PDP-8 ISA Simulator
 * Version: 	0.3
 * Date:		01/29/2015
 * Author:		Prakhya, Doguparthi, Mutyala, Produtturi
 *********************************************************************************/

#ifndef EXECUTE_H
#define EXECUTE_H

#include "Memory.h"

using namespace std;

extern int exit1;

int totalNumOfInst = 0;
int totalNumOfCC = 0;
int opcode0 = 0;
int opcode1 = 0;
int opcode2 = 0;
int opcode3 = 0;
int opcode4 = 0;
int opcode5 = 0;
int opcode6 = 0;
int opcode7 = 0;
int printerFlag = 0;
int keyBoardFlag = 0;
int keyBoardBuffer[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int printerBuffer[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int time = rand() % 100 + 1;

int acc[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};

int execute(int opcode,int pageNo,int offsetNo,int mbDec,int pcDec);
void group1uop(int instruction[]);
int group2uop(int instruction[],int pcDec);
int execute7(int mbDec, int pcDec);
int execute6(int mbDec, int pcDec);

/******************************************************************
* Execute given instruction
* Inputs: Opcode of instruction, Page number and Offset number of data, instruction in decimal (type: int), pc in decimal (type: int)
* Output: pc in decimal (type: int)
*******************************************************************/

int execute(int opcode,int pageNo,int offsetNo,int mbDec,int pcDec)
{
    int dataBinArray[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int binary1Array[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int gottwosdata[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
    int *getaccordata;
    int index = 0;
    int link = 0;
    int dataDec = 0;
    if (opcode==0 || opcode==1 || opcode==2)
    {
        dataDec = getWord(pageNo,offsetNo,false);
        int dataDec1 = dataDec;
        while(dataDec1>0)                                    //Convert decimal dataDec to binary
        {
            dataBinArray[11-index]=dataDec1%2;
            index++;
            dataDec1=dataDec1/2;
        }
    }
    switch(opcode)
    {
        case 0:
            cout << "This is opcode 0" <<endl;
            for(int index=1;index<13;index++)
            {
                acc[index]=dataBinArray[index] && acc[index];
            }
            totalNumOfInst++;
            totalNumOfCC = totalNumOfCC + 2;
            opcode0++;
        break;
        case 1:
            cout << "This is opcode 1" <<endl;
            getaccordata=twoscomplement(dataDec);
            for (int index=0;index<13;index++)
            {
                gottwosdata[index] = *(getaccordata + index);
            }
            for(int index=12;index>=0;index--)
            {
                acc[index]= ((acc[index]^gottwosdata[index]) ^ link);
                link = ((acc[index] & gottwosdata[index]) | (acc[index] & link)) | (gottwosdata[index] & link);
            }
            acc[0]=link;
            totalNumOfInst++;
            totalNumOfCC = totalNumOfCC + 2;
            opcode1++;
        break;
        case 2:
            cout << "This is opcode 2" << endl;
            setWord(pageNo,offsetNo,dataDec,true);
            if(dataDec==0)
            {
                pcDec++;
            }
            totalNumOfInst++;
            totalNumOfCC = totalNumOfCC + 2;
            opcode2++;
        break;
        case 3:
            cout << "This is opcode 3" << endl;
            setWord(pageNo,offsetNo,bin2Dec1(acc),true);
            for (int index=1; index<13; index++)
            {
                acc[index] = 0;
            }
            totalNumOfInst++;
            totalNumOfCC = totalNumOfCC + 2;
            opcode3++;
        break;
        case 4:
            cout << "This is opcode 4" <<endl;
            setWord(pageNo,offsetNo,pcDec,true);
            if (offsetNo==127)
            {
                pageNo++;
                offsetNo = 0;
            }
            else
            {
                offsetNo++;
            }
            pcDec = oct2Dec(getAddFromPageAndOffset(pageNo, offsetNo));
            totalNumOfInst++;
            totalNumOfCC = totalNumOfCC + 2;
            opcode4++;
        break;
        case 5:
            //cout << "This is opcode 5" <<endl;
            pcDec = oct2Dec(getAddFromPageAndOffset(pageNo, offsetNo));
            totalNumOfInst++;
            totalNumOfCC++;
            opcode5++;
        break;
        default:
            cout<<"You are not a opcode" << endl;
        break;
    }
    return pcDec;
}

/******************************************************************
* Execute group1uop and group2uop functions depending on given instruction
* Inputs: Instruction in decimal (type: int), pc in decimal (type: int)
* Output: pc in decimal (type: int)
*******************************************************************/

int execute7(int mbDec, int pcDec)
{
    cout << "This is opcode 7" << endl;
    int binary1Array[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int index = 0;
    while(mbDec>0)                                      //mbDec = fetch(pcDec)  equivalent to IR (Instruction Register)
    {                                                   //Convert decimal mbDec to binary
        binary1Array[11-index]=mbDec%2;
        index++;
        mbDec=mbDec/2;
    }
    if(binary1Array[3]==0)
    {
        group1uop(binary1Array);
    }
    else if((binary1Array[3]==1) & (binary1Array[11]==0))
    {
        pcDec = group2uop(binary1Array,pcDec);
    }
    else
    {
        cout<<"This is opcode 7 group 3 instruction"<<endl;
    }
    totalNumOfInst++;
    totalNumOfCC++;
    opcode7++;
    return pcDec;
}

void group1uop(int instruction[])
{
    int group1[12];
    //int binary1Array[12];
    for(int index=0;index<12;index++)
    {
        group1[index] = *(instruction + index);
    }
    if(group1[4])
    {
        for (int index=1;index<13;index++)
        {
            acc[index] = 0;
        }
    }
    if(group1[5])
    {
        acc[0]=0;
    }
    if(group1[6])
    {
        for(int index=1;index<13;index++)
        {
            if (acc[index])
            {
                acc[index]=0;
            }
            else
            {
                acc[index]=1;
            }
        }
    }
    if(group1[7])
    {
        if(acc[0])
        {
            acc[0]=0;
        }
        else
        {
            acc[0]=1;
        }
    }
    if(group1[11])
    {
        int index = 0;
        int accDec=bin2Dec1(acc);
        accDec++;
        while(accDec>0)
        {
            acc[12-index]=accDec%2;
            index++;
            accDec=accDec/2;
        }
    }
    if(group1[8])
    {
        int temp=acc[12];
        for(int index=0;index<12;index++)
        {
            acc[index+1]=acc[index];
        }
        acc[0]=temp;
    }
    if(group1[8] && group1[10])
    {
        for(int index=2;index>0;index--)
        {
            int temp=acc[12];
            for(int index=0;index<12;index++)
            {
                acc[index+1]=acc[index];
            }
            acc[0]=temp;
        }
    }
    if(group1[9])
    {
        int temp=acc[0];
        for(int index=0;index<12;index++)
        {
            acc[index]=acc[index+1];
        }
        acc[12]=temp;
    }
    if(group1[9] && group1[10])
    {
        for(int index=2;index>0;index--)
        {
            int temp=acc[0];
            for(int index=0;index<12;index++)
            {
                acc[index]=acc[index+1];
            }
            acc[12]=temp;
        }
    }
}

int group2uop(int instruction[],int pcDec)
{
    int group2[12];
    int switchReg[12] = {0,0,0,0,0,0,0,0,0,1,1,1};
    for(int index=0;index<12;index++)
    {
        group2[index] = *(instruction + index);
    }
    if (group2[8])
    {
        //subgroup B
        int increment = 0;
        if (group2[5])
        {
            if(acc[1]==0)
            {
                increment++;
            }
        }
        if (group2[6])
        {
            if(bin2Dec1(acc)!=0)
            {
                increment++;
            }
        }
        if (group2[7])
        {
            if(acc[0]==0)
            {
                increment++;
            }
        }
        if (increment==1 || increment==3)
        {
            pcDec++;
        }
    }
    else
    {
        //subgroup A
        int increment = 0;
        if (group2[5])
        {
            if(acc[1])
            {
                increment++;
            }
        }
        if (group2[6])
        {
            if(bin2Dec1(acc)==0)
            {
                increment++;
            }
        }
        if (group2[7])
        {
            if(acc[0])
            {
                increment++;
            }
        }
        if (increment>0)
        {
            pcDec++;
        }
    }
    if (group2[4])
    {
        //subgroup C
        for (int index=1; index<13; index++)
        {
            acc[index] = 0;
        }
    }
    if (group2[9])
    {
        //subgroup C
        for(int index=1;index<13;index++)
        {
            acc[index]= switchReg[index-1]|| acc[index];
        }
    }
    if (group2[10])
    {
        //subgroup C
        exit1 = 1;
    }
    return pcDec;
}

/******************************************************************
* Execute opcode 6 instruction
* Inputs: Instruction in decimal (type: int), pc in decimal (type: int)
* Output: pc in decimal (type: int)
*******************************************************************/

int execute6(int mbDec, int pcDec)
{
	//cout << "This is opcode 6" << endl;
	totalNumOfInst++;
    opcode6++;
    int binary1Array[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	int binary2Array[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int index = 0;
    while(mbDec>0)                                      //mbDec = fetch(pcDec)  equivalent to IR (Instruction Register)
    {                                                   //Convert decimal mbDec to binary
        binary1Array[11-index]=mbDec%2;
        index++;
        mbDec=mbDec/2;
    }
	for (index=8;index>2;index--)
	{
		binary2Array[index+3] = binary1Array[index];
	}
	int device = bin2Dec(binary2Array);
	for (index=11;index>8;index--)
	{
		binary2Array[index] = binary1Array[index];
	}
	int func = bin2Dec(binary2Array);
	if ((device==3) & (func==1))						//KSF
	{
		for (int time = 0; time < rand() % 100 + 1; time++)
		{}
		keyBoardFlag = 1;
        if (keyBoardFlag == 1)
			pcDec = pcDec+1;
	}
    else if ((device==3) & (func==2))					//KCC
	{
        keyBoardFlag = 0;
		for (int index=1; index<13; index++)
        {
            acc[index] = 0;
        }
	}
    else if ((device==3) & (func==6))					//KRB
	{
		keyBoardFlag = 0;
		for (int index=1; index<13; index++)
        {
            acc[index] = 0;
        }
		for (int index=5; index<13; index++)
        {
            acc[index] = keyBoardBuffer[index-5];
        }
	}
    else if ((device==4) & (func==1))					//TSF
    {
		//for (int time = 0; time < rand() % 100 + 1; time++)
		//{}
		if (time > 0)
			throw "Waiting for printer to be ready";
		//printerFlag = 1;
        if (printerFlag == 1)
			pcDec = pcDec+1;
	}
	else if ((device==4) & (func==6))					//TLS
	{
		printerFlag = 0;
		for (int index=5; index<13; index++)
        {
            printerBuffer[index] = acc[index];
        }
		cout << "Teletype: " << bin2Dec1(printerBuffer) << endl;
		time = rand() % 100 + 1;
	}
	else
	{}
	return pcDec;
}

#endif
