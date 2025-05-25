import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID

from . import CONF_OGT_BMS_BLE_ID, OgtBmsBle

DEPENDENCIES = ["ogt_bms_ble"]

CODEOWNERS = ["@syssi"]

CONF_ERRORS = "errors"
CONF_TIME_TO_EMPTY_FORMATTED = "time_to_empty_formatted"
CONF_TIME_TO_FULL_FORMATTED = "time_to_full_formatted"
CONF_SERIAL_NUMBER = "serial_number"
CONF_MANUFACTURE_DATE = "manufacture_date"

TEXT_SENSORS = [
    CONF_ERRORS,
    CONF_TIME_TO_EMPTY_FORMATTED,
    CONF_TIME_TO_FULL_FORMATTED,
    CONF_SERIAL_NUMBER,
    CONF_MANUFACTURE_DATE,
]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_OGT_BMS_BLE_ID): cv.use_id(OgtBmsBle),
        cv.Optional(CONF_ERRORS): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon="mdi:alert-circle-outline"
        ),
        cv.Optional(CONF_TIME_TO_EMPTY_FORMATTED): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon="mdi:battery-remove-outline"
        ),
        cv.Optional(CONF_TIME_TO_FULL_FORMATTED): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon="mdi:battery-charging-100"
        ),
        cv.Optional(CONF_SERIAL_NUMBER): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon="mdi:numeric"
        ),
        cv.Optional(CONF_MANUFACTURE_DATE): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon="mdi:factory"
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
