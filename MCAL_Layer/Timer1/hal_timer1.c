/* 
 * File:   hal_timer1.c
 * Author: ahmed-abdallah
 *
 * Created on October 1, 2024, 7:19 PM
 */

#include "hal_timer1.h" 


#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static volatile void (*TMR1_InterruptHandler)(void) = NULL;
#endif
static volatile uint16 timer1_preload = ZERO_INIT;

static inline std_ReturnType Timer1_Mode_Select(const timer1_t *_timer);


/**
 * @Summary Initializes the Timer1
 * @Description This routine initializes the Timer1.
 *              It configures Timer1 based on the provided settings
 *              This routine must be called before any other Timer1 routine is called.
 * @Preconditions None
 * @param   _timer Pointer to the Timer1 configurations
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer1_Init(const timer1_t *_timer)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the Timer1 */
        TIMER1_MODULE_DISABLE();
        /*Configure the Prescaler*/
        TIMER1_PRESCALER_SELECT(_timer->timer1_prescaler_value);
        /*Configure the Mode Select*/   
        Timer1_Mode_Select(_timer);       
        /*Configure the preload value*/
        TMR0H = (_timer->timer1_preload_value) >> 8;
        TMR0L = (uint8)(_timer->timer1_preload_value);
        
        timer1_preload = _timer->timer1_preload_value;
        
        /* Configure the interrupt */
#if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
         TIMER1_InterruptEnable();
         TIMER1_InterruptFlagClear();
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
         INTERRUPT_PriorityLevelsEnable();
         if(INTERRUPT_HIGH_PRIORITY == _timer->priority)
         {
             INTERRUPT_GlobalInterruptHighEnable();
             TIMER1_HighPrioritySet();
         }
         else if(INTERRUPT_LOW_PRIORITY == _timer->priority)
         {
             INTERRUPT_GlobalInterruptLowEnable();
             TIMER1_LowPrioritySet();
         }
         else{ /* Nothing */ } 
#else
         INTERRUPT_GlobalInterruptEnable();
         INTERRUPT_PeripheralInterruptEnable();
#endif        
         TMR1_InterruptHandler = _timer->TMR1_InterruptHandler;
#endif
        
        /*Enable the TIMER1*/
        TIMER1_MODULE_ENABLE();
        ret = E_OK;
    }
    return ret;
}

/**
 * @Summary Deinitializes the Timer1
 * @Description This routine deinitializes the Timer1.
 *              It stops the timer and resets its configuration registers to their default state.
 *              This routine can be called to turn off the Timer1 when it is no longer needed.
 * @Preconditions Timer1 must be initialized.
 * @param   _timer Pointer to the Timer1 configurations
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer1_DeInit(const timer1_t *_timer)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the Timer0 */
        TIMER1_MODULE_DISABLE();
        
        /* Disable the interrupt */
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptDisable();
#endif
        ret = E_OK;
    }
    return ret; 
}

/**
 * @Summary Writes a value to the Timer1 register
 * @Description This routine writes a given value to the Timer1 register.
 *              The value is written based on the configuration of Timer1
 *              This routine must be called after the Timer1 is initialized.
 * @Preconditions Timer1 must be initialized.
 * @param   _timer Pointer to the Timer1 configurations
 * @param   _value Value to write into the Timer1 register 
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer1_Write_Value(const timer1_t *_timer, uint16 _value)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        TMR1H = (_value) >> 8;
        TMR1L = (uint8)(_value);
        ret = E_OK;
    }
    return ret;
    
}


/**
 * @Summary Reads the current value from the Timer1 register
 * @Description This routine reads the current value from the Timer1 register.
 *              The value is read based on the configuration of Timer1
 *              This routine must be called after the Timer1 is initialized.
 * @Preconditions Timer1 must be initialized.
 * @param   _timer Pointer to the Timer1 configurations
 * @param   _value Pointer to store the read value from Timer1
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer1_Read_Value(const timer1_t *_timer, uint16 *_value)
{
    std_ReturnType ret = E_NOT_OK;
    uint8 l_tmr1l = ZERO_INIT, l_tmr1h = ZERO_INIT;
    if((NULL == _timer) || (NULL == _value))
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        l_tmr1l = TMR1L;
        l_tmr1h = TMR1H;
        *_value = (uint16)((l_tmr1h << 8) + l_tmr1l);
        ret = E_OK;
    }
    return ret;
}



static inline std_ReturnType Timer1_Mode_Select(const timer1_t *_timer)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(TIMER1_TIMER_MODE == _timer->timer1_mode)
        {
            TIMER1_TIMER_MODE_ENABLE();
        }
        else if(TIMER1_COUNTER_MODE == _timer->timer1_mode)
        {
            TIMER1_COUNTER_MODE_ENABLE();
            
            if(TIMER1_ASYNC_COUNTER_MODE == _timer->timer1_counter_mode)
            {
                TIMER1_ASYNC_COUNTER_MODE_ENABLE();
            }
            
            else if(TIMER1_SYNC_COUNTER_MODE == _timer->timer1_counter_mode)
            {
                TIMER1_SYNC_COUNTER_MODE_ENABLE();
            }
        else{ /* Nothing */ }  
        }  
        
        ret = E_OK;
    }
    return ret; 
}


void TMR1_ISR(void)
{
    TIMER1_InterruptFlagClear();     /* Clear the interrupt flag */
    TMR1H = (timer1_preload) >> 8;   /* Initialize the pre-loaded value again */
    TMR1L = (uint8)(timer1_preload); /* Initialize the pre-loaded value again */
    
    if(TMR1_InterruptHandler)
    {
        TMR1_InterruptHandler();     /* Call the callback function */
    }
}
