#include "../includes/cpu.h"
#include <stdint.h>

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

int cpu_execute(CPU *cpu, uint32_t inst) { return 0; }
