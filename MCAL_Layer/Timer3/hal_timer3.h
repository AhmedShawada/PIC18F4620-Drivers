/* 
 * File:   hal_timer3.h
 * Author: ahmed-abdallah
 *
 * Created on October 1, 2024, 7:20 PM
 */

#ifndef HAL_TIMER3_H
#define	HAL_TIMER3_H

/*Section       :         Includes                */
#include "../GPIO/hal_gpio.h"
#include "../../MCAL_Layer/mcal_std_types.h"
#include "../../MCAL_Layer/Interrupt/mcal_internal_interrupt.h"
#include "pic18f4620.h"
/*Section       :         Data type Decleration                 */

typedef struct{
#if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void (* TMR3_InterruptHandler)(void);
    interrupt_priority_cfg priority;
#endif
    uint16 timer3_preload_value;        /* Pre-load value configuration */
    uint8 timer3_prescaler_value : 2;   /* Pre-scale value configuration */
    uint8 timer3_mode : 1;              /* Timer Mode / Counter Mode */
    uint8 timer3_counter_mode : 1;      /* Synchronous or ASynchronous Counter */
    uint8 timer3_reg_wr_mode : 1;       /* One 16-bit operation or Two 8-bit operations */
    uint8 timer3_reserved : 3;
}timer3_t;

/*Section       :         Macro Decleration                */

/* Timer3 Clock Source (Timer or Counter) */
#define TIMER3_COUNTER_MODE            1
#define TIMER3_TIMER_MODE              0
/* Timer3 External Clock Input Synchronization */
#define TIMER3_ASYNC_COUNTER_MODE      1
#define TIMER3_SYNC_COUNTER_MODE       0
/* Timer3 Input Clock Pre-scaler */
#define TIMER3_PRESCALER_DIV_BY_1      0
#define TIMER3_PRESCALER_DIV_BY_2      1
#define TIMER3_PRESCALER_DIV_BY_4      2
#define TIMER3_PRESCALER_DIV_BY_8      3
/* Timer3 16-Bit Read/Write Mode */
#define TIMER3_RW_REG_8Bit_MODE        0
#define TIMER3_RW_REG_16Bit_MODE       1

/*Section       :         Macro Function Decleration                */

/* Enable or Disable Timer3 Module */
#define TIMER3_MODULE_ENABLE()              (T3CONbits.TMR3ON = 1)
#define TIMER3_MODULE_DISABLE()             (T3CONbits.TMR3ON = 0)
/*  Timer3 Clock Source (Timer or Counter) */
#define TIMER3_TIMER_MODE_ENABLE()          (T3CONbits.TMR3CS = 0)
#define TIMER3_COUNTER_MODE_ENABLE()        (T3CONbits.TMR3CS = 1)
/*  Timer3 External Clock Input Synchronization */
#define TIMER3_ASYNC_COUNTER_MODE_ENABLE()  (T3CONbits.T3SYNC = 1)
#define TIMER3_SYNC_COUNTER_MODE_ENABLE()   (T3CONbits.T3SYNC = 0)
/* Timer3 Input Clock Pre-scaler */
#define TIMER3_PRESCALER_SELECT(_PRESCALER_) (T3CONbits.T3CKPS = _PRESCALER_)
/* Timer3 16-Bit Read/Write Mode */
#define TIMER3_RW_REG_8BIT_MODE_ENABLE()    (T3CONbits.RD16 = 0)
#define TIMER3_RW_REG_16BIT_MODE_ENABLE()   (T3CONbits.RD16 = 1)

/*Section       :         Function Decleration                */

std_ReturnType Timer3_Init(const timer3_t *_timer);
std_ReturnType Timer3_DeInit(const timer3_t *_timer);
std_ReturnType Timer3_Write_Value(const timer3_t *_timer, uint16 _value);
std_ReturnType Timer3_Read_Value(const timer3_t *_timer, uint16 *_value);



#endif	/* HAL_TIMER3_H */

