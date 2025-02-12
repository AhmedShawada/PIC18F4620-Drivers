std_ReturnType MSSP_I2C_Master_Write_Blocking(const mssp_i2c_t *i2c_obj, uint8 i2c_data, uint8 *_ack)
{
    std_ReturnType ret = E_NOT_OK;
    
    if ((NULL == i2c_obj) || (NULL == _ack))
    {
        return E_NOT_OK;
    }
    
    /* Write Data to the Data register */
    SSPBUF = i2c_data;
    
    /* Wait for the transmission to complete with a timeout */
    uint16 timeout = 1000;  // Define a timeout counter
    while (!PIR1bits.SSPIF && timeout--)
    {
        __delay_us(1);  // Optional small delay to reduce CPU usage
    }
    
    /* Check if timeout expired */
    if (timeout == 0)
    {
        return E_NOT_OK;  // Transmission failed due to timeout
    }

    /* Clear the MSSP Interrupt Flag bit -> SSPIF */
    PIR1bits.SSPIF = 0;
    
    /* Report the acknowledge received from the slave */
    if (I2C_ACK_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT)
    {
        *_ack = I2C_ACK_RECEIVED_FROM_SLAVE;
    }
    else if (I2C_ACK_NOT_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT)
    {
        *_ack = I2C_ACK_NOT_RECEIVED_FROM_SLAVE;
    }

    ret = E_OK;
    return ret;
}
