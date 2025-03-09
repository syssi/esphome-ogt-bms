import esphome.codegen as cg
from esphome.components import ble_client
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@syssi"]

AUTO_LOAD = ["binary_sensor", "sensor", "text_sensor"]
MULTI_CONF = True

CONF_OGT_BMS_BLE_ID = "ogt_bms_ble_id"

CONF_BLE_NAME = "ble_name"

ogt_bms_ble_ns = cg.esphome_ns.namespace("ogt_bms_ble")
OgtBmsBle = ogt_bms_ble_ns.class_(
    "OgtBmsBle", ble_client.BLEClientNode, cg.PollingComponent
)

OGT_BMS_BLE_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_OGT_BMS_BLE_ID): cv.use_id(OgtBmsBle),
    }
)


def validate_ble_name(value):
    valid_prefixes = ["SmartBat-A"]
    prefix_valid = False
    current_prefix = ""

    for prefix in valid_prefixes:
        if value.startswith(prefix):
            prefix_valid = True
            current_prefix = prefix
            break

    if not prefix_valid:
        raise cv.Invalid("BLE name must start with SmartBat-A")

    suffix = value[len(current_prefix) :]
    if len(suffix) != 5:
        raise cv.Invalid("BLE name must contain 5 digits")

    if not suffix.isdigit():
        raise cv.Invalid("BLE name must end with 5 digits")

    return value


CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(OgtBmsBle),
            cv.Required(CONF_BLE_NAME): validate_ble_name,
        }
    )
    .extend(ble_client.BLE_CLIENT_SCHEMA)
    .extend(cv.polling_component_schema("10s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)

    device_type = int(config[CONF_BLE_NAME][9], 16)
    secret = [2, 5, 4, 3, 1, 4, 1, 6, 8, 3, 7, 2, 5, 8, 9, 3]
    encryption_key = sum(
        secret[int(c, 16)] for c in (f"{int(config[CONF_BLE_NAME][10:]):0>4X}")
    ) + (5 if (device_type == 0x0A) else 8)

    cg.add(var.set_device_type(device_type))
    cg.add(var.set_encryption_key(encryption_key))
