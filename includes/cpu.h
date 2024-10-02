#include <stdint.h>
#include "bus.h"

typedef struct CPU {
  uint64_t regs[32];
  uint64_t pc;
  struct BUS bus;
} CPU;

void cpu_init(CPU *cpu);
uint32_t cpu_fetch(CPU *cpu);
int cpu_execute(CPU *cpu, uint32_t inst);
void dump_registers(CPU *cpu);
