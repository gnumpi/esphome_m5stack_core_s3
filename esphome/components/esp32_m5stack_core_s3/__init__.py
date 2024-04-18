"""M5-Stack Core S3 Component."""
import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv

from esphome.const import CONF_ID

from ..aw9523 import AW9523Component

CODEOWNERS = ["@gnumpi"]
DEPENDENCIES = ["i2c", "aw9523"]

m5stack_ns = cg.esphome_ns.namespace("m5stack")
M5StackCoreS3 = m5stack_ns.class_("M5StackCoreS3", i2c.I2CDevice, cg.Component)

CONF_AW9523 = "aw9523"
CONF_USB_OTG_EN = "usb_otg_en"
CONF_BUS_OUT_EN = "bus_out_en"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(M5StackCoreS3),
        cv.Optional(CONF_AW9523): cv.use_id(AW9523Component),
        cv.Optional(CONF_USB_OTG_EN, default=False): cv.boolean,
        cv.Optional(CONF_BUS_OUT_EN, default=False): cv.boolean,
    }
).extend(i2c.i2c_device_schema(0x36))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    if CONF_AW9523 in config:
        aw9523 = await cg.get_variable(config[CONF_AW9523])
        cg.add(var.set_aw9523(aw9523))
        cg.add(var.set_usb_otg_en(config[CONF_USB_OTG_EN]))
        cg.add(var.set_bus_out_en(config[CONF_BUS_OUT_EN]))
