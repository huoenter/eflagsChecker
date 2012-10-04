#include "pin.H"
#include <string>
#include <map>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <fstream>

typedef bool MARK;

ADDRINT Flags;

INT32 Usage() {
	cerr << "Wrong!" << endl;
	return -1;
}

void binary(int number) {
	int remainder;

	if(number <= 1) {
		cout << number;
		return;
	}

	remainder = number%2;
	binary(number >> 1);    
	cout << remainder;
}


void checkFlags(int before, int after) {
	if (before - after != 0) {
			cout << endl << "Before:";
			binary(before);
			cout << endl << "After: ";
			binary(after);
			cout << endl << "Flag changed: ";
			if (((before << 11 >> 31) ^ (after << 11 >> 31)) != 0) cout << "ID ";
			if (((before << 12 >> 31) ^ (after << 12 >> 31)) != 0) cout << "VIP ";
			if (((before << 13 >> 31) ^ (after << 13 >> 31)) != 0) cout << "VIF ";
			if (((before << 14 >> 31) ^ (after << 14 >> 31)) != 0) cout << "AC ";
			if (((before << 15 >> 31) ^ (after << 15 >> 31)) != 0) cout << "VM ";
			if (((before << 16 >> 31) ^ (after << 16 >> 31)) != 0) cout << "RF ";
			if (((before << 18 >> 31) ^ (after << 18 >> 31)) != 0) cout << "NT ";
			if (((before << 19 >> 31) ^ (after << 19 >> 31)) != 0) cout << "IOPL ";
			if (((before << 20 >> 31) ^ (after << 20 >> 31)) != 0) cout << "OF ";
			if (((before << 21 >> 31) ^ (after << 21 >> 31)) != 0) cout << "DF ";
			if (((before << 22 >> 31) ^ (after << 22 >> 31)) != 0) cout << "IF ";
			if (((before << 23 >> 31) ^ (after << 23 >> 31)) != 0) cout << "TF ";
			if (((before << 24 >> 31) ^ (after << 24 >> 31)) != 0) cout << "SF ";
			if (((before << 25 >> 31) ^ (after << 25 >> 31)) != 0) cout << "ZF ";
			if (((before << 27 >> 31) ^ (after << 27 >> 31)) != 0) cout << "AF ";
			if (((before << 29 >> 31) ^ (after << 29 >> 31)) != 0) cout << "PF ";
			if (((before << 31 >> 31) ^ (after << 31 >> 31)) != 0) cout << "CF ";
			cout << endl;
	}
}

VOID PrintFlagBefore(ADDRINT eflags, char* ins){
	char add[] = "ADD";
	if (0 == strcmp(ins, add)) {
		Flags = eflags;
	}
}

VOID PrintFlagAfter(ADDRINT eflags, char* ins){
	char add[] = "ADD";
	if (0 == strcmp(ins, add)) {
		checkFlags((int) Flags, (int) eflags);
	}
}

VOID Instruction(INS ins, VOID *v){
	if (INS_HasFallThrough(ins)) {
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)PrintFlagBefore, 
						IARG_REG_VALUE, REG_RFLAGS,
						IARG_ADDRINT, INS_Mnemonic(ins).c_str(), 
						IARG_END);
	INS_InsertCall(ins, IPOINT_AFTER, (AFUNPTR)PrintFlagAfter, 
						IARG_REG_VALUE, REG_RFLAGS,
						IARG_ADDRINT, INS_Mnemonic(ins).c_str(), 
						IARG_END);
	}

}

VOID Fini(INT32 code, VOID *v)
{
	cout << endl;
}

int main(int argc, char *argv[]){
	if( PIN_Init(argc, argv)) {return Usage();}
	
	PIN_InitSymbols();
	INS_AddInstrumentFunction(Instruction, 0);
	PIN_AddFiniFunction(Fini, 0);
	PIN_StartProgram();

	return 0;
}
