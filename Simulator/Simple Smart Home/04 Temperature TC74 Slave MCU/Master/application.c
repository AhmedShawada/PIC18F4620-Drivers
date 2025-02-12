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

#define Slave 0X70

void MSSP_I2C_Send_1Byte (uint8 Slave_Add ,uint8 *Data);

mssp_i2c_t i2c_obj;
uint8 Temp_Value;



int main()
{
    std_ReturnType ret = E_NOT_OK;
    
    i2c_obj.i2c_clock = 100000;
    i2c_obj.i2c_cfg.i2c_mode = I2C_MSSP_MASTER_MODE;
    i2c_obj.i2c_cfg.i2c_mode_cfg = I2C_MASTER_MODE_DEFINED_CLOCK;
    i2c_obj.i2c_cfg.i2c_SMBus_control = I2C_SLEW_RATE_DISABLE;
    i2c_obj.i2c_cfg.i2c_slew_rate = I2C_SMBus_DISABLE;
    
    application_intialize();
    
   
    
    while (1)
    { 
        ReadTEMPSENSOR_TC74(0x9E,&Temp_Value);
        MSSP_I2C_Send_1Byte(Slave,&Temp_Value);
    }
    
    return (EXIT_SUCCESS);
}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK; 

}

void MSSP_I2C_Send_1Byte (uint8 Slave_Add ,uint8 *Data)
{
    uint8 ACK;
    std_ReturnType ret = E_OK;
    
    /* Step 1: Send Start Condition */
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);
    
    /* Step 2: Send Device Address with Write Bit  */
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, Slave_Add, &ACK);
    
    /* Step 3: Send Data */
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, Data, &ACK);
    
    /* Step 4: Send Stop Condition */
    ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);
}





