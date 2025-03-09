import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ICON, CONF_ID

from . import CONF_OGT_BMS_BLE_ID, OgtBmsBle

DEPENDENCIES = ["ogt_bms_ble"]

CODEOWNERS = ["@syssi"]

CONF_ERRORS = "errors"
CONF_RUNTIME_REMAINING = "runtime_remaining"

TEXT_SENSORS = [
    CONF_ERRORS,
    CONF_RUNTIME_REMAINING,
]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_OGT_BMS_BLE_ID): cv.use_id(OgtBmsBle),
        cv.Optional(CONF_ERRORS): text_sensor.TEXT_SENSOR_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(text_sensor.TextSensor),
                cv.Optional(CONF_ICON, default="mdi:alert-circle-outline"): cv.icon,
            }
        ),
        cv.Optional(CONF_RUNTIME_REMAINING): text_sensor.TEXT_SENSOR_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(text_sensor.TextSensor),
                cv.Optional(CONF_ICON, default="mdi:timer-outline"): cv.icon,
            }
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_OGT_BMS_BLE_ID])
    for key in TEXT_SENSORS:
        if key in config:
            conf = config[key]
            sens = cg.new_Pvariable(conf[CONF_ID])
            await text_sensor.register_text_sensor(sens, conf)
            cg.add(getattr(hub, f"set_{key}_text_sensor")(sens))
