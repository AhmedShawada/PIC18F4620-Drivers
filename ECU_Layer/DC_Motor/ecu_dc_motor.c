/* 
 * File:   ecu_dc_motor.h
 * Author: ashaw
 *
 * Created on July 28, 2024, 10:53 PM
 */

#include "ecu_dc_motor.h"


/**
 * @brief Initialize the assigned pins to be OUTPUT and turn the motor OFF or ON.
 * @param _dc_motor pointer to the motor configurations
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType dc_motor_intialize (const dc_motor_t *_dc_motor)
{
    std_ReturnType ret = E_OK;
    
    if(NULL == _dc_motor)
    {
      ret = E_NOT_OK;
    }
    else
    {
        gpio_pin_intialize(&(_dc_motor->dc_motor_pin[0]));
        gpio_pin_intialize(&(_dc_motor->dc_motor_pin[1]));
    }
    
    return ret;
}

/**
 * @brief Move the motor to the right direction
 * @param _dc_motor pointer to the motor configurations
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType dc_motor_move_right (const dc_motor_t *_dc_motor)
{
    std_ReturnType ret = E_OK;
    
    if(NULL == _dc_motor)
    {
      ret = E_NOT_OK;
    }
    else
    {
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[0]) , GPIO_HIGH);
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[1]) , GPIO_LOW);
    }
    
    
    return ret;

}

/**
 * @brief Move the motor to the left direction
 * @param _dc_motor pointer to the motor configurations
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType dc_motor_move_left (const dc_motor_t *_dc_motor)
{
  std_ReturnType ret = E_OK;
  if(NULL == _dc_motor)
  {
    ret = E_NOT_OK;
  }
  
  else
  {
    gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[0]) , GPIO_LOW);
    gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[1]) , GPIO_HIGH);
  }
    
    
    return ret; 
    
}

/**
 * @brief stop the motor movement
 * @param _dc_motor pointer to the motor configurations
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType dc_motor_stop (const dc_motor_t *_dc_motor)
{
  std_ReturnType ret = E_OK;
  
  if(NULL == _dc_motor)
  {
    ret = E_NOT_OK;
  }
  
  else
  {
    gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[0]) , GPIO_LOW);
    gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[1]) , GPIO_LOW);
  }

    return ret;  
}