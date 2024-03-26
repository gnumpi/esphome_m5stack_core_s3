"""AW9523 Component."""
import esphome.codegen as cg
from esphome.components import esp32, i2c
import esphome.config_validation as cv
from esphome import pins

from esphome.const import (
    CONF_ID,
    CONF_MODE,
    CONF_PIN,
    CONF_PORT,
    CONF_OUTPUT,
    CONF_INPUT,
    CONF_INVERTED
)

CONF_AW9523 = "aw9523"


CODEOWNERS = ["@gnumpi"]
DEPENDENCIES = ["i2c"]

aw9523_ns = cg.esphome_ns.namespace("aw9523")
AW9523Component = aw9523_ns.class_("AW9523Component",  i2c.I2CDevice, cg.Component )
AW9523GPIOPin = aw9523_ns.class_("AW9523GPIOPin", cg.GPIOPin)

AW9523Port = aw9523_ns.enum("AW9523Port", is_class=True)
AW9523_PORT = {
    0: AW9523Port.PORT_0,
    1: AW9523Port.PORT_1,
}



CONFIG_SCHEMA = (
    cv.Schema({cv.GenerateID(): cv.declare_id(AW9523Component)}).extend(
        i2c.i2c_device_schema(0x36)
    )
)

def validate_mode(value):
    if not (value[CONF_INPUT] or value[CONF_OUTPUT]):
        raise cv.Invalid("Mode must be either input or output")
    if value[CONF_INPUT] and value[CONF_OUTPUT]:
        raise cv.Invalid("Mode must be either input or output")
    return value


PIN_SCHEMA = cv.All(
    {
        cv.GenerateID(): cv.declare_id(AW9523GPIOPin),
        cv.Required(CONF_AW9523): cv.use_id(AW9523Component),
        cv.Required(CONF_PORT) : cv.enum(AW9523_PORT),
        cv.Required(CONF_PIN): cv.int_range(min=0, max=7),
        cv.Optional(CONF_MODE, default=CONF_OUTPUT): cv.All(
            {
                cv.Optional(CONF_INPUT, default=False): cv.boolean,
                cv.Optional(CONF_OUTPUT, default=False): cv.boolean,
            },
            validate_mode,
        ),
        cv.Optional(CONF_INVERTED, default=False): cv.boolean,
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    return var


@pins.PIN_SCHEMA_REGISTRY.register(CONF_AW9523, PIN_SCHEMA)
async def aw9523_pin_to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    parent = await cg.get_variable(config[CONF_AW9523])
    cg.add(var.set_parent(parent))

    cg.add(var.set_pin(config[CONF_PORT], config[CONF_PIN]))
    cg.add(var.set_inverted(config[CONF_INVERTED]))
    cg.add(var.set_flags(pins.gpio_flags_expr(config[CONF_MODE])))

    return var