/* 
 * File:   ecu_chr_lcd.c
 * Author: ashaw
 *
 * Created on August 29, 2024, 12:32 AM
 */

#include "ecu_chr_lcd.h"


/********************************* HELPER FUNCTION *********************************/

static std_ReturnType lcd_send_4bits(const chr_lcd_4bits_t *lcd,uint8 _data_command);
static std_ReturnType lcd_4bits_send_enable_signle(const chr_lcd_4bits_t *lcd);
static std_ReturnType lcd_8bits_send_enable_signle(const chr_lcd_8bits_t *lcd);
static std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bits_t *lcd, uint8 row, uint8 column);
static std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bits_t *lcd, uint8 row, uint8 column);





/**
 * 
 * @param lcd
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_4bits_intialize(const chr_lcd_4bits_t *lcd)
{
    std_ReturnType ret = E_OK;
    uint8 data_pins_counter = ZERO_INIT;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;   
    }
    
    else
    {
        ret = gpio_pin_intialize(&(lcd->lcd_rs));
        ret = gpio_pin_intialize(&(lcd->lcd_en)); 
        
        for(data_pins_counter=ZERO_INIT;data_pins_counter<4;data_pins_counter++)
        {
            ret = gpio_pin_intialize(&(lcd->lcd_data[data_pins_counter]));
        }
        
        __delay_ms(20);
        ret = lcd_4bits_send_command(lcd,_LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        ret = lcd_4bits_send_command(lcd,_LCD_8BIT_MODE_2_LINE);
        __delay_ms(150);
        ret = lcd_4bits_send_command(lcd,_LCD_8BIT_MODE_2_LINE);
        
        ret = lcd_4bits_send_command(lcd,_LCD_CLEAR);
        ret = lcd_4bits_send_command(lcd,_LCD_RETURN_HOME);
        ret = lcd_4bits_send_command(lcd,_LCD_ENTRY_MODE_INC_SHIFT_OFF);
        ret = lcd_4bits_send_command(lcd,_LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
        ret = lcd_4bits_send_command(lcd,_LCD_4BIT_MODE_2_LINE);
        ret = lcd_4bits_send_command(lcd,_LCD_DDRAM_START);
    }
    
    return ret;
}


/**
 * 
 * @param lcd
 * @param command
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_4bits_send_command(const chr_lcd_4bits_t *lcd ,uint8 command)
{
    std_ReturnType ret = E_OK;
    
    if(NULL == lcd)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        /* R/W Pin connected to the GND -> Logic (0) "Hard Wired" */
        /* Write Logic (0) to the "Register Select" Pin to select the "Instruction Register"  */
        ret = gpio_pin_write_logic(&(lcd->lcd_rs),GPIO_LOW);
        
        /* Send the Command through the (4-Pins" Data lines */
        ret = lcd_send_4bits(lcd, command >> 4);
        
        /* Send the Enable Signal on the "E" Pin */
        ret = lcd_4bits_send_enable_signle(lcd);
        
        /* Send the Command through the (4-Pins" Data lines */
        ret = lcd_send_4bits(lcd, command);
        
        /* Send the Enable Signal on the "E" Pin */
        ret = lcd_4bits_send_enable_signle(lcd);
    }
    
    return ret;
 
}

/**
 * 
 * @param lcd Pointer to the LCD configurations
 * @param data The data that we need to send to the LCD
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_4bits_send_char_data(const chr_lcd_4bits_t *lcd ,uint8 data)
{
    std_ReturnType ret = E_OK;
    
    if(NULL == lcd)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        /* R/W Pin connected to the GND -> Logic (0) "Hard Wired" */
        /* Write Logic (1) to the "Register Select" Pin to select the "Data Register"  */
        ret = gpio_pin_write_logic(&(lcd->lcd_rs),GPIO_HIGH);
        
         /* Send the Data through the (4-Pins" Data lines */
        ret = lcd_send_4bits(lcd, data >> 4);
        
        /* Send the Enable Signal on the "E" Pin */
        ret = lcd_4bits_send_enable_signle(lcd);
        
         /* Send the Data through the (4-Pins" Data lines */
        ret = lcd_send_4bits(lcd, data);
        
        /* Send the Enable Signal on the "E" Pin */
        ret = lcd_4bits_send_enable_signle(lcd);
        
    }
    
    return ret;

}



/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @param data
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_4bits_send_char_data_pos(const chr_lcd_4bits_t *lcd ,uint8 row ,uint8 column ,uint8 data)
{
    std_ReturnType ret = E_OK;
    
    if(NULL == lcd)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        ret = lcd_4bit_set_cursor(lcd,row,column);
        ret = lcd_4bits_send_char_data(lcd,data); 
    }
    
    return ret;
  
}




/**
 * 
 * @param lcd
 * @param str
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_4bits_send_string(const chr_lcd_4bits_t *lcd ,uint8 *str)
{
    std_ReturnType ret = E_OK;
    
    if((NULL == lcd) || (NULL ==str))
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        while(*str)
        {
            ret = lcd_4bits_send_char_data(lcd,*str++); 
        }
    }  
	
    return ret;

}


/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @param str
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_4bits_send_string_pos(const chr_lcd_4bits_t *lcd ,uint8 row ,uint8 column ,uint8 *str)
{
    std_ReturnType ret = E_OK;
    
    if((NULL == lcd) || (NULL ==str))
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        ret = lcd_4bit_set_cursor(lcd,row,column);
        while(*str)
        {
            ret = lcd_4bits_send_char_data(lcd,*str++); 
        }
    }
    return ret;
}




/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @param _chr
 * @param mem_pos
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_4bits_send_custom_char(const chr_lcd_4bits_t *lcd ,uint8 row ,uint8 column,
                                          const uint8 _chr[] ,uint8 mem_pos)
{
    std_ReturnType ret = E_OK;
    uint8 lcd_counter = ZERO_INIT;
    
    if(NULL == lcd)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        ret = lcd_4bits_send_command(lcd,(_LCD_CGRAM_START + (mem_pos*8)));
        for(lcd_counter = ZERO_INIT;lcd_counter<=7;lcd_counter++)
        {
            ret = lcd_4bits_send_char_data(lcd,_chr[lcd_counter]);
        }
        
        ret = lcd_4bits_send_char_data_pos(lcd,row,column,mem_pos);
    }
    
    return ret;
}



/**
 * 
 * @param lcd
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_8bits_intialize(const chr_lcd_8bits_t *lcd)
{
    std_ReturnType ret = E_OK;
    uint8 data_pins_counter = ZERO_INIT;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;   
    }
    
    else
    {
        ret = gpio_pin_intialize(&(lcd->lcd_rs));
        ret = gpio_pin_intialize(&(lcd->lcd_en)); 
        
        for(data_pins_counter=ZERO_INIT;data_pins_counter<8;data_pins_counter++)
        {
            ret = gpio_pin_intialize(&(lcd->lcd_data[data_pins_counter]));
        }
        
        __delay_ms(20);
        ret = lcd_8bits_send_command(lcd,_LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        ret = lcd_8bits_send_command(lcd,_LCD_8BIT_MODE_2_LINE);
        __delay_ms(150);
        ret = lcd_8bits_send_command(lcd,_LCD_8BIT_MODE_2_LINE);
        
        ret = lcd_8bits_send_command(lcd,_LCD_CLEAR);
        ret = lcd_8bits_send_command(lcd,_LCD_RETURN_HOME);
        ret = lcd_8bits_send_command(lcd,_LCD_ENTRY_MODE_INC_SHIFT_OFF);
        ret = lcd_8bits_send_command(lcd,_LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
        ret = lcd_8bits_send_command(lcd,_LCD_8BIT_MODE_2_LINE);
        ret = lcd_8bits_send_command(lcd,_LCD_DDRAM_START); 
    }
    
    return ret;
  
}


/**
 * 
 * @param lcd
 * @param command
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_8bits_send_command(const chr_lcd_8bits_t *lcd ,uint8 command)
{
    std_ReturnType ret = E_OK;
    uint8 pins_counter = ZERO_INIT;
    
    if(NULL == lcd)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        /* R/W Pin connected to the GND -> Logic (0) "Hard Wired" */
        /* Write Logic (0) to the "Register Select" Pin to select the "Instruction Register"  */ 
        ret = gpio_pin_write_logic(&(lcd->lcd_rs),GPIO_LOW);
        
        /* Send the Command through the (8-Pins" Data lines */
        for(pins_counter = 0;pins_counter < 8;pins_counter++)
        {
          ret = gpio_pin_write_logic(&(lcd->lcd_data[pins_counter]), (command >> pins_counter) & (uint8)0x01);
        }
        
        /* Send the Enable Signal on the "E" Pin */
        ret = lcd_8bits_send_enable_signle(lcd);
    }
    
    return ret;
  
}

/**
 * 
 * @param lcd
 * @param data
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_8bits_send_char_data(const chr_lcd_8bits_t *lcd ,uint8 data)
{
    std_ReturnType ret = E_OK;
    uint8 pins_counter = ZERO_INIT;
    
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* R/W Pin connected to the GND -> Logic (0) "Hard Wired" */
        /* Write Logic (1) to the "Register Select" Pin to select the "Data Register"  */ 
        ret = gpio_pin_write_logic(&(lcd->lcd_rs),GPIO_HIGH);
        
        /* Send the Command through the (8-Pins" Data lines */
        for(pins_counter = 0;pins_counter < 8;pins_counter++)
        {
          ret = gpio_pin_write_logic(&(lcd->lcd_data[pins_counter]), (data >> pins_counter) & (uint8)0x01);
        }
        
        /* Send the Enable Signal on the "E" Pin */
        ret = lcd_8bits_send_enable_signle(lcd);
    }
    return ret;  
    
}

/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @param data
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_8bits_send_char_data_pos(const chr_lcd_8bits_t *lcd ,uint8 row ,uint8 column ,uint8 data)
{
    std_ReturnType ret = E_OK;
    
    if(NULL == lcd)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        ret = lcd_8bit_set_cursor(lcd,row,column);
        ret = lcd_8bits_send_char_data(lcd,data); 
        
    }
    
    return ret;
  
}

/**
 * 
 * @param lcd
 * @param str
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_8bits_send_string(const chr_lcd_8bits_t *lcd ,uint8 *str)
{
    std_ReturnType ret = E_OK;
    
    if((NULL == lcd) || (NULL ==str))
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        while(*str)
        {
            lcd_8bits_send_char_data(lcd,*str++);
        }
    }
    
    return ret;

}

/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @param str
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_8bits_send_string_pos(const chr_lcd_8bits_t *lcd ,uint8 row ,uint8 column ,uint8 *str)
{
    std_ReturnType ret = E_OK;
    
    if((NULL == lcd) || (NULL ==str))
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        ret = lcd_8bit_set_cursor(lcd,row,column);
        while(*str)
        {
            ret = lcd_8bits_send_char_data(lcd,*str++);
        } 
    }
    return ret;
}



/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @param _chr
 * @param mem_pos
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType lcd_8bits_send_custom_char(const chr_lcd_8bits_t *lcd ,uint8 row ,uint8 column,
                                          const uint8 _chr[] ,uint8 mem_pos)

{
    std_ReturnType ret = E_OK;
    uint8 lcd_counter = ZERO_INIT;
    
    if(NULL == lcd)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        ret = lcd_8bits_send_command(lcd,(_LCD_CGRAM_START + (mem_pos*8)));
        for(lcd_counter = ZERO_INIT;lcd_counter<=7;lcd_counter++)
        {
            ret = lcd_8bits_send_char_data(lcd,_chr[lcd_counter]);
        }
        
        ret = lcd_8bits_send_char_data_pos(lcd,row,column,mem_pos);

    }
    
    return ret;
}



/**
 * 
 * @param value
 * @param str
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType convert_uint8_to_string(uint8 value ,uint8 *str)
{
    std_ReturnType ret = E_OK;
    
    if(NULL == str)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        memset((char *)str,'\0',4);
        sprintf(str,"%i",value);
    } 
    return ret;
}

/**
 * 
 * @param value
 * @param str
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType convert_uint16_to_string(uint16 value ,uint8 *str)
{
    std_ReturnType ret = E_OK;
    uint8 Temp_String[6] = {0};
    uint8 DataCounter = 0;
    if(NULL == str)
    {
        ret = E_NOT_OK;
    }
    else
    {    
        memset(str, ' ', 5);
        str[5] = '\0';
        sprintf((char *)Temp_String, "%i", value);
        while(Temp_String[DataCounter] != '\0')
        {
            str[DataCounter] = Temp_String[DataCounter]; 
            DataCounter++;
        }
    }
    return ret;
}



/**
 * 
 * @param value
 * @param str
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType convert_uint32_to_string(uint32 value ,uint8 *str)
{
    std_ReturnType ret = E_OK;
    
    if(NULL == str)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        memset((char *)str,'\0',11);
        sprintf(str,"%i",value);
    }
    return ret; 
} 




/**
 * 
 * @param lcd
 * @param _data_command
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType lcd_send_4bits(const chr_lcd_4bits_t *lcd,uint8 _data_command)
{
    std_ReturnType ret = E_OK;
    
    if(NULL == lcd)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
     ret = gpio_pin_write_logic(&(lcd->lcd_data[0]),(_data_command >> 0) & (uint8)0x01);
     ret = gpio_pin_write_logic(&(lcd->lcd_data[1]),(_data_command >> 1) & (uint8)0x01);
     ret = gpio_pin_write_logic(&(lcd->lcd_data[2]),(_data_command >> 2) & (uint8)0x01);
     ret = gpio_pin_write_logic(&(lcd->lcd_data[3]),(_data_command >> 3) & (uint8)0x01);  
    }
    
    return ret;
}

/**
 * 
 * @param lcd
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType lcd_4bits_send_enable_signle(const chr_lcd_4bits_t *lcd)
{
    std_ReturnType ret = E_OK;
    
    if(NULL == lcd)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
     ret = gpio_pin_write_logic(&(lcd->lcd_en),GPIO_HIGH);
     __delay_us(5);
     ret = gpio_pin_write_logic(&(lcd->lcd_en),GPIO_LOW);
    }
    
    return ret;
}

/**
 * 
 * @param lcd
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType lcd_8bits_send_enable_signle(const chr_lcd_8bits_t *lcd)
{
    std_ReturnType ret = E_OK;
    
    if(NULL == lcd)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
     ret = gpio_pin_write_logic(&(lcd->lcd_en),GPIO_HIGH);
     __delay_us(5);
     ret = gpio_pin_write_logic(&(lcd->lcd_en),GPIO_LOW);
    }
    
    return ret;
}

/**
 * 
 * @param lcd
 * @param row
 * @param coulmn
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bits_t *lcd, uint8 row, uint8 column)
{
    std_ReturnType ret = E_OK;
    column--;
    
    if(NULL == lcd)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        switch(row)
        {
            case ROW1 : ret = lcd_4bits_send_command(lcd,(0x80 + column)); break;
            case ROW2 : ret = lcd_4bits_send_command(lcd,(0xC0 + column)); break;
            case ROW3 : ret = lcd_4bits_send_command(lcd,(0x94 + column)); break;       
            case ROW4 : ret = lcd_4bits_send_command(lcd,(0xD4 + column)); break;
            default : ;
        }
     
    }
    
    return ret;
    
}

/**
 * 
 * @param lcd
 * @param row
 * @param coulmn
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bits_t *lcd, uint8 row, uint8 column)
{
    std_ReturnType ret = E_OK;
    column--;
    
    if(NULL == lcd)
    {
     ret = E_NOT_OK;   
    }
    
    else
    {
        switch(row)
        {
            case ROW1 : ret = lcd_8bits_send_command(lcd,(0x80 + column)); break;
            case ROW2 : ret = lcd_8bits_send_command(lcd,(0xC0 + column)); break;
            case ROW3 : ret = lcd_8bits_send_command(lcd,(0x94 + column)); break;       
            case ROW4 : ret = lcd_8bits_send_command(lcd,(0xD4 + column)); break;
            default : ;
        }
     
    }
    
    return ret;
    
}
