#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <vector>
#include <map>
#include <queue>
#include <stdint.h>

enum Regs{Reg0,Reg1,Reg2,Reg3};
enum XBarInput{NORTH_I,EAST_I,WEST_I,SOUTH_I,RES_I,ALU_I,INV};
enum Dir{NORTH,EAST,WEST,SOUTH};

typedef uint64_t DataType;

typedef struct{
	XBarInput P;
	XBarInput I1;
	XBarInput I2;
	XBarInput NORTH_O;
	XBarInput EAST_O;
	XBarInput WEST_O;
	XBarInput SOUTH_O;
} XbarConfig;

typedef struct{
	XbarConfig xB;
	std::map<Regs,uint8_t> regwen;
	std::map<Regs,uint8_t> regbypass; //1 = get it from reg and 0 = bypass
	uint8_t tregwen;
	uint8_t opcode;
	uint32_t constant;
	bool constValid;
	bool NPB;
} HyIns;

namespace HyInsDecode {
	const int NEGATED_PRED = 0;
	const int NEGATED_PRED_LEN = 1;

	const int CONSTANT_VALID = 1;
	const int CONSTANT_VALID_LEN = 1;

	const int CONSTANT = 2;
	const int CONSTANT_LEN = 32;

	const int OPCODE = 34;
	const int OPCODE_LEN = 8;

	const int NORTH_REG_WE = 42;
	const int NORTH_REG_WE_LEN = 1;
	
	const int WEST_REG_WE = 43;
	const int WEST_REG_WE_LEN = 1;

	const int SOUTH_REG_WE = 44;
	const int SOUTH_REG_WE_LEN = 1;

	const int EAST_REG_WE = 45;
	const int EAST_REG_WE_LEN = 1;

	const int TREG_WE = 46;
	const int TREG_WE_LEN = 1;

	const int SOUTH_REG_BYPASS = 47;
	const int SOUTH_REG_BYPASS_LEN = 1;

	const int NORTH_REG_BYPASS = 48;
	const int NORTH_REG_BYPASS_LEN = 1;

	const int WEST_REG_BYPASS = 49;
	const int WEST_REG_BYPASS_LEN = 1;

	const int EAST_REG_BYPASS = 50;
	const int EAST_REG_BYPASS_LEN = 1;

	const int ALU_P = 51;
	const int ALU_P_LEN = 3;

	const int ALU_I2 = 54;
	const int ALU_I2_LEN = 3;

	const int ALU_I1 = 57;
	const int ALU_I1_LEN = 3;

	const int NORTHO = 60;
	const int NORTHO_LEN = 3;

	const int WESTO = 63;
	const int WESTO_LEN = 3;

	const int SOUTHO = 66;
	const int SOUTHO_LEN = 3;

	const int EASTO = 69;
	const int EASTO_LEN = 3;
}

#endif
