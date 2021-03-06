#ifndef PGB_CPU_INSTRUCTION_INFO_H
#define PGB_CPU_INSTRUCTION_INFO_H

#include <stdint.h>
#include <stdbool.h>
#include <pgb/device/device.h>

enum flag_modifier {
	FLAG_MODIFIER_RESET_AFTER_INSTRUCTION = 0,
	FLAG_MODIFIER_FLAG_SET = 1,
	FLAG_MODIFIER_AFFECTED_BY_FUNCTION = 2,
	FLAG_MODIFIER_NOT_USED = 3
};

enum isa_register {
	ISA_REGISTER_B = 0x0,
	ISA_REGISTER_C = 0x1,
	ISA_REGISTER_D = 0x2,
	ISA_REGISTER_E = 0x3,
	ISA_REGISTER_H = 0x4,
	ISA_REGISTER_L = 0x5,
	ISA_REGISTER_F = 0x6,
	ISA_REGISTER_A = 0x7
};

enum instruction_class {
	/* Core operations */
	INSTRUCTION_CLASS_ADC,
	INSTRUCTION_CLASS_ADD,
	INSTRUCTION_CLASS_ADDC,
	INSTRUCTION_CLASS_AND,
	INSTRUCTION_CLASS_CALL,
	INSTRUCTION_CLASS_CCF,
	INSTRUCTION_CLASS_CP,
	INSTRUCTION_CLASS_CPL,
	INSTRUCTION_CLASS_DAA,
	INSTRUCTION_CLASS_DEC,
	INSTRUCTION_CLASS_DI,
	INSTRUCTION_CLASS_EI,
	INSTRUCTION_CLASS_HALT,
	INSTRUCTION_CLASS_INC,
	INSTRUCTION_CLASS_JP,
	INSTRUCTION_CLASS_JR,
	INSTRUCTION_CLASS_LD,
	INSTRUCTION_CLASS_LDD,
	INSTRUCTION_CLASS_LDH,
	INSTRUCTION_CLASS_LDHL,
	INSTRUCTION_CLASS_LDI,
	INSTRUCTION_CLASS_NOP,
	INSTRUCTION_CLASS_OR,
	INSTRUCTION_CLASS_POP,
	INSTRUCTION_CLASS_PREFIX,
	INSTRUCTION_CLASS_PUSH,
	INSTRUCTION_CLASS_RET,
	INSTRUCTION_CLASS_RETI,
	INSTRUCTION_CLASS_RST,
	INSTRUCTION_CLASS_SBC,
	INSTRUCTION_CLASS_SCF,
	INSTRUCTION_CLASS_STOP,
	INSTRUCTION_CLASS_SUB,
	INSTRUCTION_CLASS_XOR,
	/* Prefix Operations */
	INSTRUCTION_CLASS_BIT,
	INSTRUCTION_CLASS_RES,
	INSTRUCTION_CLASS_RL,
	INSTRUCTION_CLASS_RLA,
	INSTRUCTION_CLASS_RLC,
	INSTRUCTION_CLASS_RLCA,
	INSTRUCTION_CLASS_RR,
	INSTRUCTION_CLASS_RRA,
	INSTRUCTION_CLASS_RRC,
	INSTRUCTION_CLASS_RRCA,
	INSTRUCTION_CLASS_SET,
	INSTRUCTION_CLASS_SLA,
	INSTRUCTION_CLASS_SRA,
	INSTRUCTION_CLASS_SRL,
	INSTRUCTION_CLASS_SWAP,
	/* Place holder for invalid operations */
	INSTRUCTION_CLASS_INVALID
};

enum instruction_operand_type {
	INSTRUCTION_OPERAND_TYPE_NONE,
	INSTRUCTION_OPERAND_TYPE_I8,
	INSTRUCTION_OPERAND_TYPE_I16,
	INSTRUCTION_OPERAND_TYPE_REGISTER8,
	INSTRUCTION_OPERAND_TYPE_REGISTER16,
	INSTRUCTION_OPERAND_TYPE_U3,
	INSTRUCTION_OPERAND_TYPE_U8,
	INSTRUCTION_OPERAND_TYPE_U16,
	INSTRUCTION_OPERAND_TYPE_VEC,
	INSTRUCTION_OPERAND_TYPE_CONDITION
};

enum instruction_operand {
	INSTRUCTION_OPERAND_NONE,
	INSTRUCTION_OPERAND_A,
	INSTRUCTION_OPERAND_B,
	INSTRUCTION_OPERAND_C,
	INSTRUCTION_OPERAND_D,
	INSTRUCTION_OPERAND_E,
	INSTRUCTION_OPERAND_F,
	INSTRUCTION_OPERAND_H,
	INSTRUCTION_OPERAND_L,
	INSTRUCTION_OPERAND_AF,
	INSTRUCTION_OPERAND_BC,
	INSTRUCTION_OPERAND_DE,
	INSTRUCTION_OPERAND_HL,
	INSTRUCTION_OPERAND_SP,
	INSTRUCTION_OPERAND_PC,
	/* Generic names for immediate values */
	INSTRUCTION_OPERAND_U3_0,
	INSTRUCTION_OPERAND_U3_1,
	INSTRUCTION_OPERAND_U3_2,
	INSTRUCTION_OPERAND_U3_3,
	INSTRUCTION_OPERAND_U3_4,
	INSTRUCTION_OPERAND_U3_5,
	INSTRUCTION_OPERAND_U3_6,
	INSTRUCTION_OPERAND_U3_7,
	INSTRUCTION_OPERAND_U3,
	INSTRUCTION_OPERAND_U8,
	INSTRUCTION_OPERAND_U16,
	INSTRUCTION_OPERAND_I8,
	INSTRUCTION_OPERAND_I16,
	/* Special values for jump */
	INSTRUCTION_OPERAND_COND_Z,
	INSTRUCTION_OPERAND_COND_NZ,
	INSTRUCTION_OPERAND_COND_C,
	INSTRUCTION_OPERAND_COND_NC,
	/* Reset Vectors */
	INSTRUCTION_OPERAND_VEC_00H,
	INSTRUCTION_OPERAND_VEC_08H,
	INSTRUCTION_OPERAND_VEC_10H,
	INSTRUCTION_OPERAND_VEC_18H,
	INSTRUCTION_OPERAND_VEC_20H,
	INSTRUCTION_OPERAND_VEC_28H,
	INSTRUCTION_OPERAND_VEC_30H,
	INSTRUCTION_OPERAND_VEC_38H,
};

enum instruction_operand_modifier {
	INSTRUCTION_OPERAND_MODIFIER_NONE,
	INSTRUCTION_OPERAND_MODIFIER_MEM_READ_8,
	INSTRUCTION_OPERAND_MODIFIER_MEM_WRITE_8,
	INSTRUCTION_OPERAND_MODIFIER_MEM_READ_16,
	INSTRUCTION_OPERAND_MODIFIER_MEM_WRITE_16,
	INSTRUCTION_OPERAND_MODIFIER_CB
};

struct instruction_info {
	const char *mnemonic;
	const char *assembly;
	uint8_t opcode;
	size_t num_bytes;
	bool is_prefix;
	struct {
		size_t c0;
		size_t c1;
	} timing;
	struct {
		uint8_t flag_mask;
		uint8_t flag_zero;
		uint8_t flag_sub;
		uint8_t flag_half_carry;
		uint8_t flag_carry;
	} flag_modifiers;
	struct {
		struct {
			enum instruction_operand_type type;
			enum instruction_operand operand;
			enum instruction_operand_modifier modifier;
		} a;
		struct {
			enum instruction_operand_type type;
			enum instruction_operand operand;
			enum instruction_operand_modifier modifier;
		} b;
	} operands;
	enum instruction_class instruction_class;
};

#endif /* PGB_CPU_INSTRUCTION_INFO_H */
