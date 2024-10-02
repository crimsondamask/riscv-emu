#include "../includes/bus.h"
#include <stdint.h>

uint64_t bus_load(BUS *bus, uint64_t addr, uint64_t size) {
  return dram_load(&(bus->dram), addr, size);
}

int bus_store(BUS *bus, uint64_t addr, uint64_t size, uint64_t value) {
  return dram_store(&(bus->dram), addr, size, value);
}
