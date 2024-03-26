#include "aw9523.h"

#include "esphome/core/log.h"

namespace esphome {
namespace aw9523 {

static const char *TAG = "aw9523";

const uint8_t AW9523_REG_INPUT_P0  = 0x00; //RO, states of GPIO input pins 
const uint8_t AW9523_REG_INPUT_P1  = 0x01; 
const uint8_t AW9523_REG_OUTPUT_P0 = 0x02; //
const uint8_t AW9523_REG_OUTPUT_P1 = 0x03;

const uint8_t AW9523_REG_CFG_P0 = 0x04;  //GPIO direction 0: output (default) 1: input
const uint8_t AW9523_REG_CFG_P1 = 0x05;
const uint8_t AW9523_REG_INT_ENBL_P0 = 0x06;
const uint8_t AW9523_REG_INT_ENBL_P1 = 0x07;

const uint8_t AW9523_REG_CHIPID = 0x10;
const uint8_t AW9523_REG_GCR = 0x11;  //max current IMAX (global control register)

const uint8_t AW9523_REG_LED_P0 = 0x12; //0: LED 1: GPIO
const uint8_t AW9523_REG_LED_P1 = 0x13;
const uint8_t AW9523_REG_SOFTRESET = 0x7F;

void AW9523Component::setup(){
    uint8_t chip_id = this->reg(AW9523_REG_CHIPID).get();
    if( chip_id != 0x23 ){
        esph_log_e(TAG, "Can't find AW9523 chip.");
        this->mark_failed();
    }

    //set all pins to GPIO mode
    this->reg(AW9523_REG_LED_P0) = 0xff;
    this->reg(AW9523_REG_LED_P1) = 0xff;
}   

void AW9523Component::dump_config(){
    esph_log_config(TAG, "Pin Settings:");
    esph_log_config(TAG, "    Port 0:");
    uint8_t led_reg = this->reg(AW9523_REG_LED_P0).get();
    uint8_t gpio_reg = this->reg(AW9523_REG_CFG_P0).get();
    for( int pin = 0; pin < 8; pin++ )
    {
        const char* io = (gpio_reg & (1 << pin)) ? "I" : "O";
        esph_log_config(TAG, "      Pin %d: %s", pin, (led_reg & (1 << pin)) ? io : "L" );
    }
    esph_log_config(TAG, "    Port 1:");
    led_reg = this->reg(AW9523_REG_LED_P1).get();
    gpio_reg = this->reg(AW9523_REG_CFG_P1).get();
    for( int pin = 0; pin < 8; pin++ )
    {
        const char* io = (gpio_reg & (1 << pin)) ? "I" : "O";
        esph_log_config(TAG, "      Pin %d: %s", pin, (led_reg & (1 << pin)) ? io : "L" );
    }
    
}

void AW9523Component::set_pin_mode(AW9523Port port, uint8_t pin, AW9523PinMode mode){
    if( pin > 7) 
        return;
    
    if( mode == AW9523PinMode::GPIO_OUT){
        const uint8_t reg = port == AW9523Port::PORT_0 ? AW9523_REG_CFG_P0 : AW9523_REG_CFG_P1;
        this->reg(reg) &= ~(1 << pin);
    }
    else if( mode == AW9523PinMode::GPIO_IN){
        const uint8_t reg = port == AW9523Port::PORT_0 ? AW9523_REG_CFG_P0 : AW9523_REG_CFG_P1;
        this->reg(reg) |= (1 << pin);
    }
    else{
         const uint8_t reg = port == AW9523Port::PORT_0 ? AW9523_REG_LED_P0 : AW9523_REG_LED_P1;
         this->reg(reg) |= (1 << pin);
    }
}



void software_reset(){
  // AW9523_REG_SOFTRESET
  // Write 00H to generate a reset pulse
}

 void AW9523Component::set_pin_value(AW9523Port port, uint8_t pin, bool bit_value)
{
  if (this->is_failed())
    return;

  uint8_t value = (1 << pin);
  uint8_t port_reg = port == AW9523Port::PORT_0 ? AW9523_REG_OUTPUT_P0 : AW9523_REG_OUTPUT_P1; 
  if (bit_value){
    this->reg(port_reg) |= value;
  }
  else{
    this->reg(port_reg) &= ~value;
  }
}

bool AW9523Component::read_pin_value(AW9523Port port, uint8_t pin)
{
  if (this->is_failed())
    return false;

  uint8_t bit_mask = (1 << pin);
  uint8_t port_reg = port == AW9523Port::PORT_0 ? AW9523_REG_OUTPUT_P0 : AW9523_REG_OUTPUT_P1; 
  return (this->reg(port_reg).get() & bit_mask) > 0;
}




void AW9523GPIOPin::pin_mode(gpio::Flags flags){
  AW9523PinMode mode = AW9523PinMode::GPIO_IN;
  if( flags == gpio::Flags::FLAG_OUTPUT){
    mode = AW9523PinMode::GPIO_IN;
  } 
  else if( flags == gpio::Flags::FLAG_OUTPUT){
    mode = AW9523PinMode::GPIO_OUT;
  } 
  this->parent_->set_pin_mode(this->port_, this->pin_, mode);
}

void AW9523GPIOPin::digital_write(bool value){
  this->parent_->set_pin_value(this->port_, this->pin_, value != this->inverted_);
}

bool AW9523GPIOPin::digital_read(){
  return this->parent_->read_pin_value(this->port_, this->pin_) != this->inverted_;
}



}
}