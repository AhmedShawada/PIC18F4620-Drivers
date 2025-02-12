/* 
 * File:   mcal_external_interrupt.c
 * Author: ahmed-abdallah
 *
 * Created on September 12, 2024, 9:40 PM
 */


#include "mcal_external_interrupt.h"

/* Pointer to function to hold the callbacks for INTx */

static InterruptHandler INT0_InterruptHandler  = NULL;
static InterruptHandler INT1_InterruptHandler  = NULL;
static InterruptHandler INT2_InterruptHandler  = NULL;

/* Pointer to function to hold the callbacks for RBx */

static InterruptHandler RB4_InterruptHandler_HIGH = NULL;
static InterruptHandler RB4_InterruptHandler_LOW   = NULL;

static InterruptHandler RB5_InterruptHandler_HIGH  = NULL;
static InterruptHandler RB5_InterruptHandler_LOW   = NULL;

static InterruptHandler RB6_InterruptHandler_HIGH  = NULL;
static InterruptHandler RB6_InterruptHandler_LOW   = NULL;

static InterruptHandler RB7_InterruptHandler_HIGH  = NULL;
static InterruptHandler RB7_InterruptHandler_LOW   = NULL;

/***************************Helper_Function***************************/

static std_ReturnType Interrupt_INTx_Enable(const interrupt_INTx_t *int_obj);
static std_ReturnType Interrupt_INTx_Disable(const interrupt_INTx_t *int_obj);
static std_ReturnType Interrupt_INTx_Priority_Init(const interrupt_INTx_t *int_obj);
static std_ReturnType Interrupt_INTx_Edge_Init(const interrupt_INTx_t *int_obj);
static std_ReturnType Interrupt_INTx_Pin_Init(const interrupt_INTx_t *int_obj);
static std_ReturnType Interrupt_INTx_Clear_Flag(const interrupt_INTx_t *int_obj);

static std_ReturnType Interrupt_INTx_SetInterruptHandler (const interrupt_INTx_t *int_obj);
static std_ReturnType INT0_SetInterruptHandler (void (*InterruptHandler) (void));
static std_ReturnType INT1_SetInterruptHandler (void (*InterruptHandler) (void));
static std_ReturnType INT2_SetInterruptHandler (void (*InterruptHandler) (void));

static std_ReturnType Interrupt_RBx_Enable(const interrupt_RBx_t *int_obj);
static std_ReturnType Interrupt_RBx_Disable(const interrupt_RBx_t *int_obj);
static std_ReturnType Interrupt_RBx_Priority_Init(const interrupt_RBx_t *int_obj);
static std_ReturnType Interrupt_RBx_Pin_Init(const interrupt_RBx_t *int_obj);
static std_ReturnType Interrupt_RBx_Clear_Flag(const interrupt_RBx_t *int_obj);


static std_ReturnType Interrupt_RBx_SetInterruptHandler (const interrupt_RBx_t *int_obj);
static std_ReturnType RB4_SetInterruptHandler_HIGH (void (*InterruptHandler) (void));
static std_ReturnType RB4_SetInterruptHandler_LOW (void (*InterruptHandler) (void));
static std_ReturnType RB5_SetInterruptHandler_HIGH (void (*InterruptHandler) (void));
static std_ReturnType RB5_SetInterruptHandler_LOW (void (*InterruptHandler) (void));
static std_ReturnType RB6_SetInterruptHandler_HIGH (void (*InterruptHandler) (void));
static std_ReturnType RB6_SetInterruptHandler_LOW (void (*InterruptHandler) (void));
static std_ReturnType RB7_SetInterruptHandler_HIGH (void (*InterruptHandler) (void));
static std_ReturnType RB7_SetInterruptHandler_LOW (void (*InterruptHandler) (void));





/**
 * @brief Initialize the external interrupt INT0, INT1 and INT2
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Interrupt_INTx_Init(const interrupt_INTx_t *int_obj)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /*Disable the External Interrupt */
        ret = Interrupt_INTx_Disable(int_obj);
        
        /*Clear Interrupt Flag : External Interrupt did not occur */
        ret |= Interrupt_INTx_Clear_Flag(int_obj);
        
        /*Configure External Interrupt Edge */
        ret |= Interrupt_INTx_Edge_Init(int_obj);

        /*Configure External Interrupt Priority */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE        
        ret |= Interrupt_INTx_Priority_Init(int_obj);
#endif
        
        /*Configure External Interrupt I/O Pin */
        ret |= Interrupt_INTx_Pin_Init(int_obj);
        
        /*Configure External Interrupt CallBack */
        ret |= Interrupt_INTx_SetInterruptHandler(int_obj);
        
        /*Enable the External Interrupt */
        ret |= Interrupt_INTx_Enable(int_obj);
        
    }
    
    return ret;
}


/**
 * External Interrupt 0 MCAL Helper function 
 */
void INT0_ISR (void)
{
    /* The INT0 external interrupt occurred (must be cleared in software) */
    EXT_INT0_InterruptFlagClear();
    
    /* Code : To be executed from MCAL Interrupt Context */
    
    /* Application Callback function gets called every time this ISR executes */
    if(INT0_InterruptHandler)
    { 
        INT0_InterruptHandler();
    }
    
    else{ /* Nothing */ }
}

/**
 * External Interrupt 1 MCAL Helper function 
 */
void INT1_ISR (void)
{
    /* The INT1 external interrupt occurred (must be cleared in software) */
    EXT_INT1_InterruptFlagClear();
    
    /* Code : To be executed from MCAL Interrupt Context */
    
    /* Application Callback function gets called every time this ISR executes */
    if(INT1_InterruptHandler)
    { 
        INT1_InterruptHandler();
    }
    
    else{ /* Nothing */ }
}

/**
 * External Interrupt 2 MCAL Helper function 
 */
void INT2_ISR (void)
{
    /* The INT2 external interrupt occurred (must be cleared in software) */
    EXT_INT2_InterruptFlagClear();
    
    /* Code : To be executed from MCAL Interrupt Context */
    
    /* Application Callback function gets called every time this ISR executes */
    if(INT2_InterruptHandler)
    { 
        INT2_InterruptHandler();
    }
    
    else{ /* Nothing */ }
}


/**
 * External Interrupt RB4 MCAL Helper function 
 */
void RB4_ISR(uint8 RB4_Source)
{
    /* The RB4 external OnChange interrupt occurred (must be cleared in software) */
    
    EXT_RBx_InterruptFlagClear();
    
    /* Code : To be executed from MCAL Interrupt Context */
    
    /* Application Callback function gets called every time this ISR executes */
    
    if(1 == RB4_Source)
    {
        if(RB4_InterruptHandler_HIGH)
        { 
            RB4_InterruptHandler_HIGH();
        }
        
        else{ /* Nothing */ }
    }
    
    else if (0 == RB4_Source)
    {
        if(RB4_InterruptHandler_LOW)
        {
            RB4_InterruptHandler_LOW();
        }
        
        else{ /* Nothing */ } 
    }
    else { /* Nothing */ }   
}

/**
 * External Interrupt RB5 MCAL Helper function 
 */
void RB5_ISR(uint8 RB5_Source)
{
    /* The RB5 external OnChange interrupt occurred (must be cleared in software) */
    
    EXT_RBx_InterruptFlagClear();
    
    /* Code : To be executed from MCAL Interrupt Context */
    
    /* Application Callback function gets called every time this ISR executes */
    
    if(1 == RB5_Source)
    {
        if(RB5_InterruptHandler_HIGH)
        { 
            RB5_InterruptHandler_HIGH();
        }
        
        else{ /* Nothing */ }
    }
    
    else if (0 == RB5_Source)
    {
        if(RB5_InterruptHandler_LOW)
        {
            RB5_InterruptHandler_LOW();
        }
        
        else{ /* Nothing */ } 
    }
    else { /* Nothing */ }   
}


/**
 * External Interrupt RB6 MCAL Helper function 
 */
void RB6_ISR(uint8 RB6_Source)
{
    /* The RB6 external OnChange interrupt occurred (must be cleared in software) */
    
    EXT_RBx_InterruptFlagClear();
    
    /* Code : To be executed from MCAL Interrupt Context */
    
    /* Application Callback function gets called every time this ISR executes */
    
    if(1 == RB6_Source)
    {
        if(RB6_InterruptHandler_HIGH)
        { 
            RB6_InterruptHandler_HIGH();
        }
        
        else{ /* Nothing */ }
    }
    
    else if (0 == RB6_Source)
    {
        if(RB6_InterruptHandler_LOW)
        {
            RB6_InterruptHandler_LOW();
        }
        
        else{ /* Nothing */ } 
    }
    else { /* Nothing */ }   
}


/**
 * External Interrupt RB7 MCAL Helper function 
 */
void RB7_ISR(uint8 RB7_Source)
{
    /* The RB7 external OnChange interrupt occurred (must be cleared in software) */
    
    EXT_RBx_InterruptFlagClear();
    
    /* Code : To be executed from MCAL Interrupt Context */
    
    /* Application Callback function gets called every time this ISR executes */
    
    if(1 == RB7_Source)
    {
        if(RB7_InterruptHandler_HIGH)
        { 
            RB7_InterruptHandler_HIGH();
        }
        
        else{ /* Nothing */ }
    }
    
    else if (0 == RB7_Source)
    {
        if(RB7_InterruptHandler_LOW)
        {
            RB7_InterruptHandler_LOW();
        }
        
        else{ /* Nothing */ } 
    }
    else { /* Nothing */ }   
}



/**
 * @brief DeInitialize the interrupt module
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Interrupt_INTx_DeInit(const interrupt_INTx_t *int_obj)
{
   std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = Interrupt_INTx_Disable(int_obj);
    }
    return ret; 
    
}



/**
 * @brief  
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Interrupt_RBx_Init(const interrupt_RBx_t *int_obj)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /*Disable the External Interrupt */
        ret = Interrupt_RBx_Disable(int_obj);
        
        /*Clear Interrupt Flag : External Interrupt did not occur */
        ret |= Interrupt_RBx_Clear_Flag(int_obj);
        
        /*Configure External Interrupt Priority */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        ret |= Interrupt_RBx_Priority_Init(int_obj);
#endif

        /*Configure External Interrupt I/O Pin */
        ret |= Interrupt_RBx_Pin_Init(int_obj);
        
        /*Configure External Interrupt CallBack */
        ret |= Interrupt_RBx_SetInterruptHandler(int_obj);
        
        /*Enable the External Interrupt */
        ret |= Interrupt_RBx_Enable(int_obj);
        
    }   
    return ret; 
}




/**
 * @brief  
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType Interrupt_RBx_DeInit(const interrupt_RBx_t *int_obj)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = Interrupt_RBx_Disable(int_obj);
    }
    
    return ret;
}








/**
 * @brief Enable the INTx interrupt source
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType Interrupt_INTx_Enable(const interrupt_INTx_t *int_obj)
{
   std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
       switch(int_obj->source)
       {
            case INTERRUPT_EXTERNAL_INT0 : 
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
                INTERRUPT_GlobalInterruptHighEnable();
#else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
#endif
                EXT_INT0_InterruptEnable(); 
                ret = E_OK; 
                break;
                
                
            case INTERRUPT_EXTERNAL_INT1 : 
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_HIGH_PRIORITY == int_obj->priority)
                { 
                    INTERRUPT_GlobalInterruptHighEnable();
                }
                
                
                else if(INTERRUPT_LOW_PRIORITY == int_obj->priority)
                { 
                    INTERRUPT_GlobalInterruptLowEnable();
                }
                
                else{ /* Nothing */ }
#else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
#endif
                EXT_INT1_InterruptEnable(); 
                ret = E_OK; 
                break;
                
                
            case INTERRUPT_EXTERNAL_INT2 : 
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
                INTERRUPT_PriorityLevelsEnable();
                
                if(INTERRUPT_LOW_PRIORITY == int_obj->priority)
                { 
                    INTERRUPT_GlobalInterruptLowEnable();
                }
                
                else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority)
                { 
                    INTERRUPT_GlobalInterruptHighEnable();
                }
                
                else{ /* Nothing */ }
#else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
#endif
                EXT_INT2_InterruptEnable(); 
                ret = E_OK; 
                break;
                
            default :
                ret = E_NOT_OK;
        }
    }
    
    return ret; 
}

/**
 * @brief  
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType Interrupt_INTx_Disable(const interrupt_INTx_t *int_obj)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        switch(int_obj->source)
       {
           case INTERRUPT_EXTERNAL_INT0 :
               EXT_INT0_InterruptDisable();
               ret = E_OK;
               break;
               
           case INTERRUPT_EXTERNAL_INT1 :
               EXT_INT1_InterruptDisable();
               ret = E_OK;
               break;
               
               
           case INTERRUPT_EXTERNAL_INT2 :
               EXT_INT2_InterruptDisable();
               ret = E_OK;
               break;
               
           default :
               ret = E_NOT_OK;
       }
    }
    
    return ret;
}


/**
 * @brief  
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
static std_ReturnType Interrupt_INTx_Priority_Init(const interrupt_INTx_t *int_obj)
{
   std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        switch(int_obj->source)
       {
    
           case INTERRUPT_EXTERNAL_INT1 :
               if(INTERRUPT_LOW_PRIORITY == int_obj->priority)
               {
                   EXT_INT1_LowPrioritySet();
               }
               else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority)
               {
                   EXT_INT1_HighPrioritySet();
               } 
               else{ /* Nothing */ }
               ret = E_OK;
               break;
               
               
           case INTERRUPT_EXTERNAL_INT2 :
               if(INTERRUPT_LOW_PRIORITY == int_obj->priority)
               {
                   EXT_INT2_LowPrioritySet();
                   
               }
               else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority)
               {
                   EXT_INT2_HighPrioritySet();
               } 
               else{ /* Nothing */ }
               ret = E_OK;
               break;
               
           default :
               ret = E_NOT_OK;
       }
        
    }
    
    return ret; 
}
#endif

/**
 * @brief  
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType Interrupt_INTx_Edge_Init(const interrupt_INTx_t *int_obj)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        switch(int_obj->source)
       {
           case INTERRUPT_EXTERNAL_INT0 :
               if(INTERRUPT_FALLING_EDGE == int_obj->edge)
               {
                   EXT_INT0_FallingEdgeSet();
               }
               else if(INTERRUPT_RISING_EDGE == int_obj->edge)
               {
                   EXT_INT0_RisingEdgeSet();
               } 
               else{ /* Nothing */ }
               ret = E_OK;
               break;
               
               
           case INTERRUPT_EXTERNAL_INT1 :
               if(INTERRUPT_FALLING_EDGE == int_obj->edge)
               {
                   EXT_INT1_FallingEdgeSet();
               }
               else if(INTERRUPT_RISING_EDGE == int_obj->edge)
               {
                   EXT_INT1_RisingEdgeSet();
               } 
               else{ /* Nothing */ }
               ret = E_OK;
               break;
               
               
           case INTERRUPT_EXTERNAL_INT2 :
               if(INTERRUPT_FALLING_EDGE == int_obj->edge)
               {
                   EXT_INT2_FallingEdgeSet();
               }
               else if(INTERRUPT_RISING_EDGE == int_obj->edge)
               {
                   EXT_INT2_RisingEdgeSet();
               }
               else{ /* Nothing */ }
               ret = E_OK;
               break;
               
           default :
               ret = E_NOT_OK;
       }
    }
    
    return ret;
}

/**
 * @brief  
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType Interrupt_INTx_Pin_Init(const interrupt_INTx_t *int_obj)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Initialize the MCU Pin for INTx with the needed configurations */
        ret = gpio_pin_direction_intialize(&(int_obj->mcu_pin));
    }
    
    return ret;
}

/**
 * @brief  
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType Interrupt_INTx_Clear_Flag(const interrupt_INTx_t *int_obj)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        switch(int_obj->source)
       {
           case INTERRUPT_EXTERNAL_INT0 :
               EXT_INT0_InterruptFlagClear();
               ret = E_OK;
               break;
               
           case INTERRUPT_EXTERNAL_INT1 :
               EXT_INT1_InterruptFlagClear();
               ret = E_OK;
               break;
               
               
           case INTERRUPT_EXTERNAL_INT2 :
               EXT_INT2_InterruptFlagClear();
               ret = E_OK;
               break;
               
           default :
               ret = E_NOT_OK;
       }
    }
    return ret; 
}





/**
 * @brief  
 * @param InterruptHandler
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType INT0_SetInterruptHandler (void (*InterruptHandler) (void))
{
   std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
   
    else
    {
        /* Set Default Interrupt Handler for INT0 External Interrupt : Application ISR */
        INT0_InterruptHandler = InterruptHandler;
        ret = E_OK;
    }
   return ret; 
}


/**
 * @brief  
 * @param InterruptHandler
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType INT1_SetInterruptHandler (void (*InterruptHandler) (void))
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
   
    else
    {
        /* Set Default Interrupt Handler for INT1 External Interrupt : Application ISR */
        INT1_InterruptHandler = InterruptHandler;
        ret = E_OK;
    }
   return ret;
}

/**
 * @brief  
 * @param InterruptHandler
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType INT2_SetInterruptHandler (void (*InterruptHandler) (void))
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
   
    else
    {
        /* Set Default Interrupt Handler for INT2 External Interrupt : Application ISR */
        INT2_InterruptHandler = InterruptHandler;
        ret = E_OK;
    }
   return ret;
}

/**
 * @brief  
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType Interrupt_INTx_SetInterruptHandler (const interrupt_INTx_t *int_obj)
{
   std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
   
    else
    {
        switch(int_obj->source)
       {
           case INTERRUPT_EXTERNAL_INT0 :
               ret = INT0_SetInterruptHandler(int_obj->EXT_InterruptHandler);
               
               break;
               
           case INTERRUPT_EXTERNAL_INT1 :
               ret = INT1_SetInterruptHandler(int_obj->EXT_InterruptHandler);
               
               break;
               
               
           case INTERRUPT_EXTERNAL_INT2 :
               ret = INT2_SetInterruptHandler(int_obj->EXT_InterruptHandler);
               
               break;
               
           default :
               ret = E_NOT_OK;
       }
    }
   return ret;
}

/**
 * @brief Enable the RBx interrupt source
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType Interrupt_RBx_Enable(const interrupt_RBx_t *int_obj)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {         
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_LOW_PRIORITY == int_obj->priority)
                { 
                    INTERRUPT_GlobalInterruptLowEnable();
                }
                
                else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority)
                { 
                    INTERRUPT_GlobalInterruptHighEnable();
                }
                
                else{ /* Nothing */ }
#else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
#endif
                EXT_RBx_InterruptEnable();
                ret = E_OK;     

    }  
    return ret;
}


/**
 * @brief Disable the RBx interrupt source
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType Interrupt_RBx_Disable(const interrupt_RBx_t *int_obj)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        EXT_RBx_InterruptDisable();
        ret = E_OK;
    }  
    return ret;
}


/**
 * @brief  
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
static std_ReturnType Interrupt_RBx_Priority_Init(const interrupt_RBx_t *int_obj)
{
   std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(INTERRUPT_LOW_PRIORITY == int_obj->priority)
        {
             EXT_RBx_Priority_Low();
        }    
               
               
        else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority)
        {
             EXT_RBx_Priority_High();
        } 
               
        else{ /* Nothing */ }
        ret = E_OK;  
    }
    return ret; 
}
#endif

/**
 * @brief  
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType Interrupt_RBx_Pin_Init(const interrupt_RBx_t *int_obj)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Initialize the MCU Pin for INTx with the needed configurations */
        ret = gpio_pin_direction_intialize(&(int_obj->mcu_pin));
    }
    
    return ret;
    
}

/**
 * @brief Interrupt_RBx_Clear_Flag 
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType Interrupt_RBx_Clear_Flag(const interrupt_RBx_t *int_obj)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        EXT_RBx_InterruptFlagClear();
        ret = E_OK;        
    }  
    return ret;
    
}



/**
 * @brief RB4_InterruptHandler_HIGH 
 * @param InterruptHandler
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType RB4_SetInterruptHandler_HIGH (void (*InterruptHandler) (void))
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
   
    else
    {
        /* Set Default Interrupt Handler for RB4 External Interrupt HIGH : Application ISR */
        RB4_InterruptHandler_HIGH = InterruptHandler;
        ret = E_OK;
    }
   return ret;
    
}


/**
 * @brief RB4_InterruptHandler_LOW 
 * @param InterruptHandler
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType RB4_SetInterruptHandler_LOW (void (*InterruptHandler) (void))
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
   
    else
    {
        /* Set Default Interrupt Handler for RB4 External Interrupt LOW : Application ISR */
        RB4_InterruptHandler_LOW = InterruptHandler;
        ret = E_OK;
    }
   return ret;
    
}


/**
 * @brief RB5_InterruptHandler_HIGH 
 * @param InterruptHandler
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType RB5_SetInterruptHandler_HIGH (void (*InterruptHandler) (void))
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
   
    else
    {
        /* Set Default Interrupt Handler for RB5 External Interrupt HIGH : Application ISR */
        RB5_InterruptHandler_HIGH = InterruptHandler;
        ret = E_OK;
    }
   return ret;
    
}


/**
 * @brief RB5_InterruptHandler_LOW 
 * @param InterruptHandler
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType RB5_SetInterruptHandler_LOW (void (*InterruptHandler) (void))
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
   
    else
    {
        /* Set Default Interrupt Handler for RB5 External Interrupt LOW : Application ISR */
        RB5_InterruptHandler_LOW = InterruptHandler;
        ret = E_OK;
    }
   return ret;
    
}

/**
 * @brief RB6_InterruptHandler_HIGH 
 * @param InterruptHandler
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType RB6_SetInterruptHandler_HIGH (void (*InterruptHandler) (void))
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
   
    else
    {
        /* Set Default Interrupt Handler for RB6 External Interrupt HIGH : Application ISR */
        RB6_InterruptHandler_HIGH = InterruptHandler;
        ret = E_OK;
    }
   return ret;
    
}


/**
 * @brief RB6_InterruptHandler_LOW 
 * @param InterruptHandler
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType RB6_SetInterruptHandler_LOW (void (*InterruptHandler) (void))
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
   
    else
    {
        /* Set Default Interrupt Handler for RB6 External Interrupt LOW : Application ISR */
        RB6_InterruptHandler_LOW = InterruptHandler;
        ret = E_OK;
    }
   return ret;
    
}

/**
 * @brief RB7_InterruptHandler_HIGH 
 * @param InterruptHandler
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType RB7_SetInterruptHandler_HIGH (void (*InterruptHandler) (void))
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
   
    else
    {
        /* Set Default Interrupt Handler for RB7 External Interrupt HIGH : Application ISR */
        RB7_InterruptHandler_HIGH = InterruptHandler;
        ret = E_OK;
    }
   return ret;
    
}

/**
 * @brief RB7_InterruptHandler_LOW 
 * @param InterruptHandler
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType RB7_SetInterruptHandler_LOW (void (*InterruptHandler) (void))
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
   
    else
    {
        /* Set Default Interrupt Handler for RB7 External Interrupt LOW : Application ISR */
        RB7_InterruptHandler_LOW = InterruptHandler;
        ret = E_OK;
    }
   return ret;
}



/**
 * @brief Interrupt_RBx_SetInterruptHandler 
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType Interrupt_RBx_SetInterruptHandler (const interrupt_RBx_t *int_obj)
{
    std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj)
    {
        ret = E_NOT_OK;
    }
   
    else
    {
        switch(int_obj->mcu_pin.pin)
       {
           case  GPIO_PIN4 :
               RB4_SetInterruptHandler_HIGH(int_obj->EXT_InterruptHandler_HIGH);
               RB4_SetInterruptHandler_LOW(int_obj->EXT_InterruptHandler_LOW);
               ret = E_OK;
               break;
               
           case GPIO_PIN5 :
               RB5_SetInterruptHandler_HIGH(int_obj->EXT_InterruptHandler_HIGH);
               RB5_SetInterruptHandler_LOW(int_obj->EXT_InterruptHandler_LOW);
               ret = E_OK;
               break;
               
               
           case GPIO_PIN6 :
               RB6_SetInterruptHandler_HIGH(int_obj->EXT_InterruptHandler_HIGH);
               RB6_SetInterruptHandler_LOW(int_obj->EXT_InterruptHandler_LOW);
               ret = E_OK;
               break;
               
           case GPIO_PIN7 :
               RB7_SetInterruptHandler_HIGH(int_obj->EXT_InterruptHandler_HIGH);
               RB7_SetInterruptHandler_LOW(int_obj->EXT_InterruptHandler_LOW);
               ret = E_OK;
               break;   
               
           default :
               ret = E_NOT_OK;
       }
    }
   return ret;
    
}