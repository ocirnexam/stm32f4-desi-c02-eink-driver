#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

#include "eink/goodisplay_eink_driver.hpp"
#include "systick/systick.hpp"

void ClockSetup() {
	rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
}

int main(void)
{
  ClockSetup();
  SysTick_Setup(1000);

  GOODISPLAY_EINK_Driver driver = GOODISPLAY_EINK_Driver();
  driver.Init();

  while (1) {
    driver.ClearScreenWhite();
    delay(5000);
  }
  return 0;
}