#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

#include "../aw9523/aw9523.h"
namespace esphome {
namespace m5stack {

class AXP2101Component;

class M5StackCoreS3 : public i2c::I2CDevice, public Component {
public:
    void setup() override;
    void dump_config() override {};
    float get_setup_priority() const override { return setup_priority::DATA; };

    void set_axp2101( AXP2101Component* axp2101 ){ this->axp2101_ = axp2101; }
    void set_aw9523( aw9523::AW9523Component* aw9523 ){ this->aw9523_ = aw9523; }

    void set_GPIO35_function(bool flag);

    void write_usb_otg_en(bool flag);
    void write_bus_out_en(bool flag);

    void set_usb_otg_en(bool flag){this->usb_otg_en_ = flag;}
    void set_bus_out_en(bool flag){this->bus_out_en_ = flag;}

protected:
    AXP2101Component* axp2101_{nullptr};
    aw9523::AW9523Component* aw9523_{nullptr};

    bool usb_otg_en_{false};
    bool bus_out_en_{false};
};


} // namespace
} // namespace esphome
