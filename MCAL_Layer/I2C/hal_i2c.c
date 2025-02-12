/* 
 * File:   hal_i2c.c
 * Author: ahmed-abdallah
 *
 * Created on October 26, 2024, 7:11 PM
 */


#include "hal_i2c.h" 

/***************************Helper_Function***************************/

static inline void MSSP_I2C_Mode_GPIO_CFG(void);
static inline void I2C_Master_Mode_Clock_Configurations(const mssp_i2c_t *i2c_obj);
static inline void I2C_Slave_Mode_General_Call_Configurations(const mssp_i2c_t *i2c_obj);
static inline void MSSP_I2C_Slew_Rate_Control(const mssp_i2c_t *i2c_obj);
static inline void MSSP_I2C_SMBus_Control(const mssp_i2c_t *i2c_obj);
static inline void MSSP_I2C_Interrupt_Configurations(const mssp_i2c_t *i2c_obj);
static inline void MSSP_I2C_BUS_COLLISION_Interrupt_Configurations(const mssp_i2c_t *i2c_obj);
static inline void I2C_Slave_Mode_Configurations(const mssp_i2c_t *i2c_obj);


/******* Pointer to function to hold the call-backs for MSSP I2C *******/

#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void (*I2C_DefaultInterruptHandle)(void) = NULL;
#endif

#if MSSP_I2C_BUS_COL_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
static void (*I2C_Report_Write_Collision_InterruptHandler)(void) = NULL;
#endif

#if MSSP_I2C_Overflow_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE 
static void (*I2C_Report_Receive_Overflow_InterruptHandle)(void) = NULL;
#endif



/**
 * @Summary 
 *    Initializes the MSSP I2C module based on the configuration provided.
 * 
 * @Description 
 *    Configures the MSSP module as an I2C Master or Slave based on the given settings, 
 *    enables necessary features, and configures interrupts if required.
 * 
 * @Preconditions 
 *    Device clock and MSSP peripheral should be properly configured.
 * 
 * @param i2c_obj 
 *    Pointer to a structure containing I2C configuration settings.
 * 
 * @return  
 *    Returns E_OK if initialization is successful; otherwise, returns E_NOT_OK.
 */
std_ReturnType MSSP_I2C_Init(const mssp_i2c_t *i2c_obj)
{
    std_ReturnType ret = E_NOT_OK;
    
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable MSSP I2C Module */
        MSSP_MODULE_DISABLE_CFG();
        
        /* MSSP I2C Select Mode */
        if(I2C_MSSP_MASTER_MODE == i2c_obj->i2c_cfg.i2c_mode) /* MSSP I2C Master Mode Configurations */ 
        {
            /* I2C Master Mode Clock Configurations */
            I2C_Master_Mode_Clock_Configurations(i2c_obj);
        }
        else if(I2C_MSSP_SLAVE_MODE == i2c_obj->i2c_cfg.i2c_mode) /* MSSP I2C Slave Mode Configurations */
        {
            /* I2C Slave Mode General Call Configurations */
            I2C_Slave_Mode_General_Call_Configurations(i2c_obj);
            
            /* Clear the Write Collision Detect */
            Clear_Write_Collision();
            
            /* Clear the Receive Overflow Indicator */
            Clear_Receive_Overflow();
            
            /* Release the clock */
            I2C_CLOCK_STRETCH_DISABLE();
            
            /* Assign the I2C Slave Address */
            SSPADD = i2c_obj->i2c_cfg.i2c_slave_address;
            
            /* I2C Slave Mode Configurations */
            I2C_Slave_Mode_Configurations(i2c_obj);
        }
        
        else { /* Nothing */ }
        
        /* I2C Master Mode GPIO Configurations */  
        MSSP_I2C_Mode_GPIO_CFG();
 
        /* MSSP I2C Slew Rate Control */
        MSSP_I2C_Slew_Rate_Control(i2c_obj);
        
        /* MSSP I2C SMBus Control */
        MSSP_I2C_SMBus_Control(i2c_obj);
        
        /* Interrupt Configurations */
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        MSSP_I2C_Interrupt_Configurations(i2c_obj);
#endif
        
#if MSSP_I2C_BUS_COL_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        MSSP_I2C_BUS_COLLISION_Interrupt_Configurations(i2c_obj);
#endif
        
#if MSSP_I2C_Overflow_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE 
        I2C_Report_Receive_Overflow_InterruptHandle = i2c_obj->I2C_Report_Receive_Overflow;
#endif

        /* Enable MSSP I2C Module */
        MSSP_MODULE_ENABLE_CFG();
        
        ret = E_OK;
    }
    return ret;
}


/**
 * @Summary 
 *    De-initializes the MSSP I2C module, disabling it and clearing any interrupts.
 * 
 * @Description 
 *    Disables the MSSP module and turns off related interrupts.
 * 
 * @Preconditions 
 *    MSSP module should have been initialized.
 * 
 * @param i2c_obj 
 *    Pointer to a structure containing the current I2C configuration.
 * 
 * @return  
 *    Returns E_OK if de-initialization is successful; otherwise, returns E_NOT_OK.
 */
std_ReturnType MSSP_I2C_DeInit(const mssp_i2c_t *i2c_obj)
{
    std_ReturnType ret = E_NOT_OK;
    
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable MSSP I2C Module */
        MSSP_MODULE_DISABLE_CFG();
        
        /* Interrupt Configurations */
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        MSSP_I2C_InterruptDisable();
#endif
        
#if MSSP_I2C_BUS_COL_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        MSSP_I2C_BUS_COL_InterruptDisable();
#endif   
        
        ret = E_OK;
    }
    
    return ret;
}


/**
 * @Summary 
 *    Sends a start condition on the I2C bus in master mode.
 * 
 * @Description 
 *    Generates a start condition and waits until it completes. Reports if the start bit
 *    was successfully detected.
 * 
 * @Preconditions 
 *    MSSP module must be initialized in Master mode.
 * 
 * @param i2c_obj 
 *    Pointer to a structure containing I2C configuration.
 * 
 * @return  
 *    Returns E_OK if start condition is detected; otherwise, E_NOT_OK.
 */
std_ReturnType MSSP_I2C_Master_Send_Start(const mssp_i2c_t *i2c_obj)
{
    std_ReturnType ret = E_NOT_OK;
    
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Initiates Start condition on SDA and SCL pins */
        SSPCON2bits.SEN = 1 ;
        /* Wait for the completion of the Start condition */
        while(SSPCON2bits.SEN);
        /* Clear The MSSP Interrupt Flag bit -> SSPIF */
        PIR1bits.SSPIF = 0;
        /* Report The Start Condition Detection */
        if(I2C_START_BIT_DETECTED == SSPSTATbits.S)
        {
            ret = E_OK; /* Indicates that a Start bit has been detected last */
        }
        else
        {
            ret = E_NOT_OK; /* Start bit was not detected last */
        }
        
    }
    return ret;
}



/**
 * @Summary 
 *    Sends a repeated start condition on the I2C bus in master mode.
 * 
 * @Description 
 *    Generates a repeated start condition and waits until it completes.
 * 
 * @Preconditions 
 *    MSSP module must be initialized in Master mode.
 * 
 * @param i2c_obj 
 *    Pointer to a structure containing I2C configuration.
 * 
 * @return  
 *    Returns E_OK if the repeated start is successful; otherwise, E_NOT_OK.
 */
std_ReturnType MSSP_I2C_Master_Send_Repeated_Start(const mssp_i2c_t *i2c_obj)
{
    std_ReturnType ret = E_NOT_OK;
    
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Initiates Repeated Start Condition on SDA and SCL pins */
        SSPCON2bits.RSEN = 1 ;
        /* Wait for the completion of the Repeated Start Condition condition */
        while(SSPCON2bits.RSEN);
        /* Clear The MSSP Interrupt Flag bit -> SSPIF */
        PIR1bits.SSPIF = 0;
        
        ret = E_OK;
    }
    return ret;
}


/**
 * @Summary 
 *    Sends a stop condition on the I2C bus in master mode.
 * 
 * @Description 
 *    Generates a stop condition and waits until it completes. Reports if the stop bit
 *    was detected.
 * 
 * @Preconditions 
 *    MSSP module must be initialized in Master mode.
 * 
 * @param i2c_obj 
 *    Pointer to a structure containing I2C configuration.
 * 
 * @return  
 *    Returns E_OK if stop condition is detected; otherwise, E_NOT_OK.
 */
std_ReturnType MSSP_I2C_Master_Send_Stop(const mssp_i2c_t *i2c_obj)
{
    std_ReturnType ret = E_NOT_OK;
    
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Initiates Stop condition on SDA and SCL pins */
        SSPCON2bits.PEN = 1 ;
        /* Wait for the completion of the Stop condition */
        while(SSPCON2bits.PEN);
        /* Clear The MSSP Interrupt Flag bit -> SSPIF */
        PIR1bits.SSPIF = 0;
        /* Report The Stop Condition Detection */
        if(I2C_STOP_BIT_DETECTED == SSPSTATbits.P)
        {
            ret = E_OK; /* Indicates that a Stop bit has been detected last */
        }
        else
        {
            ret = E_NOT_OK; /* Stop bit was not detected last */
        }
    }
    return ret;
}


/**
 * @Summary 
 *    Transmits a byte over I2C in master mode with blocking behavior.
 * 
 * @Description 
 *    Sends a data byte and waits for acknowledgment. Sets `_ack` to indicate if the ACK was received.
 * 
 * @Preconditions 
 *    MSSP module must be initialized in Master mode.
 * 
 * @param i2c_obj 
 *    Pointer to a structure containing I2C configuration.
 * @param i2c_data 
 *    The data byte to be transmitted.
 * @param _ack 
 *    Pointer to a variable that will hold the acknowledgment status from the slave.
 * 
 * @return  
 *    Returns E_OK if transmission was successful; otherwise, E_NOT_OK.
 */
std_ReturnType MSSP_I2C_Master_Write_Blocking(const mssp_i2c_t *i2c_obj, uint8 i2c_data, uint8 *_ack)
{
    std_ReturnType ret = E_NOT_OK;
    
    if((NULL == i2c_obj) || (NULL == _ack))
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Write Data to the Data register */
        SSPBUF = i2c_data;
        /* Wait The transmission to be completed */
        while(!PIR1bits.SSPIF);
        /* Clear The MSSP Interrupt Flag bit -> SSPIF */
        PIR1bits.SSPIF = 0;
        /* Report the acknowledge received from the slave */
        if(I2C_ACK_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT)
        {
            /* Acknowledge was received from slave */
            *_ack = I2C_ACK_RECEIVED_FROM_SLAVE;
        }
        else if(I2C_ACK_NOT_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT)
        {
            /* Acknowledge was not received from slave */
            *_ack = I2C_ACK_NOT_RECEIVED_FROM_SLAVE;
        }
        else { /* Nothing */ }
        
        ret = E_OK;
    }
    return ret;
}





/**
 * @Summary 
 *    Reads a byte from the I2C bus in master mode with blocking behavior.
 * 
 * @Description 
 *    Receives a byte and sends an ACK or NACK based on the `ack` parameter.
 * 
 * @Preconditions 
 *    MSSP module must be initialized in Master mode.
 * 
 * @param i2c_obj 
 *    Pointer to a structure containing I2C configuration.
 * @param ack 
 *    Specifies whether to send an ACK or NACK after reading.
 * @param i2c_data 
 *    Pointer to where the read byte will be stored.
 * 
 * @return  
 *    Returns E_OK if read is successful; otherwise, E_NOT_OK.
 */
std_ReturnType MSSP_I2C_Master_Read_Blocking(const mssp_i2c_t *i2c_obj, uint8 ack, uint8 *i2c_data)
{
    std_ReturnType ret = E_NOT_OK;
    
    if((NULL == i2c_obj) || (NULL == i2c_data))
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Master Mode Receive Enable */
        I2C_MASTER_RECEIVE_ENABLE_CFG();
        /* Wait for buffer full flag : A complete byte received */
        while(!SSPSTATbits.BF);
        /* Copy The data registers to buffer variable */
        *i2c_data = SSPBUF;
        /* Send ACK or NACK after read */
        if(I2C_MASTER_SEND_ACK == ack)
        {
            /* Acknowledge */
            SSPCON2bits.ACKDT = 0;
        }
        else if(I2C_MASTER_SEND_NACK == ack)
        {
            /* Not Acknowledge */
            SSPCON2bits.ACKDT = 1;
        }
        else { /* Nothing */ }  
        
        /* Initiates Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. */
        SSPCON2bits.ACKEN = 1;
        
        ret = E_OK;
    }
    return ret;
}



/**
 * @Summary 
 *    Transmits a byte over I2C in master mode without blocking.
 * 
 * @Description 
 *    Sends a data byte without waiting for completion, and sets `_ack` based on slave response.
 * 
 * @Preconditions 
 *    MSSP module must be initialized in Master mode.
 * 
 * @param i2c_obj 
 *    Pointer to a structure containing I2C configuration.
 * @param i2c_data 
 *    The data byte to be transmitted.
 * @param _ack 
 *    Pointer to a variable to store the acknowledgment status from the slave.
 * 
 * @return  
 *    Returns E_OK if transmission initiation was successful; otherwise, E_NOT_OK.
 */
std_ReturnType MSSP_I2C_Master_Write_NonBlocking(const mssp_i2c_t *i2c_obj, uint8 i2c_data, uint8 *_ack)
{
    std_ReturnType ret = E_NOT_OK;
    
    if((NULL == i2c_obj) || (NULL == _ack))
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Write Data to the Data register */
        SSPBUF = i2c_data;
        /* Wait The transmission to be completed */
        if(1 == PIR1bits.SSPIF)
        {
            /* Clear The MSSP Interrupt Flag bit -> SSPIF */
            PIR1bits.SSPIF = 0;
            if(I2C_ACK_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT)
            {
                
                /* Acknowledge was received from slave */
                *_ack = I2C_ACK_RECEIVED_FROM_SLAVE;
            }
            else if(I2C_ACK_NOT_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT)
            {
                /* Acknowledge was not received from slave */
                *_ack = I2C_ACK_NOT_RECEIVED_FROM_SLAVE;
            }
            else { /* Nothing */ }
            
            ret = E_OK;
        } 
    }
    return ret;
}



/**
 * @Summary 
 *    Reads a byte from the I2C bus in master mode without blocking.
 * 
 * @Description 
 *    Receives a byte if available and sends an ACK or NACK after read.
 * 
 * @Preconditions 
 *    MSSP module must be initialized in Master mode.
 * 
 * @param i2c_obj 
 *    Pointer to a structure containing I2C configuration.
 * @param ack 
 *    Specifies whether to send an ACK or NACK after reading.
 * @param i2c_data 
 *    Pointer to where the read byte will be stored if received.
 * 
 * @return  
 *    Returns E_OK if read is successful; otherwise, E_NOT_OK.
 */
std_ReturnType MSSP_I2C_Master_Read_NonBlocking(const mssp_i2c_t *i2c_obj, uint8 ack, uint8 *i2c_data)
        {
    std_ReturnType ret = E_NOT_OK;
    
    if((NULL == i2c_obj) || (NULL == i2c_data))
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Master Mode Receive Enable */
        I2C_MASTER_RECEIVE_ENABLE_CFG();
        /* Wait for buffer full flag : A complete byte received */
        if(1 == SSPSTATbits.BF)
        {
            /* Copy The data registers to buffer variable */
            *i2c_data = SSPBUF;
            /* Send ACK or NACK after read */
            if(I2C_MASTER_SEND_ACK == ack)
            {
                /* Acknowledge */
                SSPCON2bits.ACKDT = 0;
            }
            else if(I2C_MASTER_SEND_NACK == ack)
            {
                /* Not Acknowledge */
                SSPCON2bits.ACKDT = 1;
            }
            else { /* Nothing */ }  
        
            /* Initiates Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. */
            SSPCON2bits.ACKEN = 1;
            
            ret = E_OK;
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    return ret;
}









/**
 * @Summary 
 * Configures the GPIO pins for I2C communication in GPIO mode.
 * 
 * @Description 
 * This function sets the Serial Clock (SCL) and Serial Data (SDA) lines as input pins,
 * allowing for I2C communication by configuring the MSSP module.
 * 
 * @Preconditions 
 * The system clock should be configured and the MSSP module should be initialized.
 * 
 * @param  
 * None
 * 
 * @return  
 * None
 */
static inline void MSSP_I2C_Mode_GPIO_CFG(void)
{
    Serial_Clock_Configuration();  /* Serial clock (SCL) is Input */
    Serial_Data_Configuration();   /* Serial data  (SDA) is Input */ 
}



/**
 * @Summary 
 * Configures the clock settings for I2C Master mode.
 * 
 * @Description 
 * This function sets the SSPCON1 register to configure the I2C master mode and calculates
 * the SSPADD value based on the desired I2C clock frequency.
 * 
 * @Preconditions 
 * The I2C object must be initialized and configured with the desired clock frequency.
 * 
 * @param  
 * i2c_obj: Pointer to the mssp_i2c_t structure containing I2C configuration settings.
 * 
 * @return  
 * None
 */
static inline void I2C_Master_Mode_Clock_Configurations(const mssp_i2c_t *i2c_obj)
{
    /*  I2C Master mode, clock = FOSC/(4 * (SSPADD + 1)) */
    SSPCON1bits.SSPM = i2c_obj->i2c_cfg.i2c_mode_cfg;
    SSPADD = (uint8)(((_XTAL_FREQ / 4.0) / i2c_obj->i2c_clock) - 1);
}


/**
 * @Summary 
 * Configures the I2C module for slave mode.
 * 
 * @Description 
 * This function sets the I2C mode to operate in slave mode by configuring the SSPM bits 
 * in the SSPCON1 register based on the specified configuration.
 * 
 * @Preconditions 
 * The I2C object must be initialized and configured with the desired slave mode settings.
 * 
 * @param  
 * i2c_obj: Pointer to the mssp_i2c_t structure containing I2C configuration settings,
 *          including the I2C mode configuration.
 * 
 * @return  
 * None
 */
static inline void I2C_Slave_Mode_Configurations(const mssp_i2c_t *i2c_obj)
{
    SSPCON1bits.SSPM = i2c_obj->i2c_cfg.i2c_mode_cfg;
}


/**
 * @Summary 
 * Configures General Call settings for I2C Slave mode.
 * 
 * @Description 
 * This function enables or disables the General Call feature for the I2C slave mode
 * based on the configuration settings in the provided I2C object.
 * 
 * @Preconditions 
 * The I2C object must be initialized and configured for Slave mode.
 * 
 * @param  
 * i2c_obj: Pointer to the mssp_i2c_t structure containing I2C configuration settings.
 * 
 * @return  
 * None
 */
static inline void I2C_Slave_Mode_General_Call_Configurations(const mssp_i2c_t *i2c_obj)
{
    if(I2C_GENERAL_CALL_ENABLE == i2c_obj->i2c_cfg.i2c_general_call)
    {
        I2C_GENERAL_CALL_ENABLE_CFG();
    }
    else if(I2C_GENERAL_CALL_DISABLE == i2c_obj->i2c_cfg.i2c_general_call)
    {
         I2C_GENERAL_CALL_DISABLE_CFG();
    }
    else { /* Nothing */ }
}




/**
 * @Summary 
 * Controls the slew rate for the I2C communication.
 * 
 * @Description 
 * This function enables or disables the slew rate control based on the I2C configuration settings.
 * 
 * @Preconditions 
 * The I2C object must be initialized and configured.
 * 
 * @param  
 * i2c_obj: Pointer to the mssp_i2c_t structure containing I2C configuration settings.
 * 
 * @return  
 * None
 */
static inline void MSSP_I2C_Slew_Rate_Control(const mssp_i2c_t *i2c_obj)
{
    if(I2C_SLEW_RATE_DISABLE == i2c_obj->i2c_cfg.i2c_slew_rate)
    {
        I2C_SLEW_RATE_DISABLE_CFG();
    }
    else if(I2C_SLEW_RATE_ENABLE == i2c_obj->i2c_cfg.i2c_slew_rate)
    {
        I2C_SLEW_RATE_ENABLE_CFG();
    }
    else { /* Nothing */ }
}




/**
 * @Summary 
 * Configures the SMBus control settings for I2C.
 * 
 * @Description 
 * This function enables or disables SMBus control features based on the provided configuration.
 * 
 * @Preconditions 
 * The I2C object must be initialized and configured.
 * 
 * @param  
 * i2c_obj: Pointer to the mssp_i2c_t structure containing I2C configuration settings.
 * 
 * @return  
 * None
 */
static inline void MSSP_I2C_SMBus_Control(const mssp_i2c_t *i2c_obj)
{
    if(I2C_SMBus_ENABLE == i2c_obj->i2c_cfg.i2c_SMBus_control)
    {
        I2C_SMBus_ENABLE_CFG();
    }
    else if(I2C_SMBus_DISABLE == i2c_obj->i2c_cfg.i2c_SMBus_control)
    {
        I2C_SMBus_DISABLE_CFG();
    }
    else { /* Nothing */ }
}




/**
 * @Summary 
 * Configures interrupts for the I2C module.
 * 
 * @Description 
 * This function sets up the necessary interrupt settings for the I2C module, including enabling
 * the interrupt and setting the priority based on the configuration settings.
 * 
 * @Preconditions 
 * The I2C object must be initialized with the appropriate interrupt handler.
 * 
 * @param  
 * i2c_obj: Pointer to the mssp_i2c_t structure containing I2C configuration settings.
 * 
 * @return  
 * None
 */
static inline void MSSP_I2C_Interrupt_Configurations(const mssp_i2c_t *i2c_obj)
{   
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    MSSP_I2C_InterruptEnable();
    MSSP_I2C_InterruptFlagClear();
    I2C_DefaultInterruptHandle = i2c_obj->I2C_DefaultInterruptHandler;
    
    /* Interrupt Priority Configurations */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    
    if(INTERRUPT_HIGH_PRIORITY == i2c_obj->i2c_cfg.mssp_i2c_priority)
    {
        /* Enables all high-priority interrupts */
        INTERRUPT_GlobalInterruptHighEnable();
        MSSP_I2C_HighPrioritySet();
    }
    
     else if(INTERRUPT_LOW_PRIORITY == i2c_obj->i2c_cfg.mssp_i2c_priority)
    {
        /* Enables all unmasked peripheral interrupts */
        INTERRUPT_GlobalInterruptLowEnable();
        MSSP_I2C_LowPrioritySet();
    }
    else{ /* Nothing */ }
    
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
    
}




/**
 * @Summary 
 * Configures bus collision interrupt settings for I2C.
 * 
 * @Description 
 * This function sets up the interrupt for bus collisions, enabling the interrupt
 * and setting the appropriate priority based on the provided configuration.
 * 
 * @Preconditions 
 * The I2C object must be initialized with the appropriate collision interrupt handler.
 * 
 * @param  
 * i2c_obj: Pointer to the mssp_i2c_t structure containing I2C configuration settings.
 * 
 * @return  
 * None
 */
static inline void MSSP_I2C_BUS_COLLISION_Interrupt_Configurations(const mssp_i2c_t *i2c_obj)
{   
#if MSSP_I2C_BUS_COL_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    MSSP_I2C_BUS_COL_InterruptEnable();
    MSSP_I2C_BUS_COL_InterruptFlagClear();
    I2C_Report_Write_Collision_InterruptHandler = i2c_obj->I2C_Report_Write_Collision;
    
    /* Interrupt Priority Configurations */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    
    if(INTERRUPT_HIGH_PRIORITY == i2c_obj->i2c_cfg.mssp_i2c_bc_priority)
    {
        /* Enables all high-priority interrupts */
        INTERRUPT_GlobalInterruptHighEnable();
        MSSP_I2C_BUS_COL_HighPrioritySet();
    }
    
     else if(INTERRUPT_LOW_PRIORITY == i2c_obj->i2c_cfg.mssp_i2c_bc_priority)
    {
        /* Enables all unmasked peripheral interrupts */
        INTERRUPT_GlobalInterruptLowEnable();
        MSSP_I2C_BUS_COL_LowPrioritySet();
    }
    else{ /* Nothing */ }
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
    
}








void MSSP_I2C_ISR(void)
{
    
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    MSSP_I2C_InterruptFlagClear();
    if(I2C_DefaultInterruptHandle)
    {
        I2C_DefaultInterruptHandle();
    }
    
#endif   
}


void MSSP_I2C_BC_ISR(void)
{
#if MSSP_I2C_BUS_COL_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    
    MSSP_I2C_BUS_COL_InterruptFlagClear();
    if(I2C_Report_Write_Collision_InterruptHandler)
    {
        I2C_Report_Write_Collision_InterruptHandler();
    }
    
#endif 
}