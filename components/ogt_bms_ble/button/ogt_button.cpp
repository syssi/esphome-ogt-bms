#include "ogt_button.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::ogt_bms_ble {

ESPHOME_LOG_TAG(TAG, "ogt_bms_ble.button");

void OgtButton::dump_config() { LOG_BUTTON("", "OgtBmsBle Button", this); }
void OgtButton::press_action() { this->parent_->send_command(this->holding_register_, 2); }

}  // namespace esphome::ogt_bms_ble
