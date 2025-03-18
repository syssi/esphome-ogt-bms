#include "ogt_bms_ble.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace ogt_bms_ble {

static const char *const TAG = "ogt_bms_ble";

static const uint16_t OGT_BMS_SERVICE_UUID = 0xFFF0;
static const uint16_t OGT_BMS_NOTIFY_CHARACTERISTIC_UUID = 0xFFF4;
static const uint16_t OGT_BMS_CONTROL_CHARACTERISTIC_UUID = 0xFFF6;

static const uint8_t MAX_RESPONSE_SIZE = 14;

static const uint8_t OGT_COMMAND_STATE_OF_CHARGE = 2;
static const uint8_t OGT_COMMAND_CAPACITY_REMAINING = 4;
static const uint8_t OGT_COMMAND_FULL_CAPACITY = 6;
static const uint8_t OGT_COMMAND_TOTAL_VOLTAGE = 8;
// static const uint8_t OGT_COMMAND_UNKNOWN = 10; // 3 bytes length
static const uint8_t OGT_COMMAND_TEMPERATURE = 12;
static const uint8_t OGT_COMMAND_CURRENT = 16;
static const uint8_t OGT_COMMAND_TIME_TO_EMPTY = 24;
static const uint8_t OGT_COMMAND_TIME_TO_FULL = 26;
static const uint8_t OGT_COMMAND_SERIAL_NUMBER = 40;
static const uint8_t OGT_COMMAND_CYCLES = 44;
static const uint8_t OGT_COMMAND_DESIGN_CAPACITY = 60;
static const uint8_t OGT_COMMAND_MANUFACTURE_DATE = 72;

using ogt_bms_command_t = struct {
  uint8_t command;
  uint8_t length;
};

static const uint8_t OGT_TYPE_A_COMMAND_QUEUE_SIZE = 10;
static const ogt_bms_command_t OGT_TYPE_A_COMMAND_QUEUE[OGT_TYPE_A_COMMAND_QUEUE_SIZE] = {
    {OGT_COMMAND_STATE_OF_CHARGE, 1},
    {OGT_COMMAND_CAPACITY_REMAINING, 3},
    {OGT_COMMAND_FULL_CAPACITY, 3},
    {OGT_COMMAND_TOTAL_VOLTAGE, 2},
    {OGT_COMMAND_TEMPERATURE, 2},
    {OGT_COMMAND_CURRENT, 3},
    {OGT_COMMAND_TIME_TO_EMPTY, 2},
    {OGT_COMMAND_TIME_TO_FULL, 2},
    // {OGT_COMMAND_SERIAL_NUMBER, 2},
    {OGT_COMMAND_CYCLES, 2},
    {OGT_COMMAND_DESIGN_CAPACITY, 3},
    // {OGT_COMMAND_MANUFACTURE_DATE, 2},
};

uint8_t ascii_hex_to_byte(char a, char b) {
  a = (a <= '9') ? a - '0' : (a & 0x7) + 9;
  b = (b <= '9') ? b - '0' : (b & 0x7) + 9;

  return (a << 4) + b;
}

void OgtBmsBle::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                    esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_OPEN_EVT: {
      break;
    }
    case ESP_GATTC_DISCONNECT_EVT: {
      this->node_state = espbt::ClientState::IDLE;

      this->publish_state_(this->state_of_charge_sensor_, NAN);
      this->publish_state_(this->capacity_remaining_sensor_, NAN);
      this->publish_state_(this->design_capacity_sensor_, NAN);
      this->publish_state_(this->full_charge_capacity_sensor_, NAN);
      this->publish_state_(this->total_voltage_sensor_, NAN);
      this->publish_state_(this->mosfet_temperature_sensor_, NAN);
      this->publish_state_(this->current_sensor_, NAN);
      this->publish_state_(this->charging_binary_sensor_, NAN);
      this->publish_state_(this->discharging_binary_sensor_, NAN);
      this->publish_state_(this->power_sensor_, NAN);
      this->publish_state_(this->charging_power_sensor_, NAN);
      this->publish_state_(this->discharging_power_sensor_, NAN);
      this->publish_state_(this->runtime_remaining_sensor_, NAN);
      this->publish_state_(this->runtime_remaining_text_sensor_, "");
      this->publish_state_(this->charging_cycles_sensor_, NAN);

      this->publish_state_(this->min_cell_voltage_sensor_, NAN);
      this->publish_state_(this->max_cell_voltage_sensor_, NAN);
      this->publish_state_(this->min_voltage_cell_sensor_, NAN);
      this->publish_state_(this->max_voltage_cell_sensor_, NAN);
      this->publish_state_(this->average_cell_voltage_sensor_, NAN);
      this->publish_state_(this->delta_cell_voltage_sensor_, NAN);
      for (auto &cell : this->cells_) {
        this->publish_state_(cell.cell_voltage_sensor_, NAN);
      }

      if (this->char_notify_handle_ != 0) {
        auto status = esp_ble_gattc_unregister_for_notify(this->parent()->get_gattc_if(),
                                                          this->parent()->get_remote_bda(), this->char_notify_handle_);
        if (status) {
          ESP_LOGW(TAG, "esp_ble_gattc_unregister_for_notify failed, status=%d", status);
        }
      }
      this->char_notify_handle_ = 0;
      this->char_command_handle_ = 0;

      break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      // [esp32_ble_client:191]: [0] [B4:52:A9:09:91:42] ESP_GATTC_SEARCH_CMPL_EVT
      // [esp32_ble_client:332]: [0] [B4:52:A9:09:91:42] Service UUID: 0x1800
      // [esp32_ble_client:334]: [0] [B4:52:A9:09:91:42]  start_handle: 0x1  end_handle: 0xb
      // [esp32_ble_client:332]: [0] [B4:52:A9:09:91:42] Service UUID: 0x1801
      // [esp32_ble_client:334]: [0] [B4:52:A9:09:91:42]  start_handle: 0xc  end_handle: 0xf
      // [esp32_ble_client:332]: [0] [B4:52:A9:09:91:42] Service UUID: 0x180A
      // [esp32_ble_client:334]: [0] [B4:52:A9:09:91:42]  start_handle: 0x10  end_handle: 0x22
      // [esp32_ble_client:332]: [0] [B4:52:A9:09:91:42] Service UUID: 0xFFF0
      // [esp32_ble_client:334]: [0] [B4:52:A9:09:91:42]  start_handle: 0x23  end_handle: 0xffff
      // [esp32_ble_client:337]: [0] [B4:52:A9:09:91:42] Connected
      // [esp32_ble_client:067]: [0] [B4:52:A9:09:91:42]  characteristic 0xFFF1, handle 0x25, properties 0xa
      // [esp32_ble_client:067]: [0] [B4:52:A9:09:91:42]  characteristic 0xFFF2, handle 0x28, properties 0x2
      // [esp32_ble_client:067]: [0] [B4:52:A9:09:91:42]  characteristic 0xFFF3, handle 0x2b, properties 0x8
      // [esp32_ble_client:067]: [0] [B4:52:A9:09:91:42]  characteristic 0xFFF4, handle 0x2e, properties 0x16
      // [esp32_ble_client:067]: [0] [B4:52:A9:09:91:42]  characteristic 0xFFF5, handle 0x32, properties 0x2
      // [esp32_ble_client:067]: [0] [B4:52:A9:09:91:42]  characteristic 0xFFF6, handle 0x35, properties 0x16
      // [esp32_ble_client:296]: [0] [B4:52:A9:09:91:42] cfg_mtu status 0, mtu 23

      auto *char_notify = this->parent_->get_characteristic(OGT_BMS_SERVICE_UUID, OGT_BMS_NOTIFY_CHARACTERISTIC_UUID);
      if (char_notify == nullptr) {
        ESP_LOGE(TAG, "[%s] No notify service found at device, not an Offgridtec BMS..?",
                 this->parent_->address_str().c_str());
        break;
      }
      this->char_notify_handle_ = char_notify->handle;

      auto status = esp_ble_gattc_register_for_notify(this->parent()->get_gattc_if(), this->parent()->get_remote_bda(),
                                                      char_notify->handle);
      if (status) {
        ESP_LOGW(TAG, "esp_ble_gattc_register_for_notify failed, status=%d", status);
      }

      auto *char_command = this->parent_->get_characteristic(OGT_BMS_SERVICE_UUID, OGT_BMS_CONTROL_CHARACTERISTIC_UUID);
      if (char_command == nullptr) {
        ESP_LOGE(TAG, "[%s] No control service found at device, not an Offgridtec BMS..?",
                 this->parent_->address_str().c_str());
        break;
      }
      this->char_command_handle_ = char_command->handle;
      break;
    }
    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      this->node_state = espbt::ClientState::ESTABLISHED;

      this->send_command(OGT_COMMAND_TOTAL_VOLTAGE, 2);
      break;
    }
    case ESP_GATTC_NOTIFY_EVT: {
      ESP_LOGV(TAG, "Notification received (handle 0x%02X): %s", param->notify.handle,
               format_hex_pretty(param->notify.value, param->notify.value_len).c_str());

      std::vector<uint8_t> data(param->notify.value, param->notify.value + param->notify.value_len);

      this->on_ogt_bms_ble_data(data);
      break;
    }
    default:
      break;
  }
}

void OgtBmsBle::update() {
  if (this->node_state != espbt::ClientState::ESTABLISHED) {
    ESP_LOGW(TAG, "[%s] Not connected", this->parent_->address_str().c_str());
    return;
  }

  // Loop through all commands if connected
  if (this->next_command_ != OGT_TYPE_A_COMMAND_QUEUE_SIZE) {
    ESP_LOGW(TAG,
             "Command queue (%d of %d) was not completely processed. "
             "Please increase the update_interval if you see this warning frequently",
             this->next_command_ + 1, OGT_TYPE_A_COMMAND_QUEUE_SIZE);
  }
  this->next_command_ = 0;

  const auto &command = OGT_TYPE_A_COMMAND_QUEUE[this->next_command_++ % OGT_TYPE_A_COMMAND_QUEUE_SIZE];
  this->send_command(command.command, command.length);
}

std::string OgtBmsBle::decrypt_response_(const std::vector<uint8_t> &data) {
  std::string result(data.size(), 0);
  for (size_t i = 0; i < data.size(); i++) {
    result[i] = data[i] ^ this->encryption_key_;
  }
  ESP_LOGVV(TAG, "Decrypted response: %s", result.c_str());

  return result;
}

std::vector<uint8_t> OgtBmsBle::extract_hex_values_(const std::string &msg) {
  uint8_t preamble_length = 4;
  if (msg.size() < preamble_length || !std::equal(msg.begin(), msg.begin() + preamble_length, "+RD,")) {
    return {};
  }

  uint8_t ascii_data_length = msg.size() - preamble_length - 2;

  std::vector<uint8_t> data;
  data.reserve(ascii_data_length / 2);

  for (size_t i = 0; i + 1 < ascii_data_length; i += 2) {
    data.push_back(ascii_hex_to_byte(msg[i + preamble_length], msg[i + 1 + preamble_length]));
  }

  ESP_LOGVV(TAG, "Raw data: %s", format_hex_pretty(&data.front(), data.size()).c_str());

  return data;
}

void OgtBmsBle::on_ogt_bms_ble_data(const std::vector<uint8_t> &encrypted_data) {
  if (encrypted_data.size() > MAX_RESPONSE_SIZE || encrypted_data.size() % 2 != 0) {
    ESP_LOGW(TAG, "Invalid response received: %s",
             format_hex_pretty(&encrypted_data.front(), encrypted_data.size()).c_str());
    return;
  }

  std::vector<uint8_t> data = this->extract_hex_values_(this->decrypt_response_(encrypted_data));

  auto ogt_get_16bit = [&](size_t i) -> uint16_t {
    return (uint16_t(data[i + 1]) << 8) | (uint16_t(data[i + 0]) << 0);
  };

  uint8_t command = data[0];
  float current = 0.0f;
  switch (command) {
    case OGT_COMMAND_STATE_OF_CHARGE:
      this->publish_state_(this->state_of_charge_sensor_, data[1] * 1.0f);
      break;
    case OGT_COMMAND_CAPACITY_REMAINING:
      this->publish_state_(this->capacity_remaining_sensor_, ogt_get_16bit(1) * data[3] * 0.001f);
      break;
    case OGT_COMMAND_DESIGN_CAPACITY:
      this->publish_state_(this->design_capacity_sensor_, ogt_get_16bit(1) * data[3] * 0.001f);
      break;
    case OGT_COMMAND_FULL_CAPACITY:
      this->publish_state_(this->full_charge_capacity_sensor_, ogt_get_16bit(1) * data[3] * 0.001f);
      break;
    case OGT_COMMAND_TOTAL_VOLTAGE:
      this->publish_state_(this->total_voltage_sensor_, ogt_get_16bit(1) * 0.001f);
      break;
    case OGT_COMMAND_TEMPERATURE:
      this->publish_state_(this->mosfet_temperature_sensor_, (ogt_get_16bit(1) * 0.1f) - 273.15f);
      break;
    case OGT_COMMAND_CURRENT:
      current = ((int16_t) ogt_get_16bit(1)) * 0.01f;  // ignore byte 3?
      this->publish_state_(this->current_sensor_, current);
      this->publish_state_(this->charging_binary_sensor_, current > 0.0f);
      this->publish_state_(this->discharging_binary_sensor_, current <= 0.0f);

      if (!std::isnan(this->total_voltage_sensor_->state)) {
        float voltage = this->total_voltage_sensor_->state;
        float power = voltage * current;
        this->publish_state_(this->power_sensor_, power);
        this->publish_state_(this->charging_power_sensor_, std::max(0.0f, power));               // 500W vs 0W -> 500W
        this->publish_state_(this->discharging_power_sensor_, std::abs(std::min(0.0f, power)));  // -500W vs 0W -> 500W
      }
      break;
    case OGT_COMMAND_TIME_TO_EMPTY:
      this->publish_state_(this->runtime_remaining_sensor_, ogt_get_16bit(1) * 60.0f);
      this->publish_state_(this->runtime_remaining_text_sensor_, format_runtime_remaining_(ogt_get_16bit(1) * 60.0f));
      break;
    case OGT_COMMAND_TIME_TO_FULL:
      ESP_LOGI(TAG, "Time to full: %d s", ogt_get_16bit(1) * 60);
      break;
    case OGT_COMMAND_SERIAL_NUMBER:
      ESP_LOGI(TAG, "Serial number: %05d", ogt_get_16bit(1));
      break;
    case OGT_COMMAND_CYCLES:
      this->publish_state_(this->charging_cycles_sensor_, ogt_get_16bit(1) * 1.0f);
      break;
    case OGT_COMMAND_MANUFACTURE_DATE:
      ESP_LOGI(TAG, "Date of manufacture: %d.%02d.%02d", 1980 + (ogt_get_16bit(1) >> 9), (ogt_get_16bit(1) >> 5) & 0x0f,
               ogt_get_16bit(1) & 0x1f);
      break;
    default:
      ESP_LOGW(TAG, "Unhandled response received (command %02d): %s", command,
               format_hex_pretty(&data.front(), data.size()).c_str());
  }

  // Send next command after each received frame
  if (this->next_command_ < OGT_TYPE_A_COMMAND_QUEUE_SIZE) {
    const auto &command = OGT_TYPE_A_COMMAND_QUEUE[this->next_command_++ % OGT_TYPE_A_COMMAND_QUEUE_SIZE];
    this->send_command(command.command, command.length);
  }
}

void OgtBmsBle::decode_status_data_(const std::vector<uint8_t> &data) {}

void OgtBmsBle::dump_config() {  // NOLINT(google-readability-function-size,readability-function-size)
  ESP_LOGCONFIG(TAG, "OgtBmsBle:");

  LOG_BINARY_SENSOR("", "Charging", this->charging_binary_sensor_);
  LOG_BINARY_SENSOR("", "Discharging", this->discharging_binary_sensor_);

  LOG_SENSOR("", "Total voltage", this->total_voltage_sensor_);
  LOG_SENSOR("", "Current", this->current_sensor_);
  LOG_SENSOR("", "Power", this->power_sensor_);
  LOG_SENSOR("", "Charging power", this->charging_power_sensor_);
  LOG_SENSOR("", "Discharging power", this->discharging_power_sensor_);
  LOG_SENSOR("", "Error bitmask", this->error_bitmask_sensor_);
  LOG_SENSOR("", "State of charge", this->state_of_charge_sensor_);
  LOG_SENSOR("", "Charging cycles", this->charging_cycles_sensor_);
  LOG_SENSOR("", "Capacity remaining", this->capacity_remaining_sensor_);
  LOG_SENSOR("", "Design capacity", this->design_capacity_sensor_);
  LOG_SENSOR("", "Full charge capacity", this->full_charge_capacity_sensor_);
  LOG_SENSOR("", "Mosfet temperature", this->mosfet_temperature_sensor_);

  LOG_SENSOR("", "Min cell voltage", this->min_cell_voltage_sensor_);
  LOG_SENSOR("", "Max cell voltage", this->max_cell_voltage_sensor_);
  LOG_SENSOR("", "Min voltage cell", this->min_voltage_cell_sensor_);
  LOG_SENSOR("", "Max voltage cell", this->max_voltage_cell_sensor_);
  LOG_SENSOR("", "Average cell voltage", this->average_cell_voltage_sensor_);
  LOG_SENSOR("", "Delta cell voltage", this->delta_cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 1", this->cells_[0].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 2", this->cells_[1].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 3", this->cells_[2].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 4", this->cells_[3].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 5", this->cells_[4].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 6", this->cells_[5].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 7", this->cells_[6].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 8", this->cells_[7].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 9", this->cells_[8].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 10", this->cells_[9].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 11", this->cells_[10].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 12", this->cells_[11].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 13", this->cells_[12].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 14", this->cells_[13].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 15", this->cells_[14].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 16", this->cells_[15].cell_voltage_sensor_);

  LOG_TEXT_SENSOR("", "Errors", this->errors_text_sensor_);
}

void OgtBmsBle::publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state) {
  if (binary_sensor == nullptr)
    return;

  binary_sensor->publish_state(state);
}

void OgtBmsBle::publish_state_(sensor::Sensor *sensor, float value) {
  if (sensor == nullptr)
    return;

  sensor->publish_state(value);
}

void OgtBmsBle::publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state) {
  if (text_sensor == nullptr)
    return;

  text_sensor->publish_state(state);
}

bool OgtBmsBle::send_command(uint8_t command, uint8_t length) {
  const char *sof_type_a = "+RAA";

  uint8_t frame[8];
  for (size_t i = 0; i < 4; i++) {
    frame[i] = sof_type_a[i];
  }
  frame[4] = (command / 16) < 10 ? '0' + (command / 16) : 'A' + (command / 16) - 10;
  frame[5] = (command % 16) < 10 ? '0' + (command % 16) : 'A' + (command % 16) - 10;
  frame[6] = (length / 16) < 10 ? '0' + (length / 16) : 'A' + (length / 16) - 10;
  frame[7] = (length % 16) < 10 ? '0' + (length % 16) : 'A' + (length % 16) - 10;

  ESP_LOGVV(TAG, "Plaintext command payload (handle 0x%02X): %s", this->char_command_handle_,
            format_hex_pretty(frame, sizeof(frame)).c_str());

  // Encrypt
  for (unsigned char &i : frame) {
    i = i ^ this->encryption_key_;
  }

  ESP_LOGD(TAG, "Send encrypted command (handle 0x%02X): %s", this->char_command_handle_,
           format_hex_pretty(frame, sizeof(frame)).c_str());

  auto status =
      esp_ble_gattc_write_char(this->parent_->get_gattc_if(), this->parent_->get_conn_id(), this->char_command_handle_,
                               sizeof(frame), frame, ESP_GATT_WRITE_TYPE_NO_RSP, ESP_GATT_AUTH_REQ_NONE);

  if (status) {
    ESP_LOGW(TAG, "[%s] esp_ble_gattc_write_char failed, status=%d", this->parent_->address_str().c_str(), status);
  }

  return (status == 0);
}

}  // namespace ogt_bms_ble
}  // namespace esphome
