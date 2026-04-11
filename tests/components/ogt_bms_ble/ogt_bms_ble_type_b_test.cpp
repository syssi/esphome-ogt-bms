#include <gtest/gtest.h>
#include <cmath>
#include "common_type_b.h"

namespace esphome::ogt_bms_ble::testing {

TEST(OgtBmsBleTypeBTest, TotalVoltageFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor voltage;
  bms.set_total_voltage_sensor(&voltage);
  bms.on_ogt_bms_ble_data(TYPE_B_TOTAL_VOLTAGE);
  EXPECT_NEAR(voltage.state, 45.684f, 0.001f);
}

TEST(OgtBmsBleTypeBTest, TimeToEmptyFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor time;
  bms.set_time_to_empty_sensor(&time);
  bms.on_ogt_bms_ble_data(TYPE_B_TIME_TO_EMPTY);
  EXPECT_NEAR(time.state, 7200.0f, 1.0f);  // 120 min * 60
}

TEST(OgtBmsBleTypeBTest, TimeToFullFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor time;
  bms.set_time_to_full_sensor(&time);
  bms.on_ogt_bms_ble_data(TYPE_B_TIME_TO_FULL);
  EXPECT_NEAR(time.state, 10800.0f, 1.0f);  // 180 min * 60
}

TEST(OgtBmsBleTypeBTest, ChargingCyclesFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor cycles;
  bms.set_charging_cycles_sensor(&cycles);
  bms.on_ogt_bms_ble_data(TYPE_B_CYCLES);
  EXPECT_FLOAT_EQ(cycles.state, 99.0f);
}

TEST(OgtBmsBleTypeBTest, CapacityRemainingFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor capacity;
  bms.set_capacity_remaining_sensor(&capacity);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPACITY_REMAINING);
  EXPECT_NEAR(capacity.state, 8.0f, 0.01f);
}

TEST(OgtBmsBleTypeBTest, FullChargeCapacityFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor capacity;
  bms.set_full_charge_capacity_sensor(&capacity);
  bms.on_ogt_bms_ble_data(TYPE_B_FULL_CHARGE_CAPACITY);
  EXPECT_NEAR(capacity.state, 12.34f, 0.01f);
}

TEST(OgtBmsBleTypeBTest, DesignCapacityFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor capacity;
  bms.set_design_capacity_sensor(&capacity);
  bms.on_ogt_bms_ble_data(TYPE_B_DESIGN_CAPACITY);
  EXPECT_NEAR(capacity.state, 12.0f, 0.01f);
}

TEST(OgtBmsBleTypeBTest, CellVoltageFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  sensor::Sensor cell1;
  bms.set_cell_voltage_sensor(0, &cell1);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_01);
  EXPECT_NEAR(cell1.state, 3.450f, 0.001f);
}

TEST(OgtBmsBleTypeBTest, CellVoltageStats) {
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

  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_01);  // 3.450V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_02);  // 3.440V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_03);  // 3.430V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_04);  // 3.420V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_05);  // 3.410V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_06);  // 3.400V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_07);  // 3.390V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_08);  // 3.380V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_09);  // 3.370V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_10);  // 3.360V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_11);  // 3.350V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_12);  // 3.340V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_13);  // 3.330V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_14);  // 3.320V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_15);  // 3.310V
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_16);  // 3.300V — triggers stats update

  EXPECT_NEAR(max_voltage.state, 3.450f, 0.001f);
  EXPECT_NEAR(min_voltage.state, 3.300f, 0.001f);
  EXPECT_FLOAT_EQ(max_cell.state, 1.0f);   // cell 1 has highest voltage
  EXPECT_FLOAT_EQ(min_cell.state, 16.0f);  // cell 16 has lowest voltage
  EXPECT_NEAR(avg_voltage.state, 3.375f, 0.001f);
  EXPECT_NEAR(delta_voltage.state, 0.150f, 0.001f);
}

TEST(OgtBmsBleTypeBTest, NullSensorsDoNotCrash) {
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);
  bms.on_ogt_bms_ble_data(TYPE_B_TOTAL_VOLTAGE);
  bms.on_ogt_bms_ble_data(TYPE_B_TIME_TO_EMPTY);
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
  TestableOgtBmsBle bms;
  bms.set_device_type('B');
  bms.set_encryption_key(TYPE_B_ENCRYPTION_KEY);

  sensor::Sensor voltage, capacity, full_capacity, design_capacity, cycles;
  sensor::Sensor time_empty, time_full;
  sensor::Sensor cells[16];
  sensor::Sensor min_voltage, max_voltage, avg_voltage, delta_voltage;

  bms.set_total_voltage_sensor(&voltage);
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
  for (uint8_t i = 0; i < 16; i++)
    bms.set_cell_voltage_sensor(i, &cells[i]);

  bms.on_ogt_bms_ble_data(TYPE_B_TOTAL_VOLTAGE);
  bms.on_ogt_bms_ble_data(TYPE_B_CAPACITY_REMAINING);
  bms.on_ogt_bms_ble_data(TYPE_B_FULL_CHARGE_CAPACITY);
  bms.on_ogt_bms_ble_data(TYPE_B_DESIGN_CAPACITY);
  bms.on_ogt_bms_ble_data(TYPE_B_CYCLES);
  bms.on_ogt_bms_ble_data(TYPE_B_TIME_TO_EMPTY);
  bms.on_ogt_bms_ble_data(TYPE_B_TIME_TO_FULL);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_01);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_02);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_03);
  bms.on_ogt_bms_ble_data(TYPE_B_CELL_VOLTAGE_04);
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

  EXPECT_NEAR(voltage.state, 45.684f, 0.001f);
  EXPECT_NEAR(capacity.state, 8.0f, 0.01f);
  EXPECT_NEAR(full_capacity.state, 12.34f, 0.01f);
  EXPECT_NEAR(design_capacity.state, 12.0f, 0.01f);
  EXPECT_FLOAT_EQ(cycles.state, 99.0f);
  EXPECT_NEAR(time_empty.state, 7200.0f, 1.0f);
  EXPECT_NEAR(time_full.state, 10800.0f, 1.0f);
  EXPECT_NEAR(max_voltage.state, 3.450f, 0.001f);
  EXPECT_NEAR(min_voltage.state, 3.300f, 0.001f);
  EXPECT_NEAR(avg_voltage.state, 3.375f, 0.001f);
  EXPECT_NEAR(delta_voltage.state, 0.150f, 0.001f);
}

}  // namespace esphome::ogt_bms_ble::testing
