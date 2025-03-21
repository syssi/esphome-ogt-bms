#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

#ifdef USE_ESP32

#include <esp_gattc_api.h>

namespace esphome {
namespace ogt_bms_ble {

namespace espbt = esphome::esp32_ble_tracker;

class OgtBmsBle : public esphome::ble_client::BLEClientNode, public PollingComponent {
 public:
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  void dump_config() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_charging_binary_sensor(binary_sensor::BinarySensor *charging_binary_sensor) {
    charging_binary_sensor_ = charging_binary_sensor;
  }
  void set_discharging_binary_sensor(binary_sensor::BinarySensor *discharging_binary_sensor) {
    discharging_binary_sensor_ = discharging_binary_sensor;
  }

  void set_total_voltage_sensor(sensor::Sensor *total_voltage_sensor) { total_voltage_sensor_ = total_voltage_sensor; }
  void set_current_sensor(sensor::Sensor *current_sensor) { current_sensor_ = current_sensor; }
  void set_power_sensor(sensor::Sensor *power_sensor) { power_sensor_ = power_sensor; }
  void set_charging_power_sensor(sensor::Sensor *charging_power_sensor) {
    charging_power_sensor_ = charging_power_sensor;
  }
  void set_discharging_power_sensor(sensor::Sensor *discharging_power_sensor) {
    discharging_power_sensor_ = discharging_power_sensor;
  }
  void set_error_bitmask_sensor(sensor::Sensor *error_bitmask_sensor) { error_bitmask_sensor_ = error_bitmask_sensor; }
  void set_state_of_charge_sensor(sensor::Sensor *state_of_charge_sensor) {
    state_of_charge_sensor_ = state_of_charge_sensor;
  }
  void set_charging_cycles_sensor(sensor::Sensor *charging_cycles_sensor) {
    charging_cycles_sensor_ = charging_cycles_sensor;
  }
  void set_mosfet_temperature_sensor(sensor::Sensor *mosfet_temperature_sensor) {
    mosfet_temperature_sensor_ = mosfet_temperature_sensor;
  }
  void set_time_to_empty_sensor(sensor::Sensor *time_to_empty_sensor) { time_to_empty_sensor_ = time_to_empty_sensor; }
  void set_time_to_full_sensor(sensor::Sensor *time_to_full_sensor) { time_to_full_sensor_ = time_to_full_sensor; }
  void set_capacity_remaining_sensor(sensor::Sensor *capacity_remaining_sensor) {
    capacity_remaining_sensor_ = capacity_remaining_sensor;
  }
  void set_design_capacity_sensor(sensor::Sensor *design_capacity_sensor) {
    design_capacity_sensor_ = design_capacity_sensor;
  }
  void set_full_charge_capacity_sensor(sensor::Sensor *full_charge_capacity_sensor) {
    full_charge_capacity_sensor_ = full_charge_capacity_sensor;
  }
  void set_min_cell_voltage_sensor(sensor::Sensor *min_cell_voltage_sensor) {
    min_cell_voltage_sensor_ = min_cell_voltage_sensor;
  }
  void set_max_cell_voltage_sensor(sensor::Sensor *max_cell_voltage_sensor) {
    max_cell_voltage_sensor_ = max_cell_voltage_sensor;
  }
  void set_min_voltage_cell_sensor(sensor::Sensor *min_voltage_cell_sensor) {
    min_voltage_cell_sensor_ = min_voltage_cell_sensor;
  }
  void set_max_voltage_cell_sensor(sensor::Sensor *max_voltage_cell_sensor) {
    max_voltage_cell_sensor_ = max_voltage_cell_sensor;
  }
  void set_delta_cell_voltage_sensor(sensor::Sensor *delta_cell_voltage_sensor) {
    delta_cell_voltage_sensor_ = delta_cell_voltage_sensor;
  }
  void set_average_cell_voltage_sensor(sensor::Sensor *average_cell_voltage_sensor) {
    average_cell_voltage_sensor_ = average_cell_voltage_sensor;
  }
  void set_cell_voltage_sensor(uint8_t cell, sensor::Sensor *cell_voltage_sensor) {
    this->cells_[cell].cell_voltage_sensor_ = cell_voltage_sensor;
  }

  void set_errors_text_sensor(text_sensor::TextSensor *errors_text_sensor) { errors_text_sensor_ = errors_text_sensor; }
  void set_time_to_empty_formatted_text_sensor(text_sensor::TextSensor *time_to_empty_formatted_text_sensor) {
    time_to_empty_formatted_text_sensor_ = time_to_empty_formatted_text_sensor;
  }
  void set_time_to_full_formatted_text_sensor(text_sensor::TextSensor *time_to_full_formatted_text_sensor) {
    time_to_full_formatted_text_sensor_ = time_to_full_formatted_text_sensor;
  }

  void on_ogt_bms_ble_data(const std::vector<uint8_t> &encrypted_data);
  bool send_command(uint8_t command, uint8_t length);
  void set_encryption_key(uint8_t encryption_key) { this->encryption_key_ = encryption_key; }
  void set_device_type(uint8_t device_type) { this->device_type_ = device_type; }

 protected:
  binary_sensor::BinarySensor *charging_binary_sensor_;
  binary_sensor::BinarySensor *discharging_binary_sensor_;

  sensor::Sensor *total_voltage_sensor_;
  sensor::Sensor *current_sensor_;
  sensor::Sensor *power_sensor_;
  sensor::Sensor *charging_power_sensor_;
  sensor::Sensor *discharging_power_sensor_;
  sensor::Sensor *error_bitmask_sensor_;
  sensor::Sensor *state_of_charge_sensor_;
  sensor::Sensor *charging_cycles_sensor_;
  sensor::Sensor *mosfet_temperature_sensor_;
  sensor::Sensor *time_to_empty_sensor_;
  sensor::Sensor *time_to_full_sensor_;
  sensor::Sensor *average_cell_voltage_sensor_;
  sensor::Sensor *min_cell_voltage_sensor_;
  sensor::Sensor *max_cell_voltage_sensor_;
  sensor::Sensor *min_voltage_cell_sensor_;
  sensor::Sensor *max_voltage_cell_sensor_;
  sensor::Sensor *delta_cell_voltage_sensor_;

  sensor::Sensor *capacity_remaining_sensor_;
  sensor::Sensor *design_capacity_sensor_;
  sensor::Sensor *full_charge_capacity_sensor_;

  text_sensor::TextSensor *errors_text_sensor_;
  text_sensor::TextSensor *time_to_empty_formatted_text_sensor_;
  text_sensor::TextSensor *time_to_full_formatted_text_sensor_;

  struct Cell {
    sensor::Sensor *cell_voltage_sensor_{nullptr};
  } cells_[16];

  uint16_t char_notify_handle_;
  uint16_t char_command_handle_;
  uint8_t next_command_{7};
  uint8_t encryption_key_;
  uint8_t device_type_;

  std::string decrypt_response_(const std::vector<uint8_t> &data);
  std::vector<uint8_t> extract_hex_values_(const std::string &msg);
  void decode_status_data_(const std::vector<uint8_t> &data);
  void publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state);
  void publish_state_(sensor::Sensor *sensor, float value);
  void publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state);

  bool check_bit_(uint16_t mask, uint16_t flag) { return (mask & flag) == flag; }

  std::string format_duration_(const uint32_t value) {
    int seconds = (int) value;
    int years = seconds / (24 * 3600 * 365);
    seconds = seconds % (24 * 3600 * 365);
    int days = seconds / (24 * 3600);
    seconds = seconds % (24 * 3600);
    int hours = seconds / 3600;
    return (years ? to_string(years) + "y " : "") + (days ? to_string(days) + "d " : "") +
           (hours ? to_string(hours) + "h " : "") + (minutes ? to_string(minutes) + "m " : "");
  }
};

}  // namespace ogt_bms_ble
}  // namespace esphome

#endif
