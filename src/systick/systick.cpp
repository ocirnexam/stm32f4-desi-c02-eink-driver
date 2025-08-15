#include "systick.hpp"
#include <libopencm3/cm3/vector.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/cortex.h>

volatile uint64_t ticks = 0;

void SysTick_Setup(uint16_t frequency) {
    systick_set_frequency(frequency, rcc_ahb_frequency);
    systick_counter_enable();
    systick_interrupt_enable();
}

uint64_t SysTick_GetCurrentTicks() {
    return ticks;
}

void sys_tick_handler(void) {
    // 32 bit arch -> 2 instructions for 64 bit add -> disable interrupts
    cm_disable_interrupts();
    ticks += 1;
    cm_enable_interrupts();
}

void delay(uint64_t duration) {
    uint64_t start = SysTick_GetCurrentTicks();
    while (SysTick_GetCurrentTicks() - start < duration) {}
}