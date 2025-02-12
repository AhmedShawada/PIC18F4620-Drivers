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

chr_lcd_4bits_t lcd_1 =
{
    .lcd_rs.port = PORTD_INDEX,
    .lcd_rs.pin = GPIO_PIN0,
    .lcd_rs.logic = GPIO_LOW,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_en.port = PORTD_INDEX,
    .lcd_en.pin = GPIO_PIN1,
    .lcd_en.logic = GPIO_LOW,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[0].port = PORTD_INDEX,
    .lcd_data[0].pin = GPIO_PIN2,
    .lcd_data[0].logic = GPIO_LOW,
    .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[1].port = PORTD_INDEX,
    .lcd_data[1].pin = GPIO_PIN3,
    .lcd_data[1].logic = GPIO_LOW,
    .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[2].port = PORTD_INDEX,
    .lcd_data[2].pin = GPIO_PIN4,
    .lcd_data[2].logic = GPIO_LOW,
    .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[3].port = PORTD_INDEX,
    .lcd_data[3].pin = GPIO_PIN5,
    .lcd_data[3].logic = GPIO_LOW,
    .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT 
};

dc_motor_t dc_motor_1 = {
    .dc_motor_pin[0].port = PORTD_INDEX,
    .dc_motor_pin[0].pin  = GPIO_PIN6,
    .dc_motor_pin[0].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pin[0].logic = DC_MOTOR_OFF_STATUS,
    .dc_motor_pin[1].port = PORTD_INDEX,
    .dc_motor_pin[1].pin  = GPIO_PIN7,
    .dc_motor_pin[1].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pin[1].logic = DC_MOTOR_OFF_STATUS
};



mssp_i2c_t i2c_obj;
volatile uint8 ACK;

static volatile uint8 i2c_slave_1_REC_Data;

void MSSP_I2C_DefaultInterruptHandler (void);





int main()
{
    std_ReturnType ret = E_NOT_OK;
  
    i2c_obj.i2c_cfg.i2c_mode = I2C_MSSP_SLAVE_MODE;
    i2c_obj.i2c_cfg.i2c_slave_address = 0X60;
    i2c_obj.i2c_cfg.i2c_mode_cfg = I2C_SLAVE_MODE_7BIT_ADDRESS;
    i2c_obj.i2c_cfg.i2c_SMBus_control = I2C_SLEW_RATE_DISABLE;
    i2c_obj.i2c_cfg.i2c_slew_rate = I2C_SMBus_DISABLE;
    i2c_obj.i2c_cfg.i2c_general_call = I2C_GENERAL_CALL_DISABLE;
    i2c_obj.I2C_DefaultInterruptHandler = MSSP_I2C_DefaultInterruptHandler;
    i2c_obj.I2C_Report_Receive_Overflow = NULL;
    i2c_obj.I2C_Report_Write_Collision = NULL;
       
    ret = MSSP_I2C_Init(&i2c_obj);
    
    application_intialize();
    
    

    while (1)
    { 
        if('a' == i2c_slave_1_REC_Data)
        {
            ret = dc_motor_move_right(&dc_motor_1);
        }
        else if('c' == i2c_slave_1_REC_Data)
        {
            ret = dc_motor_stop(&dc_motor_1);
        }
        else{ /* Nothing */ }
    }
    
    return (EXIT_SUCCESS);
}


void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
    ret = lcd_4bits_intialize(&lcd_1);
    ret = dc_motor_intialize(&dc_motor_1);
    
    
}

void MSSP_I2C_DefaultInterruptHandler (void)
{
    std_ReturnType ret = E_NOT_OK;
    /* Hold Clock Low ( Clock Stretch )*/
    I2C_CLOCK_STRETCH_ENABLE();
    
    /* Master Will Write Data, So I need to read Data*/
    if((SSPSTATbits.R_nW == 0) && (SSPSTATbits.D_nA == 0))
    {
        /* Read The Last Byte To Clear Buffer*/
        uint8 Dummy_Buffer = SSPBUF;
        
        ret = MSSP_I2C_Master_Read_Blocking(&i2c_obj,ACK,&i2c_slave_1_REC_Data);
        ret = lcd_4bits_send_char_data(&lcd_1,i2c_slave_1_REC_Data);
        
    }
    
    /* Master Will Read Data, So I need to Write Data*/
    else if (SSPSTATbits.R_nW == 1)
    {
        
    }
    
    /* Release Clock */
    I2C_CLOCK_STRETCH_DISABLE() ;
}


