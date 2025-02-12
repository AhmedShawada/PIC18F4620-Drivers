/* 
 * File:   ecu_button.h
 * Author: ashaw
 *
 * Created on July 23, 2024, 6:23 PM
 */

#ifndef ECU_BUTTON_H
#define	ECU_BUTTON_H

/*Section       :         Includes                */
#include "ecu_button_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/*Section       :         Data type Decleration                 */
typedef enum {
    BUTTON_PRESSED = 0,
    BUTTON_RELEASED  
}button_state_t;

typedef enum {
    BUTTON_ACTIVE_HIGH,
    BUTTON_ACTIVE_LOW
}button_active_t;

typedef struct {
    pin_config_t button_pin;
    button_state_t button_state;
    button_active_t button_connection;
}button_t;

/*Section       :         Macro Decleration                */

/*Section       :         Macro Function Decleration                */

/*Section       :         Function Decleration                */
std_ReturnType button_intialize (const button_t *btn);
std_ReturnType button_read_state (const button_t *btn,button_state_t *btn_state);


#endif	/* ECU_BUTTON_H */

