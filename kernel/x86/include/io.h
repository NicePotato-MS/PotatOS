#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static inline void outb(uint16_t port, uint8_t data) {
  __asm__ volatile("outb %b0, %w1" : : "a"(data), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
  uint8_t data;
  __asm__ volatile("inb %w1, %b0" : "=a"(data) : "Nd"(port));
  return data;
}

static inline void outw(uint16_t port, uint16_t data) {
  __asm__ volatile("outw %w0, %w1" : : "a"(data), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
  uint16_t data;
  __asm__ volatile("inw %w1, %w0" : "=a"(data) : "Nd"(port));
  return data;
}

static inline void outl(uint16_t port, uint32_t data) {
  __asm__ volatile("outl %0, %w1" : : "a"(data), "Nd"(port));
}

static inline uint32_t inl(uint16_t port) {
  uint32_t data;
  __asm__ volatile("inl %w1, %0" : "=a"(data) : "Nd"(port));
  return data;
}

static inline void io_Wait() { outb(0x80, 0); }

#ifdef __cplusplus
}
#endif