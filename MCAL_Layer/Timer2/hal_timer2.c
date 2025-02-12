/* 
 * File:   hal_timer2.c
 * Author: ahmed-abdallah
 *
 * Created on October 1, 2024, 7:19 PM
 */

#include "hal_timer2.h"

#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static void (*TMR2_InterruptHandler)(void) = NULL;
#endif
    
static uint8 timer2_preload = ZERO_INIT;

/**
 * @Summary Initializes the Timer2
 * @Description This routine initializes the Timer2.
 *              It configures Timer2 based on the provided settings
 *              This routine must be called before any other Timer2 routine is called.
 * @Preconditions None
 * @param   _timer Pointer to the Timer2 configurations
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer2_Init(const timer2_t *_timer)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the Timer2 */
        TIMER2_MODULE_DISABLE();
        /*Configure the Pre-scaler*/
        TIMER2_PRESCALER_SELECT(_timer->timer2_prescaler_value);
        /*Configure the Post-scaler*/
        TIMER2_POSTSCALER_SELECT(_timer->timer2_postscaler_value);
        /*Configure the pre-load value*/
        TMR2 = _timer->timer2_preload_value;
        
        timer2_preload = _timer->timer2_preload_value;
        
        /* Configure the interrupt */
#if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
         TIMER2_InterruptEnable();
         TIMER2_InterruptFlagClear();
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
         INTERRUPT_PriorityLevelsEnable();
         if(INTERRUPT_HIGH_PRIORITY == _timer->priority)
         {
             INTERRUPT_GlobalInterruptHighEnable();
             TIMER2_HighPrioritySet();
         }
         else if(INTERRUPT_LOW_PRIORITY == _timer->priority)
         {
             INTERRUPT_GlobalInterruptLowEnable();
             TIMER2_LowPrioritySet();
         }
         else{ /* Nothing */ } 
#else
         INTERRUPT_GlobalInterruptEnable();
         INTERRUPT_PeripheralInterruptEnable();
#endif        
         TMR2_InterruptHandler = _timer->TMR2_InterruptHandler;
#endif
    
        /*Enable the TIMER2*/
        TIMER2_MODULE_ENABLE();
        
        ret = E_OK;
    }
    return ret;
}

/**
 * @Summary Deinitializes the Timer2
 * @Description This routine deinitializes the Timer2.
 *              It stops the timer and resets its configuration registers to their default state.
 *              This routine can be called to turn off the Timer1 when it is no longer needed.
 * @Preconditions Timer2 must be initialized.
 * @param   _timer Pointer to the Timer2 configurations
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer2_DeInit(const timer2_t *_timer)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the Timer2 */
        TIMER2_MODULE_DISABLE();
        /* Disable the interrupt */
#if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptDisable();
#endif
        ret = E_OK;
    }
    return ret;
}

/**
 * @Summary Writes a value to the Timer2 register
 * @Description This routine writes a given value to the Timer2 register.
 *              The value is written based on the configuration of Timer2
 *              This routine must be called after the Timer2 is initialized.
 * @Preconditions Timer2 must be initialized.
 * @param   _timer Pointer to the Timer2 configurations
 * @param   _value Value to write into the Timer2 register 
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer2_Write_Value(const timer2_t *_timer, uint8 _value)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        TMR2 = _value;
        ret = E_OK;
    }
    return ret;
    
}

/**
 * @Summary Reads the current value from the Timer2 register
 * @Description This routine reads the current value from the Timer2 register.
 *              The value is read based on the configuration of Timer2
 *              This routine must be called after the Timer2 is initialized.
 * @Preconditions Timer2 must be initialized.
 * @param   _timer Pointer to the Timer2 configurations
 * @param   _value Pointer to store the read value from Timer2
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer2_Read_Value(const timer2_t *_timer, uint8 *_value)
{
    std_ReturnType ret = E_NOT_OK;
    if((NULL == _timer) || (NULL == _value))
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        *_value = TMR2;
        ret = E_OK;
    }
    return ret;
    
}

void TMR2_ISR(void)
{
    TIMER2_InterruptFlagClear();         /* Clear the interrupt flag */
    TMR2 = timer2_preload;               /* Initialize the pre-loaded value again */
    
    if(TMR2_InterruptHandler)
    {
        TMR2_InterruptHandler();         /* Call the callback function */
    }
}


