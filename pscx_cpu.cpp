#include "pscx_cpu.h"

#include "pscx_instruction.h"
#include "pscx_interconnect.h"

#include <iostream>
#include <cassert>
#include "assert.h"
#include "stdint.h"
#include "limits.h"

#define assert(message, ...) do { \
    if(!(__VA_ARGS__)) { \
        /*error code*/ \
    } \
} while(0)

#define D_DEBUG

uint32_t Cpu::load32(uint32_t addr)
{
	return m_inter.load32(addr);
}

void Cpu::store32(uint32_t addr, uint32_t value)
{
	m_inter.store32(addr, value);
}

Cpu::InstructionType Cpu::decodeAndExecute(Instruction instruction)
{
	InstructionType instructionType = INSTRUCTION_TYPE_UNKNOWN;

	switch (instruction.getInstructionCode())
	{
	case 0b000000:
		switch(instruction.subfunction())
		{

			case /*SLL*/0b000000:
				instructionType = opcodeSLL(instruction);
				break;
			case /*OR*/0b100101:
				instructionType = opcodeOR(instruction);
				break;
			default:
				std::cout << "unhandled instruction 0x" << std::hex << instruction.getInstructionCode() << std::endl;
		}
		break;
	case /*J*/0b000010:
		instructionType = opcodeJ(instruction);
		break;
	case /*BNE*/0b000101:
		instructionType = opcodeBNE(instruction);
		break;
	case /*ADDI*/0b001000:
		instructionType = opcodeADDI(instruction);
		break;
	case /*ADDIU*/0b001001:
		instructionType = opcodeADDIU(instruction);
		break;
	case /*LUI*/0b001111:
		//??
		//---------------------------------
		// TODO : call LUI instruction.
		// Rust:
		// 0b001111 => self.op_lui(instruction)
		//---------------------------------
		//??
		instructionType = opcodeLUI(instruction);
		break;
	case /*ORI*/0b001101:
		instructionType = opcodeORI(instruction);
		break;
	case /*COP0*/0b010000:
		instructionType = opcodeCOP0(instruction);
		break;
	case /*LW*/0b100011:
		instructionType = opcodeLW(instruction);
		break;
	case /*SW*/0b101011:
		instructionType = opcodeSW(instruction);
		break;
	default:
		std::cout << "unhandled instruction 0x" << std::hex << instruction.getInstructionCode() << std::endl;
	}

#ifdef D_DEBUG
	char tmp_buf[1024] = { 0 };
	sprintf(tmp_buf, "%X [%s]", instruction.m_instruction, getInstructionDesc(instructionType).c_str());

	printDebugInfo("Instruction code is 0x" + std::string(tmp_buf));

	sprintf(tmp_buf, "%X", m_pc);

	printDebugInfo("PC REG: " + std::string(tmp_buf));

//	for(int i = 0; i < 32; i++)
//	{
//		sprintf(tmp_buf, "$%d REG: %X", i, m_regs[i]);
//		printDebugInfo(tmp_buf);
//	}

#endif

	return instructionType;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

Cpu::InstructionType Cpu::decodeAndExecute_backup(Instruction instruction)
{
	InstructionType instructionType = INSTRUCTION_TYPE_UNKNOWN;

	switch (instruction.getInstructionCode())
	{
	case /*LUI*/0b001111:
		//??
		//---------------------------------
		// TODO : call LUI instruction.
		// Rust:
		// 0b001111 => self.op_lui(instruction)
		//---------------------------------
		//??
		instructionType = opcodeLUI(instruction);
		break;
	case /*ORI*/0b001101:
		instructionType = opcodeORI(instruction);
		break;
	case /*SW*/0b101011:
		instructionType = opcodeSW(instruction);
		break;
	default:
		std::cout << "unhandled instruction 0x" << std::hex << instruction.getInstructionCode() << std::endl;
	}

#ifdef D_DEBUG
	char tmp_buf[1024] = { 0 };
	sprintf(tmp_buf, "%X", instruction.m_instruction);

	printDebugInfo("Instruction code is 0x" + std::string(tmp_buf));

	sprintf(tmp_buf, "%X", m_pc);

	printDebugInfo("PC REG: " + std::string(tmp_buf));

	for(int i = 0; i < 32; i++)
	{
		sprintf(tmp_buf, "$%d REG: %X", i, m_regs[i]);
		printDebugInfo(tmp_buf);
	}

#endif

	return instructionType;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void Cpu::printDebugInfo(std::string message)
{

	printf("[DEBUG INFO]:\t%s\n", message.c_str());

}

//??
//----------------------------------------------
// TODO : to implement the runNextInstruction function.
// It should fetch an instruction at program counter (PC) register,
// increment PC to point to the next instruction and
// execute instruction.
//
// Rust:
// pub fn run_next instruction(&mut self) {
//     let pc = self.pc;
//
//     Fetch instruction at PC
//     let instruction = self.load32(pc);
//
//     Increment PC to point to the next instruction.
//     self.pc = pc.wrapping_add(4);
//
//     self.decode_and_execute(instruction);
// }
//----------------------------------------------
//??
Cpu::InstructionType Cpu::runNextInstuction()
{
	// Fixme

	uint32_t pc = m_pc;

	uint32_t tmp_offset;
//	if(!m_inter.m_range.contains(pc, tmp_offset))
//	{
//		return INSTRUCTION_TYPE_UNKNOWN;
//	}


//	if((m_pc + 4) != m_next_pc)
//	{
//		pc = m_next_pc;
//	}

	Instruction instruction = load32(pc);

	m_pc = pc + 4;


//	m_next_pc = m_pc + 4;

	return decodeAndExecute(instruction);

//	return INSTRUCTION_TYPE_UNKNOWN;
}

//??
//--------------------------------------------------------------
// TODO : to implement the Load Upper Immediate function (LUI).
// It should load the immediate value into the high 16 bits of the target register.
//
// Rust:
// fn op_lui(&mut self, instruction: Instruction)
//     let i = instruction.imm();  load immediate value
//     let t = instruction.t();    load target register
//
//     Low 16 bits are set to 0
//     let v = i << 16;
//
//     self.set_reg(t, v);
// }
//---------------------------------------------------------------
//??



Cpu::InstructionType Cpu::opcodeLUI(Instruction instruction)
{
	// Fixme
	uint32_t imm_value = instruction.getImmediateValue();
	uint32_t target_reg = instruction.getRegisterTargetIndex();

	uint32_t value = imm_value << 16;

	setRegisterValue(target_reg, value);


	return INSTRUCTION_TYPE_LUI;
}

Cpu::InstructionType Cpu::opcodeORI(Instruction instruction)
{
	setRegisterValue(instruction.getRegisterTargetIndex(), instruction.getImmediateValue() | getRegisterValue(instruction.getRegisterSourceIndex()));
	return INSTRUCTION_TYPE_ORI;
}

Cpu::InstructionType Cpu::opcodeSW(Instruction instruction)
{

	if(m_sr & 0x10000 != 0)
	{
			std::cout << "Ignoring store while cache is isolated" << std::endl;
			return INSTRUCTION_TYPE_SW;
	}
	uint32_t registerSourceIndex = instruction.getRegisterSourceIndex();
	uint32_t registerTargetIndex = instruction.getRegisterTargetIndex();
	store32(getRegisterValue(registerSourceIndex) + instruction.getSignExtendedImmediateValue(), getRegisterValue(registerTargetIndex));
	return INSTRUCTION_TYPE_SW;
}

uint32_t Cpu::getRegisterValue(uint32_t index) const
{
	assert(index < _countof(m_regs), "Index out of boundaries");
	return m_regs[index];
}

void Cpu::setRegisterValue(uint32_t index, uint32_t value)
{
	assert(index < _countof(m_regs), "Index out of boundaries");
	if (index > 0) m_regs[index] = value;
}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

std::string Cpu::getInstructionDesc(InstructionType Type)
{

	std::string result = ins_type_str.at((size_t)Type);
	return result;


}


Cpu::InstructionType Cpu::opcodeSLL(Instruction instruction)
{
	uint32_t imm_val = instruction.shift();
	uint32_t target_reg = instruction.getRegisterTargetIndex();
	uint32_t dest_reg = instruction.getRegisterDestinationIndex();

	uint32_t value = getRegisterValue(target_reg) << imm_val;

	setRegisterValue(target_reg, value);

	return INSTRUCTION_TYPE_SLL;
}

Cpu::InstructionType Cpu::opcodeADDIU(Instruction instruction)
{
	uint32_t imm_val = instruction.getSignExtendedImmediateValue();
	uint32_t target_reg = instruction.getRegisterTargetIndex();
	uint32_t src_reg = instruction.getRegisterSourceIndex();

	uint32_t value = getRegisterValue(src_reg) + imm_val;

	setRegisterValue(target_reg, value);

	return INSTRUCTION_TYPE_ADDIU;

}


Cpu::InstructionType Cpu::opcodeJ(Instruction instruction)
{
	uint32_t imm_jump = instruction.imm_jump();

	m_pc = (m_pc & 0xf0000000) | (imm_jump << 2);


	return INSTRUCTION_TYPE_J;

}


Cpu::InstructionType Cpu::opcodeOR(Instruction instruction)
{

	uint32_t dest_reg = instruction.getRegisterDestinationIndex();
	uint32_t src_reg = instruction.getRegisterSourceIndex();
	uint32_t target_reg = instruction.getRegisterTargetIndex();

	uint32_t value = getRegisterValue(src_reg) | getRegisterValue(target_reg);

	setRegisterValue(dest_reg, value);

	return INSTRUCTION_TYPE_OR;

}

Cpu::InstructionType Cpu::opcodeCOP0(Instruction instruction)
{

	InstructionType result = INSTRUCTION_TYPE_COP0;

	switch(instruction.cop_opcode())
	{
//		case /*mfc0*/ 0b00000:
//			break;
		case /*mtc0*/ 0b00100:
			opcodeMTC0(instruction);
			break;
//		case /*rfe*/ 0b10000:
//			break;
		default:
			std::cout << "unhandled cop0 instruction 0x" << std::hex << instruction.getInstructionCode() << std::endl;

	}

	return result;

}


void Cpu::opcodeMTC0(Instruction instruction)
{

	uint32_t cpu_r = instruction.getRegisterTargetIndex();
	uint32_t cop_r = instruction.getRegisterDestinationIndex();

	uint32_t value = getRegisterValue(cpu_r);

	switch(cop_r)
	{
		case 12:
			m_sr = value;
			break;
		default:
			std::cout << "unhandled cop0 register 0x" << std::hex << cop_r << std::endl;
	}

}


void Cpu::branch(uint32_t offset)
{

//	int16_t off = (int16_t)offset;
//	offset = offset << 2;
//	off = (int32_t)(off << 2);
	uint32_t pc = m_pc;
	int32_t off = (int32_t)offset;
//	off *= 4;
/*	if(off < 0)
	{
		off = -off;
		pc -= (uint32_t)off;
	}
	else
	{
		pc += (uint32_t)off;
	} */
	pc = (uint32_t)(pc - (off - 2)*16);

//	pc -= 4;

//	pc = pc + (uint32_t)offset - 4;

	m_pc = pc;

//	m_next_pc = pc;

}

Cpu::InstructionType Cpu::opcodeBNE(Instruction instruction)
{

	uint32_t imm_val = instruction.getSignExtendedImmediateValue();
	uint32_t src_reg = instruction.getRegisterSourceIndex();
	uint32_t target_reg = instruction.getRegisterTargetIndex();

	if(getRegisterValue(src_reg) != getRegisterValue(target_reg))
	{
		branch(imm_val);
//		uint32_t offset = imm_val << 2;
//		m_pc = m_pc - offset;
	}

	m_regs[0] = 0x0;

	return INSTRUCTION_TYPE_BNE;
}

Cpu::InstructionType Cpu::opcodeADDI(Instruction instruction)
{

	int32_t imm_val = (int32_t)instruction.getSignExtendedImmediateValue();
	uint32_t src_reg = instruction.getRegisterSourceIndex();
	uint32_t target_reg = instruction.getRegisterTargetIndex();

	int32_t src_val = (int32_t)getRegisterValue(src_reg);

	int32_t value;
	bool flag_overflow = false;

	if((src_val > 0) && (imm_val > INT32_MAX - src_val))
		flag_overflow = true;
	if((src_val < 0) && (imm_val < INT32_MIN - src_val))
		flag_overflow = true;

	if(flag_overflow)
		std::cout << "ADDI operation value overflow" << std::endl;
	else
	{
		setRegisterValue(target_reg, (uint32_t)(src_val + imm_val));
	}

	return INSTRUCTION_TYPE_ADDI;

}

Cpu::InstructionType Cpu::opcodeLW(Instruction instruction)
{

	if(m_sr & 0x10000 != 0)
	{
		std::cout << "Ignoring load while cache is isolated" << std::endl;
		return INSTRUCTION_TYPE_LW;
	}

	uint32_t imm_val = (uint32_t)instruction.getSignExtendedImmediateValue();
	uint32_t src_reg = instruction.getRegisterSourceIndex();
	uint32_t target_reg = instruction.getRegisterTargetIndex();

	uint32_t addr = getRegisterValue(src_reg) + imm_val;

	uint32_t value = load32(addr);

	setRegisterValue(target_reg, value);

	return INSTRUCTION_TYPE_LW;

}

/*
void Cpu::run_next_instruction()
{

	uint32_t pc = m_pc;

	Instruction next_instruction = m_next_instruction;
	m_next_instruction = Instruction(load32(pc));

	m_pc = pc + 4;

	decodeAndExecute(next_instruction);

}

*/
