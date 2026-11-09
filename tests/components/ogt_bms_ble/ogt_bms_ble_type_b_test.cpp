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

TEST(OgtBmsBleTypeBTest, TimeToFullFrame) {
  // Synthetic: the real captures were taken while discharging (time to full = 0xFFFF)
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor time;
  text_sensor::TextSensor time_formatted;
  bms.set_time_to_full_sensor(&time);
  bms.set_time_to_full_formatted_text_sensor(&time_formatted);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_TIME_TO_FULL);
  EXPECT_NEAR(time.state, 10800.0f, 1.0f);  // 180 min * 60
  EXPECT_EQ(time_formatted.state, "3h ");
}

TEST(OgtBmsBleTypeBTest, ChargingCurrentFrame) {
  // Synthetic: the real captures only contain discharge currents
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
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_CURRENT_CHARGING);
  EXPECT_NEAR(current.state, 5.0f, 0.001f);
  EXPECT_NEAR(power.state, 66.64f, 0.001f);  // 13.328V * 5.0A
  EXPECT_NEAR(charging_power.state, 66.64f, 0.001f);
  EXPECT_NEAR(discharging_power.state, 0.0f, 0.001f);
  EXPECT_TRUE(charging.state);
  EXPECT_FALSE(discharging.state);
}

TEST(OgtBmsBleTypeBTest, CellVoltageRegisterMapping) {
  // Register 0x3F is cell 1 ... register 0x30 is cell 16 (issue #86).
  // Feed the real frames one by one: each register must update exactly its own
  // cell and leave all cells that have not been received yet untouched.
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor cells[16];
  for (uint8_t i = 0; i < 16; i++) {
    cells[i].state = -1.0f;
    bms.set_cell_voltage_sensor(i, &cells[i]);
  }

  const std::vector<uint8_t> *frames[16] = {
      &TYPE_B_CELL_VOLTAGE_01, &TYPE_B_CELL_VOLTAGE_02, &TYPE_B_CELL_VOLTAGE_03, &TYPE_B_CELL_VOLTAGE_04,
      &TYPE_B_CELL_VOLTAGE_05, &TYPE_B_CELL_VOLTAGE_06, &TYPE_B_CELL_VOLTAGE_07, &TYPE_B_CELL_VOLTAGE_08,
      &TYPE_B_CELL_VOLTAGE_09, &TYPE_B_CELL_VOLTAGE_10, &TYPE_B_CELL_VOLTAGE_11, &TYPE_B_CELL_VOLTAGE_12,
      &TYPE_B_CELL_VOLTAGE_13, &TYPE_B_CELL_VOLTAGE_14, &TYPE_B_CELL_VOLTAGE_15, &TYPE_B_CELL_VOLTAGE_16,
  };
  for (uint8_t i = 0; i < 16; i++) {
    bms.on_ogt_bms_ble_data(*frames[i]);
    EXPECT_NE(cells[i].state, -1.0f) << "cell " << (i + 1) << " not updated";
    for (uint8_t j = i + 1; j < 16; j++) {
      EXPECT_FLOAT_EQ(cells[j].state, -1.0f) << "frame of cell " << (i + 1) << " updated cell " << (j + 1);
    }
  }

  EXPECT_NEAR(cells[0].state, 3.329f, 0.001f);
  EXPECT_NEAR(cells[1].state, 3.335f, 0.001f);
  EXPECT_NEAR(cells[2].state, 3.333f, 0.001f);
  EXPECT_NEAR(cells[3].state, 3.336f, 0.001f);
  for (uint8_t i = 4; i < 16; i++) {
    EXPECT_NEAR(cells[i].state, 0.0f, 0.001f) << "cell " << (i + 1);  // not present on a 4S pack
  }
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

TEST(OgtBmsBleTypeBTest, CellVoltageStats16S) {
  // Synthetic 16S pack: min and max are located beyond the first four cells
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);

  sensor::Sensor cells[16];
  for (uint8_t i = 0; i < 16; i++)
    bms.set_cell_voltage_sensor(i, &cells[i]);

  sensor::Sensor min_voltage, max_voltage, min_cell, max_cell, avg_voltage, delta_voltage;
  bms.set_min_cell_voltage_sensor(&min_voltage);
  bms.set_max_cell_voltage_sensor(&max_voltage);
  bms.set_min_voltage_cell_sensor(&min_cell);
  bms.set_max_voltage_cell_sensor(&max_cell);
  bms.set_average_cell_voltage_sensor(&avg_voltage);
  bms.set_delta_cell_voltage_sensor(&delta_voltage);

  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_01);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_02);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_03);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_04);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_05);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_06);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_07);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_08);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_09);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_10);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_11);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_12);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_13);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_14);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_15);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_16);  // triggers stats update

  EXPECT_NEAR(cells[6].state, 3.290f, 0.001f);
  EXPECT_NEAR(cells[11].state, 3.350f, 0.001f);
  EXPECT_NEAR(max_voltage.state, 3.350f, 0.001f);
  EXPECT_NEAR(min_voltage.state, 3.290f, 0.001f);
  EXPECT_FLOAT_EQ(max_cell.state, 12.0f);
  EXPECT_FLOAT_EQ(min_cell.state, 7.0f);
  EXPECT_NEAR(avg_voltage.state, 3.320f, 0.0001f);
  EXPECT_NEAR(delta_voltage.state, 0.060f, 0.0001f);
}

TEST(OgtBmsBleTypeBTest, NullSensorsDoNotCrash) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  bms.on_ogt_bms_ble_data(TYPE_B_TOTAL_VOLTAGE);
  bms.on_ogt_bms_ble_data(TYPE_B_CURRENT);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_CURRENT_CHARGING);
  bms.on_ogt_bms_ble_data(TYPE_B_TIME_TO_EMPTY);
  bms.on_ogt_bms_ble_data(TYPE_B_TIME_TO_FULL);
  bms.on_ogt_bms_ble_data(TYPE_B_SYNTHETIC_TIME_TO_FULL);
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

TEST(OgtBmsBleTypeBTest, FullDataRoundTripCapture2) {
  // Second complete update cycle of the same device (docs/traffic-type-b.txt)
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);

  sensor::Sensor temperature, voltage, current, power, soc, capacity, full_capacity, design_capacity, cycles;
  sensor::Sensor time_empty, time_full;
  sensor::Sensor cells[4];
  sensor::Sensor min_voltage, max_voltage, min_cell, max_cell, avg_voltage, delta_voltage;
  binary_sensor::BinarySensor charging, discharging;

  bms.set_mosfet_temperature_sensor(&temperature);
  bms.set_total_voltage_sensor(&voltage);
  bms.set_current_sensor(&current);
  bms.set_power_sensor(&power);
  bms.set_charging_binary_sensor(&charging);
  bms.set_discharging_binary_sensor(&discharging);
  bms.set_state_of_charge_sensor(&soc);
  bms.set_capacity_remaining_sensor(&capacity);
  bms.set_full_charge_capacity_sensor(&full_capacity);
  bms.set_design_capacity_sensor(&design_capacity);
  bms.set_charging_cycles_sensor(&cycles);
  bms.set_time_to_empty_sensor(&time_empty);
  bms.set_time_to_full_sensor(&time_full);
  bms.set_min_cell_voltage_sensor(&min_voltage);
  bms.set_max_cell_voltage_sensor(&max_voltage);
  bms.set_min_voltage_cell_sensor(&min_cell);
  bms.set_max_voltage_cell_sensor(&max_cell);
  bms.set_average_cell_voltage_sensor(&avg_voltage);
  bms.set_delta_cell_voltage_sensor(&delta_voltage);
  for (uint8_t i = 0; i < 4; i++)
    bms.set_cell_voltage_sensor(i, &cells[i]);

  // Same order as the device was polled; cells 5-16 are byte-identical to the shared zero frames
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_TEMPERATURE);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_TOTAL_VOLTAGE);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_CURRENT);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_STATE_OF_CHARGE);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_CAPACITY_REMAINING);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_FULL_CHARGE_CAPACITY);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_TIME_TO_EMPTY);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_TIME_TO_FULL);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_CYCLES);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_DESIGN_CAPACITY);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_CELL_VOLTAGE_01);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_CELL_VOLTAGE_02);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_CELL_VOLTAGE_03);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPTURE2_CELL_VOLTAGE_04);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_05);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_06);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_07);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_08);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_09);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_10);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_11);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_12);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_13);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_14);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_15);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_16);

  EXPECT_NEAR(temperature.state, 8.15f, 0.01f);
  EXPECT_NEAR(voltage.state, 13.256f, 0.001f);
  EXPECT_NEAR(current.state, -0.694f, 0.001f);
  EXPECT_NEAR(power.state, -9.200f, 0.001f);  // 13.256V * -0.694A
  EXPECT_FALSE(charging.state);
  EXPECT_TRUE(discharging.state);
  EXPECT_FLOAT_EQ(soc.state, 99.0f);
  EXPECT_NEAR(capacity.state, 102.81f, 0.01f);
  EXPECT_NEAR(full_capacity.state, 103.76f, 0.01f);
  EXPECT_NEAR(design_capacity.state, 100.0f, 0.01f);
  EXPECT_FLOAT_EQ(cycles.state, 20.0f);
  EXPECT_NEAR(time_empty.state, 489540.0f, 1.0f);  // 8159 min * 60
  EXPECT_TRUE(std::isnan(time_full.state));
  EXPECT_NEAR(cells[0].state, 3.313f, 0.001f);
  EXPECT_NEAR(cells[1].state, 3.315f, 0.001f);
  EXPECT_NEAR(cells[2].state, 3.315f, 0.001f);
  EXPECT_NEAR(cells[3].state, 3.316f, 0.001f);
  EXPECT_NEAR(max_voltage.state, 3.316f, 0.001f);
  EXPECT_NEAR(min_voltage.state, 3.313f, 0.001f);
  EXPECT_FLOAT_EQ(max_cell.state, 4.0f);
  EXPECT_FLOAT_EQ(min_cell.state, 1.0f);
  EXPECT_NEAR(avg_voltage.state, 3.31475f, 0.0001f);
  EXPECT_NEAR(delta_voltage.state, 0.003f, 0.0001f);
  // Sum of cell voltages matches the reported total voltage
  float cell_sum = cells[0].state + cells[1].state + cells[2].state + cells[3].state;
  EXPECT_NEAR(cell_sum, voltage.state, 0.01f);
}

}  // namespace esphome::ogt_bms_ble::testing
