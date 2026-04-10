#pragma once
#include <cstdint>
#include <vector>
#include "esphome/components/ogt_bms_ble/ogt_bms_ble.h"

namespace esphome::ogt_bms_ble::testing {

class TestableOgtBmsBle : public OgtBmsBle {
 public:
  using OgtBmsBle::on_ogt_bms_ble_data;
};

}  // namespace esphome::ogt_bms_ble::testing
