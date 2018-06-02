#include <assert.h>
#include <errno.h>
#include <stddef.h>

#include <pgb/cpu/isa.h>
#include <pgb/cpu/private/isa.h>
#include <pgb/cpu/registers.h>
#include <pgb/debug.h>
#include <pgb/utils.h>

/*
Instruction STOP has according to manuals opcode 10 00 and thus is 2 bytes long. Anyhow it seems there is no reason for it so some assemblers code it simply as one byte instruction 10.
Flags affected are always shown in Z H N C order. If flag is marked by "0" it means it is reset after the instruction. If it is marked by "1" it is set. If it is marked by "-" it is not changed. If it is marked by "Z", "N", "H" or "C" corresponding flag is affected as expected by its function.

d8  means immediate 8 bit data
d16 means immediate 16 bit data
a8  means 8 bit unsigned data, which are added to $FF00 in certain instructions (replacement for missing IN and OUT instructions)
a16 means 16 bit address
r8  means 8 bit signed data, which are added to program counter

LD A,(C) has alternative mnemonic LD A,($FF00+C)
LD C,(A) has alternative mnemonic LD ($FF00+C),A
LDH A,(a8) has alternative mnemonic LD A,($FF00+a8)
LDH (a8),A has alternative mnemonic LD ($FF00+a8),A
LD A,(HL+) has alternative mnemonic LD A,(HLI) or LDI A,(HL)
LD (HL+),A has alternative mnemonic LD (HLI),A or LDI (HL),A
LD A,(HL-) has alternative mnemonic LD A,(HLD) or LDD A,(HL)
LD (HL-),A has alternative mnemonic LD (HLD),A or LDD (HL),A
LD HL,SP+r8 has alternative mnemonic LDHL SP,r8
*/

static
struct isa_instruction isa_instruction_set[] = {
#define SET(MNEMONIC, ASSEMBLY, OPCODE, OPERATION_CLASS, OPERAND_A, MODIFIER_A, OPERAND_B, MODIFIER_B, NUM_BYTES, C0, C1, FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C, IS_PREFIX) \
	{#MNEMONIC, ASSEMBLY, OPCODE, NUM_BYTES, {C0, C1}, {FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C}, IS_PREFIX, {OPERAND_A, OPERAND_B, MODIFIER_A, MODIFIER_B}, OPERATION_CLASS},
#include <pgb/cpu/private/isa.def>
#undef SET
};

static
struct isa_instruction prefix_isa_instruction_set[] = {
#define SET(MNEMONIC, ASSEMBLY, OPCODE, OPERATION_CLASS, OPERAND_A, MODIFIER_A, OPERAND_B, MODIFIER_B, NUM_BYTES, C0, C1, FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C, IS_PREFIX) \
	{#MNEMONIC, ASSEMBLY, OPCODE, NUM_BYTES, {C0, C1}, {FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C}, IS_PREFIX, {OPERAND_A, OPERAND_B, MODIFIER_A, MODIFIER_B}, OPERATION_CLASS},
#include <pgb/cpu/private/prefix_cb.def>
#undef SET
};

int isa_get_instruction(uint8_t opcode, struct isa_instruction **instruction)
{
	unsigned i;

	// XXX: Make lookup faster
	for (i = 0; i < ARRAY_SIZE(isa_instruction_set); i++) {
		if (opcode == isa_instruction_set[i].opcode) {
			*instruction = &isa_instruction_set[i];
			return 0;
		}
	}

	return -EINVAL;
}

int isa_prefix_cb_get_instruction(uint8_t opcode, struct isa_instruction **instruction)
{
	unsigned i;

	// XXX: Make lookup faster
	for (i = 0; i < ARRAY_SIZE(prefix_isa_instruction_set); i++) {
		if (opcode == prefix_isa_instruction_set[i].opcode) {
			*instruction = &prefix_isa_instruction_set[i];
			return 0;
		}
	}

	return -EINVAL;
}

static
int isa_execute_instruction_adc(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_add8(struct device *device, struct decoded_instruction *instruction)
{
	int ret;
	struct cpu *cpu;
	uint8_t reg_a = 0, reg_b = 0, result;
	enum isa_operand operand_a, operand_b;

	cpu = &device->cpu;
	operand_a = instruction->info->operands.a;
	operand_b = instruction->info->operands.b;

	OK_OR_RETURN(operand_a >= ISA_OPERAND_A && operand_a <= ISA_OPERAND_L, -EINVAL);
	OK_OR_RETURN(operand_b >= ISA_OPERAND_A && operand_b <= ISA_OPERAND_L, -EINVAL);

	ret = cpu_register_read8(cpu, operand_a, &reg_a);
	OK_OR_RETURN(ret == 0, ret);

	ret = cpu_register_read8(cpu, operand_b, &reg_b);
	OK_OR_RETURN(ret == 0, ret);

	ret = cpu_register_write8(cpu, operand_a, reg_a + reg_b);
	OK_OR_RETURN(ret == 0, ret);

	result = reg_a + reg_b;
	(void) result;
	/*
	Z: Set if the result is 0; otherwise reset.
	H: Set if there is a carry from bit 3; otherwise reset.
	N: Reset
	CY: Set if there is a carry from bit 7; otherwise reset.
	*/

	return 0;
}

static
int isa_execute_instruction_add(struct device *device, struct decoded_instruction *instruction)
{
	int ret;

	ret = isa_execute_instruction_add8(device, instruction);
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int isa_execute_instruction_addc(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_and(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_call(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_ccf(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_cp(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_cpl(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_daa(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_dec(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_di(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_ei(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_halt(struct device *device, struct decoded_instruction *instruction)
{
	device->cpu.status.halted = true;

	return 0;
}

static
int isa_execute_instruction_inc(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_jp(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_jr(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_ld(struct device *device, struct decoded_instruction *instruction)
{
	// LD r, r'
	// LD r, n
	// LD H, (HL)
	// LD (HL), r
	// LD (HL), n
	//

	return 0;
}

static
int isa_execute_instruction_ldd(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_ldh(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_ldhl(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_ldi(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_nop(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_or(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_pop(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_prefix(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_push(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_ret(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_reti(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_rst(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_sbc(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_scf(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_stop(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_sub(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_xor(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_bit(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_res(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_rl(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_rla(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_rlc(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_rlca(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_rr(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_rra(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_rrc(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_rrca(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_set(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_sla(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_sra(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_srl(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int isa_execute_instruction_swap(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

int isa_execute_instruction(struct device *device, struct decoded_instruction *instruction)
{
	int ret = 0;

	assert(instruction->info != NULL && "Instruction information should not be NULL");

	switch (instruction->info->isa_operation) {
	case ISA_OPERATION_ADC:
		ret = isa_execute_instruction_adc(device, instruction);
		break;
	case ISA_OPERATION_ADD:
		ret = isa_execute_instruction_add(device, instruction);
		break;
	case ISA_OPERATION_ADDC:
		ret = isa_execute_instruction_addc(device, instruction);
		break;
	case ISA_OPERATION_AND:
		ret = isa_execute_instruction_and(device, instruction);
		break;
	case ISA_OPERATION_CALL:
		ret = isa_execute_instruction_call(device, instruction);
		break;
	case ISA_OPERATION_CCF:
		ret = isa_execute_instruction_ccf(device, instruction);
		break;
	case ISA_OPERATION_CP:
		ret = isa_execute_instruction_cp(device, instruction);
		break;
	case ISA_OPERATION_CPL:
		ret = isa_execute_instruction_cpl(device, instruction);
		break;
	case ISA_OPERATION_DAA:
		ret = isa_execute_instruction_daa(device, instruction);
		break;
	case ISA_OPERATION_DEC:
		ret = isa_execute_instruction_dec(device, instruction);
		break;
	case ISA_OPERATION_DI:
		ret = isa_execute_instruction_di(device, instruction);
		break;
	case ISA_OPERATION_EI:
		ret = isa_execute_instruction_ei(device, instruction);
		break;
	case ISA_OPERATION_HALT:
		ret = isa_execute_instruction_halt(device, instruction);
		break;
	case ISA_OPERATION_INC:
		ret = isa_execute_instruction_inc(device, instruction);
		break;
	case ISA_OPERATION_JP:
		ret = isa_execute_instruction_jp(device, instruction);
		break;
	case ISA_OPERATION_JR:
		ret = isa_execute_instruction_jr(device, instruction);
		break;
	case ISA_OPERATION_LD:
		ret = isa_execute_instruction_ld(device, instruction);
		break;
	case ISA_OPERATION_LDD:
		ret = isa_execute_instruction_ldd(device, instruction);
		break;
	case ISA_OPERATION_LDH:
		ret = isa_execute_instruction_ldh(device, instruction);
		break;
	case ISA_OPERATION_LDHL:
		ret = isa_execute_instruction_ldhl(device, instruction);
		break;
	case ISA_OPERATION_LDI:
		ret = isa_execute_instruction_ldi(device, instruction);
		break;
	case ISA_OPERATION_NOP:
		ret = isa_execute_instruction_nop(device, instruction);
		break;
	case ISA_OPERATION_OR:
		ret = isa_execute_instruction_or(device, instruction);
		break;
	case ISA_OPERATION_POP:
		ret = isa_execute_instruction_pop(device, instruction);
		break;
	case ISA_OPERATION_PREFIX:
		ret = isa_execute_instruction_prefix(device, instruction);
		break;
	case ISA_OPERATION_PUSH:
		ret = isa_execute_instruction_push(device, instruction);
		break;
	case ISA_OPERATION_RET:
		ret = isa_execute_instruction_ret(device, instruction);
		break;
	case ISA_OPERATION_RETI:
		ret = isa_execute_instruction_reti(device, instruction);
		break;
	case ISA_OPERATION_RST:
		ret = isa_execute_instruction_rst(device, instruction);
		break;
	case ISA_OPERATION_SBC:
		ret = isa_execute_instruction_sbc(device, instruction);
		break;
	case ISA_OPERATION_SCF:
		ret = isa_execute_instruction_scf(device, instruction);
		break;
	case ISA_OPERATION_STOP:
		ret = isa_execute_instruction_stop(device, instruction);
		break;
	case ISA_OPERATION_SUB:
		ret = isa_execute_instruction_sub(device, instruction);
		break;
	case ISA_OPERATION_XOR:
		ret = isa_execute_instruction_xor(device, instruction);
		break;
	/* Start of prefix instructions */
	case ISA_OPERATION_BIT:
		ret = isa_execute_instruction_bit(device, instruction);
		break;
	case ISA_OPERATION_RES:
		ret = isa_execute_instruction_res(device, instruction);
		break;
	case ISA_OPERATION_RL:
		ret = isa_execute_instruction_rl(device, instruction);
		break;
	case ISA_OPERATION_RLA:
		ret = isa_execute_instruction_rla(device, instruction);
		break;
	case ISA_OPERATION_RLC:
		ret = isa_execute_instruction_rlc(device, instruction);
		break;
	case ISA_OPERATION_RLCA:
		ret = isa_execute_instruction_rlca(device, instruction);
		break;
	case ISA_OPERATION_RR:
		ret = isa_execute_instruction_rr(device, instruction);
		break;
	case ISA_OPERATION_RRA:
		ret = isa_execute_instruction_rra(device, instruction);
		break;
	case ISA_OPERATION_RRC:
		ret = isa_execute_instruction_rrc(device, instruction);
		break;
	case ISA_OPERATION_RRCA:
		ret = isa_execute_instruction_rrca(device, instruction);
		break;
	case ISA_OPERATION_SET:
		ret = isa_execute_instruction_set(device, instruction);
		break;
	case ISA_OPERATION_SLA:
		ret = isa_execute_instruction_sla(device, instruction);
		break;
	case ISA_OPERATION_SRA:
		ret = isa_execute_instruction_sra(device, instruction);
		break;
	case ISA_OPERATION_SRL:
		ret = isa_execute_instruction_srl(device, instruction);
		break;
	case ISA_OPERATION_SWAP:
		ret = isa_execute_instruction_swap(device, instruction);
		break;
	default:
		ret = -EINVAL;
		fprintf(stderr, "Encountered unexpected operation type '%02x'. (Emulator error?)\n", instruction->info->isa_operation);
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}