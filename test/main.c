#include "stm32f4xx.h"
#include "physical/gpio/gpio.h"
#include "physical/sysclock/systemclock_config.h"
#include "physical/dma/dma.h"
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

    DMA_Unit dma_c = {
        .channel = DMA_CHANNEL_1, 
        .dma_unit = DMA_UNIT_1, 
        .transfer_direction = DMA_M2P
    };
    if(DMA_Configure(&dma_c) != DMA_CONFIG_SUCCESS) {
        return -1;
    }

    // blinky code
    IOPin led = {
        .port = GPIOA, 
        .pin = PIN5, 
        .io_type = GPIO_GENERAL_OUTPUT, 
        .alt_function = GPIO_ALTF_SYSTEM
    };
    GPIO_Config(&led);
    while (1)
    {
        GPIO_WritePin(&led, PIN_HIGH);
        msleep(500);
        GPIO_WritePin(&led, PIN_LOW);
        msleep(500);
    }
    return 0;
}