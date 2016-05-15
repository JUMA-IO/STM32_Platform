/**
  */
#include "bsp_common.h"
#include "clock.h"

const uint32_t CLOCK_SECOND = 1000;
void Clock_Init(void)
{
    // FIXME: as long as Cube HAL is initialized this is OK
    // Cube HAL default is one clock each 1 ms
}

tClockTime Clock_Time(void)
{
    return HAL_GetTick();
}

void Clock_Wait(uint32_t i)
{
    HAL_Delay(i);
}
