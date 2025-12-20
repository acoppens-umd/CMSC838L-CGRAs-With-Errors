//============================================================================
// Name        : hycube_sim.cpp
// Author      : Manupa Karunaratne
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include "data_structures.h"
#include "CGRA.h"
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include "debug.h"

//Uncomment this for 16-bit full chip
//#define ARCHI_16BIT
struct arguments
{
	string cmemfileName;
	string dmemfileName;
	string memallocfileName;
	string paniccodefileName;
	int MEM_SIZE = 4096; // for total memory size
	int xdim = 4;
	int ydim = 4;
	int type = 1; //for the momory arrangement 1: memories on left 2: memories on both sides
};

arguments parse_arguments(int argc, char *argv[])
{
	arguments ret;

	int aflag = 0;
	int bflag = 0;
	char *cvalue = NULL;
	int index;
	int c;

	opterr = 0;

	while ((c = getopt(argc, argv, "c:d:a:m:x:y:t:p:")) != -1)
		switch (c)
		{
		case 'c':
			ret.cmemfileName = string(optarg);
			break;
		case 'd':
			ret.dmemfileName = string(optarg);
			break;
		case 'a':
			ret.memallocfileName = string(optarg);
			break;
		case 'p':
			ret.paniccodefileName = string(optarg);
			break;
		case 'm':
			ret.MEM_SIZE = atoi(optarg);
			break;
		case 'x':
			ret.xdim = atoi(optarg);
			break;
		case 'y':
			ret.ydim = atoi(optarg);
			break;
		case 't':
			ret.type = atoi(optarg);
			break;
		case '?':
			if (optopt == 'c')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr,
						"Unknown option character `\\x%x'.\n",
						optopt);
		default:
			abort();
		}
		return ret;
}

//int MEM_SIZE = 4096;

void parsePanicCodeFile(string &paniccodefileName, map<int, string> &panicCodes) {
	ifstream file(paniccodefileName);
    if (!file.is_open()) {
        throw runtime_error("Failed to open panic code file: " + paniccodefileName);
    }

    string line;

    // Read header line
    if (!std::getline(file, line)) {
        throw std::runtime_error("Panic code file is empty");
    }

    // Read data lines
    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        stringstream ss(line);
		string panicCode;
		string panicString;

        if (!std::getline(ss, panicCode, ',') ||
            !std::getline(ss, panicString)) {
            throw std::runtime_error("Invalid data line: expected two columns");
        }

        panicCodes[stoi(panicCode)] = panicString;
    }
}

int main(int argc, char* argv[]) {

	if(argc < 4){
		cout << "HyCUBE_Sim expect four arguments : instruction trc, data trace, base address allocation and panic codes, optional( mem size (default 4096), cgra size x,y(default 4x4))\n";
		return -1;
	}

	arguments args = parse_arguments(argc,argv);
	string cmemfileName = args.cmemfileName;
	string dmemfileName = args.dmemfileName;
	string memallocfileName = args.memallocfileName;
	string paniccodefileName = args.paniccodefileName;
	int MEM_SIZE = args.MEM_SIZE;
	int xdim = args.xdim;
	int ydim = args.ydim;
	int type = args.type;
	
	map<int, string> panicCodes;

	parsePanicCodeFile(paniccodefileName, panicCodes);

//	string cmemfileName(argv[1]);
//	string dmemfileName(argv[2]);
//	string memsize(argv[4]);

//	MEM_SIZE = std::stoi(memsize);

	HyCUBESim::CGRA cgraInstance(xdim,ydim,type,MEM_SIZE);
	cgraInstance.configCGRA(cmemfileName,xdim,ydim);
//	if(argc==4 || argc==5){
//		cout << "Parsing data file with base address pointers\n";
//		string memallocfileName(argv[3]);
	if(memallocfileName[0] != '\0'){
		cgraInstance.parseDMEM(dmemfileName,memallocfileName);
	}else{
		cgraInstance.parseDMEM(dmemfileName);
	}
        cgraInstance.dumpRawData();
	//cgraInstance.printInterestedAddrOutcome();

	int count=0;
#ifdef ARCHI_16BIT
	cout << "XXX->" << cgraInstance.dmem[MEM_SIZE-2] << " " << MEM_SIZE-2 << "\n";
	while(cgraInstance.dmem[MEM_SIZE-2]==0){ 
		cout << "XXX->" << cgraInstance.dmem[MEM_SIZE-2] << "\n";
		cgraInstance.executeCycle(count);
		count++;
	}
#else
	while(cgraInstance.dmem[MEM_SIZE/2-1]==0){
		cgraInstance.executeCycle(count);
		count++;
	}
#endif

	//20 cycles for epilogue
	if (cgraInstance.dmem[MEM_SIZE / 2 - 1] == 1) {
		for(int i = 0; i < 20;i++){
			cgraInstance.executeCycle(count);
			count++;
		}
	}
	
	cgraInstance.printInterestedAddrOutcome();

#ifdef ARCHI_16BIT
	int exitCode = cgraInstance.dmem[MEM_SIZE-2];
#else
	int exitCode = cgraInstance.dmem[MEM_SIZE/2-1];
#endif

	if (panicCodes.find(exitCode) != panicCodes.end()) {
		cout << "Exited with Panic Message: \"" << panicCodes[exitCode] << "\"" << endl;
	} else {
		cout << "Exited Normally" << endl;
	}


	//cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
