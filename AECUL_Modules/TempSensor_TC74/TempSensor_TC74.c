/* 
 * File:   TempSensor_TC7.h
 * Author: ahmed-abdallah
 *
 * Created on November 4, 2024, 2:43 PM
 */

#include "TempSensor_TC74.h"

static std_ReturnType I2C_Read_TEMP_SENSOR_TC74(uint8 TC74_Address, uint8 TC74_Command);
static void MSSP_I2C_Initialize(void);


static mssp_i2c_t i2c_obj;






uint8 ReadTEMPSENSOR_TC74(uint8 TC74_Address)
{
    uint8 Data;
   
    Data = I2C_Read_TEMP_SENSOR_TC74(TC74_Address,0x00);
    return Data;
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
static std_ReturnType I2C_Read_TEMP_SENSOR_TC74(uint8 TC74_Address, uint8 TC74_Command)
{
    uint8 ACK;
    uint16 Read_Data;
    std_ReturnType ret = E_OK;
  
    MSSP_I2C_Initialize();
    
    /* Step 1: Send Start Condition */
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);
    if (ret != E_OK) return ret;

    /* Step 2: Send Device Address with Write Bit  */
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, TC74_Address, &ACK);
    if (ret != E_OK || ACK != I2C_ACK_RECEIVED_FROM_SLAVE) return ret;

    /* Step 3: Send Register Address (0x00)*/
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, TC74_Command, &ACK);
    if (ret != E_OK || ACK != I2C_ACK_RECEIVED_FROM_SLAVE) return ret;

    /* Step 4: Send Repeated Start Condition */
    ret = MSSP_I2C_Master_Send_Repeated_Start(&i2c_obj);
    if (ret != E_OK) return ret;

    /* Step 5: Send Device Address with Read Bit */ 
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, TC74_Address|0x01, &ACK);
    if (ret != E_OK || ACK != I2C_ACK_RECEIVED_FROM_SLAVE) return ret;

    /* Step 6: Read each byte */
    ret = MSSP_I2C_Master_Read_Blocking(&i2c_obj, I2C_ACK_NOT_RECEIVED_FROM_SLAVE, &Read_Data); 
    if (ret != E_OK) return ret;

    
    /* Step 7: Send Stop Condition */
    ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);
    if (ret != E_OK) return ret;

  
    return Read_Data;
}



