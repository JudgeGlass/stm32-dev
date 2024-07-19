#ifndef SPI_H
#define SPI_H

#include <hal/hal.h>

void spi_init(SPI_TypeDef *spi, uint32_t freq, uint8_t bit);

#endif