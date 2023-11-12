#include <hal/hal.h>

void gpio_set_mode(uint16_t pin, uint8_t mode){
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    RCC->AHB1ENR |= BIT(PINBANK(pin));
    gpio->MODER &= ~(3U << (n * 2));
    gpio->MODER |= (mode & 3) << (n * 2);
}

void gpio_write(uint16_t pin, uint8_t value){
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (value ? 0 : 16);
}

void gpio_set_af(uint16_t pin, uint8_t af_num){
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    gpio->AFR[n >> 3] &= ~(15UL << ((n & 7) * 4));
    gpio->AFR[n >> 3] |= ((uint32_t) af_num) << ((n & 7) * 4);
}

void systick_init(uint32_t ticks){
    if((ticks - 1) > 0xFFFFFF) return;
    SYSTICK->LOAD = ticks - 1;
    SYSTICK->VAL = 0;
    SYSTICK->CTRL = BIT(0) | BIT(1) | BIT(2);
    RCC->APB2ENR |= BIT(14);
}

static volatile uint32_t s_ticks;
void systick_handler(void) {
    s_ticks++;
}

void sleep_ms(volatile uint32_t value_ms){
    while(s_ticks < (s_ticks + value_ms)) (void) 0;
}

void spin(volatile uint32_t count){
    while(count-- > 0) (void) 0;
}

uint32_t current_tick(){
    return s_ticks;
}

void uart_init(USART_TypeDef *uart, unsigned long baud){
    uint8_t af = 7;
    uint16_t rx = 0, tx = 0;

    if (uart == UART1) RCC->APB2ENR |= BIT(4);
    if (uart == UART2) RCC->APB1ENR |= BIT(17);
    if (uart == UART3) RCC->APB1ENR |= BIT(18);

    if (uart == UART1) tx = PIN('A', 9), rx = PIN('A', 10);
    if (uart == UART2) tx = PIN('A', 2), rx = PIN('A', 3);
    if (uart == UART3) tx = PIN('D', 8), rx = PIN('D', 9);

    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(tx, af);
    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_af(rx, af);

    uart->CR1 = 0;
    uart->BRR = FREQ / baud;
    uart->CR1 |= BIT(13) | BIT(2) | BIT(3);
}

void uart_write_byte(USART_TypeDef *uart, uint8_t byte){
    uart->DR = byte;
    while((uart->SR & BIT(7)) == 0) spin(1);
}

void uart_write_buffer(USART_TypeDef *uart, char *buff, size_t len){
    while(len-- > 0) uart_write_byte(uart, *(uint8_t *) buff++);
}

int uart_read_ready(USART_TypeDef *uart){
    return uart->SR & BIT(5);
}

uint8_t uart_read_byte(USART_TypeDef *uart){
    return (uint8_t) (uart->DR & 255);
}