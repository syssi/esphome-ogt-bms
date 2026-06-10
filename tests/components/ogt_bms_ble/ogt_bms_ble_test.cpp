#include <gtest/gtest.h>
#include "common.h"
#include "common_type_a.h"

namespace esphome::ogt_bms_ble::testing {

// ── Online status tracker ─────────────────────────────────────────────────────

TEST(OgtBmsBleOnlineStatusTrackerTest, ReachesThreshold) {
  TestableOgtBmsBle bms;
  binary_sensor::BinarySensor online_status;
  bms.set_online_status_binary_sensor(&online_status);

  for (int i = 0; i < 10; i++)
    bms.track_online_status_();

  EXPECT_FALSE(online_status.state);
}

TEST(OgtBmsBleOnlineStatusTrackerTest, DoesNotRepeatAfterThreshold) {
  TestableOgtBmsBle bms;
  binary_sensor::BinarySensor online_status;
  bms.set_online_status_binary_sensor(&online_status);

  for (int i = 0; i < 10; i++)
    bms.track_online_status_();
  EXPECT_EQ(bms.get_no_response_count(), 11);

  bms.track_online_status_();
  EXPECT_EQ(bms.get_no_response_count(), 11);
}

TEST(OgtBmsBleOnlineStatusTrackerTest, ResetRestoresOnlineStatus) {
  TestableOgtBmsBle bms;
  binary_sensor::BinarySensor online_status;
  bms.set_online_status_binary_sensor(&online_status);

  for (int i = 0; i < 10; i++)
    bms.track_online_status_();
  EXPECT_FALSE(online_status.state);

  bms.reset_online_status_tracker_();
  EXPECT_TRUE(online_status.state);
  EXPECT_EQ(bms.get_no_response_count(), 0);
}

TEST(OgtBmsBleOnlineStatusTrackerTest, CanRetriggerAfterReset) {
  TestableOgtBmsBle bms;
  binary_sensor::BinarySensor online_status;
  bms.set_online_status_binary_sensor(&online_status);

  for (int i = 0; i < 10; i++)
    bms.track_online_status_();
  bms.reset_online_status_tracker_();

  for (int i = 0; i < 10; i++)
    bms.track_online_status_();
  EXPECT_FALSE(online_status.state);
}

TEST(OgtBmsBleOnlineStatusTrackerTest, ValidFrameResetsCounter) {
  TestableOgtBmsBle bms;
  bms.set_encryption_key(TYPE_A_ENCRYPTION_KEY);
  bms.set_device_type('A');

  for (int i = 0; i < 5; i++)
    bms.track_online_status_();
  EXPECT_EQ(bms.get_no_response_count(), 5);

  bms.on_ogt_bms_ble_data(TYPE_A_SOC_70);
  EXPECT_EQ(bms.get_no_response_count(), 0);
}

TEST(OgtBmsBleOnlineStatusTrackerTest, NotYetAtThresholdStaysOnline) {
  TestableOgtBmsBle bms;
  binary_sensor::BinarySensor online_status;
  bms.set_online_status_binary_sensor(&online_status);

  online_status.publish_state(true);
  for (int i = 0; i < 9; i++)
    bms.track_online_status_();

  EXPECT_TRUE(online_status.state);
}

// ── publish_device_unavailable_ ───────────────────────────────────────────────

TEST(OgtBmsBlePublishDeviceUnavailableTest, SetsOnlineStatusFalse) {
  TestableOgtBmsBle bms;
  binary_sensor::BinarySensor online_status;
  bms.set_online_status_binary_sensor(&online_status);

  online_status.publish_state(true);
  bms.publish_device_unavailable_();

  EXPECT_FALSE(online_status.state);
}

TEST(OgtBmsBlePublishDeviceUnavailableTest, SetsNumericSensorsToNAN) {
  TestableOgtBmsBle bms;
  sensor::Sensor total_voltage, current, power, charging_power, discharging_power;
  sensor::Sensor error_bitmask, state_of_charge, charging_cycles;
  sensor::Sensor capacity_remaining, design_capacity, full_charge_capacity;
  bms.set_total_voltage_sensor(&total_voltage);
  bms.set_current_sensor(&current);
  bms.set_power_sensor(&power);
  bms.set_charging_power_sensor(&charging_power);
  bms.set_discharging_power_sensor(&discharging_power);
  bms.set_error_bitmask_sensor(&error_bitmask);
  bms.set_state_of_charge_sensor(&state_of_charge);
  bms.set_charging_cycles_sensor(&charging_cycles);
  bms.set_capacity_remaining_sensor(&capacity_remaining);
  bms.set_design_capacity_sensor(&design_capacity);
  bms.set_full_charge_capacity_sensor(&full_charge_capacity);

  bms.publish_device_unavailable_();

  EXPECT_TRUE(std::isnan(total_voltage.state));
  EXPECT_TRUE(std::isnan(current.state));
  EXPECT_TRUE(std::isnan(power.state));
  EXPECT_TRUE(std::isnan(charging_power.state));
  EXPECT_TRUE(std::isnan(discharging_power.state));
  EXPECT_TRUE(std::isnan(error_bitmask.state));
  EXPECT_TRUE(std::isnan(state_of_charge.state));
  EXPECT_TRUE(std::isnan(charging_cycles.state));
  EXPECT_TRUE(std::isnan(capacity_remaining.state));
  EXPECT_TRUE(std::isnan(design_capacity.state));
  EXPECT_TRUE(std::isnan(full_charge_capacity.state));
}

TEST(OgtBmsBlePublishDeviceUnavailableTest, SetsCellSensorsToNAN) {
  TestableOgtBmsBle bms;
  sensor::Sensor cell0, cell1;
  bms.set_cell_voltage_sensor(0, &cell0);
  bms.set_cell_voltage_sensor(1, &cell1);

  bms.publish_device_unavailable_();

  EXPECT_TRUE(std::isnan(cell0.state));
  EXPECT_TRUE(std::isnan(cell1.state));
}

TEST(OgtBmsBlePublishDeviceUnavailableTest, SetsDynamicTextSensorsToOffline) {
  TestableOgtBmsBle bms;
  text_sensor::TextSensor errors, time_to_empty_formatted, time_to_full_formatted;
  bms.set_errors_text_sensor(&errors);
  bms.set_time_to_empty_formatted_text_sensor(&time_to_empty_formatted);
  bms.set_time_to_full_formatted_text_sensor(&time_to_full_formatted);

  bms.publish_device_unavailable_();

  EXPECT_EQ(errors.state, "Offline");
  EXPECT_EQ(time_to_empty_formatted.state, "Offline");
  EXPECT_EQ(time_to_full_formatted.state, "Offline");
}

TEST(OgtBmsBlePublishDeviceUnavailableTest, LeavesStaticTextSensorsUnchanged) {
  TestableOgtBmsBle bms;
  text_sensor::TextSensor manufacture_date, serial_number;
  manufacture_date.publish_state("2020.03.17");
  serial_number.publish_state("03747");
  bms.set_manufacture_date_text_sensor(&manufacture_date);
  bms.set_serial_number_text_sensor(&serial_number);

  bms.publish_device_unavailable_();

  EXPECT_EQ(manufacture_date.state, "2020.03.17");
  EXPECT_EQ(serial_number.state, "03747");
}

TEST(OgtBmsBlePublishDeviceUnavailableTest, NullSensorsDoNotCrash) {
  TestableOgtBmsBle bms;

  EXPECT_NO_FATAL_FAILURE(bms.publish_device_unavailable_());
}

}  // namespace esphome::ogt_bms_ble::testing
