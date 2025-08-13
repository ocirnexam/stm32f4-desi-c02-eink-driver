#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

#include "eink/goodisplay_eink_driver.hpp"

void clockSetup() {
	rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
}

int main(void)
{
  clockSetup();
  GOODISPLAY_EINK_Driver driver = GOODISPLAY_EINK_Driver();
  driver.Init();

  while (1) {
    driver.ClearScreenWhite();
    for(int i = 0; i < 10000000; i++) {
      asm("nop");
    }
  }
  return 0;
}