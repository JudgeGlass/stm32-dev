#include "hal/hal.h"

#include <string.h>

uint8_t timer_expired(uint32_t *t, uint32_t prd, uint32_t now) {
  if (now + prd < *t) *t = 0;                    // Time wrapped? Reset timer
  if (*t == 0) *t = now + prd;                   // Firt poll? Set expiration
  if (*t > now) return 0;                    // Not expired yet, return
  *t = (now - *t) > prd ? now + prd : *t + prd;  // Next expiration time
  return 1;                                   // Expired, return true
} 


int main(void) {
    uint16_t led = PIN('B', 14);
    uint16_t led2 = PIN('B', 7);
    uint16_t led3 = PIN('B', 0);

    gpio_set_mode(led, GPIO_MODE_OUTPUT);
    gpio_set_mode(led2, GPIO_MODE_OUTPUT);
    gpio_set_mode(led3, GPIO_MODE_OUTPUT);

    systick_init(16000000 / 1000);

    uart_init(UART3, 115200);
    char *msg = "Hello World!\r\n";
    uint32_t timer = 0, period = 500;
    for(;;){
        
        if(timer_expired(&timer, period, current_tick())){
            uart_write_buffer(UART3, msg, strlen(msg));
            static uint8_t on;
            gpio_write(led, on);
            gpio_write(led2, on);
            gpio_write(led3, on);
            on = !on;
        }

        // gpio_write(led, 1);
        // sleep_ms(20);
        // gpio_write(led, 0);
        // sleep_ms(20);
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