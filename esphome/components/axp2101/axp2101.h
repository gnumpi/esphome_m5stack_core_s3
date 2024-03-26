#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace axp2101 {

class AXP2101Component : public i2c::I2CDevice, public Component {
    void setup() override;
    void dump_config() override;
}


} // namespace
} // namespace esphome


