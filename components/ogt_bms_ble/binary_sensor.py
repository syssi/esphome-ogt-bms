import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import ENTITY_CATEGORY_DIAGNOSTIC

from . import OGT_BMS_BLE_COMPONENT_SCHEMA

DEPENDENCIES = ["ogt_bms_ble"]

CODEOWNERS = ["@syssi"]

CONF_CHARGING = "charging"
CONF_DISCHARGING = "discharging"

# key: binary_sensor_schema kwargs
BINARY_SENSOR_DEFS = {
    CONF_CHARGING: {
        "icon": "mdi:battery-charging",
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_DISCHARGING: {
        "icon": "mdi:power-plug",
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
}

CONFIG_SCHEMA = OGT_BMS_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(key): binary_sensor.binary_sensor_schema(**kwargs)
        for key, kwargs in BINARY_SENSOR_DEFS.items()
    }
)


async def to_code(config):
    from . import CONF_OGT_BMS_BLE_ID

    hub = await cg.get_variable(config[CONF_OGT_BMS_BLE_ID])
    for key in BINARY_SENSOR_DEFS:
        if key in config:
            conf = config[key]
            sens = await binary_sensor.new_binary_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_binary_sensor")(sens))
