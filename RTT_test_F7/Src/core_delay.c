#include "core_delay.h"

/* 参考野火的RT_Thread 的内核的实现和应用开发指南*/

#define DWT_CR          *(__IO uint32_t *)0xE0001000
#define DWT_CYCCNT      *(__IO uint32_t *)0xE0001004
#define DEM_CR          *(__IO uint32_t *)0xE000EDFC


#define DEM_CR_TRCENA       (1<<24)
#define DWT_CR_CYCCNTENA    (1<<0)

/**
 * @brief: 初始化时间戳
 * @pare:  none
 * @retval:none
 * @note  :  使用延时函数前，必须先调用本函数
 */
 
 HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
 {
     /* 使能DWT外设*/
     DEM_CR |= (uint32_t) DEM_CR_TRCENA;

     /* DWT CYCCNT 寄存器清零 */
     DWT_CYCCNT =  (uint32_t)0u;

     /* 使能cortem-M的 DWT CYCCNT 寄存器 */
     DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;

     return HAL_OK;
 }

/** 
 * @brief: 读取当前的时间戳
 * @input：none
 * @retval:none
 */
uint32_t HAL_GetTick(void)
{
    return ((uint32_t)DWT_CYCCNT*1000/SysClockFreq);
}

/**
 * @brief: 采用CPU的内部计数来实现精确的延时
 * @para ： 延时长度，单位1us
 * @retval: none
 * @note:  使用本函数前必须先调用CPU_TS_TmrInit()函数使能计数器
 *         或者使能宏CPU_TS_INIT_IN_DELAY_FUCION
 *         最大的时延为8S*
 */
void CPU_TS_Tmr_Delay_US(uint32_t us)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt = 0;

    /* 初始化时间戳寄存器*/
    #if (CPU_TS_INIT_IN_DELAY_FUNCTION)
    /* 初始化时间戳并清0*/
    HAL_InitTick(5);
    #endif

    tick = us * (GET_CPU_CLKFreq()/1000000);
    tcnt = 0;
    told = (uint32_t)CPU_TS_TmrRd();

    while(1){
        tnow = (uint32_t) CPU_TS_TmrRd();
        if(tnow != told)
        {
            if(tnow>told)
            {
                tcnt += tnow -told;
            }
            else
            {
                tcnt += UINT32_MAX -told + tnow;
            }
            
        }
        told = tnow;

        /* 超过需要延时的时间*/
        if(tcnt >= ticks)
        {
            break;
        }

    }
}