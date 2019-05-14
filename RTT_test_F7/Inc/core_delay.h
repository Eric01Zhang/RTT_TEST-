#ifndef _CORE_DELAY_H
#define _CORE_DELAY_H

#include "stm32f7xx_hal_conf.h"

#define GET_CPU_ClkFreq()       HAL_RCC_GetSysClockFrq()
#define SysClockFreq            (210000000)

#define CPU_TS_INIT_IN_DELAY_FUNCTION  0

uint32_t CPU_TS_TmrRd(void);
HAL_StatusTypeDef HAL_initTick(uint32_t TickPriority);

uint32_t CPU_TS_TmrRd(void);

#endif