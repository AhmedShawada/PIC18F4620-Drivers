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

#define Slave_1  0X60
#define Slave_2  0X62

mssp_i2c_t i2c_obj;
uint8 ACK;
uint8 I2C_Counter;



void MSSP_I2C_Send_1Byte (uint8 Slave_Add ,uint8 Data)
{
    std_ReturnType ret = E_NOT_OK;
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);
    
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj,Slave_Add,&ACK);
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj,Data,&ACK);
    
    ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);
}



int main()
{
    std_ReturnType ret = E_NOT_OK;
    
    i2c_obj.i2c_clock = 100000;
    i2c_obj.i2c_cfg.i2c_mode = I2C_MSSP_MASTER_MODE;
    i2c_obj.i2c_cfg.i2c_mode_cfg = I2C_MASTER_MODE_DEFINED_CLOCK;
    i2c_obj.i2c_cfg.i2c_SMBus_control = I2C_SLEW_RATE_DISABLE;
    i2c_obj.i2c_cfg.i2c_slew_rate = I2C_SMBus_DISABLE;

//  i2c_obj.I2C_DefaultInterruptHandler = NULL;
//  i2c_obj.I2C_Report_Receive_Overflow = NULL;
//  i2c_obj.I2C_Report_Write_Collision = NULL;
       
    ret = MSSP_I2C_Init(&i2c_obj);
    
    application_intialize();
    
    

    while (1)
    { 
        MSSP_I2C_Send_1Byte(Slave_1,'a');
        __delay_ms(1000);
        MSSP_I2C_Send_1Byte(Slave_2,'b');
        __delay_ms(1000);
        MSSP_I2C_Send_1Byte(Slave_1,'c');
        __delay_ms(1000);
        MSSP_I2C_Send_1Byte(Slave_2,'d');
        __delay_ms(1000);
        I2C_Counter++;
    }
    
    return (EXIT_SUCCESS);
}


void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
    
}




