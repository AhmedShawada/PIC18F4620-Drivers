/* 
 * File:   RealTimeClockDS1307.h
 * Author: ahmed-abdallah
 *
 * Created on November 2, 2024, 11:28 PM
 */

#ifndef EEPROM_24C02C_H
#define	EEPROM_24C02C_H

/*Section       :         Includes                */

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/I2C/hal_i2c.h"



/*Section       :         Data type Decleration                 */


/*Section       :         Macro Decleration                */


/*Section       :         Macro Function Decleration                */

/*Section       :         Function Decleration                */
void Write_EEPROM_24C02C(uint8 EEPROM_Address, uint8 Byte_Address, uint8 Data);
void Read_EEPROM_24C02C(uint8 EEPROM_Address, uint8 Byte_Address, uint8 *Data);





#endif	/* EEPROM_24C02C_H */