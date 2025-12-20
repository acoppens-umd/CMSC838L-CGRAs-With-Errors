/*
 * CGRA.cpp
 *
 *  Created on: 6 Mar 2017
 *      Author: manupa
 */

#include "CGRA.h"
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <assert.h>

bool ends_with(const std::string& str, const std::string& suffix) {
    // Check if the string is shorter than the suffix
    if (str.length() < suffix.length()) {
        return false;
    }
    
    // Compare the suffix part of the main string
    // str.compare(position, length, other_string)
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

namespace HyCUBESim {

CGRA::CGRA(int SizeX, int SizeY,int type, int MemSize) {
	// TODO Auto-generated constructor stub
	sizeX = SizeX;
	sizeY = SizeY;
	MEM_SIZE = MemSize;

	for (int y = 0; y < SizeY; ++y) {
		for (int x = 0; x < SizeX; ++x) {
			if(type == 1)
				CGRATiles[y][x] = new CGRATile(x,y,(x==0),&dmem);
			else if(type == 2)
				CGRATiles[y][x] = new CGRATile(x,y,(x==0 or (x==SizeX-1)),&dmem);
		}
	}

	for (int y = 0; y < SizeY; ++y) {
		for (int x = 0; x < SizeX; ++x) {

			if(x+1 < SizeX){
				CGRATiles[y][x]->connectedTiles[EAST]=CGRATiles[y][x+1];
			}

			if(y+1 < SizeY){
				CGRATiles[y][x]->connectedTiles[SOUTH]=CGRATiles[y+1][x];
			}

			if(x-1 >= 0){
				CGRATiles[y][x]->connectedTiles[WEST]=CGRATiles[y][x-1];
			}

			if(y-1 >= 0){
				CGRATiles[y][x]->connectedTiles[NORTH]=CGRATiles[y-1][x];
			}

		}
	}

	for (int i = 0; i < MEM_SIZE; ++i) {
		dmem[i]=0;
	}

}

int CGRA::configCGRA(std::string CMEMFileName,int xdim, int ydim) {

	std::ifstream cmemfile(CMEMFileName.c_str());
	std::string line;
	LOG(SIMULATOR) << CMEMFileName << "\n";
	assert(cmemfile.is_open() && "Error opening cmem file");

	//ignoring the first line
	std::getline(cmemfile,line);

	while(1){
		if(!std::getline(cmemfile,line)){
			break;
		}

		if(line.empty()){
			break;
		}

	    std::istringstream iss(line);
	    int t;

	    iss >> t;
	    for (int i = 0; i < xdim*ydim; ++i) {
		    std::getline(cmemfile,line);
		    std::istringstream iss(line);
		    std::string phyloc;
		    std::string op;

		    std::getline(iss,phyloc,',');
		    std::getline(iss,op,',');

		    int y=i/ydim;
		    int x=i%xdim;

		    LOG(SIMULATOR) << "T=" << t << ",Y=" << y << ",X=" << x << "," << op << "\n";

		    HyIns currIns;
		    if(atoi(op.substr(HyInsDecode::NEGATED_PRED,HyInsDecode::NEGATED_PRED_LEN).c_str())==1){
		    	currIns.NPB=true;
		    }
		    else{
		    	currIns.NPB=false;
		    }

		    if(atoi(op.substr(HyInsDecode::CONSTANT_VALID,HyInsDecode::CONSTANT_VALID_LEN).c_str())==1){
		    	currIns.constValid=true;
		    }
		    else{
		    	currIns.constValid=false;
		    }

			currIns.constant = std::stoi(op.substr(HyInsDecode::CONSTANT + 1,HyInsDecode::CONSTANT_LEN - 1),nullptr,2);
    
			if (op[HyInsDecode::CONSTANT] == '1') {
				currIns.constant = currIns.constant | 0b10000000000000000000000000000000;
			}

			//if((currIns.constant >> 26) == 1){ //negative number identification
			//	  currIns.constant = currIns.constant | 0b11111000000000000000000000000000;
			//}


			currIns.opcode = std::stoi(op.substr(HyInsDecode::OPCODE,HyInsDecode::OPCODE_LEN),nullptr,2);

		    currIns.regwen[Reg0] = std::stoi(op.substr(HyInsDecode::NORTH_REG_WE,HyInsDecode::NORTH_REG_WE_LEN),nullptr,2);
		    currIns.regwen[Reg2] = std::stoi(op.substr(HyInsDecode::WEST_REG_WE,HyInsDecode::WEST_REG_WE_LEN),nullptr,2);
		    currIns.regwen[Reg3] = std::stoi(op.substr(HyInsDecode::SOUTH_REG_WE,HyInsDecode::SOUTH_REG_WE_LEN),nullptr,2);
		    currIns.regwen[Reg1] = std::stoi(op.substr(HyInsDecode::EAST_REG_WE,HyInsDecode::EAST_REG_WE_LEN),nullptr,2);

		    currIns.tregwen = std::stoi(op.substr(HyInsDecode::TREG_WE,HyInsDecode::TREG_WE_LEN),nullptr,2);

		    currIns.regbypass[Reg3] = std::stoi(op.substr(HyInsDecode::SOUTH_REG_BYPASS,HyInsDecode::SOUTH_REG_BYPASS_LEN),nullptr,2);
		    currIns.regbypass[Reg0] = std::stoi(op.substr(HyInsDecode::NORTH_REG_BYPASS,HyInsDecode::NORTH_REG_BYPASS_LEN),nullptr,2);
		    currIns.regbypass[Reg2] = std::stoi(op.substr(HyInsDecode::WEST_REG_BYPASS,HyInsDecode::WEST_REG_BYPASS_LEN),nullptr,2);
		    currIns.regbypass[Reg1] = std::stoi(op.substr(HyInsDecode::EAST_REG_BYPASS,HyInsDecode::EAST_REG_BYPASS_LEN),nullptr,2);

		    LOG(SIMULATOR) << "XbConfig : " << op.substr(43,21) << "\n";
		    LOG(SIMULATOR) << "xB.I2 : " << op.substr(49,3) << "\n";

		    currIns.xB.P = convertStrtoXBI(op.substr(HyInsDecode::ALU_P,HyInsDecode::ALU_P_LEN));

//		    if(currIns.constValid){
//		    	currIns.xB.I2 = INV;
//		    }
//		    else{
		    	currIns.xB.I2 = convertStrtoXBI(op.substr(HyInsDecode::ALU_I2,HyInsDecode::ALU_I2_LEN));
//		    }
		    currIns.xB.I1 = convertStrtoXBI(op.substr(HyInsDecode::ALU_I1,HyInsDecode::ALU_I1_LEN));
		    currIns.xB.NORTH_O = convertStrtoXBI(op.substr(HyInsDecode::NORTHO,HyInsDecode::NORTHO_LEN));
		    currIns.xB.WEST_O = convertStrtoXBI(op.substr(HyInsDecode::WESTO,HyInsDecode::WESTO_LEN));
		    currIns.xB.SOUTH_O = convertStrtoXBI(op.substr(HyInsDecode::SOUTHO,HyInsDecode::SOUTHO_LEN));
		    currIns.xB.EAST_O = convertStrtoXBI(op.substr(HyInsDecode::EASTO,HyInsDecode::EASTO_LEN));

		    CGRATiles[y][x]->printIns(currIns);

		    CGRATiles[y][x]->configMem.push_back(currIns);
		}
	    std::getline(cmemfile,line);
	}
}

int CGRA::parseDMEM(std::string DMEMFileName) {

	std::ifstream dmemfile(DMEMFileName.c_str());
	std::string line;

	assert(dmemfile.is_open() && "Error opening dmem file");

	//ignore the first line
	std::getline(dmemfile,line);

	for (int i = 0; i < 4096; ++i) {
		dmem[i]=0;
	}

	while(std::getline(dmemfile,line)){
		std::istringstream iss(line);

		std::string addr;
		std::string pre;
		std::string post;

		std::getline(iss,addr,',');
		std::getline(iss,pre,',');
		std::getline(iss,post,',');

		LOG(SIMULATOR) << addr << "," << pre << "\n";
		InterestedAddrList.push_back(atoi(addr.c_str()));

		dmem[(DataType)atoi(addr.c_str())]=atoi(pre.c_str());
	}
}

int CGRA::parseDMEM(std::string DMEMFileName,std::string memallocFileName) {

	std::ifstream dmemfile(DMEMFileName.c_str());
	std::ifstream memallocfile(memallocFileName.c_str());
	std::string line;

	assert(dmemfile.is_open() && "Error opening dmem file");
	assert(memallocfile.is_open() && "Error opening memalloc file");
	//ignore the first line
	std::getline(dmemfile,line);
	std::getline(memallocfile,line);

	for (int i = 0; i < MEM_SIZE; ++i) {
		dmem[i]=0;
	}
	std::map<std::string, int> spm_base_addr;
	while(std::getline(memallocfile,line)){
		std::istringstream iss2(line);

		std::string var_name;
		std::string base_addr;

		std::getline(iss2,var_name,',');
		std::getline(iss2,base_addr,',');


		spm_base_addr[var_name]= atoi(base_addr.c_str());
		LOG(SIMULATOR) << var_name << "," << spm_base_addr[var_name] << "\n";
	}

	while(std::getline(dmemfile,line)){
		std::istringstream iss(line);

		std::string var_name;
		std::string offset;
		int addr;
		std::string pre;
		std::string post;

		std::getline(iss,var_name,',');
		std::getline(iss,offset,',');
		std::getline(iss,pre,',');
		std::getline(iss,post,',');

		if (ends_with(var_name, "carry")) { 
			addr = spm_base_addr[var_name.substr(0, var_name.length() - 5)]+atoi(offset.c_str()) + 4;
		} else {
			addr = spm_base_addr[var_name]+atoi(offset.c_str());
		}

		//LOG(SIMULATOR) << addr << "," << pre << "\n";
		InterestedAddrList.push_back(addr);

		dmem[(DataType)addr]=atoi(pre.c_str());
		dmem_post[(DataType)addr]=atoi(post.c_str());
		dmem_pre[(DataType)addr]=atoi(post.c_str());
	}
#ifndef ARCHI_16BIT
	dmem[MEM_SIZE-2]=1;//dmem[4094]=1;
	InterestedAddrList.push_back(MEM_SIZE-2);//InterestedAddrList.push_back(4094);
#endif
//	LOG(SIMULATOR) << "Data Memory Content\n";
//	for (int i = 0; i < 4096; ++i) {
//		LOG(SIMULATOR) << i << "," << (int)dmem[i] << "\n";
//	}
}

//dmem is byte addressable memory, base_addr is the byte address
//data size should be number of bytes
void CGRA::writeDMEM(HyCUBESim::CGRA& cgraInstance, int base_addr, uint8_t* data, int data_size) {
    for (int i = 0; i < data_size; ++i) {
        cgraInstance.dmem[base_addr + i] = data[i];
    }
}

void CGRA::readDMEM(HyCUBESim::CGRA& cgraInstance, int base_addr, uint8_t* data, int data_size) {
    for (int i = 0; i < data_size; ++i) {
        data[i] = cgraInstance.dmem[base_addr + i];
    }
}

void CGRA::invokeCGRA(HyCUBESim::CGRA& cgraInstance) {
    int count = 0;

#ifdef ARCHI_16BIT
    while (cgraInstance.dmem[MEM_SIZE - 2] == 0) {
        cgraInstance.executeCycle(count);
        count++;
    }
	if (cgraInstance.dmem[MEM_SIZE - 2] != 1) {
		return;
	}
#else
    while (cgraInstance.dmem[MEM_SIZE / 2 - 1] == 0) {
        cgraInstance.executeCycle(count);
        count++;
    }
	if (cgraInstance.dmem[MEM_SIZE / 2 - 1] != 1) {
		return;
	}
#endif

    // 20 cycles for epilogue
    for (int i = 0; i < 20; i++) {
        cgraInstance.executeCycle(count);
        count++;
    }
}

int CGRA::executeCycle(int kII) {
	std::cout << "executing cycle " << kII << std::endl;

	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			CGRATiles[y][x]->execute(kII);
		}
	}

	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			CGRATiles[y][x]->updateRegisters();
			CGRATiles[y][x]->updatePC();
		}
	}
}
void CGRA::dumpRawData(){
	std::ofstream myfile;
 	myfile.open ("dumped_raw_data.txt");
 
 
	int overall = dmem.size();
	for(int addr  = 0; addr < overall; addr ++){
		myfile<< addr << "," <<(int)dmem_pre[addr] << "," << (int)dmem_post[addr] << "\n";
	}
	 myfile.close();
}


void CGRA::printInterestedAddrOutcome() {
	LOG(SIMULATOR) << "Interested Addresses :: \n";
	for(int addr : InterestedAddrList){
		LOG(SIMULATOR) << addr << "," << (int)dmem[addr] << "\n";
	}
	int correct_count = 0;
	int wrong_count = 0;

	std::map<DataType,std::pair<uint8_t, uint8_t>> mismatches;

	for(int addr : InterestedAddrList){
		if((int)dmem[addr] == (int)dmem_post[addr]){
			correct_count++;
		}else{
			wrong_count++;

			mismatches[addr] = std::pair<uint8_t, uint8_t>((int)dmem[addr], (int)dmem_post[addr]);
			LOG(SIMULATOR) << "Data mismatch at address: "<< addr << ", result:" << (int)dmem[addr]<<", expected:" << (int)dmem_post[addr] << "\n";
			
		}
	}
	std::cout << "Simulation Result: Matches::"<<correct_count<<", Mismatches::"<<wrong_count<< "\n";
	std::ofstream rsltfile;
	rsltfile.open ("sim_result.txt");
	rsltfile<<correct_count<<","<<wrong_count<< "\n";
	rsltfile.close();

	std::ofstream mismatchfile;
	mismatchfile.open ("mismatches.txt");
	mismatchfile << "addr,expected_val,result_val" << "\n";
	for (const auto& [addr, vals] : mismatches) {
		mismatchfile << addr << "," << (int)vals.second << "," << (int)vals.first << "\n";
	}

	mismatchfile.close();
}

XBarInput CGRA::convertStrtoXBI(std::string str) {

	LOG(SIMULATOR) << "convertStr called : " << std::stoi(str,nullptr,2) << "\n";

	switch(std::stoi(str,nullptr,2)){
		case 0:
			return EAST_I;
		case 1:
			return SOUTH_I;
		case 2:
			return WEST_I;
		case 3:
			return NORTH_I;
		case 4:
			return ALU_I;
		case 5:
			return RES_I;
		case 7:
			return INV;
		default:
			LOG(SIMULATOR) << str;
			assert(false);
	}
}

} /* namespace HyCUBESim */
