/* 
 * File:   ecu_keypad.h
 * Author: ashaw
 *
 * Created on August 20, 2024, 4:04 PM
 */

#ifndef ECU_KEYPAD_H
#define	ECU_KEYPAD_H

/*Section       :         Includes                */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecu_keypad_cfg.h"

/*Section       :         Macro Decleration                */
#define ECU_KEYPAD_ROWS  4
#define ECU_KEYPAD_COLUMNS  4

/*Section       :         Data type Decleration                 */
typedef struct 
{
    pin_config_t keypad_row_pins[ECU_KEYPAD_ROWS];
    pin_config_t keypad_columns_pins[ECU_KEYPAD_COLUMNS];
}keypad_t;

/*Section       :         Macro Function Decleration                */

/*Section       :         Function Decleration                */
std_ReturnType keypad_intialize (const keypad_t *_keypad_obj);
std_ReturnType keypad_get_value (const keypad_t *_keypad_obj, uint8 *value);
#endif	/* ECU_KEYPAD_H */

