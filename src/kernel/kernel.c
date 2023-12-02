#include "kernel/kernel.h"

void k_init(){
    uart_init(UART3, 119200);
}

void* k_malloc(size_t m_size){
    // Kernel memory allocation

    return malloc(m_size); // Use this for now
}

void k_free(void* m_mem){
    // Kernel memory de-allocation

    free(m_mem); // Use this for now
}

void k_print_msg(char *m_msg){
    uart_write_buffer(UART3, m_msg, strlen(m_msg));
}

static uint32_t panic(){
    return 0;
}