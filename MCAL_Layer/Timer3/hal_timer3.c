/* 
 * File:   hal_timer3.c
 * Author: ahmed-abdallah
 *
 * Created on October 1, 2024, 7:20 PM
 */

#include "hal_timer3.h"

#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static volatile void (*TMR3_InterruptHandler)(void) = NULL;
#endif
static volatile uint16 timer3_preload = ZERO_INIT;

static inline std_ReturnType Timer3_Mode_Select(const timer3_t *_timer);


/**
 * @Summary Initializes the Timer3
 * @Description This routine initializes the Timer3.
 *              It configures Timer3 based on the provided settings
 *              This routine must be called before any other Timer3 routine is called.
 * @Preconditions None
 * @param   _timer Pointer to the Timer3 configurations
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer3_Init(const timer3_t *_timer)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the Timer3 */
        TIMER3_MODULE_DISABLE();
        /*Configure the Pre-scaler*/
        TIMER3_PRESCALER_SELECT(_timer->timer3_prescaler_value);
        /*Configure the Mode Select*/   
        Timer3_Mode_Select(_timer);       
        /*Configure the pre-load value*/
        TMR3H = (_timer->timer3_preload_value) >> 8;
        TMR3L = (uint8)(_timer->timer3_preload_value);
        
        timer3_preload = _timer->timer3_preload_value;
        
        /* Configure the interrupt */
#if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
         TIMER3_InterruptEnable();
         TIMER3_InterruptFlagClear();
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
         INTERRUPT_PriorityLevelsEnable();
         if(INTERRUPT_HIGH_PRIORITY == _timer->priority)
         {
             INTERRUPT_GlobalInterruptHighEnable();
             TIMER3_HighPrioritySet();
         }
         else if(INTERRUPT_LOW_PRIORITY == _timer->priority)
         {
             INTERRUPT_GlobalInterruptLowEnable();
             TIMER3_LowPrioritySet();
         }
         else{ /* Nothing */ } 
#else
         INTERRUPT_GlobalInterruptEnable();
         INTERRUPT_PeripheralInterruptEnable();
#endif        
         TMR3_InterruptHandler = _timer->TMR3_InterruptHandler;
#endif
        
        /*Enable the TIMER3*/
        TIMER3_MODULE_ENABLE();
        ret = E_OK;
    }
    return ret;
}

/**
 * @Summary Deinitializes the Timer3
 * @Description This routine deinitializes the Timer3.
 *              It stops the timer and resets its configuration registers to their default state.
 *              This routine can be called to turn off the Timer3 when it is no longer needed.
 * @Preconditions Timer1 must be initialized.
 * @param   _timer Pointer to the Timer3 configurations
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer3_DeInit(const timer3_t *_timer)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the Timer0 */
        TIMER3_MODULE_DISABLE();
        
        /* Disable the interrupt */
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER3_InterruptDisable();
#endif
        ret = E_OK;
    }
    return ret; 
}

/**
 * @Summary Writes a value to the Timer3 register
 * @Description This routine writes a given value to the Timer3 register.
 *              The value is written based on the configuration of Timer3
 *              This routine must be called after the Timer1 is initialized.
 * @Preconditions Timer3 must be initialized.
 * @param   _timer Pointer to the Timer3 configurations
 * @param   _value Value to write into the Timer3 register 
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer3_Write_Value(const timer3_t *_timer, uint16 _value)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        TMR3H = (_value) >> 8;
        TMR3L = (uint8)(_value);
        ret = E_OK;
    }
    return ret;
    
}


/**
 * @Summary Reads the current value from the Timer3 register
 * @Description This routine reads the current value from the Timer3 register.
 *              The value is read based on the configuration of Timer3
 *              This routine must be called after the Timer3 is initialized.
 * @Preconditions Timer3 must be initialized.
 * @param   _timer Pointer to the Timer1 configurations
 * @param   _value Pointer to store the read value from Timer3
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer3_Read_Value(const timer3_t *_timer, uint16 *_value)
{
    std_ReturnType ret = E_NOT_OK;
    uint8 l_tmr3l = ZERO_INIT, l_tmr3h = ZERO_INIT;
    if((NULL == _timer) || (NULL == _value))
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        l_tmr3l = TMR3L;
        l_tmr3h = TMR3H;
        *_value = (uint16)((l_tmr3h << 8) + l_tmr3l);
        ret = E_OK;
    }
    return ret;
}



static inline std_ReturnType Timer3_Mode_Select(const timer3_t *_timer)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(TIMER3_TIMER_MODE == _timer->timer3_mode)
        {
            TIMER3_TIMER_MODE_ENABLE();
        }
        else if(TIMER3_COUNTER_MODE == _timer->timer3_mode)
        {
            TIMER3_COUNTER_MODE_ENABLE();
            
            if(TIMER3_ASYNC_COUNTER_MODE == _timer->timer3_counter_mode)
            {
                TIMER3_ASYNC_COUNTER_MODE_ENABLE();
            }
            
            else if(TIMER3_SYNC_COUNTER_MODE == _timer->timer3_counter_mode)
            {
                TIMER3_SYNC_COUNTER_MODE_ENABLE();
            }
        else{ /* Nothing */ }  
        }  
        
        ret = E_OK;
    }
    return ret; 
}


void TMR3_ISR(void)
{
    TIMER3_InterruptFlagClear();     /* Clear the interrupt flag */
    TMR3H = (timer3_preload) >> 8;   /* Initialize the pre-loaded value again */
    TMR3L = (uint8)(timer3_preload); /* Initialize the pre-loaded value again */
    
    if(TMR3_InterruptHandler)
    {
        TMR3_InterruptHandler();     /* Call the callback function */
    }
}