/* 
 * File:   RealTimeClockDS1307.h
 * Author: ahmed-abdallah
 *
 * Created on November 1, 2024, 11:28 PM
 */

#include "RealTimeClockDS1307.h" 


static std_ReturnType I2C_Read_RealTimeClockDS1307(uint8 Address, uint8 Register);
static void MSSP_I2C_Initialize(void);
static void USART_Initialize(void);
static void LCD_Initialize(void);

static RealTimeClock_T DS1307;
static mssp_i2c_t i2c_obj;
static usart_t usart_obj;
static chr_lcd_4bits_t lcd_1;
 



    

/**
 * @Summary 
 *   Retrieves the current date and time from the DS1307 Real Time Clock.
 * 
 * @Description 
 *   This function reads the seconds, minutes, hours, day, month, and year 
 *   from the DS1307 I2C real-time clock. It also updates the RealTimeClock_T 
 *   structure with the retrieved values and displays the time and date on an LCD.
 * 
 * @Preconditions 
 *   The I2C communication must be initialized, and the DS1307 must be properly connected.
 * 
 * @param 
 *   None
 * 
 * @return  
 *   Returns a RealTimeClock_T structure containing the current time and date.
 */
RealTimeClock_T GET_DATA_DS1307(void)
{
    std_ReturnType ret = E_NOT_OK;
    
    DS1307.Seconds = I2C_Read_RealTimeClockDS1307(0xD0, 0x00);
    DS1307.Minutes = I2C_Read_RealTimeClockDS1307(0xD0, 0x01);
    DS1307.Hours = I2C_Read_RealTimeClockDS1307(0xD0, 0x02);
    DS1307.Days = I2C_Read_RealTimeClockDS1307(0xD0, 0x04);
    DS1307.Months = I2C_Read_RealTimeClockDS1307(0xD0, 0x05);
    DS1307.Years = I2C_Read_RealTimeClockDS1307(0xD0, 0x06);
 
    return DS1307;
}

void Print_RealTimeClockDS1307(void)
{
    static uint8 RealTimeClockDS1307_Pack[18];
    std_ReturnType ret = E_NOT_OK;
    
    USART_Initialize();
  
    RealTimeClockDS1307_Pack[0] = ((DS1307.Days >> 4) + 0x30);
    RealTimeClockDS1307_Pack[1] = ((DS1307.Days & 0x0F) + 0x30);
    RealTimeClockDS1307_Pack[2] = '-';
    RealTimeClockDS1307_Pack[3] = ((DS1307.Months >> 4) + 0x30);
    RealTimeClockDS1307_Pack[4] = ((DS1307.Months & 0x0F) + 0x30);
    RealTimeClockDS1307_Pack[5] = '-';
    RealTimeClockDS1307_Pack[6] = ((DS1307.Years >> 4) + 0x30);
    RealTimeClockDS1307_Pack[7] = ((DS1307.Years & 0x0F) + 0x30);
    
    RealTimeClockDS1307_Pack[8] = '/';
    
    RealTimeClockDS1307_Pack[9]  = ((DS1307.Hours >> 4) + 0x30);
    RealTimeClockDS1307_Pack[10] = ((DS1307.Hours & 0x0F) + 0x30);
    RealTimeClockDS1307_Pack[11] = '-';
    RealTimeClockDS1307_Pack[12] = ((DS1307.Minutes >> 4) + 0x30);
    RealTimeClockDS1307_Pack[13] = ((DS1307.Minutes & 0x0F) + 0x30);
    RealTimeClockDS1307_Pack[14] = '-';
    RealTimeClockDS1307_Pack[15] = ((DS1307.Seconds >> 4) + 0x30);
    RealTimeClockDS1307_Pack[16] = ((DS1307.Seconds & 0x0F) + 0x30);

    RealTimeClockDS1307_Pack[17] = '\r';
    
    ret = EUSART_ASYNC_WriteStringBlocking("Date : ", 8);
    ret = EUSART_ASYNC_WriteStringBlocking(RealTimeClockDS1307_Pack, 18);
}

void LCD_RealTimeClockDS1307(void)
{
    uint8 RealTimeClockDS1307_Pack_LCD[9];
    std_ReturnType ret = E_NOT_OK;
    
    LCD_Initialize();
    
    
    // Display Date


    ret = lcd_4bits_send_string_pos(&lcd_1, 1, 1, "Date: ");

    RealTimeClockDS1307_Pack_LCD[0] = ((DS1307.Days >> 4) + 0x30);
    RealTimeClockDS1307_Pack_LCD[1] = ((DS1307.Days & 0x0F) + 0x30);
    RealTimeClockDS1307_Pack_LCD[2] = '-';
    RealTimeClockDS1307_Pack_LCD[3] = ((DS1307.Months >> 4) + 0x30);
    RealTimeClockDS1307_Pack_LCD[4] = ((DS1307.Months & 0x0F) + 0x30);
    RealTimeClockDS1307_Pack_LCD[5] = '-';
    RealTimeClockDS1307_Pack_LCD[6] = ((DS1307.Years >> 4) + 0x30);
    RealTimeClockDS1307_Pack_LCD[7] = ((DS1307.Years & 0x0F) + 0x30);
    RealTimeClockDS1307_Pack_LCD[8] = '\0';
    ret = lcd_4bits_send_string_pos(&lcd_1, 1, 7, RealTimeClockDS1307_Pack_LCD);

    // Display Time
    ret = lcd_4bits_send_string_pos(&lcd_1, 3, 1, "Time: ");
    RealTimeClockDS1307_Pack_LCD[0]  = ((DS1307.Hours >> 4) + 0x30);
    RealTimeClockDS1307_Pack_LCD[1] = ((DS1307.Hours & 0x0F) + 0x30);
    RealTimeClockDS1307_Pack_LCD[2] = ':';
    RealTimeClockDS1307_Pack_LCD[3] = ((DS1307.Minutes >> 4) + 0x30);
    RealTimeClockDS1307_Pack_LCD[4] = ((DS1307.Minutes & 0x0F) + 0x30);
    RealTimeClockDS1307_Pack_LCD[5] = ':';
    RealTimeClockDS1307_Pack_LCD[6] = ((DS1307.Seconds >> 4) + 0x30);
    RealTimeClockDS1307_Pack_LCD[7] = ((DS1307.Seconds & 0x0F) + 0x30);
    RealTimeClockDS1307_Pack_LCD[8] = '\0';

    ret = lcd_4bits_send_string_pos(&lcd_1, 3, 7, RealTimeClockDS1307_Pack_LCD);
}




/**
 * @Summary 
 *   Reads a specific register from the DS1307 Real Time Clock via I2C.
 * 
 * @Description 
 *   This function initializes the MSSP I2C module, sends a start condition, 
 *   addresses the DS1307 with the appropriate register to read, and retrieves 
 *   the data. The data is masked with 0x7F before being returned to ensure 
 *   the correct value is provided, excluding any status flags.
 * 
 * @Preconditions 
 *   The I2C module must be initialized, and the DS1307 should be correctly connected 
 *   and operational.
 * 
 * @param 
 *   Address: The I2C address of the DS1307 device (e.g., 0xD0 for write and 0xD1 for read).
 *   Register: The register address from which to read the data (e.g., seconds, minutes).
 * 
 * @return  
 *   Returns a std_ReturnType indicating success (E_OK) or failure (E_NOT_OK) 
 *   and the read data from the specified register.
 */
static std_ReturnType I2C_Read_RealTimeClockDS1307(uint8 Address, uint8 Register)
{
    uint8 ACK;
    uint8 Read_Data;
    std_ReturnType ret = E_OK;
  
    MSSP_I2C_Initialize();
    
    /* Step 1: Send Start Condition */
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);
    if (ret != E_OK) return ret;

    /* Step 2: Send Device Address with Write Bit (0xD0) */
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, Address, &ACK);
    if (ret != E_OK || ACK != I2C_ACK_RECEIVED_FROM_SLAVE) return ret;

    /* Step 3: Send Register Address (0x00), starting from seconds register */
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, Register, &ACK);
    if (ret != E_OK || ACK != I2C_ACK_RECEIVED_FROM_SLAVE) return ret;

    /* Step 4: Send Repeated Start Condition */
    ret = MSSP_I2C_Master_Send_Repeated_Start(&i2c_obj);
    if (ret != E_OK) return ret;

    /* Step 5: Send Device Address with Read Bit (0xD1) */ 
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0xD1, &ACK);
    if (ret != E_OK || ACK != I2C_ACK_RECEIVED_FROM_SLAVE) return ret;

    /* Step 6: Read each byte (seconds, minutes, hours, day, month, year) */
    ret = MSSP_I2C_Master_Read_Blocking(&i2c_obj, I2C_ACK_NOT_RECEIVED_FROM_SLAVE, &Read_Data); 
    if (ret != E_OK) return ret;

    Read_Data &= 0x7F;
    
    /* Step 7: Send Stop Condition */
    ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);
    if (ret != E_OK) return ret;

  
    return Read_Data;
}



static void MSSP_I2C_Initialize(void)
{
    std_ReturnType ret = E_NOT_OK;

    i2c_obj.i2c_clock = 100000;
    i2c_obj.i2c_cfg.i2c_mode = I2C_MSSP_MASTER_MODE;
    i2c_obj.i2c_cfg.i2c_mode_cfg = I2C_MASTER_MODE_DEFINED_CLOCK;
    i2c_obj.i2c_cfg.i2c_SMBus_control = I2C_SLEW_RATE_DISABLE;
    i2c_obj.i2c_cfg.i2c_slew_rate = I2C_SMBus_DISABLE;
    
    ret = MSSP_I2C_Init(&i2c_obj);
}



static void USART_Initialize(void)
{
    std_ReturnType ret = E_NOT_OK;
 
    usart_obj.baudrate = 9600;
    usart_obj.baudrate_gen_gonfig = BAUDRATE_ASYN_8BIT_lOW_SPEED;
    
    usart_obj.usart_tx_cfg.usart_tx_enable = EUSART_ASYNCHRONOUS_TX_ENABLE;
    usart_obj.usart_tx_cfg.usart_tx_9bit_enable = EUSART_ASYNCHRONOUS_9Bit_TX_DISABLE;
    usart_obj.usart_tx_cfg.usart_tx_interrupt_enable = EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE;
    
    usart_obj.usart_rx_cfg.usart_rx_enable = EUSART_ASYNCHRONOUS_RX_ENABLE;
    usart_obj.usart_rx_cfg.usart_rx_9bit_enable = EUSART_ASYNCHRONOUS_9Bit_RX_DISABLE;
    usart_obj.usart_rx_cfg.usart_rx_interrupt_enable = EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE;
    
    ret = EUSART_ASYNC_Init(&usart_obj);
}


static void LCD_Initialize(void)
{  
    std_ReturnType ret = E_NOT_OK;
    
    lcd_1.lcd_rs.port = PORTD_INDEX;
    lcd_1.lcd_rs.pin = GPIO_PIN0;
    lcd_1.lcd_rs.logic = GPIO_LOW;
    lcd_1.lcd_rs.direction = GPIO_DIRECTION_OUTPUT;

    lcd_1.lcd_en.port = PORTD_INDEX;
    lcd_1.lcd_en.pin = GPIO_PIN1;
    lcd_1.lcd_en.logic = GPIO_LOW;
    lcd_1.lcd_en.direction = GPIO_DIRECTION_OUTPUT;

    lcd_1.lcd_data[0].port = PORTD_INDEX;
    lcd_1.lcd_data[0].pin = GPIO_PIN2;
    lcd_1.lcd_data[0].logic = GPIO_LOW;
    lcd_1.lcd_data[0].direction = GPIO_DIRECTION_OUTPUT;

    lcd_1.lcd_data[1].port = PORTD_INDEX;
    lcd_1.lcd_data[1].pin = GPIO_PIN3;
    lcd_1.lcd_data[1].logic = GPIO_LOW;
    lcd_1.lcd_data[1].direction = GPIO_DIRECTION_OUTPUT;

    lcd_1.lcd_data[2].port = PORTD_INDEX;
    lcd_1.lcd_data[2].pin = GPIO_PIN4;
    lcd_1.lcd_data[2].logic = GPIO_LOW;
    lcd_1.lcd_data[2].direction = GPIO_DIRECTION_OUTPUT;

    lcd_1.lcd_data[3].port = PORTD_INDEX;
    lcd_1.lcd_data[3].pin = GPIO_PIN5;
    lcd_1.lcd_data[3].logic = GPIO_LOW;
    lcd_1.lcd_data[3].direction = GPIO_DIRECTION_OUTPUT;
  
    ret = lcd_4bits_intialize(&lcd_1);
}


