#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"
#include "esphome/core/gpio.h"

namespace esphome {
namespace aw9523 {

enum class AW9523Port : uint8_t { PORT_0 = 0, PORT_1 };
enum class AW9523PinMode : uint8_t { GPIO_IN = 0, GPIO_OUT, LED };

class AW9523Component : public i2c::I2CDevice, public Component {
public:
    void setup() override;
    void dump_config() override;

    void set_pin_mode(AW9523Port port, uint8_t pin, AW9523PinMode mode);
    void set_pin_value(AW9523Port port, uint8_t pin, bool bit_value);
    bool read_pin_value(AW9523Port port, uint8_t pin);
    void write_gcr_register(){}
};


class AW9523GPIOPin : public GPIOPin
{
public:
    void setup() override {};
    void pin_mode(gpio::Flags flags) override;
    bool digital_read() override;
    void digital_write(bool value) override;

    void set_parent(AW9523Component *parent) { this->parent_ = parent; }
    void set_pin(AW9523Port port, uint8_t pin) { this->port_ = port; this->pin_ = pin; }
    void set_inverted(bool inverted) { this->inverted_ = inverted; }
    void set_flags(gpio::Flags flags) { this->flags_ = flags; }

    std::string dump_summary() const override {return ""; };

protected:
    AW9523Component *parent_;
    AW9523Port port_;
    uint8_t pin_;
    bool inverted_;
    gpio::Flags flags_;
};


} // namespace
} // namespace esphome
