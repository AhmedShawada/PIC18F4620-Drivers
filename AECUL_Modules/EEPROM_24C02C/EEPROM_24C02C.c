/* 
 * File:   RealTimeClockDS1307.h
 * Author: ahmed-abdallah
 *
 * Created on November 2, 2024, 11:28 PM
 */

#include "EEPROM_24C02C.h"

static std_ReturnType I2C_Write_EEPROM_24C02C(uint8 EEPROM_Address, uint8 Byte_Address, uint8 Data);
static std_ReturnType I2C_Read_EEPROM_24C02C(uint8 Address, uint8 Register);
static void MSSP_I2C_Initialize(void);

static mssp_i2c_t i2c_obj;

void Write_EEPROM_24C02C(uint8 EEPROM_Address, uint8 Byte_Address, uint8 Data)
{
    I2C_Write_EEPROM_24C02C(EEPROM_Address,Byte_Address,Data);
}


void Read_EEPROM_24C02C(uint8 EEPROM_Address, uint8 Byte_Address, uint8 *Data)
{
    *Data = I2C_Read_EEPROM_24C02C(EEPROM_Address,Byte_Address);
}



static std_ReturnType I2C_Write_EEPROM_24C02C(uint8 EEPROM_Address, uint8 Byte_Address, uint8 Data)
{
    uint8 ACK;
    std_ReturnType ret = E_OK;
  
    MSSP_I2C_Initialize();
    
    /* Step 1: Send Start Condition */
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);
    if (ret != E_OK) return ret;

    /* Step 2: Send Device Address with Write Bit  */
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, EEPROM_Address, &ACK);
    if (ret != E_OK || ACK != I2C_ACK_RECEIVED_FROM_SLAVE) return ret;

    /* Step 3: Send Register Address  */
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, Byte_Address, &ACK);
    if (ret != E_OK || ACK != I2C_ACK_RECEIVED_FROM_SLAVE) return ret;


    /* Step 4: Read each byte  */
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, Data, &ACK);
    if (ret != E_OK || ACK != I2C_ACK_RECEIVED_FROM_SLAVE) return ret;

    
    /* Step 5: Send Stop Condition */
    ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);
    if (ret != E_OK) return ret;
  
    return ret;
}



static std_ReturnType I2C_Read_EEPROM_24C02C(uint8 EEPROM_Address, uint8 Byte_Address)
{
    uint8 ACK;
    uint8 Read_Data;
    std_ReturnType ret = E_OK;
  
    MSSP_I2C_Initialize();
    
    /* Step 1: Send Start Condition */
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);
    if (ret != E_OK) return ret;

    /* Step 2: Send Device Address with Write Bit ) */
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, EEPROM_Address, &ACK);
    if (ret != E_OK || ACK != I2C_ACK_RECEIVED_FROM_SLAVE) return ret;

    /* Step 3: Send Register Address  */
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, Byte_Address, &ACK);
    if (ret != E_OK || ACK != I2C_ACK_RECEIVED_FROM_SLAVE) return ret;

    /* Step 4: Send Repeated Start Condition */
    ret = MSSP_I2C_Master_Send_Repeated_Start(&i2c_obj);
    if (ret != E_OK) return ret;

    /* Step 5: Send Device Address with Read Bit */ 
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, EEPROM_Address|0x01, &ACK);
    if (ret != E_OK || ACK != I2C_ACK_RECEIVED_FROM_SLAVE) return ret;

    /* Step 6: Read each byte */
    ret = MSSP_I2C_Master_Read_Blocking(&i2c_obj, I2C_ACK_NOT_RECEIVED_FROM_SLAVE, &Read_Data); 
    if (ret != E_OK) return ret;

    
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