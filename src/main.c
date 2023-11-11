#include "hal/hal.h"


int main(void) {
    uint16_t led = PIN('B', 14);
    uint16_t led2 = PIN('B', 7);
    uint16_t led3 = PIN('B', 0);
    
    RCC->AHB1ENR |= BIT(PINBANK(led));
    RCC->AHB1ENR |= BIT(PINBANK(led2));
    RCC->AHB1ENR |= BIT(PINBANK(led3));

    gpio_set_mode(led, GPIO_MODE_OUTPUT);
    gpio_set_mode(led2, GPIO_MODE_OUTPUT);
    gpio_set_mode(led3, GPIO_MODE_OUTPUT);
    for(;;){
        gpio_write(led, 1);
        sleep_ms(100000);
        gpio_write(led, 0);
        sleep_ms(100000);

        gpio_write(led2, 1);
        sleep_ms(100000);
        gpio_write(led2, 0);
        sleep_ms(100000);

        gpio_write(led3, 1);
        sleep_ms(100000);
        gpio_write(led3, 0);
        sleep_ms(100000);
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
    _estack, _reset
};