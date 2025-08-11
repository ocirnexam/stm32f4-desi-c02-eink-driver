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

  uint8_t cmd = 0x00;
  while (1) {
    driver.SendCMD(cmd);
    cmd++;
    cmd %= 0x20;
    for(int i = 0; i < 1000000; i++) {
      asm("nop");
    }
  }
  return 0;
}