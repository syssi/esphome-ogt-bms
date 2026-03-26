import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import ENTITY_CATEGORY_DIAGNOSTIC

from . import OGT_BMS_BLE_COMPONENT_SCHEMA

DEPENDENCIES = ["ogt_bms_ble"]

CODEOWNERS = ["@syssi"]

CONF_CHARGING = "charging"
CONF_DISCHARGING = "discharging"

BINARY_SENSORS = [
    CONF_CHARGING,
    CONF_DISCHARGING,
]

CONFIG_SCHEMA = OGT_BMS_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_CHARGING): binary_sensor.binary_sensor_schema(
            icon="mdi:battery-charging",
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_DISCHARGING): binary_sensor.binary_sensor_schema(
            icon="mdi:power-plug",
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)


async def to_code(config):
    from . import CONF_OGT_BMS_BLE_ID

    hub = await cg.get_variable(config[CONF_OGT_BMS_BLE_ID])
    for key in BINARY_SENSORS:
        if key in config:
            conf = config[key]
            sens = await binary_sensor.new_binary_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_binary_sensor")(sens))
