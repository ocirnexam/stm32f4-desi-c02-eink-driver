#include "stm32f4xx.h"
#include "physical/gpio/gpio.h"
#include "physical/sysclock/systemclock_config.h"
#include "functional/helpers/helpers.h"

void SystemInit(void)
{
/* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif
}

int main(void)
{
    if (SysClockConfig() == COUNTDOWN_EXCEEDED_ERROR)
    {
        return COUNTDOWN_EXCEEDED_ERROR;
    }

    // blinky code
    IOPin led = {GPIOA, PIN5, GPIO_GENERAL_OUTPUT, GPIO_ALTF_SYSTEM};
    GPIO_Config(led);
    while (1)
    {
        GPIO_WritePin(led, PIN_HIGH);
        msleep(500);
        GPIO_WritePin(led, PIN_LOW);
        msleep(500);
    }
    return 0;
}