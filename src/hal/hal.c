#include <hal/hal.h>

void gpio_set_mode(uint16_t pin, uint8_t mode){
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    gpio->MODER &= ~(3U << (n * 2));
    gpio->MODER |= (mode & 3) << (n * 2);
}

void gpio_write(uint16_t pin, uint8_t value){
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (value ? 0 : 16);
}

void sleep_ms(volatile uint32_t value_ms){
    while(value_ms--) (void) 0;
}