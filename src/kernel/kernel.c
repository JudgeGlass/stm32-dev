#include "kernel/kernel.h"

void k_init(){
    systick_init(16000000 / 1000);
    uart_init(UART3, 119200);
}

void k_main(){
    uint16_t led = PIN('B', 14);
    uint16_t led2 = PIN('B', 7);
    uint16_t led3 = PIN('B', 0);

    gpio_set_mode(led, GPIO_MODE_OUTPUT);
    gpio_set_mode(led2, GPIO_MODE_OUTPUT);
    gpio_set_mode(led3, GPIO_MODE_OUTPUT);

    uint32_t timer = 0, period = 500;
    uint32_t counter = 0;
    char msg[256];
    for(;;){
        if(timer_expired(&timer, period, current_tick())){
            sprintf(msg, "Counter %ld\r\n", counter++);
            k_print_msg(msg);
            static uint8_t on;
            gpio_write(led, on);
            gpio_write(led2, on);
            gpio_write(led3, on);
            on = !on;
        }
    }
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