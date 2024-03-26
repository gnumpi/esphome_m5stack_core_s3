"""M5-Stack Core S3 Component."""
import esphome.codegen as cg
from esphome.components import esp32, i2c
import esphome.config_validation as cv

from esphome.const import (
    CONF_ID
)

CODEOWNERS = ["@gnumpi"]
DEPENDENCIES = ["i2c"]

m5stack_ns = cg.esphome_ns.namespace("m5stack")
M5StackCoreS3 = m5stack_ns.class_("M5StackCoreS3",  i2c.I2CDevice, cg.Component )

CONFIG_SCHEMA = (
    cv.Schema({cv.GenerateID(): cv.declare_id(M5StackCoreS3)}).extend(
        i2c.i2c_device_schema(0x36)
    )
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)

