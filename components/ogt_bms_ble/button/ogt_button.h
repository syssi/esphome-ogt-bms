#pragma once

#include "../ogt_bms_ble.h"
#include "esphome/components/button/button.h"

namespace esphome::ogt_bms_ble {

class OgtBmsBle;
class OgtButton : public button::Button {
 public:
  void set_parent(OgtBmsBle *parent) { this->parent_ = parent; };
  void set_holding_register(uint8_t holding_register) { this->holding_register_ = holding_register; };

 protected:
  void press_action() override;
  OgtBmsBle *parent_;
  uint8_t holding_register_;
};

}  // namespace esphome::ogt_bms_ble
