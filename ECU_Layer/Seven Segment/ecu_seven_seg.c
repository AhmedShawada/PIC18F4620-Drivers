/* 
 * File:   ecu_seven_seg.c
 * Author: ashaw
 *
 * Created on August 13, 2024, 5:15 PM
 */

#include "ecu_seven_seg.h"

/**
 * This Interface to make the needed GPIO and segment initialization
 * @param seg pointer to the segment configurations 
 * @return  Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue while performing this action
 */
std_ReturnType seven_segment_intialize(const segment_t *seg)
{
    std_ReturnType ret = E_OK;
    if(NULL == seg)
    {
      ret = E_NOT_OK;  
    }
    
    else
    {
      ret = gpio_pin_intialize(&(seg->segment_pins[SEGMENT_PIN0]));
      ret = gpio_pin_intialize(&(seg->segment_pins[SEGMENT_PIN1]));
      ret = gpio_pin_intialize(&(seg->segment_pins[SEGMENT_PIN2]));
      ret = gpio_pin_intialize(&(seg->segment_pins[SEGMENT_PIN3]));
    }
    
    
    return ret;
}



/**
 * Write a number to the segment 0 ~ 9
 * @param seg pointer to the segment configurations 
 * @param number the needed number to show
 * @return  Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue while performing this action
 */
std_ReturnType seven_segment_write_number(const segment_t *seg ,uint8 number)
{
    std_ReturnType ret = E_OK;
    
    if((NULL == seg) && (number > 9))
    {
      ret = E_NOT_OK;  
    }
    
    else
    {
     ret = gpio_pin_write_logic(&(seg->segment_pins[SEGMENT_PIN0]), number & 0x01);
     ret = gpio_pin_write_logic(&(seg->segment_pins[SEGMENT_PIN1]), (number>>1) & 0x01);
     ret = gpio_pin_write_logic(&(seg->segment_pins[SEGMENT_PIN2]), (number>>2) & 0x01);
     ret = gpio_pin_write_logic(&(seg->segment_pins[SEGMENT_PIN3]), (number>>3) & 0x01);
        
    }
    
    
    return ret;
    
}