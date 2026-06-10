#pragma once
#include <cstdint>
#include <vector>
#include "esphome/components/ogt_bms_ble/ogt_bms_ble.h"

namespace esphome::ogt_bms_ble::testing {

class TestableOgtBmsBle : public OgtBmsBle {
 public:
  using OgtBmsBle::on_ogt_bms_ble_data;
  using OgtBmsBle::track_online_status_;
  using OgtBmsBle::reset_online_status_tracker_;
  using OgtBmsBle::publish_device_unavailable_;
  uint8_t get_no_response_count() const { return no_response_count_; }
};

}  // namespace esphome::ogt_bms_ble::testing
