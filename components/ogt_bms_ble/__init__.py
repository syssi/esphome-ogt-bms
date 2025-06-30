import esphome.codegen as cg
from esphome.components import ble_client
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@syssi"]

AUTO_LOAD = ["binary_sensor", "button", "sensor", "text_sensor"]
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
    valid_prefixes = ["SmartBat-A", "SmartBat-B"]
    prefix_valid = False
    current_prefix = ""

    for prefix in valid_prefixes:
        if value.startswith(prefix):
            prefix_valid = True
            current_prefix = prefix
            break

    if not prefix_valid:
        raise cv.Invalid("BLE name must start with SmartBat-A or SmartBat-B")

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

    ble_name = config[CONF_BLE_NAME]

    # Extract device type from name (SmartBat-A... or SmartBat-B...)
    device_type_char = ble_name[9]  # Character at position 9 ('A' or 'B')
    device_type_ord = ord(device_type_char)  # Convert to ASCII value for C++
    device_type_hex = int(
        device_type_char, 16
    )  # Convert to hex value for encryption (A=10, B=11)

    # Extract device ID (5 digits after SmartBat-X)
    device_id = ble_name[10:]

    crypt_seq = [2, 5, 4, 3, 1, 4, 1, 6, 8, 3, 7, 2, 5, 8, 9, 3]

    # Convert device ID to 4-digit hex string and calculate key
    hex_id = f"{int(device_id):04X}"
    key_sum = sum(crypt_seq[int(c, 16)] for c in hex_id)

    # Add offset: +5 for Type A (0x0A), +8 for Type B (0x0B)
    encryption_key = key_sum + (5 if device_type_hex == 0x0A else 8)

    cg.add(var.set_device_type(device_type_ord))
    cg.add(var.set_encryption_key(encryption_key))
