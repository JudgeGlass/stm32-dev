#include "hal/hal.h"

#include <string.h>

#include <kernel/kernel.h>

int main(void) {
    systick_init(16000000 / 1000);
    k_init();

    uint16_t led = PIN('B', 14);
    uint16_t led2 = PIN('B', 7);
    uint16_t led3 = PIN('B', 0);

    gpio_set_mode(led, GPIO_MODE_OUTPUT);
    gpio_set_mode(led2, GPIO_MODE_OUTPUT);
    gpio_set_mode(led3, GPIO_MODE_OUTPUT);

    uint32_t timer = 0, period = 500;
    for(;;){
        
        if(timer_expired(&timer, period, current_tick())){
            k_print_msg("Hello world!!!\r\n");
            static uint8_t on;
            gpio_write(led, on);
            gpio_write(led2, on);
            gpio_write(led3, on);
            on = !on;
        }
    }


    return 0; 
}

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
    // memset .bss to zero, and copy .data section to RAM region
    extern long _sbss, _ebss, _sdata, _edata, _sidata;
    for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    main();             // Call main()
    for (;;) (void) 0;  // Infinite loop in the case if main() returns
}

extern void _estack(void);  // Defined in link.ld

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, systick_handler
};