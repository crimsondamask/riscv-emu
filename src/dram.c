#include "../includes/dram.h"
#include <stdint.h>

uint64_t dram_load_8(DRAM *dram, uint64_t addr) {
  return (uint64_t)dram->mem[addr - DRAM_BASE];
}

uint64_t dram_load_16(DRAM *dram, uint64_t addr) {
  return (uint64_t)dram->mem[addr - DRAM_BASE] |
         (uint64_t)dram->mem[addr - DRAM_BASE + 1] << 8;
}

uint64_t dram_load_32(DRAM *dram, uint64_t addr) {
  return (uint64_t)dram->mem[addr - DRAM_BASE] |
         (uint64_t)dram->mem[addr - DRAM_BASE + 1] << 8 |
         (uint64_t)dram->mem[addr - DRAM_BASE + 2] << 16 |
         (uint64_t)dram->mem[addr - DRAM_BASE + 3] << 24;
}

uint64_t dram_load_64(DRAM *dram, uint64_t addr) {
  return (uint64_t)dram->mem[addr - DRAM_BASE] |
         (uint64_t)dram->mem[addr - DRAM_BASE + 1] << 8 |
         (uint64_t)dram->mem[addr - DRAM_BASE + 2] << 16 |
         (uint64_t)dram->mem[addr - DRAM_BASE + 3] << 24 |
         (uint64_t)dram->mem[addr - DRAM_BASE + 4] << 32 |
         (uint64_t)dram->mem[addr - DRAM_BASE + 5] << 40 |
         (uint64_t)dram->mem[addr - DRAM_BASE + 6] << 48 |
         (uint64_t)dram->mem[addr - DRAM_BASE + 7] << 56;
}

uint64_t dram_load(DRAM *dram, uint64_t addr, uint64_t size) {
  switch (size) {
  case 8:
    return dram_load_8(dram, addr);
    break;
  case 16:
    return dram_load_16(dram, addr);
    break;
  case 32:
    return dram_load_32(dram, addr);
    break;
  case 64:
    return dram_load_64(dram, addr);
    break;
  default:;
  }
  return 1;
}

int dram_store_8(DRAM *dram, uint64_t addr, uint64_t value) {
  if ((value < -UINT8_MAX) && (value > UINT8_MAX)) {
    return -1;
  } else {
    dram->mem[addr - DRAM_BASE] = (uint8_t)(value & 0xff);
    return 0;
  }
}

int dram_store_16(DRAM *dram, uint64_t addr, uint64_t value) {
  if ((value < -UINT16_MAX) && (value > UINT16_MAX)) {
    return -1;
  } else {
    dram->mem[addr - DRAM_BASE] = (uint8_t)(value & 0xff);
    dram->mem[addr - DRAM_BASE + 1] = (uint8_t)((value >> 8) & 0xff);
    return 0;
  }
}
int dram_store_32(DRAM *dram, uint64_t addr, uint64_t value) {
  if ((value < -UINT32_MAX) && (value > UINT32_MAX)) {
    return -1;
  } else {
    dram->mem[addr - DRAM_BASE] = (uint8_t)(value & 0xff);
    dram->mem[addr - DRAM_BASE + 1] = (uint8_t)((value >> 8) & 0xff);
    dram->mem[addr - DRAM_BASE + 2] = (uint8_t)((value >> 16) & 0xff);
    dram->mem[addr - DRAM_BASE + 3] = (uint8_t)((value >> 24) & 0xff);
    return 0;
  }
}
int dram_store_64(DRAM *dram, uint64_t addr, uint64_t value) {
  if ((value < -UINT64_MAX) && (value > UINT64_MAX)) {
    return -1;
  } else {
    dram->mem[addr - DRAM_BASE] = (uint8_t)(value & 0xff);
    dram->mem[addr - DRAM_BASE + 1] = (uint8_t)((value >> 8) & 0xff);
    dram->mem[addr - DRAM_BASE + 2] = (uint8_t)((value >> 16) & 0xff);
    dram->mem[addr - DRAM_BASE + 3] = (uint8_t)((value >> 24) & 0xff);
    dram->mem[addr - DRAM_BASE + 4] = (uint8_t)((value >> 32) & 0xff);
    dram->mem[addr - DRAM_BASE + 5] = (uint8_t)((value >> 40) & 0xff);
    dram->mem[addr - DRAM_BASE + 6] = (uint8_t)((value >> 48) & 0xff);
    dram->mem[addr - DRAM_BASE + 7] = (uint8_t)((value >> 56) & 0xff);
    return 0;
  }
}
int dram_store(DRAM *dram, uint64_t addr, uint64_t size, uint64_t value) {
  switch (size) {
  case 8:
    return dram_store_8(dram, addr, value);
    break;
  case 16:
    return dram_store_16(dram, addr, value);
    break;
  case 32:
    return dram_store_32(dram, addr, value);
    break;
  case 64:
    return dram_store_64(dram, addr, value);
    break;
  default:
    return -1;
  }
}
