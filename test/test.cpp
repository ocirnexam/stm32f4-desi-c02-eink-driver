#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

void clockSetup() {
	rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
  
	rcc_periph_clock_enable(RCC_SPI1);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_reset_pulse(RST_SPI1);
}

int main(void)
{
  clockSetup();

  spi_init_master(SPI1, 
                  SPI_CR1_BAUDRATE_FPCLK_DIV_4, 
                  SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                  SPI_CR1_CPHA_CLK_TRANSITION_1, 
                  SPI_CR1_DFF_8BIT, 
                  SPI_CR1_LSBFIRST);

  // set gpia port 5 and 7 to af5 (spi)
  gpio_mode_setup(GPIOA, 
                  GPIO_MODE_AF,
                  GPIO_PUPD_NONE, 
                  GPIO5 | GPIO7 );
	gpio_set_af(GPIOA, GPIO_AF5, GPIO5 | GPIO7);

	/* Chip select line */
	gpio_set(GPIOB, GPIO6);
	gpio_mode_setup(GPIOB, 
                  GPIO_MODE_OUTPUT, 
                  GPIO_PUPD_NONE, 
                  GPIO6);

  spi_enable(SPI1);
  while (1) {
    gpio_clear(GPIOB, GPIO6); /* CS* select */
    spi_send(SPI1, 0x55);
    (void) spi_read(SPI1); // wait for transaction finished!
    gpio_set(GPIOB, GPIO6); /* CS* deselect */
    for(int i = 0; i < 1000000; i++) {
      asm("nop");
    }
  }
  return 0;
}