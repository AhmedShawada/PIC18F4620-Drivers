/* 
 * File:   ecu_seven_seg.h
 * Author: ashaw
 *
 * Created on August 13, 2024, 5:08 PM
 */

#ifndef ECU_SEVEN_SEG_H
#define	ECU_SEVEN_SEG_H

/*Section       :         Includes                */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecu_seven_seg_cfg.h"

/*Section       :         Data type Decleration                 */
typedef enum{
    SEGMENT_COMMON_ANODE,
    SEGMENT_COMMON_CATHODE        
}segment_type_t;

typedef struct{
    pin_config_t segment_pins[4];
    segment_type_t segment_type;      
}segment_t;


/*Section       :         Macro Decleration                */
#define SEGMENT_PIN0 0
#define SEGMENT_PIN1 1
#define SEGMENT_PIN2 2
#define SEGMENT_PIN3 3

/*Section       :         Macro Function Decleration                */


/*Section       :         Function Decleration                */
std_ReturnType seven_segment_intialize(const segment_t *seg);
std_ReturnType seven_segment_write_number(const segment_t *seg ,uint8 number);

#endif	/* ECU_SEVEN_SEG_H */

