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

    volatile uint32_t src = 1;
    volatile uint32_t dest = 0;

    IOPin led = {
        .port = GPIOA, 
        .pin = PIN5, 
        .io_type = GPIO_GENERAL_OUTPUT, 
        .alt_function = GPIO_ALTF_SYSTEM
    };
    GPIO_Config(&led);

    DMA_Unit dma = {.unit = DMA2, .stream = DMA2_Stream0};
    (void) DMA_SetTransferDirection(&dma, DMA_M2M);
    (void) DMA_SetFlowController(&dma, DMA_FLOW_CONTROLLER_DMA);
    (void) DMA_SetTransferMode(&dma, DMA_FIFO_MODE);
    (void) DMA_SetPeripheralSize(&dma, DMA_WORD);
    (void) DMA_SetPeripheralAddress(&dma, (uint32_t) &src);
    (void) DMA_SetMemorySize(&dma, DMA_WORD);
    (void) DMA_SetMemoryAddress(&dma, (uint32_t) &dest);
    (void) DMA_SetNrOfItemsToTransfer(&dma, 1);

    if(DMA_Configure(&dma) != DMA_SUCCESS) {
        return -1;
    }

    DMA_StreamEnable(&dma);
    for (int i = 0; i < 100000000; i++)
    {
        GPIO_WritePin(&led, dest);
        msleep(500);
    }
    return 0;
}