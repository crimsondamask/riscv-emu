#include "../includes/cpu.h"
#include "../includes/opcodes.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[31m"
#define ANSI_RESET "\x1b[0m"

#define ADDR_MISALIGNED(addr) (addr & 0x3)

void print_op(char *s) { printf("%s%s%s", ANSI_BLUE, s, ANSI_RESET); }

void cpu_init(CPU *cpu) {
  cpu->regs[0] = 0x00;
  cpu->regs[2] = DRAM_BASE + DRAM_SIZE;
  cpu->pc = DRAM_BASE;
}

uint32_t cpu_fetch(CPU *cpu) {
  uint32_t inst = bus_load(&(cpu->bus), cpu->pc, 32);
  return inst;
}

uint64_t cpu_load(CPU *cpu, uint64_t addr, uint64_t size) {
  return bus_load(&(cpu->bus), addr, size);
}

int cpu_store(CPU *cpu, uint64_t addr, uint64_t size, uint64_t value) {
  return bus_store(&(cpu->bus), addr, size, value);
}

uint64_t rd(uint32_t inst) { return (inst >> 7) & 0x1f; }

uint64_t rs1(uint32_t inst) { return (inst >> 15) & 0x1f; }

uint64_t rs2(uint32_t inst) { return (inst >> 20) & 0x1f; }

// RISC-V Reference Manual
// https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf p.12

uint64_t imm_I(uint32_t inst) {
  return ((int64_t)(int32_t)(inst & 0xfff00000)) >> 20;
}
uint64_t imm_S(uint32_t inst) {
  return ((int64_t)(int32_t)(inst & 0xfe000000) >> 20 | (inst >> 7) & 0x1f);
}
uint64_t imm_B(uint32_t inst) {
  return ((int64_t)(int32_t)(inst & 0x80000000) >> 19) // imm[12]
         | ((inst & 0x80) << 4)                        // imm[11]
         | ((inst >> 20) & 0x7e0)                      // imm[10:5]
         | ((inst >> 7) & 0x1e);                       // imm[4:1]
}
uint64_t imm_U(uint32_t inst) { return (int64_t)(int32_t)(inst & 0xfffff000); }

uint64_t imm_J(uint32_t inst) {
  return ((int64_t)(int32_t)(inst & 0x80000000) >> 11) // imm[20]
         | ((inst & 0xff000))                          // imm[19:12]
         | ((inst >> 9) & 0x800)                       // imm[11]
         | ((inst >> 20) & 0x7fe);                     // imm[10:1]
}
uint32_t shamt(uint32_t inst) {
  return (uint32_t)(imm_I(inst) & 0x1f); // No idea?
}

uint32_t csr(uint32_t inst) {
  return ((inst & 0xfff00000) >> 20); // csr[11:0] = inst[31:20]
}

int cpu_execute(CPU *cpu, uint32_t inst) { return 0; }

// Instruction execution functions

void exec_LUI(CPU *cpu, uint32_t inst) {
  // Places the upper 20 bits of U_imm in th rd register.
  cpu->regs[(inst)] = (uint64_t)(int64_t)(int32_t)(inst & 0xfffff000);
  print_op("LUI\n");
}

void exec_AUIPC(CPU *cpu, uint32_t inst) {
  // AUIPC forms a 32 bit offset from the 20 upper bits of U_imm.
  cpu->regs[rd(inst)] = ((int64_t)cpu->pc + (int64_t)imm_U(inst)) - 4;
  print_op("AUIPC\n");
}

void exec_JAL(CPU *cpu, uint32_t inst) {
  // JAL jump and link p.16 from RISC-V reference manual.
  uint64_t imm_j = imm_J(inst);
  cpu->regs[rd(inst)] = cpu->pc;
  cpu->pc = cpu->pc + (int64_t)imm_j - 4;
  print_op("JAL\n");

  if (ADDR_MISALIGNED(cpu->pc)) {
    fprintf(stderr, "JAL pc address is misalligned");
    exit(0);
  }
}

void exec_BEQ(CPU *cpu, uint32_t inst) {
  uint64_t imm_b = imm_B(inst);
  if ((int64_t)cpu->regs[rs1(inst)] == (int64_t)cpu->regs[rs2(inst)]) {
    cpu->pc = cpu->pc + (int64_t)imm_b - 4;
  }
  print_op("BEQ\n");
}

void exec_BNE(CPU *cpu, uint32_t inst) {
  uint64_t imm_b = imm_B(inst);
  if ((int64_t)cpu->regs[rs1(inst)] != (int64_t)cpu->regs[rs2(inst)]) {
    cpu->pc = cpu->pc + (int64_t)imm_b - 4;
  }
  print_op("BEQ\n");
}

void exec_BLT(CPU *cpu, uint32_t inst) {
  uint64_t imm_b = imm_B(inst);
  if ((int64_t)cpu->regs[rs1(inst)] < (int64_t)cpu->regs[rs2(inst)]) {
    cpu->pc = cpu->pc + (int64_t)imm_b - 4;
  }
  print_op("BLT\n");
}

void exec_BGE(CPU *cpu, uint32_t inst) {
  uint64_t imm_b = imm_B(inst);
  if ((int64_t)cpu->regs[rs1(inst)] >= (int64_t)cpu->regs[rs2(inst)]) {
    cpu->pc = cpu->pc + (int64_t)imm_b - 4;
  }
  print_op("BGE\n");
}

void exec_BLTU(CPU *cpu, uint32_t inst) {
  uint64_t imm_b = imm_B(inst);
  if ((int64_t)cpu->regs[rs1(inst)] < cpu->regs[rs2(inst)]) {
    cpu->pc = cpu->pc + (int64_t)imm_b - 4;
  }
  print_op("BLTU\n");
}

void exec_BGEU(CPU *cpu, uint32_t inst) {
  uint64_t imm_b = imm_B(inst);
  if ((int64_t)cpu->regs[rs1(inst)] >= cpu->regs[rs2(inst)]) {
    cpu->pc = cpu->pc + (int64_t)imm_b - 4;
  }
  print_op("BGEU\n");
}
