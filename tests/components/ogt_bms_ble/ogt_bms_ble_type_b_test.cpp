#include <gtest/gtest.h>
#include <cmath>
#include "common_type_b.h"

namespace esphome::ogt_bms_ble::testing {

TEST(OgtBmsBleTypeBTest, TemperatureFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor temperature;
  bms.set_mosfet_temperature_sensor(&temperature);
  bms.on_ogt_bms_ble_data(TYPE_B_TEMPERATURE);
  EXPECT_NEAR(temperature.state, 9.25f, 0.01f);
}

TEST(OgtBmsBleTypeBTest, TotalVoltageFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor voltage;
  bms.set_total_voltage_sensor(&voltage);
  bms.on_ogt_bms_ble_data(TYPE_B_TOTAL_VOLTAGE);
  EXPECT_NEAR(voltage.state, 13.328f, 0.001f);
}

TEST(OgtBmsBleTypeBTest, CurrentFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor voltage, current, power, charging_power, discharging_power;
  binary_sensor::BinarySensor charging, discharging;
  bms.set_total_voltage_sensor(&voltage);
  bms.set_current_sensor(&current);
  bms.set_power_sensor(&power);
  bms.set_charging_power_sensor(&charging_power);
  bms.set_discharging_power_sensor(&discharging_power);
  bms.set_charging_binary_sensor(&charging);
  bms.set_discharging_binary_sensor(&discharging);
  bms.on_ogt_bms_ble_data(TYPE_B_TOTAL_VOLTAGE);
  bms.on_ogt_bms_ble_data(TYPE_B_CURRENT);
  EXPECT_NEAR(current.state, -0.324f, 0.001f);
  EXPECT_NEAR(power.state, -4.318f, 0.001f);  // 13.328V * -0.324A
  EXPECT_NEAR(charging_power.state, 0.0f, 0.001f);
  EXPECT_NEAR(discharging_power.state, 4.318f, 0.001f);
  EXPECT_FALSE(charging.state);
  EXPECT_TRUE(discharging.state);
}

TEST(OgtBmsBleTypeBTest, StateOfChargeFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor soc;
  bms.set_state_of_charge_sensor(&soc);
  bms.on_ogt_bms_ble_data(TYPE_B_STATE_OF_CHARGE);
  EXPECT_FLOAT_EQ(soc.state, 100.0f);
}

TEST(OgtBmsBleTypeBTest, TimeToEmptyFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor time;
  bms.set_time_to_empty_sensor(&time);
  bms.on_ogt_bms_ble_data(TYPE_B_TIME_TO_EMPTY);
  EXPECT_NEAR(time.state, 1078200.0f, 1.0f);  // 17970 min * 60
}

TEST(OgtBmsBleTypeBTest, TimeToFullFrameNotAvailable) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor time;
  time.state = 0.0f;
  bms.set_time_to_full_sensor(&time);
  bms.on_ogt_bms_ble_data(TYPE_B_TIME_TO_FULL);
  EXPECT_TRUE(std::isnan(time.state));  // 0xFFFF → not available
}

TEST(OgtBmsBleTypeBTest, ChargingCyclesFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor cycles;
  bms.set_charging_cycles_sensor(&cycles);
  bms.on_ogt_bms_ble_data(TYPE_B_CYCLES);
  EXPECT_FLOAT_EQ(cycles.state, 20.0f);
}

TEST(OgtBmsBleTypeBTest, CapacityRemainingFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor capacity;
  bms.set_capacity_remaining_sensor(&capacity);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPACITY_REMAINING);
  EXPECT_NEAR(capacity.state, 103.03f, 0.01f);
}

TEST(OgtBmsBleTypeBTest, FullChargeCapacityFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor capacity;
  bms.set_full_charge_capacity_sensor(&capacity);
  bms.on_ogt_bms_ble_data(TYPE_B_FULL_CHARGE_CAPACITY);
  EXPECT_NEAR(capacity.state, 103.76f, 0.01f);
}

TEST(OgtBmsBleTypeBTest, DesignCapacityFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor capacity;
  bms.set_design_capacity_sensor(&capacity);
  bms.on_ogt_bms_ble_data(TYPE_B_DESIGN_CAPACITY);
  EXPECT_NEAR(capacity.state, 100.0f, 0.01f);
}

TEST(OgtBmsBleTypeBTest, CellVoltageRegisterMapping) {
  // Register 0x3F is cell 1, 0x3B is cell 5 and 0x30 is cell 16 (issue #86)
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor cells[16];
  for (uint8_t i = 0; i < 16; i++) {
    cells[i].state = -1.0f;
    bms.set_cell_voltage_sensor(i, &cells[i]);
  }

  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_01);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_02);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_03);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_04);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_05);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_16);

  EXPECT_NEAR(cells[0].state, 3.329f, 0.001f);
  EXPECT_NEAR(cells[1].state, 3.335f, 0.001f);
  EXPECT_NEAR(cells[2].state, 3.333f, 0.001f);
  EXPECT_NEAR(cells[3].state, 3.336f, 0.001f);
  EXPECT_NEAR(cells[4].state, 0.0f, 0.001f);
  EXPECT_FLOAT_EQ(cells[5].state, -1.0f);  // not received
  EXPECT_NEAR(cells[15].state, 0.0f, 0.001f);
}

TEST(OgtBmsBleTypeBTest, CellVoltageStats) {
  // 4S pack: only cells 1-4 are configured, cells 5-16 report 0V and are ignored
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);

  sensor::Sensor cells[4];
  for (uint8_t i = 0; i < 4; i++)
    bms.set_cell_voltage_sensor(i, &cells[i]);

  sensor::Sensor min_voltage, max_voltage, min_cell, max_cell, avg_voltage, delta_voltage;
  bms.set_min_cell_voltage_sensor(&min_voltage);
  bms.set_max_cell_voltage_sensor(&max_voltage);
  bms.set_min_voltage_cell_sensor(&min_cell);
  bms.set_max_voltage_cell_sensor(&max_cell);
  bms.set_average_cell_voltage_sensor(&avg_voltage);
  bms.set_delta_cell_voltage_sensor(&delta_voltage);

  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_01);  // 3.329V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_02);  // 3.335V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_03);  // 3.333V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_04);  // 3.336V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_05);  // 0.000V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_06);  // 0.000V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_16);  // 0.000V — triggers stats update

  EXPECT_NEAR(max_voltage.state, 3.336f, 0.001f);
  EXPECT_NEAR(min_voltage.state, 3.329f, 0.001f);
  EXPECT_FLOAT_EQ(max_cell.state, 4.0f);
  EXPECT_FLOAT_EQ(min_cell.state, 1.0f);
  EXPECT_NEAR(avg_voltage.state, 3.33325f, 0.0001f);
  EXPECT_NEAR(delta_voltage.state, 0.007f, 0.0001f);
}

TEST(OgtBmsBleTypeBTest, NullSensorsDoNotCrash) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  bms.on_ogt_bms_ble_data(TYPE_B_TOTAL_VOLTAGE);
  bms.on_ogt_bms_ble_data(TYPE_B_CURRENT);
  bms.on_ogt_bms_ble_data(TYPE_B_TIME_TO_EMPTY);
  bms.on_ogt_bms_ble_data(TYPE_B_TIME_TO_FULL);
  bms.on_ogt_bms_ble_data(TYPE_B_CYCLES);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_16);  // triggers stats with no sensors registered
}

TEST(OgtBmsBleTypeBTest, InvalidFrameTooShort) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor voltage;
  voltage.state = -1.0f;
  bms.set_total_voltage_sensor(&voltage);
  bms.on_ogt_bms_ble_data(TYPE_B_INVALID_FRAME_TOO_SHORT);
  EXPECT_FLOAT_EQ(voltage.state, -1.0f);
}

TEST(OgtBmsBleTypeBTest, InvalidFrameOddLength) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor voltage;
  voltage.state = -1.0f;
  bms.set_total_voltage_sensor(&voltage);
  bms.on_ogt_bms_ble_data(TYPE_B_INVALID_FRAME_ODD_LENGTH);
  EXPECT_FLOAT_EQ(voltage.state, -1.0f);
}

TEST(OgtBmsBleTypeBTest, FullDataRoundTrip) {
  // Complete update cycle as captured on the real device (4S pack)
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);

  sensor::Sensor temperature, voltage, current, soc, capacity, full_capacity, design_capacity, cycles;
  sensor::Sensor time_empty, time_full;
  sensor::Sensor cells[4];
  sensor::Sensor min_voltage, max_voltage, avg_voltage, delta_voltage;

  bms.set_mosfet_temperature_sensor(&temperature);
  bms.set_total_voltage_sensor(&voltage);
  bms.set_current_sensor(&current);
  bms.set_state_of_charge_sensor(&soc);
  bms.set_capacity_remaining_sensor(&capacity);
  bms.set_full_charge_capacity_sensor(&full_capacity);
  bms.set_design_capacity_sensor(&design_capacity);
  bms.set_charging_cycles_sensor(&cycles);
  bms.set_time_to_empty_sensor(&time_empty);
  bms.set_time_to_full_sensor(&time_full);
  bms.set_min_cell_voltage_sensor(&min_voltage);
  bms.set_max_cell_voltage_sensor(&max_voltage);
  bms.set_average_cell_voltage_sensor(&avg_voltage);
  bms.set_delta_cell_voltage_sensor(&delta_voltage);
  for (uint8_t i = 0; i < 4; i++)
    bms.set_cell_voltage_sensor(i, &cells[i]);

  bms.on_ogt_bms_ble_data(TYPE_B_TEMPERATURE);
  bms.on_ogt_bms_ble_data(TYPE_B_TOTAL_VOLTAGE);
  bms.on_ogt_bms_ble_data(TYPE_B_CURRENT);
  bms.on_ogt_bms_ble_data(TYPE_B_STATE_OF_CHARGE);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPACITY_REMAINING);
  bms.on_ogt_bms_ble_data(TYPE_B_FULL_CHARGE_CAPACITY);
  bms.on_ogt_bms_ble_data(TYPE_B_TIME_TO_EMPTY);
  bms.on_ogt_bms_ble_data(TYPE_B_TIME_TO_FULL);
  bms.on_ogt_bms_ble_data(TYPE_B_CYCLES);
  bms.on_ogt_bms_ble_data(TYPE_B_DESIGN_CAPACITY);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_01);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_02);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_03);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_04);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_05);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_06);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_16);

  EXPECT_NEAR(temperature.state, 9.25f, 0.01f);
  EXPECT_NEAR(voltage.state, 13.328f, 0.001f);
  EXPECT_NEAR(current.state, -0.324f, 0.001f);
  EXPECT_FLOAT_EQ(soc.state, 100.0f);
  EXPECT_NEAR(capacity.state, 103.03f, 0.01f);
  EXPECT_NEAR(full_capacity.state, 103.76f, 0.01f);
  EXPECT_NEAR(design_capacity.state, 100.0f, 0.01f);
  EXPECT_FLOAT_EQ(cycles.state, 20.0f);
  EXPECT_NEAR(time_empty.state, 1078200.0f, 1.0f);
  EXPECT_TRUE(std::isnan(time_full.state));
  EXPECT_NEAR(max_voltage.state, 3.336f, 0.001f);
  EXPECT_NEAR(min_voltage.state, 3.329f, 0.001f);
  EXPECT_NEAR(avg_voltage.state, 3.33325f, 0.0001f);
  EXPECT_NEAR(delta_voltage.state, 0.007f, 0.0001f);
  // Sum of cell voltages matches the reported total voltage
  float cell_sum = cells[0].state + cells[1].state + cells[2].state + cells[3].state;
  EXPECT_NEAR(cell_sum, voltage.state, 0.01f);
}

}  // namespace esphome::ogt_bms_ble::testing
