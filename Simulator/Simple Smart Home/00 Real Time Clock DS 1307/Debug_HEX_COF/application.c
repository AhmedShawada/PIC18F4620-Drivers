/* 
 * File:   application.c
 * Author: ahmed.shawada
 * https://www.linkedin.com/in/ahmed-shawada/
 * Created on August 31, 2024, 12:32 AM
 */

#include "application.h"
#include "MCAL_Layer/EEPROM/hal_eeprom.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/ADC/hal_adc.h"
#include "MCAL_Layer/Timer0/hal_timer0.h"
#include "MCAL_Layer/Timer1/hal_timer1.h"
#include "MCAL_Layer/Timer2/hal_timer2.h"
#include "MCAL_Layer/Timer3/hal_timer3.h"
#include "MCAL_Layer/CCP/hal_ccp.h"
#include "MCAL_Layer/USART/hal_usart.h"
#include "MCAL_Layer/I2C/hal_i2c.h"

mssp_i2c_t i2c_obj;

typedef struct
{
    uint8 Second;
    uint8 Minutes;
    uint8 Hours;
    uint8 Days;
    uint8 Months;
    uint8 Years;
}RealTimeClock_T;

RealTimeClock_T DS1307;

RealTimeClock_T Read_Second_DS1307(void);
RealTimeClock_T Read_Minutes_DS1307(void);
RealTimeClock_T Read_Hours_DS1307(void);
RealTimeClock_T Read_Days_DS1307(void);
RealTimeClock_T Read_Months_DS1307(void);
RealTimeClock_T Read_Years_DS1307(void);



int main()
{
    
    std_ReturnType ret = E_NOT_OK;
    
    i2c_obj.i2c_clock = 100000;
    i2c_obj.i2c_cfg.i2c_mode = I2C_MSSP_MASTER_MODE;
    i2c_obj.i2c_cfg.i2c_mode_cfg = I2C_MASTER_MODE_DEFINED_CLOCK;
    i2c_obj.i2c_cfg.i2c_SMBus_control = I2C_SLEW_RATE_DISABLE;
    i2c_obj.i2c_cfg.i2c_slew_rate = I2C_SMBus_DISABLE;
//    i2c_obj.I2C_DefaultInterruptHandler = NULL;
//    i2c_obj.I2C_Report_Receive_Overflow = NULL;
//    i2c_obj.I2C_Report_Write_Collision = NULL;
    
    ret = MSSP_I2C_Init(&i2c_obj);
    
    

    application_intialize();
    
    
    
    
    
    while (1)
    { 
        DS1307 = Read_Second_DS1307();
        DS1307 = Read_Minutes_DS1307();
        DS1307 = Read_Hours_DS1307();
        DS1307 = Read_Days_DS1307();
        DS1307 = Read_Months_DS1307();
        DS1307 = Read_Years_DS1307();
    }
    
    return (EXIT_SUCCESS);
}


void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK; 
}


RealTimeClock_T Read_Second_DS1307(void)
{
    uint8 ACK;
    uint16 Read_Data;
    std_ReturnType ret = E_NOT_OK;

    // Step 1: Send Start Condition
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);

    // Step 2: Send Device Address with Write Bit (0xD0)
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0xD0, &ACK);

    // Step 3: Send Register Address (0x00), starting from seconds register
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0x00, &ACK);

    // Step 4: Send Repeated Start Condition
    ret = MSSP_I2C_Master_Send_Repeated_Start(&i2c_obj);

    // Step 5: Send Device Address with Read Bit (0xD1)
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0xD1, &ACK);

    // Step 6: Read each byte (seconds, minutes, hours, day, month, year)
    DS1307.Second = MSSP_I2C_Master_Read_Blocking(&i2c_obj, I2C_ACK_NOT_RECEIVED_FROM_SLAVE, &Read_Data); // NACK on last read
    DS1307.Second = Read_Data & 0x7F;

     

    // Step 7: Send Stop Condition
    ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);

    return DS1307;
}

RealTimeClock_T Read_Minutes_DS1307(void)
{
    uint8 ACK;
    uint16 Read_Data;
    std_ReturnType ret = E_NOT_OK;

    // Step 1: Send Start Condition
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);

    // Step 2: Send Device Address with Write Bit (0xD0)
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0xD0, &ACK);

    // Step 3: Send Register Address (0x00), starting from seconds register
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0x01, &ACK);

    // Step 4: Send Repeated Start Condition
    ret = MSSP_I2C_Master_Send_Repeated_Start(&i2c_obj);

    // Step 5: Send Device Address with Read Bit (0xD1)
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0xD1, &ACK);

    // Step 6: Read each byte (seconds, minutes, hours, day, month, year)
    DS1307.Minutes = MSSP_I2C_Master_Read_Blocking(&i2c_obj, I2C_ACK_NOT_RECEIVED_FROM_SLAVE, &Read_Data);
    DS1307.Minutes = Read_Data & 0x7F;

     

    // Step 7: Send Stop Condition
    ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);

    return DS1307;
}


RealTimeClock_T Read_Hours_DS1307(void)
{
    uint8 ACK;
    uint16 Read_Data;
    std_ReturnType ret = E_NOT_OK;

    // Step 1: Send Start Condition
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);

    // Step 2: Send Device Address with Write Bit (0xD0)
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0xD0, &ACK);

    // Step 3: Send Register Address (0x00), starting from seconds register
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0x02, &ACK);

    // Step 4: Send Repeated Start Condition
    ret = MSSP_I2C_Master_Send_Repeated_Start(&i2c_obj);

    // Step 5: Send Device Address with Read Bit (0xD1)
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0xD1, &ACK);

    // Step 6: Read each byte (seconds, minutes, hours, day, month, year)
    DS1307.Hours = MSSP_I2C_Master_Read_Blocking(&i2c_obj, I2C_ACK_NOT_RECEIVED_FROM_SLAVE, &Read_Data);
    DS1307.Hours = Read_Data & 0x7F;

     

    // Step 7: Send Stop Condition
    ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);

    return DS1307;
}

RealTimeClock_T Read_Days_DS1307(void)
{
    uint8 ACK;
    uint16 Read_Data;
    std_ReturnType ret = E_NOT_OK;

    // Step 1: Send Start Condition
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);

    // Step 2: Send Device Address with Write Bit (0xD0)
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0xD0, &ACK);

    // Step 3: Send Register Address (0x00), starting from seconds register
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0x04, &ACK);

    // Step 4: Send Repeated Start Condition
    ret = MSSP_I2C_Master_Send_Repeated_Start(&i2c_obj);

    // Step 5: Send Device Address with Read Bit (0xD1)
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0xD1, &ACK);

    // Step 6: Read each byte (seconds, minutes, hours, day, month, year)
    DS1307.Days = MSSP_I2C_Master_Read_Blocking(&i2c_obj, I2C_ACK_NOT_RECEIVED_FROM_SLAVE, &Read_Data);
    DS1307.Days = Read_Data & 0x7F; 

    // Step 7: Send Stop Condition
    ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);

    return DS1307;
}


RealTimeClock_T Read_Months_DS1307(void)
{
    uint8 ACK;
    uint16 Read_Data;
    std_ReturnType ret = E_NOT_OK;

    // Step 1: Send Start Condition
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);

    // Step 2: Send Device Address with Write Bit (0xD0)
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0xD0, &ACK);

    // Step 3: Send Register Address (0x00), starting from seconds register
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0x05, &ACK);

    // Step 4: Send Repeated Start Condition
    ret = MSSP_I2C_Master_Send_Repeated_Start(&i2c_obj);

    // Step 5: Send Device Address with Read Bit (0xD1)
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0xD1, &ACK);

    // Step 6: Read each byte (seconds, minutes, hours, day, month, year)
    DS1307.Months = MSSP_I2C_Master_Read_Blocking(&i2c_obj, I2C_ACK_NOT_RECEIVED_FROM_SLAVE, &Read_Data);
    DS1307.Months = Read_Data & 0x7F; 
     

    // Step 7: Send Stop Condition
    ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);

    return DS1307;
}



RealTimeClock_T Read_Years_DS1307(void)
{
    uint8 ACK;
    uint16 Read_Data;
    std_ReturnType ret = E_NOT_OK;

    // Step 1: Send Start Condition
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);

    // Step 2: Send Device Address with Write Bit (0xD0)
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0xD0, &ACK);

    // Step 3: Send Register Address (0x00), starting from seconds register
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0x06, &ACK);

    // Step 4: Send Repeated Start Condition
    ret = MSSP_I2C_Master_Send_Repeated_Start(&i2c_obj);

    // Step 5: Send Device Address with Read Bit (0xD1)
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, 0xD1, &ACK);

    // Step 6: Read each byte (seconds, minutes, hours, day, month, year)
    DS1307.Years = MSSP_I2C_Master_Read_Blocking(&i2c_obj, I2C_ACK_NOT_RECEIVED_FROM_SLAVE, &Read_Data);
    DS1307.Years = Read_Data & 0x7F; 
     

    // Step 7: Send Stop Condition
    ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);

    return DS1307;
}

    

