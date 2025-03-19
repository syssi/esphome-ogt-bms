import esphome.codegen as cg
from esphome.components import button
import esphome.config_validation as cv
from esphome.const import CONF_ICON, CONF_ID

from .. import CONF_OGT_BMS_BLE_ID, OGT_BMS_BLE_COMPONENT_SCHEMA, ogt_bms_ble_ns

DEPENDENCIES = ["ogt_bms_ble"]

CODEOWNERS = ["@syssi"]

CONF_RETRIEVE_SERIAL_NUMBER = "retrieve_serial_number"
CONF_RETRIEVE_MANUFACTURE_DATE = "retrieve_manufacture_date"

BUTTONS = {
    CONF_RETRIEVE_SERIAL_NUMBER: 40,
    CONF_RETRIEVE_MANUFACTURE_DATE: 72,
}

OgtButton = ogt_bms_ble_ns.class_("OgtButton", button.Button, cg.Component)

CONFIG_SCHEMA = OGT_BMS_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_RETRIEVE_SERIAL_NUMBER): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(OgtButton),
                cv.Optional(CONF_ICON, default="mdi:numeric"): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_RETRIEVE_MANUFACTURE_DATE): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(OgtButton),
                cv.Optional(CONF_ICON, default="mdi:factory"): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_OGT_BMS_BLE_ID])
    for key, address in BUTTONS.items():
        if key in config:
            conf = config[key]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await button.register_button(var, conf)
            cg.add(var.set_parent(hub))
            cg.add(var.set_holding_register(address))
