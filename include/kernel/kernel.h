#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include <hal/hal.h>

typedef struct {
    void* m_alloc;
    uint32_t pid;
} process_t;

void k_init();

void* k_malloc(size_t m_size);

void k_free(void* m_mem);

void k_print_msg(char *m_msg);

static uint32_t panic();

static inline uint8_t timer_expired(uint32_t *t, uint32_t prd, uint32_t now) {
  if (now + prd < *t) *t = 0;
  if (*t == 0) *t = now + prd;
  if (*t > now) return 0;
  *t = (now - *t) > prd ? now + prd : *t + prd;
  return 1;
} 

#endif