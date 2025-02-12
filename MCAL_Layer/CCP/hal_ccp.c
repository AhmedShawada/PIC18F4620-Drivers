/* 
 * File:   hal_ccp.c
 * Author: ahmed-abdallah
 *
 * Created on October 11, 2024, 7:04 PM
 */

#include "hal_ccp.h"

#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static void (*CCP1_InterruptHandler)(void) = NULL;
#endif
    
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static void (*CCP2_InterruptHandler)(void) = NULL;
#endif
    
    
static void CCP_Mode_Timer_Select(const ccp_t *_ccp_obj);   
static void CCP_Interrupt_Config(const ccp_t *_ccp_obj);
static std_ReturnType CCP_Capture_Mode_Config(const ccp_t *_ccp_obj);
static std_ReturnType CCP_Compare_Mode_Config(const ccp_t *_ccp_obj);
static void CCP_PWM_Mode_Config(const ccp_t *_ccp_obj);

    
/**
 * @Summary Initializes the CCP
 * @Description This routine initializes the CCP.
 *              It configures CCP based on the provided settings
 *              This routine must be called before any other CCP routine is called.
 * 
 * @Preconditions The configuration object (_ccp_obj) must be initialized before calling this function.
 * @param   _ccp_obj Pointer to the CCP configurations
 * @return  Status of the function
 *          (E_OK) : The function completed successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */   
std_ReturnType CCP_Init(const ccp_t *_ccp_obj)
{
    std_ReturnType ret = E_NOT_OK;
    
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* CCP Module Disable */
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            CCP2_SET_MODE(CCP_MODULE_DISABLE);
        }       
        else{ /* Nothing */ }
        
        
        /* CCP Module Capture Mode Initialization */
        if(CCP_CAPTURE_MODE_SELECTED == _ccp_obj->ccp_mode)
        {
           ret = CCP_Capture_Mode_Config(_ccp_obj);
        }
        
        /* CCP Module Compare Mode Initialization */
        else if(CCP_COMPARE_MODE_SELECTED == _ccp_obj->ccp_mode)
        {
            ret = CCP_Compare_Mode_Config(_ccp_obj);
        }
        
#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)        
        /* CCP Module PWM Mode Initialization */
        else if(CCP_PWM_MODE_SELECTED == _ccp_obj->ccp_mode)
        {
            CCP_PWM_Mode_Config(_ccp_obj);
        }
#endif
        else {/* Nothing */}
        
        /* PIN Configurations */
        ret = gpio_pin_intialize(&(_ccp_obj->ccp_pin));
        
        /* Interrupt Configurations for CCP1 and CCP2 Modules */
        CCP_Interrupt_Config(_ccp_obj);
        
        ret = E_OK;
    }
    
    return ret;
}

/**
 * @Summary Deinitializes the CCP module.
 * 
 * @Description 
 *    This function deinitializes the Capture/Compare/PWM (CCP) module by performing necessary cleanup operations.
 *    It checks whether the pointer to the configuration object is valid. If it is, the deinitialization proceeds;
 *    otherwise, it returns an error status.
 *             
 * @Preconditions  The CCP module must be initialized before calling this function.
 * @param _ccp_obj 
 *    Pointer to a configuration structure of type ccp_t representing the CCP instance to be deinitialized.
 * @return  
 *    - E_OK: If the CCP module is deinitialized successfully.
 *    - E_NOT_OK: If the input pointer (_ccp_obj) is NULL or deinitialization fails.
 */
std_ReturnType CCP_DeInit(const ccp_t *_ccp_obj)
{
    std_ReturnType ret = E_NOT_OK;
    
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            CCP1_SET_MODE(CCP_MODULE_DISABLE); /* CCP1 Module Disable */
            /* Interrupt Configurations */ 
#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
            CCP1_InterruptDisable();
#endif 
        }
        
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            CCP2_SET_MODE(CCP_MODULE_DISABLE); /* CCP2 Module Disable */
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
            CCP2_InterruptDisable();
#endif  
        }
        
        else { /* Nothing */ }
        ret = E_OK;
    }
    return ret;
}

#if (CCP1_CFG_SELECTED_MODE == CCP_CFG_CAPTURE_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE == CCP_CFG_CAPTURE_MODE_SELECTED)

/**
 * @Summary        Checks if CCP1 has captured data ready.
 * @Description    This function checks if the CCP1 module has valid captured data ready by writing the capture status to the provided variable.
 *                
 * @Preconditions  The CCP1 module must be configured and initialized in capture mode.
 * 
 * @param _capture_status 
 *    Pointer to a uint8 variable where the function will store the capture status.
 *    - 1: Data is ready.
 *    - 0: Data is not ready.
 * 
 * @return  
 *    - E_OK: If the status was successfully retrieved.
 *    - E_NOT_OK: If the input pointer (_capture_status) is NULL.
 */
std_ReturnType CCP1_IsCapturedDataReady(uint8 *_capture_status)
{
    std_ReturnType ret = E_NOT_OK;
    
    if(NULL == _capture_status)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(CCP1_CAPTURE_READY  == PIR1bits.CCP1IF)
        {
            *_capture_status = CCP1_CAPTURE_READY;
            CCP1_InterruptFlagClear();
        }
        else
        {
            *_capture_status = CCP1_CAPTURE_NOT_READY;
        }
        
        ret = E_OK;
    }
    
    return ret;
    
}

/**
 * @Summary        Reads the captured value from CCP1 in capture mode.
 * 
 * @Description    This function retrieves the captured value from the CCP1 module when it is operating in capture mode.
 *               
 * @Preconditions  The CCP1 module must be configured and initialized in capture mode, and the capture event should have occurred.
 *    
 * @param capture_value 
 *    Pointer to a uint16 variable where the function will store the captured value.
 * 
 * @return  
 *    - E_OK: If the capture value was successfully read.
 *    - E_NOT_OK: If the input pointer (capture_value) is NULL or if no valid data is available.
 */
std_ReturnType CCP1_Capture_Mode_Read_Value(const ccp_t *_ccp_obj ,uint16 *capture_value)
{
    std_ReturnType ret = E_NOT_OK;
    CCP_REG_T Capture_Temp_Value = {.ccpr_low = ZERO_INIT, .ccpr_high = ZERO_INIT};
    if((NULL == capture_value) ||(NULL == _ccp_obj))
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            /* Copy captured value */
            Capture_Temp_Value.ccpr_low  = CCPR1L;
            Capture_Temp_Value.ccpr_high = CCPR1H;
            /* Write the 16-bit capture value */
            *capture_value = Capture_Temp_Value.ccpr_16bit;
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            /* Copy captured value */
            Capture_Temp_Value.ccpr_low  = CCPR2L;
            Capture_Temp_Value.ccpr_high = CCPR2H;
            /* Write the 16-bit capture value */
            *capture_value = Capture_Temp_Value.ccpr_16bit;
        }
        else{ /* Nothing */ }
        
        
        ret = E_OK;
    }
    
    return ret;
}

#endif


#if (CCP1_CFG_SELECTED_MODE == CCP_CFG_COMPARE_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE == CCP_CFG_COMPARE_MODE_SELECTED)

/**
 * @Summary        Checks if the CCP compare operation has completed. 
 * 
 * @Description    This function checks whether the CCP module has completed the compare operation and stores the status in the provided variable.    
 *              
 * @Preconditions  The CCP module must be configured and initialized in compare mode.
 *    
 * @param _compare_status 
 *    Pointer to a uint8 variable where the function will store the compare status.
 *    - 1: Compare complete.
 *    - 0: Compare not complete.
 * 
 * @return  
 *    - E_OK: If the compare status was successfully retrieved.
 *    - E_NOT_OK: If the input pointer (_compare_status) is NULL.
 */
std_ReturnType CCP_IsCompareComplete(uint8 *_compare_status)
{
    std_ReturnType ret = E_NOT_OK;
    
    if(NULL == _compare_status)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(CCP1_COMPARE_READY  == PIR1bits.CCP1IF)
        {
            *_compare_status = CCP1_COMPARE_READY;
            CCP1_InterruptFlagClear();
        }
        else
        {
            *_compare_status = CCP1_COMPARE_NOT_READY;
        }
        
        ret = E_OK;
    }
    
    return ret;
    
}

/**
 * @Summary        Sets the compare value for the CCP module in compare mode. 
 *  
 * @Description    This function sets the compare value for the CCP module when it is operating in compare mode.
 *     
 * @Preconditions  The CCP module must be configured and initialized in compare mode.
 *   
 * @param _ccp_obj 
 *    Pointer to a configuration structure of type ccp_t representing the CCP instance.
 * 
 * @param compare_value 
 *    The 16-bit value to be set as the compare value for the CCP module.
 * 
 * @return  
 *    - E_OK: If the compare value was successfully set.
 *    - E_NOT_OK: If the input pointer (_ccp_obj) is NULL.
 */
std_ReturnType CCP_Compare_Mode_Set_Value(const ccp_t *_ccp_obj, uint16 compare_value)
{
    std_ReturnType ret = E_NOT_OK;
    CCP_REG_T Compare_Temp_Value = {.ccpr_low = ZERO_INIT, .ccpr_high = ZERO_INIT};
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Copy compared value */
        Compare_Temp_Value.ccpr_16bit = compare_value;
        
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            /* Copy captured value */
            CCPR1L = Compare_Temp_Value.ccpr_low;
            CCPR1H = Compare_Temp_Value.ccpr_high;
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            /* Copy captured value */
            CCPR1L = Compare_Temp_Value.ccpr_low;
            CCPR2H = Compare_Temp_Value.ccpr_high;
            /* Write the 16-bit capture value */
        }
        else{ /* Nothing */ }
        
        ret = E_OK;
    }
    
    return ret;
    
}
#endif



#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)

/**
 * @Summary       Sets the PWM duty cycle for the CCP module.
 *    
 * @Description 
 *    This function configures the duty cycle for the CCP module in PWM mode. The duty cycle value is provided as an 8-bit value,
 *    which corresponds to the percentage of time the signal remains high during one PWM period.
 *              
 * @Preconditions  The CCP module must be configured and initialized in PWM mode.
 *   
 * @param _ccp_obj 
 *    Pointer to a configuration structure of type ccp_t representing the CCP instance.
 * 
 * @param _duty 
 *    8-bit value representing the PWM duty cycle (0-255), where 0 means 0% duty cycle (always low), 
 *    and 255 means 100% duty cycle (always high).
 * 
 * @return  
 *    - E_OK: If the duty cycle was successfully set.
 *    - E_NOT_OK: If the input pointer (_ccp_obj) is NULL.
 */
std_ReturnType CCP_PWM_Set_Duty(const ccp_t *_ccp_obj, const uint8 _duty)
{
    std_ReturnType ret = E_NOT_OK;
    uint16 l_Duty_Temp = ZERO_INIT;
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        l_Duty_Temp = (uint16)(4 * (PR2 + 1) * (_duty / 100.0));
        
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            CCP1CONbits.DC1B = (uint8)(l_Duty_Temp &0x0003);
            CCPR1L = (uint8)(l_Duty_Temp >> 2);
        }
        
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            CCP2CONbits.DC2B = (uint8)(l_Duty_Temp &0x0003);
            CCPR2L = (uint8)(l_Duty_Temp >> 2);
        }
        
        else{ /* Nothing */ }
                
        ret = E_OK;
    }
    
    return ret;
}

/**
 * @Summary        Starts PWM operation in the CCP module.
 *    
 * @Description    This function starts PWM signal generation by enabling the necessary hardware components of the CCP module.
 *                  
 * @Preconditions  The CCP module must be configured in PWM mode and the duty cycle must be set before starting.
 *    
 * @param _ccp_obj 
 *    Pointer to a configuration structure of type ccp_t representing the CCP instance.
 * 
 * @return  
 *    - E_OK: If the PWM was successfully started.
 *    - E_NOT_OK: If the input pointer (_ccp_obj) is NULL.
 */
std_ReturnType CCP_PWM_Start(const ccp_t *_ccp_obj)
{
    std_ReturnType ret = E_NOT_OK;
    
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            CCP1CONbits.CCP1M = CCP_PWM_MODE;
        }
        
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            CCP2CONbits.CCP2M = CCP_PWM_MODE;
        }
        
        else{ /* Nothing */ }
        ret = E_OK;
    }
    
    return ret;
}

/**
 * @Summary        Stops PWM operation in the CCP module.
 *    
 * @Description    This function stops PWM signal generation by disabling the CCP module's hardware components.
 *    
 * @Preconditions  The CCP module must be running in PWM mode.
 *    
 * @param _ccp_obj 
 *    Pointer to a configuration structure of type ccp_t representing the CCP instance.
 * 
 * @return  
 *    - E_OK: If the PWM was successfully stopped.
 *    - E_NOT_OK: If the input pointer (_ccp_obj) is NULL.
 */
std_ReturnType CCP_PWM_Stop(const ccp_t *_ccp_obj)
{
    std_ReturnType ret = E_NOT_OK;
    
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            CCP1CONbits.CCP1M = CCP_MODULE_DISABLE;
        }
        
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            CCP2CONbits.CCP2M = CCP_MODULE_DISABLE;
        }
        
        else{ /* Nothing */ }
        ret = E_OK;
    }
    
    return ret;
}
    
#endif


static void CCP_Mode_Timer_Select(const ccp_t *_ccp_obj)
{
    if(CCP1_CCP2_TIMER3 == _ccp_obj->ccp_capture_timer){
        /* Timer3 is the capture/compare clock source for the CCP modules */
        T3CONbits.T3CCP1 = 0; 
        T3CONbits.T3CCP2 = 1;
    }
    else if(CCP1_TIMER1_CCP2_TIMER3 == _ccp_obj->ccp_capture_timer){
        /* Timer3 is the capture/compare clock source for CCP2 */
        /* Timer1 is the capture/compare clock source for CCP1 */
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 0;
    }
    else if(CCP1_CCP2_TIMER1 == _ccp_obj->ccp_capture_timer){
        /* Timer1 is the capture/compare clock source for the CCP modules */
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 0;
    }
    else{ /* Nothing */ }
}


static std_ReturnType CCP_Capture_Mode_Config(const ccp_t *_ccp_obj)
{
    std_ReturnType ret = E_NOT_OK;
    
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            switch(_ccp_obj->ccp_mode_variant)
            {
                case CCP_CAPTURE_MODE_1_FALLING_EDGE :
                    CCP1_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE);
                    break;
                    
                case CCP_CAPTURE_MODE_1_RISING_EDGE :
                    CCP1_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);
                    break;
                    
                case CCP_CAPTURE_MODE_4_RISING_EDGE : 
                    CCP1_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);
                    break;
                    
                case CCP_CAPTURE_MODE_16_RISING_EDGE :
                    CCP1_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE);
                    break;
                    
                default :
                    ret = E_NOT_OK; /* Not supported variant */   
            }            
        }
        
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            switch(_ccp_obj->ccp_mode_variant)
            {
                case CCP_CAPTURE_MODE_1_FALLING_EDGE :
                    CCP2_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE);
                    break;
                    
                case CCP_CAPTURE_MODE_1_RISING_EDGE :
                    CCP2_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);
                    break;
                    
                case CCP_CAPTURE_MODE_4_RISING_EDGE : 
                    CCP2_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);
                    break;
                    
                case CCP_CAPTURE_MODE_16_RISING_EDGE :
                    CCP2_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE);
                    break;
                    
                default :
                    ret = E_NOT_OK; /* Not supported variant */
             }
        }
    }
    CCP_Mode_Timer_Select(_ccp_obj);
    return ret;
}

static std_ReturnType CCP_Compare_Mode_Config(const ccp_t *_ccp_obj)
{
    std_ReturnType ret = E_NOT_OK;
    
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            switch(_ccp_obj->ccp_mode_variant)
            {
                case CCP_COMPARE_MODE_SET_PIN_LOW :
                    CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);
                    break;
                    
                case CCP_COMPARE_MODE_SET_PIN_HIGH :
                    CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);
                    break;
                    
                case CCP_COMPARE_MODE_TOGGLE_ON_MATCH : 
                    CCP1_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);
                    break;
                    
                case CCP_COMPARE_MODE_GEN_SW_INTERRUPT :
                    CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT);
                    break;
                 
                case CCP_COMPARE_MODE_GEN_EVENT :
                    CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT);
                    break;     
                    
                default :
                    ret = E_NOT_OK; /* Not supported variant */   
            }            
        }
        
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            switch(_ccp_obj->ccp_mode_variant)
            {
                case CCP_COMPARE_MODE_SET_PIN_LOW :
                    CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);
                    break;
                    
                case CCP_COMPARE_MODE_SET_PIN_HIGH :
                    CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);
                    break;
                    
                case CCP_COMPARE_MODE_TOGGLE_ON_MATCH : 
                    CCP2_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);
                    break;
                    
                case CCP_COMPARE_MODE_GEN_SW_INTERRUPT :
                    CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT);
                    break;
                    
                case CCP_COMPARE_MODE_GEN_EVENT :
                    CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT);
                    break;    
                    
                default :
                    ret = E_NOT_OK; /* Not supported variant */
             }
        }
    }
    CCP_Mode_Timer_Select(_ccp_obj);
    return ret;
}

#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)
static void CCP_PWM_Mode_Config(const ccp_t *_ccp_obj)
{
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            if(CCP_PWM_MODE == _ccp_obj->ccp_mode_variant)
            {
                CCP1_SET_MODE(CCP_PWM_MODE);
            }
            else{ /* Nothing */ }
        }
        
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            if(CCP_PWM_MODE == _ccp_obj->ccp_mode_variant)
            {
                CCP2_SET_MODE(CCP_PWM_MODE);
            }
            else{ /* Nothing */ }
        }
        
        else{ /* Nothing */ }

        /* PWM Frequency Initialization */
        PR2 = (uint8)((_XTAL_FREQ / (_ccp_obj->PWM_Frequency * 4.0 * _ccp_obj->timer2_prescaler_value * 
                        _ccp_obj->timer2_postscaler_value)) - 1);
}
#endif

static void CCP_Interrupt_Config(const ccp_t *_ccp_obj)
{
    /* CCP1 Interrupt Configurations */ 
#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    CCP1_InterruptEnable();
    CCP1_InterruptFlagClear();
    CCP1_InterruptHandler = _ccp_obj->CCP1_InterruptHandler;
/* Interrupt Priority Configurations */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == _ccp_obj->CCP1_priority)
    {
        /* Enables all high-priority interrupts */
        INTERRUPT_GlobalInterruptHighEnable();
        CCP1_HighPrioritySet();
    }
    else if(INTERRUPT_LOW_PRIORITY == _ccp_obj->CCP1_priority)
    {
        /* Enables all unmasked peripheral interrupts */
        INTERRUPT_GlobalInterruptLowEnable();
        CCP1_LowPrioritySet();
    }
    else{ /* Nothing */ }
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif          
#endif    
        
/* CCP2 Interrupt Configurations */ 
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    CCP2_InterruptEnable();
    CCP2_InterruptFlagClear();
    CCP2_InterruptHandler = _ccp_obj->CCP2_InterruptHandler;
/* Interrupt Priority Configurations */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == _ccp_obj->CCP2_priority)
    {
        /* Enables all high-priority interrupts */
        INTERRUPT_GlobalInterruptHighEnable();
        CCP2_HighPrioritySet();
    }
    else if(INTERRUPT_LOW_PRIORITY == _ccp_obj->CCP2_priority)
    {
        /* Enables all unmasked peripheral interrupts */
        INTERRUPT_GlobalInterruptLowEnable();
        CCP2_LowPrioritySet();
    }
    else{ /* Nothing */ }
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif          
#endif 
}


void CCP1_ISR(void)
{
    CCP1_InterruptFlagClear();
    if(CCP1_InterruptHandler)
    {
        CCP1_InterruptHandler();
    }
    else{ /* Nothing */ }
}

void CCP2_ISR(void)
{
    CCP2_InterruptFlagClear();
    if(CCP2_InterruptHandler)
    {
        CCP2_InterruptHandler();
    }
    else{ /* Nothing */ }
}