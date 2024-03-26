#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace m5stack {

class AXP2101Component;
class AW9523Component;

class M5StackCoreS3 : public i2c::I2CDevice, public Component {
public:
    void setup() override;
    void dump_config() override {};
    float get_setup_priority() const override { return setup_priority::DATA; };

    void set_axp2101_component( AXP2101Component* axp2101 ){ this->axp2101_ = axp2101; }
    void set_aw9523_component( AW9523Component* aw9523 ){ this->aw9523_ = aw9523; }
    
    void set_GPIO35_function(bool flag);

protected:
    AXP2101Component* axp2101_{};
    AW9523Component* aw9523_{};
};


} // namespace
} // namespace esphome


