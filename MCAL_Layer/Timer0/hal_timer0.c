/* 
 * File:   hal_timer0.c
 * Author: ahmed-abdallah
 *
 * Created on September 30, 2024, 7:13 PM
 */

#include "hal_timer0.h"

static uint16 timer0_preload = ZERO_INIT;

#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static void (*TMR0_InterruptHandler)(void) = NULL;
#endif

static inline std_ReturnType Timer0_Prescaler_Config(const timer0_t *_timer);
static inline std_ReturnType Timer0_Mode_Select(const timer0_t *_timer);
static inline std_ReturnType Timer0_Register_Size_Config(const timer0_t *_timer);
    

/**
 * @Summary Initializes the Timer0
 * @Description This routine initializes the Timer0.
 *              It configures Timer0 based on the provided settings
 *              This routine must be called before any other Timer0 routine is called.
 * @Preconditions None
 * @param   _timer Pointer to the Timer0 configurations
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer0_Init(const timer0_t *_timer)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the Timer0 */
        TIMER0_MODULE_DISABLE();
        /*Configure the Prescaler*/
        Timer0_Prescaler_Config(_timer);
        /*Configure the Mode Select*/   
        Timer0_Mode_Select(_timer);       
        /*Configure the Register Size*/
        Timer0_Register_Size_Config(_timer);
        /*Configure the preload value*/
        TMR0H = (_timer->timer0_preload_value) >> 8;
        TMR0L = (uint8)(_timer->timer0_preload_value);
        
        timer0_preload = _timer->timer0_preload_value;
        
        /* Configure the interrupt */
#if TIMER0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
         TIMER0_InterruptEnable();
         TIMER0_InterruptFlagClear();
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
         INTERRUPT_PriorityLevelsEnable();
         if(INTERRUPT_HIGH_PRIORITY == _timer->priority)
         {
             INTERRUPT_GlobalInterruptHighEnable();
             TIMER0_HighPrioritySet();
         }
         else if(INTERRUPT_LOW_PRIORITY == _timer->priority)
         {
             INTERRUPT_GlobalInterruptLowEnable();
             TIMER0_LowPrioritySet();
         }
         else{ /* Nothing */ } 
#else
         INTERRUPT_GlobalInterruptEnable();
         INTERRUPT_PeripheralInterruptEnable();
#endif        
         TMR0_InterruptHandler = _timer->TMR0_InterruptHandler;
#endif
        
        /*Enable the TIMER0*/
        TIMER0_MODULE_ENABLE();
                
        ret = E_OK; 
    }    
      
    return ret;
}


/**
 * @Summary Deinitializes the Timer0
 * @Description This routine deinitializes the Timer0.
 *              It stops the timer and resets its configuration registers to their default state.
 *              This routine can be called to turn off the Timer0 when it is no longer needed.
 * @Preconditions Timer0 must be initialized.
 * @param   _timer Pointer to the Timer0 configurations
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer0_DeInit(const timer0_t *_timer)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the Timer0 */
        TIMER0_MODULE_DISABLE();
        
        /* Disable the interrupt */
#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER0_InterruptDisable();
#endif
        ret = E_OK;
    }        
    return ret;
}

/**
 * @Summary Writes a value to the Timer0 register
 * @Description This routine writes a given value to the Timer0 register.
 *              The value is written based on the configuration of Timer0
 *              This routine must be called after the Timer0 is initialized.
 * @Preconditions Timer0 must be initialized.
 * @param   _timer Pointer to the Timer0 configurations
 * @param   _value Value to write into the Timer0 register 
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer0_Write_Value(const timer0_t *_timer, uint16 _value)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        TMR0H = (_value) >> 8;
        TMR0L = (uint8)(_value);
    }     
    return ret;
}

/**
 * @Summary Reads the current value from the Timer0 register
 * @Description This routine reads the current value from the Timer0 register.
 *              The value is read based on the configuration of Timer0
 *              This routine must be called after the Timer0 is initialized.
 * @Preconditions Timer0 must be initialized.
 * @param   _timer Pointer to the Timer0 configurations
 * @param   _value Pointer to store the read value from Timer0
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Timer0_Read_Value(const timer0_t *_timer, uint16 *_value)
{
    std_ReturnType ret = E_NOT_OK;
    uint8 l_tmr0l = ZERO_INIT, l_tmr0h = ZERO_INIT;
    if((NULL == _timer) || (NULL == _value))
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        l_tmr0l = TMR0L;
        l_tmr0h = TMR0H;
        *_value = (uint16)((l_tmr0h << 8) + l_tmr0l);
    }    
      
    return ret;
}






/**
 * @Summary Configures the Timer0 prescaler
 * @Description This inline function configures the Timer0 prescaler based on the configuration provided.
 * @Preconditions Timer0 must be initialized.
 * @param   _timer Pointer to the Timer0 configurations, which includes prescaler enable/disable and prescaler value.
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : There was an error (e.g., null pointer)
 */
static inline std_ReturnType Timer0_Prescaler_Config(const timer0_t *_timer)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(TIMER0_PRESCALER_ENABLE_CFG == _timer->prescaler_enable)
        {
            TIMER0_PRESCALER_ENABLE();
            T0CONbits.T0PS = _timer->prescaler_value;
        }
        
        else if(TIMER0_PRESCALER_DISABLE_CFG == _timer->prescaler_enable)
        {
            TIMER0_PRESCALER_DISABLE();
        }
        
         else{ /* Nothing */ }
        
        ret = E_OK;
    }
    return ret; 
}


/**
 * @Summary Configures the Timer0 mode (Timer or Counter)
 * @Description This inline function sets the Timer0 mode based on the provided settings.
 *              It supports both Timer mode and Counter mode (with rising or falling edge).
 * @Preconditions Timer0 must be initialized.
 * @param   _timer Pointer to the Timer0 configuration structure.
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : There was an error (e.g., null pointer)
 */
static inline std_ReturnType Timer0_Mode_Select(const timer0_t *_timer)
{
    
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(TIMER0_TIMER_MODE == _timer->timer0_mode)
        { 
            TIMER0_TIMER_MODE_ENABLE(); 
        }
        
        else if(TIMER0_COUNTER_MODE == _timer->timer0_mode)
        {
            TIMER0_COUNTER_MODE_ENABLE();
            if(TIMER0_COUNTER_RISING_EDGE_CFG == _timer->timer0_counter_edge)
            {
                TIMER0_RISING_EDGE_ENABLE();
            }
            else if (TIMER0_COUNTER_FALLING_EDGE_CFG == _timer->timer0_counter_edge)
            {
                TIMER0_FALLING_EDGE_ENABLE();
            }
            
        else{ /* Nothing */ }
            
        }
        
    else{ /* Nothing */ }
        
        ret = E_OK;
    }
    return ret; 
}

/**
 * @Summary Configures the Timer0 register size (8-bit or 16-bit)
 * @Description This inline function sets the register size of Timer0 based on the configuration provided.
 *              It supports selecting between 8-bit and 16-bit register modes.
 * @Preconditions Timer0 must be initialized.
 * @param   _timer Pointer to the Timer0 configuration structure.
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : There was an error (e.g., null pointer)
 */
static inline std_ReturnType Timer0_Register_Size_Config(const timer0_t *_timer)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(TIMER0_8BIT_REGISTER_MODE == _timer->timer0_register_size)
        {
            TIMER0_8BIT_REGISTER_MODE_ENABLE();
        }
        else if(TIMER0_16BIT_REGISTER_MODE == _timer->timer0_register_size)
        {
            TIMER0_16BIT_REGISTER_MODE_ENABLE();
        }
        else{ /* Nothing */ }
        
        ret = E_OK;
    }
    
    
    return ret;
}


void TMR0_ISR(void)
{
    TIMER0_InterruptFlagClear();
    TMR0H = (timer0_preload) >> 8;
    TMR0L = (uint8)(timer0_preload);
    if(TMR0_InterruptHandler)
    {
        TMR0_InterruptHandler();
    }
}
