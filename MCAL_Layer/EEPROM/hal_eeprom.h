/* 
 * File:   hal_eeprom.h
 * Author: ahmed-abdallah
 *
 * Created on September 21, 2024, 3:05 AM
 */

#ifndef HAL_EEPROM_H
#define	HAL_EEPROM_H

/* ======================= Section : Includes ======================= */

#include "../Interrupt/mcal_internal_interrupt.h"
#include "../../MCAL_Layer/mcal_std_types.h"
#include "pic18f4620.h"


/* ======================= Section : Data type Decleration ======================= */


/* ======================= Section : Macro Decleration ======================= */

/* Flash Program or Data EEPROM Memory Select  */

#define ACCESS_FLASH_PROGRAM_MEMORY       1
#define ACCESS_EEPROM_PROGRAM_MEMORY      0

/* Flash Program/Data EEPROM or Configuration Select */

#define ACCESS_CONFIG_REGISTERS           1
#define ACCESS_FLASH_EEPROM_MEMORY        0

/* Flash Program/Data EEPROM Write Enable */

#define ALLOW_WRITE_CYCLES_FLASH_EEPROM   1
#define INHIBTS_WRITE_CYCLES_FLASH_EEPROM 0

/* Write Control */

#define INITIATE_DATA_EEPROM_WRITE_ERASE  1
#define DATA_EEPROM_WRITE_ERASE_COMPLETED 0

/* Read Control */

#define INITIATE_DATA_EEPROM_READ         1

/* ======================= Section : Macro Function Decleration ======================= */

/* ======================= Section : Function Decleration ======================= */
std_ReturnType Data_EEPROM_WriteByte(uint16 bAdd, uint8 bData);
std_ReturnType Data_EEPROM_ReadByte(uint16 bAdd, uint8 *bData);

#endif	/* HAL_EEPROM_H */

