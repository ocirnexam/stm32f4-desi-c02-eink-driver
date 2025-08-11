#include "goodisplay_eink_driver.hpp"

GOODISPLAY_EINK_Driver::GOODISPLAY_EINK_Driver(): spiInstance(SPI1), cs(IOPin(GPIOB, GPIO6)), mosi(IOPin(GPIOA, GPIO6)), miso(IOPin(GPIOA, GPIO7)), clock(IOPin(GPIOA, GPIO5)) { 
}

void GOODISPLAY_EINK_Driver::Init() {
    // enable clocks for spi instance, miso, mosi, clock and cs pins
	rcc_periph_clock_enable(RCC_SPI1);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_reset_pulse(RST_SPI1);

    // setup io pins
    gpio_mode_setup(mosi.port, 
                  GPIO_MODE_AF,
                  GPIO_PUPD_NONE, 
                  mosi.pin);
	gpio_set_af(mosi.port, GPIO_AF5, mosi.pin);
    gpio_mode_setup(miso.port, 
                  GPIO_MODE_AF,
                  GPIO_PUPD_NONE, 
                  miso.pin);
	gpio_set_af(miso.port, GPIO_AF5, miso.pin);
    gpio_mode_setup(clock.port, 
                  GPIO_MODE_AF,
                  GPIO_PUPD_NONE, 
                  clock.pin);
	gpio_set_af(clock.port, GPIO_AF5, clock.pin);
	gpio_mode_setup(cs.port, 
                  GPIO_MODE_OUTPUT, 
                  GPIO_PUPD_NONE, 
                  cs.pin);
	gpio_set(cs.port, cs.pin);

    // setup spi
    spi_init_master(spiInstance, 
                  SPI_CR1_BAUDRATE_FPCLK_DIV_4, 
                  SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                  SPI_CR1_CPHA_CLK_TRANSITION_1, 
                  SPI_CR1_DFF_8BIT, 
                  SPI_CR1_LSBFIRST);
    spi_enable(spiInstance);
}

void GOODISPLAY_EINK_Driver::SendCMD(uint8_t command) {
    gpio_clear(cs.port, cs.pin); /* CS* select */
    spi_send(spiInstance, command);
    (void) spi_read(spiInstance); // wait for transaction finished!
    gpio_set(cs.port, cs.pin); /* CS* deselect */
}