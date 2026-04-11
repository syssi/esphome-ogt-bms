#include <gtest/gtest.h>
#include <cmath>
#include "common_type_a.h"

namespace esphome::ogt_bms_ble::testing {

TEST(OgtBmsBleTypeATest, StateOfChargeFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  sensor::Sensor soc;
  bms.set_state_of_charge_sensor(&soc);
  bms.on_ogt_bms_ble_data(TYPE_A_SOC_70);
  EXPECT_FLOAT_EQ(soc.state, 70.0f);
}

TEST(OgtBmsBleTypeATest, TotalVoltageFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  sensor::Sensor voltage;
  bms.set_total_voltage_sensor(&voltage);
  bms.on_ogt_bms_ble_data(TYPE_A_TOTAL_VOLTAGE);
  EXPECT_NEAR(voltage.state, 13.312f, 0.001f);
}

TEST(OgtBmsBleTypeATest, TemperatureFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  sensor::Sensor temp;
  bms.set_mosfet_temperature_sensor(&temp);
  bms.on_ogt_bms_ble_data(TYPE_A_TEMPERATURE);
  EXPECT_NEAR(temp.state, 23.65f, 0.01f);
}

TEST(OgtBmsBleTypeATest, CurrentDischargeFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  sensor::Sensor current;
  bms.set_current_sensor(&current);
  binary_sensor::BinarySensor charging;
  bms.set_charging_binary_sensor(&charging);
  binary_sensor::BinarySensor discharging;
  bms.set_discharging_binary_sensor(&discharging);

  bms.on_ogt_bms_ble_data(TYPE_A_CURRENT_DISCHARGE_A);

  EXPECT_NEAR(current.state, -0.39f, 0.01f);
  EXPECT_FALSE(charging.state);
  EXPECT_TRUE(discharging.state);
}

TEST(OgtBmsBleTypeATest, TimeToEmptyFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  sensor::Sensor time;
  bms.set_time_to_empty_sensor(&time);
  bms.on_ogt_bms_ble_data(TYPE_A_TIME_TO_EMPTY);
  EXPECT_NEAR(time.state, 701040.0f, 1.0f);  // 11684 min * 60
}

TEST(OgtBmsBleTypeATest, TimeToFullInvalidFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  sensor::Sensor time;
  bms.set_time_to_full_sensor(&time);
  bms.on_ogt_bms_ble_data(TYPE_A_TIME_TO_FULL_INVALID);
  EXPECT_TRUE(std::isnan(time.state));
}

TEST(OgtBmsBleTypeATest, ChargingCyclesFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  sensor::Sensor cycles;
  bms.set_charging_cycles_sensor(&cycles);
  bms.on_ogt_bms_ble_data(TYPE_A_CYCLES);
  EXPECT_FLOAT_EQ(cycles.state, 26.0f);
}

TEST(OgtBmsBleTypeATest, CapacityRemainingFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  sensor::Sensor capacity;
  bms.set_capacity_remaining_sensor(&capacity);
  bms.on_ogt_bms_ble_data(TYPE_A_CAPACITY_REMAINING);
  EXPECT_NEAR(capacity.state, 72.05f, 0.01f);
}

TEST(OgtBmsBleTypeATest, FullChargeCapacityFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  sensor::Sensor capacity;
  bms.set_full_charge_capacity_sensor(&capacity);
  bms.on_ogt_bms_ble_data(TYPE_A_FULL_CHARGE_CAPACITY);
  EXPECT_NEAR(capacity.state, 102.06f, 0.01f);
}

TEST(OgtBmsBleTypeATest, DesignCapacityFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  sensor::Sensor capacity;
  bms.set_design_capacity_sensor(&capacity);
  bms.on_ogt_bms_ble_data(TYPE_A_DESIGN_CAPACITY);
  EXPECT_NEAR(capacity.state, 100.0f, 0.01f);
}

TEST(OgtBmsBleTypeATest, SerialNumberFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  text_sensor::TextSensor serial;
  bms.set_serial_number_text_sensor(&serial);
  bms.on_ogt_bms_ble_data(TYPE_A_SERIAL_NUMBER);
  EXPECT_STREQ(serial.state.c_str(), "03747");
}

TEST(OgtBmsBleTypeATest, ManufactureDateFrame) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  text_sensor::TextSensor date;
  bms.set_manufacture_date_text_sensor(&date);
  bms.on_ogt_bms_ble_data(TYPE_A_MANUFACTURE_DATE);
  EXPECT_STREQ(date.state.c_str(), "2020.03.17");
}

TEST(OgtBmsBleTypeATest, MultipleDischargeCurrentFrames) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  sensor::Sensor current;
  bms.set_current_sensor(&current);
  binary_sensor::BinarySensor charging;
  bms.set_charging_binary_sensor(&charging);
  binary_sensor::BinarySensor discharging;
  bms.set_discharging_binary_sensor(&discharging);

  for (const auto *frame : {&TYPE_A_CURRENT_DISCHARGE_A, &TYPE_A_CURRENT_DISCHARGE_B, &TYPE_A_CURRENT_DISCHARGE_C}) {
    bms.on_ogt_bms_ble_data(*frame);
    EXPECT_LT(current.state, 0.0f);
    EXPECT_FALSE(charging.state);
    EXPECT_TRUE(discharging.state);
  }
}

TEST(OgtBmsBleTypeATest, NullSensorsDoNotCrash) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  bms.on_ogt_bms_ble_data(TYPE_A_SOC_70);
  bms.on_ogt_bms_ble_data(TYPE_A_TOTAL_VOLTAGE);
  bms.on_ogt_bms_ble_data(TYPE_A_TEMPERATURE);
  bms.on_ogt_bms_ble_data(TYPE_A_CURRENT_DISCHARGE_A);
  bms.on_ogt_bms_ble_data(TYPE_A_TIME_TO_EMPTY);
  bms.on_ogt_bms_ble_data(TYPE_A_CYCLES);
}

TEST(OgtBmsBleTypeATest, InvalidFrameTooShort) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  sensor::Sensor soc;
  soc.state = -1.0f;
  bms.set_state_of_charge_sensor(&soc);
  bms.on_ogt_bms_ble_data(TYPE_A_INVALID_FRAME_TOO_SHORT);
  EXPECT_FLOAT_EQ(soc.state, -1.0f);
}

TEST(OgtBmsBleTypeATest, InvalidFrameOddLength) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  sensor::Sensor soc;
  soc.state = -1.0f;
  bms.set_state_of_charge_sensor(&soc);
  bms.on_ogt_bms_ble_data(TYPE_A_INVALID_FRAME_ODD_LENGTH);
  EXPECT_FLOAT_EQ(soc.state, -1.0f);
}

TEST(OgtBmsBleTypeATest, FullDataRoundTrip) {
  TestableOgtBmsBle bms;
  bms.set_device_type('A');
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);

  sensor::Sensor soc, voltage, temp, current, capacity, full_capacity, design_capacity, cycles;
  text_sensor::TextSensor serial, date;

  bms.set_state_of_charge_sensor(&soc);
  bms.set_total_voltage_sensor(&voltage);
  bms.set_mosfet_temperature_sensor(&temp);
  bms.set_current_sensor(&current);
  bms.set_capacity_remaining_sensor(&capacity);
  bms.set_full_charge_capacity_sensor(&full_capacity);
  bms.set_design_capacity_sensor(&design_capacity);
  bms.set_charging_cycles_sensor(&cycles);
  bms.set_serial_number_text_sensor(&serial);
  bms.set_manufacture_date_text_sensor(&date);

  bms.on_ogt_bms_ble_data(TYPE_A_SOC_70);
  bms.on_ogt_bms_ble_data(TYPE_A_TOTAL_VOLTAGE);
  bms.on_ogt_bms_ble_data(TYPE_A_TEMPERATURE);
  bms.on_ogt_bms_ble_data(TYPE_A_CURRENT_DISCHARGE_A);
  bms.on_ogt_bms_ble_data(TYPE_A_TIME_TO_EMPTY);
  bms.on_ogt_bms_ble_data(TYPE_A_CAPACITY_REMAINING);
  bms.on_ogt_bms_ble_data(TYPE_A_FULL_CHARGE_CAPACITY);
  bms.on_ogt_bms_ble_data(TYPE_A_DESIGN_CAPACITY);
  bms.on_ogt_bms_ble_data(TYPE_A_CYCLES);
  bms.on_ogt_bms_ble_data(TYPE_A_SERIAL_NUMBER);
  bms.on_ogt_bms_ble_data(TYPE_A_MANUFACTURE_DATE);

  EXPECT_FLOAT_EQ(soc.state, 70.0f);
  EXPECT_NEAR(voltage.state, 13.312f, 0.001f);
  EXPECT_NEAR(temp.state, 23.65f, 0.01f);
  EXPECT_NEAR(current.state, -0.39f, 0.01f);
  EXPECT_NEAR(capacity.state, 72.05f, 0.01f);
  EXPECT_NEAR(full_capacity.state, 102.06f, 0.01f);
  EXPECT_NEAR(design_capacity.state, 100.0f, 0.01f);
  EXPECT_FLOAT_EQ(cycles.state, 26.0f);
  EXPECT_STREQ(serial.state.c_str(), "03747");
  EXPECT_STREQ(date.state.c_str(), "2020.03.17");
}

}  // namespace esphome::ogt_bms_ble::testing
