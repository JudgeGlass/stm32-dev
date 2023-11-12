#ifndef __HAL_H__
#define __HAL_H__

#include <inttypes.h>
#include <stdlib.h>

#include "stm32f439xx.h"

#define FREQ 16000000

#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

#define GPIO(bank) ((GPIO_TypeDef *) (0x40020000 + 0x400 * (bank)))

#define SYSTICK ((SysTick_Type *) (0xE000E010)) 

#define UART1 ((USART_TypeDef *) 0x40011000)
#define UART2 ((USART_TypeDef *) 0x40004400)
#define UART3 ((USART_TypeDef *) 0x40004800)

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };

void gpio_set_mode(uint16_t pin, uint8_t mode);
void gpio_write(uint16_t pin, uint8_t value);
void gpio_set_af(uint16_t pin, uint8_t af_num);

void systick_init(uint32_t ticks);
void systick_handler(void);
void sleep_ms(volatile uint32_t value_ms);
void spin(volatile uint32_t count);

uint32_t current_tick();

void uart_init(USART_TypeDef *uart, unsigned long baud);
void uart_write_byte(USART_TypeDef *uart, uint8_t byte);
void uart_write_buffer(USART_TypeDef *uart, char *buff, size_t len);
int uart_read_ready(USART_TypeDef *uart);
uint8_t uart_read_byte(USART_TypeDef *uart);


#endif