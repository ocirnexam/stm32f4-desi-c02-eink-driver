#ifndef SYSTICK_HPP_INCLUDED
#define SYSTICK_HPP_INCLUDED

#include <cstdint>

void SysTick_Setup(uint16_t frequency);
uint64_t SysTick_GetCurrentTicks();
void delay(uint64_t ticks);

#endif