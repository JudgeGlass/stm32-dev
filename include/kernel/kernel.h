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

#endif