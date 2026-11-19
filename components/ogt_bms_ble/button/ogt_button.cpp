#include "ogt_button.h"

namespace esphome::ogt_bms_ble {

void OgtButton::press_action() { this->parent_->send_command(this->holding_register_, 2); }

}  // namespace esphome::ogt_bms_ble
